#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Economy.hpp" // Подключаем библиотеку

using namespace std;

class Country {
private:
    // Поля по 6 варианту
    string name;
    string capital;
    string foundationDate;
    double area;
    int totalPopulation;
    vector<City> cities; // Изменено со string на City

    // Твой вспомогательный метод
    void addCitySafe(vector<City>& target, const City& city);

public:
    Country();
    Country(string n, string c, string date, double a, int pop, vector<City> cityList);
    Country(const Country& other); // Конструктор копирования
    ~Country(); // Деструктор

    Country& operator=(const Country& other); // Присваивание

    // Методы из прошлой лабы
    void addTerritory(double extraArea);
    void changePopulation(int delta);

    // Геттеры
    string getName() const;
    string getCapital() const;
    double getArea() const;
    int getPopulation() const;
    vector<City> getCities() const;

    // Сеттеры
    void setName(string n);
    void setCapital(string c);
    void setArea(double a);
    void setCities(const vector<City>& list);

    void print();

    // Новый метод из 3-й лабы: распределение бюджета
    void distributeBudget(Budget& b);

    // Твои перегрузки операторов
    Country operator+(const Country& other) const;
    Country& operator+=(const Country& other);
    Country operator*(const Country& other) const;
};