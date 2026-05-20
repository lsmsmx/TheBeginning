#include "LoginDialog.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Подключение");
    setFixedSize(250, 160);

    editAddress = new QLineEdit("127.0.0.1", this);
    editAddress->setFixedWidth(130);

    editPort = new QLineEdit("12345", this);
    editPort->setFixedWidth(80);

    editID = new QLineEdit(this);
    editID->setFixedWidth(60);
    // УБРАЛИ QIntValidator, чтобы окно не "молчало", а явно выдавало ошибку

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(new QLabel("Адрес", this), 0, 0);
    grid->addWidget(editAddress, 0, 1);
    
    grid->addWidget(new QLabel("Порт", this), 1, 0);
    grid->addWidget(editPort, 1, 1);
    
    grid->addWidget(new QLabel("Мой ID", this), 2, 0);
    grid->addWidget(editID, 2, 1);

    grid->setAlignment(editAddress, Qt::AlignLeft);
    grid->setAlignment(editPort, Qt::AlignLeft);
    grid->setAlignment(editID, Qt::AlignLeft);

    btnConnect = new QPushButton("Коннект", this);
    // Теперь кнопка не закрывает окно сразу, а идет в нашу проверку
    connect(btnConnect, &QPushButton::clicked, this, &LoginDialog::onConnectClicked);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(grid);
    mainLayout->addWidget(btnConnect, 0, Qt::AlignHCenter);

    setStyleSheet("QDialog { border: 2px solid gray; }"); 
}

void LoginDialog::onConnectClicked() {
    bool isNumber;
    int id = editID->text().toInt(&isNumber);

    // ВОТ ТУТ ЖЕСТКАЯ ПРОВЕРКА: Если не число, меньше 0 или больше 1000
    if (!isNumber || id < 0 || id > 1000) {
        QMessageBox::warning(this, "Ошибка ввода", "ID должен быть числом строго от 0 до 1000!");
        return; // Прерываем процесс, окно логина остаётся открытым!
    }

    if (editAddress->text().isEmpty() || editPort->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода", "Поля Адрес и Порт не могут быть пустыми!");
        return;
    }

    // Если всё идеально, закрываем диалог и идём дальше
    accept();
}

QString LoginDialog::getAddress() const { return editAddress->text(); }
quint16 LoginDialog::getPort() const { return editPort->text().toUShort(); }
int LoginDialog::getID() const { return editID->text().toInt(); }