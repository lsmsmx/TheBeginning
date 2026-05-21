#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class InputWindow : public QWidget {
    Q_OBJECT
private:
    QLineEdit *editName, *editDesc, *editDiff, *editX, *editY;
    QPushButton *btnLoadTxt, *btnOk;

public:
    InputWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Новый объект"); 
        setFixedSize(360, 200); 

        QGridLayout *grid = new QGridLayout(this);
        grid->setSpacing(4); 
        grid->setContentsMargins(10, 10, 10, 10);

        editName = new QLineEdit(this); editName->setFixedWidth(130); 
        editDesc = new QLineEdit(this); editDesc->setFixedWidth(180); 
        editDiff = new QLineEdit(this); editDiff->setFixedWidth(70);  
        editX    = new QLineEdit(this); editX->setFixedWidth(90);     
        editY    = new QLineEdit(this); editY->setFixedWidth(110);    

        QString lineStyle = "QLineEdit { border: 2px solid black; }";
        editName->setStyleSheet(lineStyle); editDesc->setStyleSheet(lineStyle);
        editDiff->setStyleSheet(lineStyle); editX->setStyleSheet(lineStyle);
        editY->setStyleSheet(lineStyle);

        btnLoadTxt = new QPushButton("из .txt", this); btnLoadTxt->setFixedSize(70, 50); 
        btnOk = new QPushButton("Ok", this); btnOk->setFixedSize(70, 50);

        QString btnStyle = "QPushButton { background-color: #f8cecc; border: 2px solid black; border-radius: 8px; font-weight: bold; }";
        btnLoadTxt->setStyleSheet(btnStyle); btnOk->setStyleSheet(btnStyle);
        
        grid->addWidget(new QLabel("Название:"), 0, 0); grid->addWidget(editName, 0, 1, Qt::AlignLeft); 
        grid->addWidget(new QLabel("Описание:"), 1, 0); grid->addWidget(editDesc, 1, 1, Qt::AlignLeft);
        grid->addWidget(btnLoadTxt, 0, 2, 2, 1, Qt::AlignCenter);
        grid->addWidget(new QLabel("Сложность:"), 2, 0); grid->addWidget(editDiff, 2, 1, Qt::AlignLeft);
        grid->addWidget(new QLabel("Коорд X:"), 3, 0); grid->addWidget(editX, 3, 1, Qt::AlignLeft);
        grid->addWidget(new QLabel("Коорд Y:"), 4, 0); grid->addWidget(editY, 4, 1, Qt::AlignLeft);
        grid->addWidget(btnOk, 3, 2, 2, 1, Qt::AlignCenter);

        connect(btnLoadTxt, &QPushButton::clicked, this, &InputWindow::loadFromTxt);
        connect(btnOk, &QPushButton::clicked, this, &InputWindow::saveToJson);
    }

private slots:
    void loadFromTxt() {
        QString fileName = QFileDialog::getOpenFileName(this, "Выберите TXT", "", "Text Files (*.txt)");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString line = file.readLine().trimmed();
            QStringList parts = line.split("/");
            if (parts.size() >= 5) {
                editName->setText(parts[0]); editDesc->setText(parts[1]); editDiff->setText(parts[2]);
                editX->setText(parts[3]); editY->setText(parts[4]);
            } else {
                QMessageBox::warning(this, "Ошибка", "Неверный формат TXT файла.");
            }
            file.close();
        }
    }

    void saveToJson() {
        QJsonObject newObj;
        newObj["name"] = editName->text();
        newObj["description"] = editDesc->text();
        
        bool okDiff, okX, okY;
        int diff = editDiff->text().toInt(&okDiff);
        double x = editX->text().replace(",", ".").toDouble(&okX); 
        double y = editY->text().replace(",", ".").toDouble(&okY);

        // ПРАВКА 1: Теперь если ввели текст вместо числа сложности, сохранится ошибка
        if (!okDiff) newObj["difficulty"] = ""; else newObj["difficulty"] = diff;
        if (!okX) newObj["coordX"] = ""; else newObj["coordX"] = x;
        if (!okY) newObj["coordY"] = ""; else newObj["coordY"] = y;

        QJsonArray jsonArray;
        QFile file("data.json");
        
        // ПРОВЕРКА НА СУЩЕСТВОВАНИЕ ФАЙЛА
        if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            if (doc.isArray()) jsonArray = doc.array();
            file.close();
        }

        jsonArray.append(newObj);

        // ФИЗИЧЕСКОЕ СОЗДАНИЕ/ПЕРЕЗАПИСЬ ФАЙЛА
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(QJsonDocument(jsonArray).toJson(QJsonDocument::Indented));
            file.close();
            QMessageBox::information(this, "Успех", "Объект добавлен в data.json");
            editName->clear(); editDesc->clear(); editDiff->clear(); editX->clear(); editY->clear();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить JSON файл!");
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    InputWindow w;
    w.show();
    return a.exec();
}