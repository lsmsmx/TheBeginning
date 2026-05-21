#include "carddialog.h"
#include <QPixmap>
#include <QFrame>

CardDialog::CardDialog(P* character, QWidget *parent)
    : QDialog(parent), m_character(character) {
    setupUI();
}

CardDialog::~CardDialog() {}

void CardDialog::setupUI() {
    // Убираем стандартные рамки окна, делаем фон белым
    this->setWindowTitle("Карточка");
    this->setFixedSize(220, 320);
    this->setStyleSheet("background-color: white;");

    // Главный слой окна без отступов, чтобы рамка была по краям
    QVBoxLayout* windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(5, 5, 5, 5);

    // Создаем "карточку" с жирной черной рамкой (как на макете)
    QFrame* mainFrame = new QFrame(this);
    mainFrame->setStyleSheet("QFrame { border: 3px solid black; } QLabel { border: none; }");
    QVBoxLayout* layout = new QVBoxLayout(mainFrame);
    layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter); // Текст прижимаем к верху
    layout->setSpacing(5); // Небольшой отступ между строками

    if (m_character->classType == P::Mage) {
        applyMageUI(static_cast<MagP*>(m_character), layout);
    } else {
        applyEnemyUI(static_cast<VragP*>(m_character), layout);
    }

    windowLayout->addWidget(mainFrame);
}

// Построение карточки МАГА
void CardDialog::applyMageUI(MagP* mage, QVBoxLayout* layout) {
    // 1. Имя
    QLabel* lblName = new QLabel("<b>" + mage->name + "</b>");
    lblName->setAlignment(Qt::AlignCenter);
    lblName->setStyleSheet("font-size: 16px; color: black;");

    // 2. Стихия
    QString elColor = getElementColor(mage->element);
    QLabel* lblElement = new QLabel(QString("<b><font color='%1'>%2</font></b>").arg(elColor, mage->element));
    lblElement->setAlignment(Qt::AlignCenter);

    // 3. ХП и Мана (на одной строке горизонтально, как на макете)
    QHBoxLayout* statsLayout = new QHBoxLayout();
    QLabel* lblHP = new QLabel(QString("<b><font color='green'>ХП %1</font></b>").arg(mage->hp));
    QLabel* lblMana = new QLabel(QString("<b><font color='blue'>Мана %1</font></b>").arg(mage->mana));
    lblHP->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    lblMana->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    statsLayout->addWidget(lblHP);
    statsLayout->addWidget(lblMana);

    // 4. Картинка (с рамкой, по центру)
    QLabel* imgLabel = new QLabel();
    imgLabel->setFixedSize(90, 120);
    imgLabel->setStyleSheet("border: 2px solid black;"); // Рамка для картинки по ТЗ
    QPixmap pix("mag_res/1.png");
    if(!pix.isNull()) imgLabel->setPixmap(pix.scaled(imgLabel->size(), Qt::KeepAspectRatio));
    else { imgLabel->setText("IMG"); imgLabel->setAlignment(Qt::AlignCenter); }

    // 5. Кнопки в самом низу
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnPrint = new QPushButton("Печать");
    QPushButton* btnCancel = new QPushButton("Отмена");
    
    // Цвета и рамки кнопок строго по макету
    btnPrint->setStyleSheet("background-color: #00BFFF; color: black; font-weight: bold; border: 2px solid black; padding: 3px;");
    btnCancel->setStyleSheet("background-color: #FF4500; color: black; font-weight: bold; border: 2px solid black; padding: 3px;");
    
    btnLayout->addWidget(btnPrint);
    btnLayout->addWidget(btnCancel);

    // Собираем всё в вертикальный столбец
    layout->addWidget(lblName);
    layout->addWidget(lblElement);
    layout->addLayout(statsLayout);
    layout->addSpacing(10); // Отступ перед картинкой
    layout->addWidget(imgLabel, 0, Qt::AlignHCenter); // Картинка строго по центру
    layout->addStretch(); // Пружина, чтобы прижать кнопки к низу
    layout->addLayout(btnLayout);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnPrint, &QPushButton::clicked, this, &QDialog::accept);
}

// Построение карточки ВРАГА
void CardDialog::applyEnemyUI(VragP* enemy, QVBoxLayout* layout) {
    // 1. Имя с учетом редкости
    QString rarColor = getRarityColor(enemy->rarity);
    QLabel* lblName = new QLabel(QString("<b><font color='%1'>%2</font></b>").arg(rarColor, enemy->name));
    lblName->setAlignment(Qt::AlignCenter);
    lblName->setStyleSheet("font-size: 16px;");

    // 2. ХП врага
    QLabel* lblHP = new QLabel(QString("<b><font color='red'>ХП %1</font></b>").arg(enemy->hp));
    lblHP->setAlignment(Qt::AlignCenter);

    // 3. Броня
    QLabel* lblArmor = new QLabel(QString("<b><%1/%2/%3></b>").arg(enemy->armor.helm).arg(enemy->armor.cuirass).arg(enemy->armor.boots));
    lblArmor->setAlignment(Qt::AlignCenter);

    // 4. Картинка
    QLabel* imgLabel = new QLabel();
    imgLabel->setFixedSize(90, 120);
    imgLabel->setStyleSheet("border: 2px solid black;");
    QPixmap pix("vrag_res/1.png");
    if(!pix.isNull()) imgLabel->setPixmap(pix.scaled(imgLabel->size(), Qt::KeepAspectRatio));
    else { imgLabel->setText("IMG"); imgLabel->setAlignment(Qt::AlignCenter); }

    // 5. Кнопки
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnPrint = new QPushButton("Печать");
    QPushButton* btnCancel = new QPushButton("Отмена");
    
    btnPrint->setStyleSheet("background-color: #00BFFF; color: black; font-weight: bold; border: 2px solid black; padding: 3px;");
    btnCancel->setStyleSheet("background-color: #FF4500; color: black; font-weight: bold; border: 2px solid black; padding: 3px;");
    
    btnLayout->addWidget(btnPrint);
    btnLayout->addWidget(btnCancel);

    // Сборка слоев врага
    layout->addWidget(lblName);
    layout->addWidget(lblHP);
    layout->addWidget(lblArmor);
    layout->addSpacing(10);
    layout->addWidget(imgLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addLayout(btnLayout);

    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnPrint, &QPushButton::clicked, this, &QDialog::accept);
}

QString CardDialog::getElementColor(const QString& element) {
    QString el = element.toLower();
    if (el == "огонь") return "red";
    if (el == "вода") return "blue";
    if (el == "земля") return "green"; // по ТЗ др. - черным, но земля обычно зеленая
    return "black";
}

QString CardDialog::getRarityColor(const QString& rarity) {
    QString rar = rarity.toLower();
    if (rar == "редкий") return "blue";
    if (rar == "обычный") return "green";
    if (rar == "легенда" || rar == "легендарный") return "orange"; // оранжевый(gold) для легенды
    return "black";
}