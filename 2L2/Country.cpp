#include "Country.h"

void Country::addCitySafe(vector<string>& target, const string& city) {
    bool exists = false; // Флаг: нашли ли мы такой же город?

    for (size_t i = 0; i < target.size(); ++i) {
        if (target[i] == city) { // Если элемент в векторе совпал с новым городом
            exists = true;    
        }
    }

    if (exists) {
        target.push_back(city + " новый"); // Если нашли дубликат — добавляем с пометкой
    } else {
        target.push_back(city);          // Если не нашли — просто добавляем
    }
}

// --- Конструкторы ---

Country::Country() : name("Unnamed"), capital("-"), area(0.0) {}

Country::Country(string n, string c, double a, vector<string> cityList) 
    : name(n), capital(c), area(a), cities(cityList) {}

// Конструктор копирования
Country::Country(const Country& other) 
    : name(other.name), capital(other.capital), area(other.area), cities(other.cities) {
}

// --- Деструктор ---
Country::~Country() {
    cities.clear(); 
    cout << "Destructor: Memory cleared for " << name << endl;
}

// --- Оператор присваивания ---
Country& Country::operator=(const Country& other) {
    if (this == &other) return *this; // Защита от самоприсваивания

    name = other.name;
    capital = other.capital;
    area = other.area;
    cities = other.cities; // Глубокое копирование вектора

    return *this;
}

// --- Геттеры и Сеттеры ---
string Country::getName() const { return name; }
string Country::getCapital() const { return capital; }
double Country::getArea() const { return area; }
vector<string> Country::getCities() const { return cities; }

void Country::setName(string n) { name = n; }
void Country::setCapital(string c) { capital = c; }
void Country::setArea(double a) { if(a >= 0) area = a; }
void Country::setCities(const vector<string>& list) { cities = list; }

// --- Вывод ---
void Country::print() {
    cout << "[" << name << ", Cap: " << capital << ", Area: " << area << ", Cities: (";
    for (size_t i = 0; i < cities.size(); ++i) {
        cout << cities[i] << (i < cities.size() - 1 ? ", " : "");
    }
    cout << ")]" << endl;
}

// --- Перегрузка операторов ---

// --- Оператор СЛОЖЕНИЯ (+) ---
// Название1-Название2, Столица большей страны, Сумма площадей
Country Country::operator+(const Country& other) const {
    // 1. Формируем новое название через дефис
    string newName = this->name + "-" + other.name;
    
    // 2. Выбираем столицу той страны, у которой площадь (area) больше
    string newCapital;
    if (this->area > other.area) newCapital = this->capital;
    else newCapital = other.capital;

    // 3. Складываем площади обеих стран
    double newArea = this->area + other.area;

    // 4. Объединяем векторы городов
    vector<string> newCities = this->cities; // Сначала копируем все города текущей страны
    
    // Перебираем города второй страны (other)
    for (const auto& city : other.cities) {
        bool exists = false;
        // Вложенный цикл: ищем, нет ли такого города уже в нашем новом списке
        for (const auto& nc : newCities) {
            if (nc == city) { 
                exists = true; 
                break; 
            }
        }
        // Если город уже был — добавляем с пометкой " новый", если нет — просто имя
        if (exists) newCities.push_back(city + " новый");
        else newCities.push_back(city);
    }

    // Возвращаем созданный объект
    return Country(newName, newCapital, newArea, newCities);
}

// --- Оператор СЛОЖЕНИЯ С ПРИСВАИВАНИЕМ (+=) ---
Country& Country::operator+=(const Country& other) {
    // Используем уже готовую логику оператора + и оператора присваивания (=) Это избавляет от дублирования кода
    *this = *this + other; 
    return *this; // Возвращаем ссылку на самого себя
}

// --- Оператор УМНОЖЕНИЯ (*) ---
// По заданию: "путь Имя1->Имя2", Столица "нет", Минимальная площадь
Country Country::operator*(const Country& other) const {
    // 1. Формируем название-путь по шаблону из таблицы
    string newName = "путь " + this->name + "->" + other.name;
    // 2. Столица по условию варианта всегда "нет"
    string newCapital = "нет";
    // 3. Выбираем меньшую площадь из двух объектов (тернарный оператор)
    double newArea = (this->area < other.area) ? this->area : other.area;

    // 4. Формируем новый список городов (две столицы + по одному городу)
    vector<string> newCities;
    
    // Добавляем столицу первой страны
    newCities.push_back(this->capital);
    
    // Добавляем столицу второй страны (с проверкой на дубликат)
    if (other.capital == this->capital) newCities.push_back(other.capital + " новый");
    else newCities.push_back(other.capital);

    // Добавляем по одному первому городу из каждой страны (если списки не пусты)
    if (!this->cities.empty()) {
        string c = this->cities[0];
        // Проверяем на совпадение с уже добавленными столицами (newCities[0] и [1])
        bool exists = (c == newCities[0] || c == newCities[1]);
        if (exists) newCities.push_back(c + " новый");
        else newCities.push_back(c);
    }

    if (!other.cities.empty()) {
        string c = other.cities[0];
        // Полная проверка на дубликаты по всему текущему вектору newCities
        bool exists = false;
        for(const auto& existing : newCities) {
            if(existing == c) { exists = true; break; }
        }
        
        if (exists) newCities.push_back(c + " новый");
        else newCities.push_back(c);
    }

    return Country(newName, newCapital, newArea, newCities);
}