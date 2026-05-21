#ifndef MODELS_H
#define MODELS_H

#include <QString>

// Структура брони
struct Armor {
    int helm;
    int cuirass;
    int boots;
};

// Родительский класс P
class P {
public:
    enum Type { Mage, Enemy }; // Для удобного определения типа потомка
    Type classType;

    QString name;
    int hp;
    Armor armor;

    P(Type t) : classType(t), hp(0) {
        armor = {0, 0, 0};
    }
    virtual ~P() {} // Виртуальный деструктор обязателен для полиморфизма
};

// Наследник 1: Маг
class MagP : public P {
public:
    QString element;
    int mana;

    MagP() : P(Mage), mana(0) {}
};

// Наследник 2: Враг
class VragP : public P {
public:
    QString rarity;
    int damage;

    VragP() : P(Enemy), damage(0) {}
};

#endif // MODELS_H