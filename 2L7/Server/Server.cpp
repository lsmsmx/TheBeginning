#include "Server.h"
#include <QRandomGenerator>
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent), m_isVoting(false) {
    m_currentWeather = m_weatherStates[0];
    
    m_weatherTimer = new QTimer(this);
    connect(m_weatherTimer, &QTimer::timeout, this, &Server::changeWeather);
    m_weatherTimer->start(10000);

    m_voteTimer = new QTimer(this);
    m_voteTimer->setSingleShot(true);
    connect(m_voteTimer, &QTimer::timeout, this, &Server::checkVoteTimeout);

    loadData();
}

Server::~Server() {
    saveData();
}

bool Server::start(quint16 port) {
    // ВАЖНО: AnyIPv4 заставит сервер слушать именно 127.0.0.1
    if (listen(QHostAddress::AnyIPv4, port)) {
        qDebug() << "СЕРВЕР УСПЕШНО ЗАПУЩЕН! Жду подключений на порту:" << port;
        return true;
    } else {
        qDebug() << "СЕРВЕР НЕ СМОГ ЗАПУСТИТЬСЯ. Причина:" << errorString();
        return false;
    }
}
void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
}

void Server::onReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    // Чтение строго по строкам
    while (socket->canReadLine()) {
        QByteArray data = socket->readLine();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) continue;

        QJsonObject obj = doc.object();
        QString type = obj["type"].toString();

        if (type == "login") {
            int id = obj["id"].toInt();
            
            bool idExists = false;
            for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
                if (it.value().id == id) { idExists = true; break; }
            }

            if (idExists) {
                QJsonObject err; err["type"] = "error"; err["message"] = "Этот ID уже онлайн!";
                sendToClient(socket, err);
                socket->disconnectFromHost();
                continue;
            }

            PlayerData pd;
            pd.id = id;
            
            if (m_savedData.contains(id)) {
                pd.x = m_savedData[id].x;
                pd.y = m_savedData[id].y;
                pd.color = m_savedData[id].color;
            } else {
                pd.x = 0; pd.y = 0; pd.color = "Синий";
            }
            
            m_clients[socket] = pd;

            QJsonObject ok; ok["type"] = "login_ok";
            sendToClient(socket, ok);
            sendGameState();
        } 
        else if (type == "update_state") {
            if (m_clients.contains(socket)) {
                m_clients[socket].x = obj["x"].toInt();
                m_clients[socket].y = obj["y"].toInt();
                m_clients[socket].color = obj["color"].toString();
                sendGameState();
            }
        }
        else if (type == "start_vote") {
            if (!m_isVoting && m_clients.size() > 1) {
                m_isVoting = true;
                m_votesYes = 0;
                m_votesNo = 0;
                m_expectedVotes = m_clients.size() - 1;

                QJsonObject voteReq; voteReq["type"] = "vote_request";
                broadcast(voteReq, socket);
                m_voteTimer->start(30000);
            }
        }
        else if (type == "vote_answer") {
            if (m_isVoting) {
                if (obj["answer"].toBool()) m_votesYes++;
                else m_votesNo++;

                if (m_votesYes + m_votesNo >= m_expectedVotes) checkVoteTimeout();
            }
        }
    }
}

void Server::checkVoteTimeout() {
    m_voteTimer->stop();
    m_isVoting = false;
    
    if (m_votesYes > m_expectedVotes / 2) {
        for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
            it.value().x = 0;
            it.value().y = 0;
        }
    }
    sendGameState();
}

void Server::onClientDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        if (m_clients.contains(socket)) {
            m_savedData[m_clients[socket].id] = m_clients[socket];
        }
        m_clients.remove(socket);
        socket->deleteLater();
        sendGameState();
    }
}

void Server::changeWeather() {
    QString oldW = m_currentWeather;
    while (m_currentWeather == oldW) {
        m_currentWeather = m_weatherStates[QRandomGenerator::global()->bounded(m_weatherStates.size())];
    }
    sendGameState();
}

void Server::sendGameState(QTcpSocket* specificClient) {
    QJsonObject state;
    state["type"] = "game_state";
    state["weather"] = m_currentWeather;

    QJsonArray playersArray;
    QJsonArray takenColors;
    
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
        QJsonObject pObj;
        pObj["id"] = it.value().id;
        pObj["x"] = it.value().x;
        pObj["y"] = it.value().y;
        pObj["color"] = it.value().color;
        playersArray.append(pObj);
        
        if (!takenColors.contains(it.value().color)) {
            takenColors.append(it.value().color);
        }
    }
    state["players"] = playersArray;
    state["taken_colors"] = takenColors; 

    if (specificClient) sendToClient(specificClient, state);
    else broadcast(state);
}

// Отправка с \n
void Server::sendToClient(QTcpSocket* socket, const QJsonObject& doc) {
    QByteArray data = QJsonDocument(doc).toJson(QJsonDocument::Compact);
    data.append('\n');
    socket->write(data);
}

void Server::broadcast(const QJsonObject& doc, QTcpSocket* exclude) {
    QByteArray data = QJsonDocument(doc).toJson(QJsonDocument::Compact);
    data.append('\n');
    for (QTcpSocket* socket : m_clients.keys()) {
        if (socket != exclude) {
            socket->write(data);
        }
    }
}

void Server::loadData() {
    QFile file("server_save.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonArray arr = QJsonDocument::fromJson(data).array();
        for (int i = 0; i < arr.size(); ++i) {
            QJsonObject obj = arr[i].toObject();
            PlayerData pd;
            pd.id = obj["id"].toInt();
            pd.x = obj["x"].toInt();
            pd.y = obj["y"].toInt();
            pd.color = obj["color"].toString();
            m_savedData[pd.id] = pd;
        }
        file.close();
    }
}

void Server::saveData() {
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
        m_savedData[it.value().id] = it.value();
    }
    QJsonArray arr;
    for (auto it = m_savedData.begin(); it != m_savedData.end(); ++it) {
        QJsonObject obj;
        obj["id"] = it.value().id; obj["x"] = it.value().x; 
        obj["y"] = it.value().y; obj["color"] = it.value().color;
        arr.append(obj);
    }
    QFile file("server_save.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(arr).toJson());
        file.close();
    }
}