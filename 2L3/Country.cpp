#include "Country.hpp"
#include <cstdlib> 

// Твоя логика добавления города с проверкой дубликатов
void Country::addCitySafe(vector<City>& target, const City& city) {
    bool exists = false; 

    for (size_t i = 0; i < target.size(); ++i) {
        if (target[i].name == city.name) { 
            exists = true; 
        }
    }

    City newCity = city; // Копируем город, чтобы можно было изменить имя
    if (exists) {
        newCity.name = newCity.name + " новый"; 
        target.push_back(newCity);
    } else {
        target.push_back(newCity);          
    }
}

// --- Конструкторы ---
Country::Country() : name("Unnamed"), capital("-"), foundationDate("01.01.1900"), area(0.0), totalPopulation(0) {}

Country::Country(string n, string c, string date, double a, int pop, vector<City> cityList) 
    : name(n), capital(c), foundationDate(date), area(a), totalPopulation(pop), cities(cityList) {}

// Конструктор копирования (твоя реализация)
Country::Country(const Country& other) 
    : name(other.name), capital(other.capital), foundationDate(other.foundationDate), 
      area(other.area), totalPopulation(other.totalPopulation), cities(other.cities) {}

// Деструктор (твоя реализация)
Country::~Country() {
    cities.clear(); 
}

// Оператор присваивания (твоя реализация)
Country& Country::operator=(const Country& other) {
    if (this == &other) return *this; 

    name = other.name;
    capital = other.capital;
    foundationDate = other.foundationDate;
    area = other.area;
    totalPopulation = other.totalPopulation;
    cities = other.cities; 

    return *this;
}

// --- Методы из Лабы 2 ---
void Country::addTerritory(double extraArea) {
    if (extraArea > 0) area += extraArea;
}

void Country::changePopulation(int delta) {
    totalPopulation += delta;
    if (totalPopulation < 0) totalPopulation = 0; 
}

// --- Геттеры и Сеттеры ---
string Country::getName() const { return name; }
string Country::getCapital() const { return capital; }
double Country::getArea() const { return area; }
int Country::getPopulation() const { return totalPopulation; }
vector<City> Country::getCities() const { return cities; }

void Country::setName(string n) { name = n; }
void Country::setCapital(string c) { capital = c; }
void Country::setArea(double a) { if(a >= 0) area = a; }
void Country::setCities(const vector<City>& list) { cities = list; }

// --- Распределение бюджета (Новая логика по заданию) ---
void Country::distributeBudget(Budget& b) {
    if (cities.empty() || b.getAmount() <= 0) return;

    double totalEffectivePop = 0;
    
    // 1. Считаем общее население (столицу умножаем на коэффициент)
    for (size_t i = 0; i < cities.size(); ++i) {
        if (cities[i].name == capital) {
            totalEffectivePop += cities[i].population * b.getCoef();
        } else {
            totalEffectivePop += cities[i].population;
        }
    }

    if (totalEffectivePop == 0) return;

    double moneyToDistribute = b.getAmount();
    
    // 2. Раздаем деньги городам пропорционально
    for (size_t i = 0; i < cities.size(); ++i) {
        double currentEffectivePop = cities[i].population;
        if (cities[i].name == capital) {
            currentEffectivePop = cities[i].population * b.getCoef();
        }
        
        // Высчитываем долю города
        double share = moneyToDistribute * (currentEffectivePop / totalEffectivePop);
        cities[i].money += share;
    }
    
    // 3. Отнимаем деньги из бюджета
    b.changeMoney(-moneyToDistribute);
}

// --- Вывод ---
void Country::print() {
    cout << "[" << name << ", Столица: " << capital << ", Площадь: " << area << ", Нас-е: " << totalPopulation << "]" << endl;
    cout << "Города: ";
    for (size_t i = 0; i < cities.size(); ++i) {
        cities[i].print();
        if (i < cities.size() - 1) cout << ", ";
    }
    cout << endl;
}

// --- Перегрузка операторов (ТВОЯ ЛОГИКА) ---

// Оператор СЛОЖЕНИЯ (+)
Country Country::operator+(const Country& other) const {
    string newName = this->name + "-" + other.name;
    string newCapital;
    if (this->area > other.area) newCapital = this->capital;
    else newCapital = other.capital;
    
    string newDate = this->foundationDate;
    double newArea = this->area + other.area;
    int newPop = this->totalPopulation + other.totalPopulation;

    vector<City> newCities = this->cities; 
    
    for (const auto& city : other.cities) {
        bool exists = false;
        for (const auto& nc : newCities) {
            if (nc.name == city.name) { 
                exists = true; 
                break; 
            }
        }
        
        City tempCity = city;
        if (exists) tempCity.name = tempCity.name + " новый";
        newCities.push_back(tempCity);
    }

    return Country(newName, newCapital, newDate, newArea, newPop, newCities);
}

// Оператор СЛОЖЕНИЯ С ПРИСВАИВАНИЕМ (+=)
Country& Country::operator+=(const Country& other) {
    this->name = this->name + "-" + other.name;
    
    if (other.area >= this->area) {
        this->capital = other.capital;
    }

    this->area += other.area;
    this->totalPopulation += other.totalPopulation;

    for (const auto& city : other.cities) {
        this->addCitySafe(this->cities, city);
    }

    return *this;
}

// Оператор УМНОЖЕНИЯ (*)
Country Country::operator*(const Country& other) const {
    string newName = "путь " + this->name + "->" + other.name;
    string newCapital = "нет";
    string newDate = "-";
    // Возвращаем твой тернарный оператор для проверки площади
    double newArea = (this->area < other.area) ? this->area : other.area;
    int newPop = 0; 

    vector<City> newCities;
    
    // Ищем столицу первой страны
    City c1(this->capital, 0);
    for(const auto& c : this->cities) {
        if(c.name == this->capital) { c1 = c; break; }
    }
    
    // Ищем столицу второй страны
    City c2(other.capital, 0);
    for(const auto& c : other.cities) {
        if(c.name == other.capital) { c2 = c; break; }
    }

    newCities.push_back(c1);
    
    if (c2.name == c1.name) c2.name = c2.name + " новый";
    newCities.push_back(c2);

    // Твоя логика случайного города
    if (!this->cities.empty()) {
        int randomIndex = rand() % this->cities.size();
        City c = this->cities[randomIndex]; 
        
        bool exists = (c.name == newCities[0].name || c.name == newCities[1].name);
        if (exists) c.name = c.name + " новый";
        newCities.push_back(c);
    }

    if (!other.cities.empty()) {
        int randomIndex = rand() % other.cities.size();
        City c = other.cities[randomIndex];
        
        bool exists = false;
        for(const auto& existing : newCities) {
            if(existing.name == c.name) { exists = true; break; }
        }
        
        if (exists) c.name = c.name + " новый";
        newCities.push_back(c);
    }

    return Country(newName, newCapital, newDate, newArea, newPop, newCities);
}