#include "GameCanvas.h"

GameCanvas::GameCanvas(QWidget *parent) : QWidget(parent), m_myId(-1) {
    setFixedSize(400, 400); // Канвас 20x20 ед. (от -10 до 10). Пусть 1 ед = 20 пикселей.
    setStyleSheet("background-color: white; border: 1px solid black;");
}

void GameCanvas::updateState(const QJsonArray& players, int myId) {
    m_players = players;
    m_myId = myId;
    update(); // Перерисовать
}

QColor GameCanvas::getColorFromString(const QString& str) {
    if (str == "Красный") return Qt::red;
    if (str == "Зеленый") return Qt::green;
    if (str == "Желтый") return Qt::yellow;
    return Qt::blue; // По умолчанию Синий
}

void GameCanvas::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Размер логического поля 20x20. Координаты [-10; 10]
    // X_pix = (X_log + 10) * 20
    // Y_pix = (-Y_log + 10) * 20 (Y инвертирован, т.к. в Qt 0 сверху)
    
    int cellSize = 20;

    for (int i = 0; i < m_players.size(); ++i) {
        QJsonObject obj = m_players[i].toObject();
        int id = obj["id"].toInt();
        int lx = obj["x"].toInt();
        int ly = obj["y"].toInt();
        QColor color = getColorFromString(obj["color"].toString());

        int px = (lx + 10) * cellSize;
        int py = (-ly + 10) * cellSize;

        p.setBrush(color);
        p.setPen(Qt::NoPen);

        if (id == m_myId) {
            // Ваш персонаж - кружок
            p.drawEllipse(px, py, cellSize, cellSize);
        } else {
            // Другие - квадраты
            p.drawRect(px, py, cellSize, cellSize);
        }
    }
}