#include <iostream>
#include "Country.hpp"
#include "Economy.hpp"
#include <ctime>
#include <cstdlib>

using namespace std;

// Функция для вывода личной оценки (требование из методички)
void printLabReview() {
    cout << "\n=== Личная оценка лабораторной работы ===" << endl;
    cout << "Интерес: 8/10 (Было интересно связывать разные классы через библиотеку)" << endl;
    cout << "Наполненность: 9/10 (Много методов, все перегрузки задействованы)" << endl;
    cout << "Сложность: 6/10 (Самым сложным было перестроить работу со строк на структуры)" << endl;
    cout << "=========================================\n" << endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    srand(time(NULL));

    // Создаем вектора городов (теперь это структуры City)
    vector<City> citiesA;
    citiesA.push_back(City("Minsk", 2000000));
    citiesA.push_back(City("Gomel", 500000));
    citiesA.push_back(City("Brest", 340000));
    
    vector<City> citiesB;
    citiesB.push_back(City("Berlin", 3600000));
    citiesB.push_back(City("Minsk", 150000)); // Дубликат для проверки
    citiesB.push_back(City("Hamburg", 1800000));

    Country belarus("Belarus", "Minsk", "25.08.1991", 207600, 9200000, citiesA);
    Country germany("Germany", "Berlin", "03.10.1990", 357022, 83200000, citiesB);

    cout << "--- Исходные страны ---" << endl;
    belarus.print();
    germany.print();
    cout << endl;

    cout << "--- Тест распределения бюджета ---" << endl;
    // Бюджет: 1 миллион, приоритет столицы = 2.0
    Budget stateBudget(1000000.0, 2.0); 
    stateBudget.print();
    
    belarus.distributeBudget(stateBudget); // Распределяем деньги
    belarus.print();
    cout << "Остаток в гос. казне: " << stateBudget.getAmount() << "\n\n";

    cout << "--- Тест Оператора + ---" << endl;
    Country unionCountry = belarus + germany;
    unionCountry.print();
    cout << endl;

    cout << "--- Тест Оператора * ---" << endl;
    Country pathCountry = belarus * germany;
    pathCountry.print();
    cout << endl;

    // Вывод оценки в самом конце
    printLabReview();

    return 0;
}