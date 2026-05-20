#include <QCoreApplication>
#include "Server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Server server;
    if (!server.start(12345)) {
        qDebug() << "Не удалось запустить сервер!";
        return 1;
    }
    return a.exec();
}