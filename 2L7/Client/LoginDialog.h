#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getAddress() const;
    quint16 getPort() const;
    int getID() const;

private slots:
    void onConnectClicked(); // Свой обработчик кнопки Коннект

private:
    QLineEdit* editAddress;
    QLineEdit* editPort;
    QLineEdit* editID;
    QPushButton* btnConnect;
};

#endif // LOGINDIALOG_H