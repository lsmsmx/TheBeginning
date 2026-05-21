#include "mainwindow.h"
#include "Lesson.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Пара - Вариант 6");
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *cw = new QWidget(this);
    cw->setStyleSheet("background-color: #f0f0f0;"); // Светло-серый фон как в Windows
    setCentralWidget(cw);

    QVBoxLayout *mainLayout = new QVBoxLayout(cw);
    mainLayout->setAlignment(Qt::AlignTop); // Прижимаем всё наверх, чтобы не было дыр
    mainLayout->setContentsMargins(10, 20, 10, 20); // Аккуратные отступы окна

    // === БЛОК ФОРМЫ ===
    QHBoxLayout *centerFormWrapper = new QHBoxLayout();
    centerFormWrapper->addStretch(); // Пружина слева

    QVBoxLayout *formBlock = new QVBoxLayout(); 
    formBlock->setSpacing(8); // Плотное расположение строк

    // Функция: текст слева, поле справа
    auto addRow = [&](const QString &text, QWidget *widget) {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *lbl = new QLabel(text, this);
        row->addWidget(lbl);
        row->addWidget(widget);
        row->addStretch(); 
        formBlock->addLayout(row);
    };

    // Размеры полей (в точности как на фото)
    subjectEdit = new QLineEdit(this); subjectEdit->setFixedWidth(160);
    teacherEdit = new QLineEdit(this); teacherEdit->setFixedWidth(180);
    auditoriumEdit = new QLineEdit(this); auditoriumEdit->setFixedWidth(60);
    numEdit = new QLineEdit(this); numEdit->setFixedWidth(60);

    addRow("Предмет", subjectEdit);
    addRow("Преподаватель", teacherEdit);
    addRow("Аудитория", auditoriumEdit);
    addRow("Номер в расписании", numEdit);

    // Тип (Радиокнопки)
    QHBoxLayout *typeRow = new QHBoxLayout();
    typeRow->addWidget(new QLabel("Тип", this));
    lecRadio = new QRadioButton("Лекция", this); pracRadio = new QRadioButton("Практика", this);
    typeGroup = new QButtonGroup(this); typeGroup->addButton(lecRadio); typeGroup->addButton(pracRadio);
    typeRow->addWidget(lecRadio);
    typeRow->addWidget(pracRadio);
    typeRow->addStretch();
    formBlock->addLayout(typeRow);

    // Ожидание (Чекбоксы столбиком)
    QHBoxLayout *checkRow = new QHBoxLayout();
    QLabel *expLbl = new QLabel("Ожидание", this);
    expLbl->setAlignment(Qt::AlignTop);
    checkRow->addWidget(expLbl);

    QVBoxLayout *checkCol = new QVBoxLayout();
    checkCol->setSpacing(2); 
    check1 = new QCheckBox("Контрольная", this); 
    check2 = new QCheckBox("Проверка посещений", this); 
    check3 = new QCheckBox("Отмена", this);
    checkCol->addWidget(check1); checkCol->addWidget(check2); checkCol->addWidget(check3);
    
    checkRow->addLayout(checkCol);
    checkRow->addStretch();
    formBlock->addLayout(checkRow);

    centerFormWrapper->addLayout(formBlock);
    centerFormWrapper->addStretch(); // Пружина справа

    mainLayout->addLayout(centerFormWrapper);
    
    // Небольшой отступ до кнопок
    mainLayout->addSpacing(15); 

    // === КНОПКИ (ШИРЕ РАССТАВЛЕНЫ, ОДНА ЧУТЬ ВЫШЕ ДРУГОЙ) ===
    QHBoxLayout *btnRow = new QHBoxLayout();
    btnRow->addSpacing(50); // Отступ от левого края окна

    // Сброс (сдвиг вверх на 4 пикселя)
    QVBoxLayout *resetCol = new QVBoxLayout();
    resetBtn = new QPushButton("Сброс", this); 
    resetBtn->setFixedSize(100, 32); 
    resetBtn->setStyleSheet("background-color: #e57373; color: black; border: 1px solid #aaa;");
    resetCol->addWidget(resetBtn);
    resetCol->addSpacing(4); // Выталкивает Сброс ВВЕРХ

    // Сохранить (сдвиг вниз на 4 пикселя)
    QVBoxLayout *saveCol = new QVBoxLayout();
    saveBtn = new QPushButton("Сохранить", this); 
    saveBtn->setFixedSize(100, 32); 
    saveBtn->setStyleSheet("background-color: #81c784; color: black; border: 1px solid #aaa;");
    saveCol->addSpacing(4); // Вдавливает Сохранить ВНИЗ
    saveCol->addWidget(saveBtn);

    btnRow->addLayout(resetCol);
    btnRow->addStretch(); // ОГРОМНАЯ ПРУЖИНА МЕЖДУ КНОПКАМИ (Расталкивает их)
    btnRow->addLayout(saveCol);
    btnRow->addSpacing(50); // Отступ от правого края окна

    mainLayout->addLayout(btnRow);

    // ДЕЛАЕМ ОКНО КОМПАКТНЫМ ПО ВЫСОТЕ
    setFixedSize(400, 340);

    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);
}

void MainWindow::onSaveClicked() {
    QString sub = subjectEdit->text().trimmed();
    QString tea = teacherEdit->text().trimmed();
    QString aud = auditoriumEdit->text().trimmed();
    QString num = numEdit->text().trimmed();

    QRegularExpression r1("^[А-ЯЁ][а-яё]+(\\s[а-яё]+)*$"); 
    QRegularExpression r2("^[А-ЯЁ][а-яё]+\\s[А-ЯЁ]{1,2}$"); 
    QRegularExpression r3("^[1-4](0[1-9]|[1-3][0-9]|40)[АA]?$"); 
    QRegularExpression r4("^[1-8]$"); 

    if (!r1.match(sub).hasMatch()) { QMessageBox::warning(this, "Ошибка", "Предмет: Русские, первая заглавная."); return; }
    if (!r2.match(tea).hasMatch()) { QMessageBox::warning(this, "Ошибка", "Преподаватель: Фамилия ИИ."); return; }
    if (!r3.match(aud).hasMatch()) { QMessageBox::warning(this, "Ошибка", "Аудитория: от 101 до 440 (возможна А)."); return; }
    if (!r4.match(num).hasMatch()) { QMessageBox::warning(this, "Ошибка", "Номер пары: 1-8."); return; }

    QString type = lecRadio->isChecked() ? "Лекция" : (pracRadio->isChecked() ? "Практика" : "");
    if (type.isEmpty()) { QMessageBox::warning(this, "Ошибка", "Выберите тип."); return; }

    QStringList ex;
    if (check1->isChecked()) ex << "Контрольная";
    if (check2->isChecked()) ex << "Проверка посещений";
    if (check3->isChecked()) ex << "Отмена";

    Lesson lessonObj(sub, tea, aud, num, type, ex);
    if (lessonObj.saveToFile()) {
        QMessageBox::information(this, "Успех", "Сохранено в result.txt!");
    }
}

void MainWindow::onResetClicked() {
    subjectEdit->clear(); 
    teacherEdit->clear(); 
    auditoriumEdit->clear(); 
    numEdit->clear();
    
    check1->setChecked(false); 
    check2->setChecked(false); 
    check3->setChecked(false);
    
    typeGroup->setExclusive(false);
    lecRadio->setChecked(false); 
    pracRadio->setChecked(false);
    typeGroup->setExclusive(true);
}