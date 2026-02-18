#include <iostream>
#include <string>

using namespace std;

class Country {
private:
    string name;
    string capital;
    string foundationDate;
    double area;       // Площадь
    long population;   // Население

public:
    // 1. Конструктор по умолчанию
    Country() : name("Unknown"), capital("None"), foundationDate("01.01.1900"), area(0.0), population(0) {}

    // 2. Конструктор полного заполнения (с параметрами)
    Country(string n, string c, string date, double a, long p) {
        name = n;
        capital = c;
        foundationDate = date;
        setArea(a); // Используем сеттер для проверки
        setPopulation(p);
    }

    // 3. Конструктор копирования
    Country(const Country& other) 
        : name(other.name), capital(other.capital), foundationDate(other.foundationDate), 
          area(other.area), population(other.population) {}

    // Деструктор (обязателен вывод сообщения)
    ~Country() {
        cout << "Destructor called for country: " << name << endl;
    }

    //  Геттеры 
    string getName() const { return name; }
    string getCapital() const { return capital; }
    string getFoundationDate() const { return foundationDate; }
    double getArea() const { return area; }
    long getPopulation() const { return population; }

    // --- Сеттеры с проверками ---
    void setName(string n) { name = n; }
    
    // Сеттер столицы
    void setCapital(const string& c) { capital = c; }

    void setFoundationDate(string date) {
        // Проверка: длина должна быть 10 (например "01.01.2000")
        // И символы на позициях 2 и 5 должны быть точками
        if (date.length() == 10 && date[2] == '.' && date[5] == '.') {
            foundationDate = date;
        } else {
            cout << "Ошибка: Неверный формат даты! Используйте ДД.ММ.ГГГГ" << endl;
            // Можно задать значение по умолчанию, чтобы поле не было пустым
            foundationDate = "01.01.1900"; 
        }
    }

    void setArea(double a) {
        if (a >= 0) area = a;
        else cout << "Error: Area cannot be negative!" << endl;
    }

    void setPopulation(long p) {
        if (p >= 0) population = p;
        else cout << "Error: Population cannot be negative!" << endl;
    }


    // Вывод всей информации
    void printInfo() const {
        cout << "--- Country Info ---" << endl;
        cout << "Name: " << name << endl;
        cout << "Capital: " << capital << endl;
        cout << "Founded: " << foundationDate << endl;
        cout << "Area: " << area << " sq. km" << endl;
        cout << "Population: " << population << endl;
        cout << "--------------------" << endl;
    }

    // Присоединение новой территории (параметр есть)
    void addTerritory(double newArea) {
        if (newArea > 0) {
            area += newArea;
            cout << "Territory added. New area: " << area << endl;
        }
    }

    // Рост населения (параметров нет, просто логика роста)
    void populationGrowth() {
        long growth = population * 0.05;
        population += growth;
        cout << "Population grew by " << growth << ". Total: " << population << endl;
    }
};

int main() {

    // Тест конструктора с параметрами
    Country russia("Russia", "Moscow", "862", 17098246, 144100000);
    russia.printInfo();

    // Тест конструктора копирования
    Country russiaCopy = russia;
    russiaCopy.setName("Russia (Copy)"); // Меняем имя, чтобы отличить при удалении
    
    // Тест конструктора по умолчанию
    Country emptyCountry;
    emptyCountry.setName("Atlantis");
    emptyCountry.setArea(500);
    emptyCountry.printInfo();

    // Проверка методов
    russia.addTerritory(50000); // Присоединение территории
    russia.populationGrowth();  // Рост населения

    // Проверка валидации
    russia.setArea(-100); // Ошибка

    
    return 0;
}