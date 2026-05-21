#include "Economy.hpp"

// --- Методы структуры City ---
void City::print() const {
    cout << name << " (Нас: " << population << ", Бюджет: " << money << ")";
}

// --- Методы класса Budget ---

// 1. Конструктор по умолчанию
Budget::Budget() : amount(0.0), capitalCoef(1.0) {}

// 2. Конструктор с параметрами
Budget::Budget(double amt, double coef) : amount(amt), capitalCoef(coef) {}

// 3. Конструктор копирования
Budget::Budget(const Budget& other) : amount(other.amount), capitalCoef(other.capitalCoef) {}

// Изменение на X денег
void Budget::changeMoney(double x) {
    amount += x;
}

double Budget::getAmount() const { return amount; }
double Budget::getCoef() const { return capitalCoef; }

void Budget::print() const {
    cout << "Бюджет: " << amount << ", Коэф. для столицы: " << capitalCoef << endl;
}