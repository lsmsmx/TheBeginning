#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <vector>
#include "models.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadButtonClicked();
    void onTableItemDoubleClicked(int row, int column);

private:
    QTableWidget* tableWidget;
    QPushButton* loadButton;
    
    // ВЕКТОР УКАЗАТЕЛЕЙ БАЗОВОГО КЛАССА (по ТЗ)
    std::vector<P*> characterList;

    void setupUI();
    void loadDataFromFile(const QString& filePath);
    void updateTable();
    void clearData();
};

#endif // MAINWINDOW_H