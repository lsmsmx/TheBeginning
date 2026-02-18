#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Country {
private:
    string name;
    string capital;
    double area;
    vector<string> cities; // Динамическое поле (через вектор)

    // Вспомогательный метод для добавления города с проверкой дубликатов
    void addCitySafe(vector<string>& target, const string& city);

public:
    // Конструкторы
    Country();
    Country(string n, string c, double a, vector<string> cityList);
    Country(const Country& other); // Конструктор копирования

    // Деструктор
    ~Country();

    // Оператор присваивания (часть правила трех)
    Country& operator=(const Country& other);

    // Геттеры
    string getName() const;
    string getCapital() const;
    double getArea() const;
    vector<string> getCities() const;

    // Сеттеры
    void setName(string n);
    void setCapital(string c);
    void setArea(double a);
    void setCities(const vector<string>& list);

    // Вывод
    void print();

    // Перегрузка операторов (Вариант 6)
    
    // Сложение: Новая страна
    Country operator+(const Country& other) const;
    
    // Сложение с присваиванием
    Country& operator+=(const Country& other);

    // Умножение: Специфическая логика ("Путь")
    Country operator*(const Country& other) const;
};