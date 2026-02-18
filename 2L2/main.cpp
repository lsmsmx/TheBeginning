#include <iostream>
#include "Country.h"

using namespace std;

int main() {

    // Создаем вектора городов
    vector<string> citiesA = {"Minsk", "Gomel", "Brest"};
    vector<string> citiesB = {"Berlin", "Minsk", "Hamburg"}; // Минск повторяется

    // 1. Создание объектов
    Country belarus("Belarus", "Minsk", 207600, citiesA);
    Country germany("Germany", "Berlin", 357022, citiesB);

    cout << "--- Initial Countries ---" << endl;
    belarus.print();
    germany.print();
    cout << endl;

    // 2. Тест оператора + (Создание союза)
    cout << "--- Test Operator + ---" << endl;
    Country unionCountry = belarus + germany;
    unionCountry.print();
    cout << endl;

    // 3. Тест оператора +=
    cout << "--- Test Operator += ---" << endl;
    Country tempC("Small", "City", 100, {"Village"});
    tempC += belarus;
    tempC.print();
    cout << endl;

    // 4. Тест оператора * (Путь)
    cout << "--- Test Operator * ---" << endl;
    Country pathCountry = belarus * germany;
    pathCountry.print();
    cout << endl;

    // 5. Тест присваивания и копирования (Правило трех)
    cout << "--- Test Rule of Three ---" << endl;
    Country copyTest = belarus; // Copy constructor
    copyTest.setName("Belarus Copy");
    
    Country assignTest;
    assignTest = germany; // Operator =
    assignTest.setName("Germany Assigned");

    copyTest.print();
    assignTest.print();

    return 0;
}