#include "MainWindow.h"
#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), m_myId(-1), m_myX(0), m_myY(0), m_isImitation(false) {
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);

    m_imitationTimer = new QTimer(this);
    connect(m_imitationTimer, &QTimer::timeout, this, &MainWindow::doImitationStep);

    m_myColor = "Синий";
    setupUI();
    
    // Запускаем окно логина сразу после создания окна (без мелькания главного окна)
    QTimer::singleShot(0, this, &MainWindow::connectToServer);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    m_canvas = new GameCanvas(this);
    
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_canvas);
    
    QHBoxLayout* colorLayout = new QHBoxLayout();
    colorLayout->addWidget(new QLabel("Цвет:", this));
    
    m_cbColor = new QComboBox(this);
    m_cbColor->addItems({"Синий", "Красный", "Зеленый", "Желтый"});
    
    // Цвета в списке
    m_cbColor->setItemData(0, QColor(Qt::blue), Qt::ForegroundRole);
    m_cbColor->setItemData(1, QColor(Qt::red), Qt::ForegroundRole);
    m_cbColor->setItemData(2, QColor(Qt::green), Qt::ForegroundRole);
    m_cbColor->setItemData(3, QColor(180, 180, 0), Qt::ForegroundRole);
    
    updateComboboxColors("Синий");

    colorLayout->addWidget(m_cbColor);
    colorLayout->addStretch();
    leftLayout->addLayout(colorLayout);

    connect(m_cbColor, &QComboBox::currentTextChanged, [this](const QString& text){
        if(!text.isEmpty()) { 
            m_myColor = text; 
            updateComboboxColors(text);
            sendMyState(); 
        }
    });

    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    m_lblWeather = new QLabel("Погода:\nПасмурно", this);
    QFont f = m_lblWeather->font(); f.setPointSize(14); f.setBold(true);
    m_lblWeather->setFont(f);
    rightLayout->addWidget(m_lblWeather);
    rightLayout->addStretch();

    QGridLayout* grid = new QGridLayout();
    btnUp = new QPushButton("↑", this);
    btnDown = new QPushButton("↓", this);
    btnLeft = new QPushButton("←", this);
    btnRight = new QPushButton("→", this);
    
    btnUp->setFixedSize(40,40); btnDown->setFixedSize(40,40);
    btnLeft->setFixedSize(40,40); btnRight->setFixedSize(40,40);

    grid->addWidget(btnUp, 0, 1);
    grid->addWidget(btnLeft, 1, 0);
    grid->addWidget(btnDown, 1, 1);
    grid->addWidget(btnRight, 1, 2);
    rightLayout->addLayout(grid);

    btnImitate = new QPushButton("Имитация", this);
    rightLayout->addWidget(btnImitate);
    
    btnRestart = new QPushButton("Рестарт", this);
    rightLayout->addWidget(btnRestart);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    connect(btnUp, &QPushButton::clicked, this, &MainWindow::moveUp);
    connect(btnDown, &QPushButton::clicked, this, &MainWindow::moveDown);
    connect(btnLeft, &QPushButton::clicked, this, &MainWindow::moveLeft);
    connect(btnRight, &QPushButton::clicked, this, &MainWindow::moveRight);
    connect(btnImitate, &QPushButton::clicked, this, &MainWindow::toggleImitation);
    connect(btnRestart, &QPushButton::clicked, this, &MainWindow::requestRestart); 
    
    hide();
}

void MainWindow::updateComboboxColors(const QString& text) {
    if (text == "Синий") m_cbColor->setStyleSheet("color: blue; border: 2px solid black;");
    else if (text == "Красный") m_cbColor->setStyleSheet("color: red; border: 2px solid black;");
    else if (text == "Зеленый") m_cbColor->setStyleSheet("color: green; border: 2px solid black;");
    else if (text == "Желтый") m_cbColor->setStyleSheet("color: #B4B400; border: 2px solid black;");
}

void MainWindow::connectToServer() {
    this->hide(); 
    
    LoginDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        m_myId = dlg.getID();
        
        m_socket->abort(); 
        // .trimmed() нужен, чтобы случайно введенный пробел после IP не ломал сеть
        m_socket->connectToHost(dlg.getAddress().trimmed(), dlg.getPort());
        
        if (m_socket->waitForConnected(2000)) {
            QJsonObject obj;
            obj["type"] = "login";
            obj["id"] = m_myId;
            QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
            data.append('\n');
            m_socket->write(data);
        } else {
            // ТЕПЕРЬ ОН БУДЕТ ПИСАТЬ ТОЧНУЮ ПРИЧИНУ:
            QString err = m_socket->errorString();
            QMessageBox::critical(this, "Ошибка связи", 
                "Не могу подключиться!\nПричина: " + err + 
                "\n\nВозможно, порт занят зависшим процессом или сервер выключен.");
            
            QTimer::singleShot(100, this, &MainWindow::connectToServer);
        }
    } else {
        QApplication::quit(); // Если закрыть крестиком - процесс полностью убьется, без зомби
    }
}

