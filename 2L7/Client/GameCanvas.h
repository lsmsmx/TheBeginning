#ifndef GAMECANVAS_H
#define GAMECANVAS_H

#include <QWidget>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

class GameCanvas : public QWidget {
    Q_OBJECT
public:
    explicit GameCanvas(QWidget *parent = nullptr);
    void updateState(const QJsonArray& players, int myId);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QJsonArray m_players;
    int m_myId;
    QColor getColorFromString(const QString& str);
};

#endif // GAMECANVAS_H