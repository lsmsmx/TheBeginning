#include <iostream>
#include <string>

using namespace std;

class Country {
private:
    string name;
    string capital;
    string foundationDate;
    double area;
    long population;

public:
    // 1. Конструктор по умолчанию
    Country() {
        name = "Unknown";
        capital = "None";
        foundationDate = "01.01.1900";
        area = 0.0;
        population = 0;
    }

    // 2. Конструктор полного заполнения
    Country(string n, string c, string date, double a, long p) {
        name = n;
        capital = c;
        foundationDate = "01.01.1900"; 
        area = 0;
        population = 0;
        setFoundationDate(date); 
        setArea(a);              
        setPopulation(p);
    }

    // 3. Конструктор копирования (ОБЯЗАТЕЛЬНО ТЕСТИРУЕМ)
    Country(const Country& other) {
        name = other.name + " (Copy)";
        capital = other.capital;
        foundationDate = other.foundationDate;
        area = other.area;
        population = other.population;
        cout << "[LOG] Copy constructor called for: " << name << endl;
    }

    // Деструктор
    ~Country() {
        cout << "Destructor called for country: " << name << endl;
    }

    // --- Геттеры ---
    string getName() const { return name; }
    string getCapital() const { return capital; }
    string getFoundationDate() const { return foundationDate; }
    double getArea() const { return area; } 
    long getPopulation() const { return population; }

    // --- Сеттеры ---
    void setName(string n) { name = n; }
    void setCapital(const string& c) { capital = c; }

    void setFoundationDate(string date) {
        if (date.length() == 10 && date[2] == '.' && date[5] == '.') {
            foundationDate = date;
        } else {
            cout << "Error: Invalid date format!" << endl;
        }
    }

    void setArea(double a) {
        if (a >= 0) {
            area = a;
        } else {
            cout << "Error: Area cannot be negative!" << endl;
        }
    }

    void setPopulation(long p) {
        if (p >= 0) population = p;
        else cout << "Error: Population cannot be negative!" << endl;
    }

    // Вывод всей информации (как в задании)
    void printInfo() const {
        cout << "--- Country Info ---" << endl;
        cout << "Name: " << name << endl;
        cout << "Capital: " << capital << endl;
        cout << "Founded: " << foundationDate << endl;
        cout << "Area: " << area << " sq. km" << endl;
        cout << "Population: " << population << endl;
        cout << "--------------------" << endl;
    }

    // Присоединение территории (как на скриншоте 1)
    void addTerritory(double newArea) {
        if (newArea > 0) {
            area += newArea;
            cout << "Territory added. New area: " << area << endl;
        }
    }

    // Рост населения (как на скриншоте 1)
    void populationGrowth() {
        long growth = (long)(population * 0.05); // допустим, рост 5%
        population += growth;
        cout << "Population grew by " << growth << ". Total: " << population << endl;
    }
};

int main() {
    // 1. Тест конструктора по умолчанию и сеттеров
    cout << "--- TEST 1: Default Constructor & Setters ---" << endl;
    Country country1;
    country1.setName("Atlantis");
    country1.setCapital("None");
    country1.setArea(500);
    country1.printInfo();

    // 2. Тест конструктора со всеми параметрами
    cout << "\n--- TEST 2: Parameterized Constructor ---" << endl;
    Country country2("Russia", "Moscow", "01.01.0862", 17098246, 144100000);
    country2.printInfo();

    // 3. ТЕСТ КОНСТРУКТОРА КОПИРОВАНИЯ (Этого не было в твоем main!)
    cout << "\n--- TEST 3: Copy Constructor ---" << endl;
    Country country3 = country2; 
    country3.printInfo();

    // 4. ТЕСТ МЕТОДОВ (Присоединение и рост)
    cout << "\n--- TEST 4: Methods (Territory & Growth) ---" << endl;
    country2.addTerritory(50000);
    country2.populationGrowth();

    // 5. ТЕСТ ГЕТТЕРОВ И ВАЛИДАЦИИ
    cout << "\n--- TEST 5: Getters & Validation ---" << endl;
    cout << "Getting capital via getter: " << country2.getCapital() << endl;
    
    cout << "Trying to set negative area:" << endl;
    country2.setArea(-100); // Ожидаем ошибку

    cout << "\n--- End of program, destructors will follow ---" << endl;
    return 0;
}