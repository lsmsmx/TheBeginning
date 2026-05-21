#pragma once
#include <iostream>
#include <string>

using namespace std;

// Структура города (по заданию)
struct City {
    string name;
    int population;
    double money; // Доступные деньги

    // Простой конструктор
    City(string n = "Unknown", int pop = 0) : name(n), population(pop), money(0.0) {}

    void print() const;
};

// Класс бюджета (по заданию)
class Budget {
private:
    double amount;      // Количество денег
    double capitalCoef; // Приоритет (коэффициент) для столицы

public:
    // Конструкторы (3 шт., как просили)
    Budget();
    Budget(double amt, double coef);
    Budget(const Budget& other);

    // Метод: Изменение на X денег
    void changeMoney(double x);
    
    // Геттеры
    double getAmount() const;
    double getCoef() const;

    void print() const;
};