void MainWindow::onReadyRead() {
    while (m_socket->canReadLine()) {
        QByteArray data = m_socket->readLine();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) continue;

        QJsonObject obj = doc.object();
        QString type = obj["type"].toString();

        if (type == "login_ok") {
            this->show(); 
        } else if (type == "error") {
            QMessageBox::critical(this, "Ошибка", obj["message"].toString());
            this->hide();
            m_socket->disconnectFromHost(); // Очищаем сокет от старого подключения, чтобы не было конфликтов
            m_myId = -1;
            // Открываем окно логина заново с небольшой задержкой
            QTimer::singleShot(0, this, &MainWindow::connectToServer);
        } else if (type == "game_state") {
            parseGameState(obj);
        } else if (type == "vote_request") { 
            handleVoteRequest();
        }
    }
}

void MainWindow::parseGameState(const QJsonObject& obj) {
    m_lblWeather->setText("Погода:\n" + obj["weather"].toString());
    
    QJsonArray players = obj["players"].toArray();
    m_canvas->updateState(players, m_myId);

    for(int i=0; i<players.size(); ++i) {
        QJsonObject p = players[i].toObject();
        if (p["id"].toInt() == m_myId) {
            m_myX = p["x"].toInt();
            m_myY = p["y"].toInt();
        }
    }

    QJsonArray takenColors = obj["taken_colors"].toArray();
    QString currentTxt = m_cbColor->currentText();
    m_cbColor->blockSignals(true);
    m_cbColor->clear();
    
    QStringList allColors = {"Синий", "Красный", "Зеленый", "Желтый"};
    for (const QString& c : allColors) {
        bool taken = false;
        for (int i=0; i<takenColors.size(); ++i) {
            if (takenColors[i].toString() == c && c != m_myColor) { taken = true; break; }
        }
        if (!taken) {
            m_cbColor->addItem(c);
            int idx = m_cbColor->count() - 1;
            if (c == "Синий") m_cbColor->setItemData(idx, QColor(Qt::blue), Qt::ForegroundRole);
            if (c == "Красный") m_cbColor->setItemData(idx, QColor(Qt::red), Qt::ForegroundRole);
            if (c == "Зеленый") m_cbColor->setItemData(idx, QColor(Qt::green), Qt::ForegroundRole);
            if (c == "Желтый") m_cbColor->setItemData(idx, QColor(180, 180, 0), Qt::ForegroundRole);
        }
    }
    
    if (m_cbColor->findText(currentTxt) != -1) m_cbColor->setCurrentText(currentTxt);
    m_cbColor->blockSignals(false);
}

void MainWindow::sendMyState() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj["type"] = "update_state";
        obj["x"] = m_myX;
        obj["y"] = m_myY;
        obj["color"] = m_myColor;
        QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
        data.append('\n');
        m_socket->write(data);
    }
}

void MainWindow::moveUp() { if(m_myY < 10) { m_myY++; sendMyState(); } }
void MainWindow::moveDown() { if(m_myY > -10) { m_myY--; sendMyState(); } }
void MainWindow::moveLeft() { if(m_myX > -10) { m_myX--; sendMyState(); } }
void MainWindow::moveRight() { if(m_myX < 10) { m_myX++; sendMyState(); } }

void MainWindow::toggleImitation() {
    m_isImitation = !m_isImitation;
    btnUp->setEnabled(!m_isImitation);
    btnDown->setEnabled(!m_isImitation);
    btnLeft->setEnabled(!m_isImitation);
    btnRight->setEnabled(!m_isImitation);
    
    if (m_isImitation) m_imitationTimer->start(1000);
    else m_imitationTimer->stop();
}

void MainWindow::doImitationStep() {
    int dir = QRandomGenerator::global()->bounded(4);
    if (dir == 0) moveUp();
    else if (dir == 1) moveDown();
    else if (dir == 2) moveLeft();
    else moveRight();
}

void MainWindow::requestRestart() {
    QJsonObject obj;
    obj["type"] = "start_vote";
    QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    data.append('\n');
    m_socket->write(data);
    QMessageBox::information(this, "Голосование", "Запрос отправлен. Ожидайте.");
}

void MainWindow::handleVoteRequest() {
    QMessageBox::StandardButton res = QMessageBox::question(this, "Рестарт", 
       "Кто-то запросил рестарт. Все переместятся в (0,0). Согласны?", 
       QMessageBox::Yes | QMessageBox::No);
       
    QJsonObject obj;
    obj["type"] = "vote_answer";
    obj["answer"] = (res == QMessageBox::Yes);
    QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    data.append('\n');
    m_socket->write(data);
}

void MainWindow::onDisconnected() {
    // Реагируем на обрыв связи только если мы УЖЕ авторизованы в игре (m_myId != -1)
    if (m_myId != -1) {
        QMessageBox::warning(this, "Отключение", "Связь с сервером потеряна.");
        close();
    }
}