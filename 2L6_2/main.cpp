#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

struct ValidPoint {
    QString name, description;
    int difficulty;
    double coordX, coordY;
};

class ViewerWindow : public QWidget {
private:
    QTableWidget *tableCorrect, *tableErrors;
    QPushButton *btnLoad;

public:
    ViewerWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Вывод данных");
        resize(900, 500);

        tableCorrect = new QTableWidget(this); tableCorrect->setColumnCount(5);
        tableCorrect->setHorizontalHeaderLabels({"Название", "Описание", "Сложн.", "X", "Y"});
        tableCorrect->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        tableErrors = new QTableWidget(this); tableErrors->setColumnCount(5);
        tableErrors->setHorizontalHeaderLabels({"Название", "Описание", "Сложн.", "X", "Y"});
        tableErrors->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        btnLoad = new QPushButton("Загрузить .json", this);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QHBoxLayout *tablesLayout = new QHBoxLayout();
        
        QVBoxLayout *leftLayout = new QVBoxLayout();
        leftLayout->addWidget(new QLabel("Корректные")); leftLayout->addWidget(tableCorrect);

        QVBoxLayout *rightLayout = new QVBoxLayout();
        rightLayout->addWidget(new QLabel("Ошибки")); rightLayout->addWidget(tableErrors);

        tablesLayout->addLayout(leftLayout); tablesLayout->addLayout(rightLayout);
        mainLayout->addLayout(tablesLayout); mainLayout->addWidget(btnLoad, 0, Qt::AlignHCenter);

        connect(btnLoad, &QPushButton::clicked, this, &ViewerWindow::processData);
    }

private slots:
    void processData() {
        QString fileName = QFileDialog::getOpenFileName(this, "Выберите JSON", "", "JSON Files (*.json)");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        QByteArray data;
        while (!file.atEnd()) { data.append(file.read(1024)); }
        file.close();

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
            QMessageBox::warning(this, "Ошибка", "Файл поврежден (невалидный JSON).");
            return; 
        }

        QJsonArray jsonArray = doc.array();
        std::vector<ValidPoint> validItems;
        QJsonArray brokenArrayOut;

        tableCorrect->setRowCount(0);
        tableErrors->setRowCount(0);

        for (int i = 0; i < jsonArray.size(); ++i) {
            QJsonObject obj = jsonArray[i].toObject();
            bool isBroken = false;

            QString name = obj["name"].toString();
            QString desc = obj["description"].toString();
            
            // ПРАВКА 2: Убрали desc.isEmpty(). Пустое описание теперь допустимо!
            if (name.isEmpty() || 
                !obj.contains("difficulty") || !obj["difficulty"].isDouble() ||
                !obj.contains("coordX") || !obj["coordX"].isDouble() ||
                !obj.contains("coordY") || !obj["coordY"].isDouble()) {
                isBroken = true;
            }

            if (isBroken) {
                brokenArrayOut.append(obj); 
                int row = tableErrors->rowCount();
                tableErrors->insertRow(row);
                
                auto setErrItem = [&](int col, const QString& text, bool isBad) {
                    QTableWidgetItem *item = new QTableWidgetItem(text);
                    if (isBad) item->setBackground(QColor(255, 100, 100)); 
                    tableErrors->setItem(row, col, item);
                };

                setErrItem(0, name, name.isEmpty());
                setErrItem(1, desc, false); // Описание больше не красится красным
                setErrItem(2, obj.contains("difficulty") && obj["difficulty"].isDouble() ? QString::number(obj["difficulty"].toInt()) : "ERR", !obj.contains("difficulty") || !obj["difficulty"].isDouble());
                setErrItem(3, obj.contains("coordX") && obj["coordX"].isDouble() ? QString::number(obj["coordX"].toDouble()) : "ERR", !obj.contains("coordX") || !obj["coordX"].isDouble());
                setErrItem(4, obj.contains("coordY") && obj["coordY"].isDouble() ? QString::number(obj["coordY"].toDouble()) : "ERR", !obj.contains("coordY") || !obj["coordY"].isDouble());

            } else {
                validItems.push_back({name, desc, obj["difficulty"].toInt(), obj["coordX"].toDouble(), obj["coordY"].toDouble()});
            }
        }

        std::sort(validItems.begin(), validItems.end(), [](const ValidPoint& a, const ValidPoint& b) {
            return a.name.compare(b.name, Qt::CaseInsensitive) > 0;
        });

        QJsonArray validArrayOut;
        for (const auto& item : validItems) {
            int row = tableCorrect->rowCount();
            tableCorrect->insertRow(row);
            tableCorrect->setItem(row, 0, new QTableWidgetItem(item.name));
            tableCorrect->setItem(row, 1, new QTableWidgetItem(item.description));
            tableCorrect->setItem(row, 2, new QTableWidgetItem(QString::number(item.difficulty)));
            tableCorrect->setItem(row, 3, new QTableWidgetItem(QString::number(item.coordX)));
            tableCorrect->setItem(row, 4, new QTableWidgetItem(QString::number(item.coordY)));

            QJsonObject validObj;
            validObj["name"] = item.name; validObj["description"] = item.description;
            validObj["difficulty"] = item.difficulty; validObj["coordX"] = item.coordX; validObj["coordY"] = item.coordY;
            validArrayOut.append(validObj);
        }

        QFile fCorrect("correct.json");
        if (fCorrect.open(QIODevice::WriteOnly | QIODevice::Text)) {
            fCorrect.write(QJsonDocument(validArrayOut).toJson(QJsonDocument::Indented));
            fCorrect.close();
        }

        QFile fErrors("errors.json");
        if (fErrors.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // ПРАВКА 3: Если ошибок нет, оставляем файл абсолютно пустым
            if (!brokenArrayOut.isEmpty()) {
                fErrors.write(QJsonDocument(brokenArrayOut).toJson(QJsonDocument::Indented));
            }
            fErrors.close();
        }
        
        QMessageBox::information(this, "Готово", "Данные загружены, отсортированы и распределены.");
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ViewerWindow w;
    w.show();
    return a.exec();
}