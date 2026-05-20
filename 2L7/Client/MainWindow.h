#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include "GameCanvas.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToServer();
    void onReadyRead();
    void onDisconnected();
    void sendMyState();
    
    void moveUp(); void moveDown(); void moveLeft(); void moveRight();
    void toggleImitation();
    void doImitationStep();
    void requestRestart();

private:
    void setupUI();
    void parseGameState(const QJsonObject& obj);
    void updateComboboxColors(const QString& currentText);
    void handleVoteRequest();
    
    QTcpSocket* m_socket;
    int m_myId;
    int m_myX;
    int m_myY;
    QString m_myColor;
    
    bool m_isImitation;
    QTimer* m_imitationTimer;

    GameCanvas* m_canvas;
    QLabel* m_lblWeather;
    QComboBox* m_cbColor;
    QPushButton *btnUp, *btnDown, *btnLeft, *btnRight, *btnImitate, *btnRestart;
};

#endif // MAINWINDOW_H