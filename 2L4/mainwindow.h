#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveClicked();
    void onResetClicked();

private:
    // Поля ввода
    QLineEdit *subjectEdit;
    QLineEdit *teacherEdit;
    QLineEdit *auditoriumEdit;
    QLineEdit *numEdit;

    // Радиокнопки
    QRadioButton *lecRadio;
    QRadioButton *pracRadio;
    QButtonGroup *typeGroup;

    // Чекбоксы
    QCheckBox *check1;
    QCheckBox *check2;
    QCheckBox *check3;

    // Кнопки
    QPushButton *saveBtn;
    QPushButton *resetBtn;

    void setupUI();
};

#endif // MAINWINDOW_H