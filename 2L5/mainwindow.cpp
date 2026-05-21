#include "mainwindow.h"
#include "carddialog.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

MainWindow::~MainWindow() {
    clearData(); // Очищаем память при закрытии
}

void MainWindow::setupUI() {
    this->setWindowTitle("Вариант 2 НПС");
    this->resize(450, 300);
    this->setStyleSheet("background-color: white;"); // Общий белый фон

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Таблица данных с жирной рамкой как на макете
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(4);
    // Скрываем заголовки, чтобы было похоже на простой список из ТЗ, либо оставляем для удобства. 
    // На скриншоте заголовков нет, просто данные. Отключим их:
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);
    
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setShowGrid(false); // Убираем внутреннюю сетку, чтобы было как текст на картинке
    tableWidget->setStyleSheet("QTableWidget { border: 3px solid black; font-weight: bold; font-size: 14px; }");

    // Кнопка Загрузить (выровнена по центру, голубой фон, черная рамка)
    loadButton = new QPushButton("Загрузить", this);
    loadButton->setFixedSize(120, 30);
    loadButton->setStyleSheet("background-color: #ADD8E6; color: black; font-weight: bold; border: 2px solid black;");

    layout->addWidget(tableWidget);
    layout->addWidget(loadButton, 0, Qt::AlignHCenter); // Строго по центру горизонтали

    this->setCentralWidget(centralWidget);

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableItemDoubleClicked);
}

void MainWindow::onLoadButtonClicked() {
    // Выбор файла через диалоговое окно (из раздела "Доп")
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        loadDataFromFile(filePath);
    }
}

void MainWindow::loadDataFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    clearData(); // Очищаем старые данные
    QTextStream in(&file);
    in.setCodec("UTF-8"); // Убедитесь, что файл в кодировке UTF-8

    // Читаем по две строки (строгий формат ТЗ)
    while (!in.atEnd()) {
        QString line1 = in.readLine();
        if (line1.isEmpty()) continue;
        QString line2 = in.readLine();

        // Парсинг 1 строки: число(код), строка(И), строка(С/Р), число(М/У)
        QStringList parts1 = line1.split(',');
        // Парсинг 2 строки: число(ХП),число(Ш),число(К),число(С)
        QStringList parts2 = line2.split(',');

        if (parts1.size() >= 4 && parts2.size() >= 4) {
            int code = parts1[0].trimmed().toInt();
            QString name = parts1[1].trimmed();
            QString strParam = parts1[2].trimmed();
            int numParam = parts1[3].trimmed().toInt();

            int hp = parts2[0].trimmed().toInt();
            int helm = parts2[1].trimmed().toInt();
            int cuirass = parts2[2].trimmed().toInt();
            int boots = parts2[3].trimmed().toInt();

            if (code == 1) { // Маг
                MagP* mag = new MagP();
                mag->name = name;
                mag->element = strParam;
                mag->mana = numParam;
                mag->hp = hp;
                mag->armor = {helm, cuirass, boots};
                characterList.push_back(mag);
            } 
            else if (code == 2) { // Враг
                VragP* vrag = new VragP();
                vrag->name = name;
                vrag->rarity = strParam;
                vrag->damage = numParam;
                vrag->hp = hp;
                vrag->armor = {helm, cuirass, boots};
                characterList.push_back(vrag);
            }
        }
    }
    file.close();
    updateTable();
}

void MainWindow::updateTable() {
    tableWidget->setRowCount(0); // Очистка таблицы

    for (size_t i = 0; i < characterList.size(); ++i) {
        tableWidget->insertRow(i);
        P* item = characterList[i];

        QString armorStr = QString("<%1/%2/%3>").arg(item->armor.helm).arg(item->armor.cuirass).arg(item->armor.boots);

        if (item->classType == P::Mage) {
            MagP* mag = static_cast<MagP*>(item);
            tableWidget->setItem(i, 0, new QTableWidgetItem(mag->name));
            tableWidget->setItem(i, 1, new QTableWidgetItem(mag->element));
            tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(mag->hp) + " " + armorStr));
            tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(mag->mana)));
        } else {
            VragP* vrag = static_cast<VragP*>(item);
            tableWidget->setItem(i, 0, new QTableWidgetItem(vrag->name));
            tableWidget->setItem(i, 1, new QTableWidgetItem(vrag->rarity));
            tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(vrag->hp) + " " + armorStr));
            tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(vrag->damage)));
        }
    }
}

void MainWindow::onTableItemDoubleClicked(int row, int column) {
    if (row < 0 || row >= characterList.size()) return;

    P* selectedCharacter = characterList[row];
    CardDialog dialog(selectedCharacter, this);

    // Если в диалоге нажали "Печать" (dialog.accept())
    if (dialog.exec() == QDialog::Accepted) {
        // Удаляем из памяти
        delete characterList[row];
        // Удаляем из вектора
        characterList.erase(characterList.begin() + row);
        // Обновляем таблицу
        updateTable();
    }
    // Если "Отмена" (dialog.reject()), ничего не делаем, окно просто закроется
}

void MainWindow::clearData() {
    for (P* p : characterList) {
        delete p;
    }
    characterList.clear();
}