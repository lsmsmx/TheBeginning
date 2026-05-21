#ifndef CARDDIALOG_H
#define CARDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "models.h"

class CardDialog : public QDialog {
    Q_OBJECT

public:
    explicit CardDialog(P* character, QWidget *parent = nullptr);
    ~CardDialog();

private:
    P* m_character;

    void setupUI();
    void applyMageUI(MagP* mage);
    void applyEnemyUI(VragP* enemy);

    QString getElementColor(const QString& element);
    QString getRarityColor(const QString& rarity);
};

#endif // CARDDIALOG_H