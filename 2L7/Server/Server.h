#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

struct PlayerData {
    int id;
    int x;
    int y;
    QString color;
};

class Server : public QTcpServer {
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    ~Server();
    bool start(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();
    void changeWeather();
    void checkVoteTimeout();

private:
    void sendToClient(QTcpSocket* socket, const QJsonObject& doc);
    void broadcast(const QJsonObject& doc, QTcpSocket* exclude = nullptr);
    void loadData();
    void saveData();
    void sendGameState(QTcpSocket* specificClient = nullptr);

    QMap<QTcpSocket*, PlayerData> m_clients; // Кто онлайн
    QMap<int, PlayerData> m_savedData;       // Вся база (сохранение/загрузка)
    
    QStringList m_weatherStates = {"Ясно", "Пасмурно", "Дождливо"};
    QString m_currentWeather;
    QTimer* m_weatherTimer;

    bool m_isVoting;
    QTimer* m_voteTimer;
    int m_votesYes;
    int m_votesNo;
    int m_expectedVotes;
};

#endif // SERVER_H