#include <iostream>
#include <cmath>
#include <limits>
#include <locale>

void setupRussian() { setlocale(LC_ALL, "ru_RU.UTF-8"); }

/* ─────────────────────── ПУНКТ 1 ─────────────────────── */

// для одного числа
int operateNumbers(int a) {
    std::cout << "Используется функция для одного числа (противоположное)\n";
    return -a;
}

// подсчет цифр числа
int count_digits(int n) {
    if (n == 0) return 1;
    int cnt = 0;
    while (n > 0) { n /= 10; ++cnt; }
    return cnt;
}
// для двух чисел
int operateNumbers(int a, int b) {
    std::cout << "Используется функция для двух чисел (склейка)\n";

    int digs = count_digits(b); // вызов функции
    long long pow10 = 1; // для повышенного макс значения, вероятность большого числа
    for (int i = 0; i < digs; ++i) pow10 *= 10;

    long long result = (long long)a * pow10 + b;

// проверка на переполнение
    if (result > std::numeric_limits<int>::max()) {
        std::cout << "Переполнение! Возвращаем INT_MAX.\n";
        return std::numeric_limits<int>::max();
    }
    if (result < std::numeric_limits<int>::min()) {
        std::cout << "Переполнение! Возвращаем INT_MIN.\n";
        return std::numeric_limits<int>::min();
    }
    return int(result);
}

/* ─────────────────────── ПУНКТ 2 ─────────────────────── */
void calculateTriangleArea(int a, int b, int c) {
    std::cout << "Используется void-функция (площадь треугольника по Герону)\n";

    if (a <= 0 || b <= 0 || c <= 0) {
        std::cout << "Ошибка: стороны должны быть > 0\n";
        return;
    }
    if (a + b <= c || a + c <= b || b + c <= a) {
        std::cout << "Ошибка: треугольник невозможен\n";
        return;
    }
    // для вещественных чисел
    double s = (a + b + c) / 2.0;
    double area = std::sqrt(s * (s - a) * (s - b) * (s - c));
    std::cout << "Площадь = " << area << " (double)\n";

    std::cout << "int:           " << sizeof(int) * 8 << " бит, ["
        << std::numeric_limits<int>::min() << " .. "
        << std::numeric_limits<int>::max() << "]\n";
    std::cout << "double:        " << sizeof(double) * 8 << " бит, ["
        << std::numeric_limits<double>::lowest() << " .. "
        << std::numeric_limits<double>::max() << "]\n";
    std::cout << "unsigned short:" << sizeof(unsigned short) * 8 << " бит, ["
        << std::numeric_limits<unsigned short>::min() << " .. "
        << std::numeric_limits<unsigned short>::max() << "]\n";
}

/* ─────────────────────── MAIN ─────────────────────── */
int main() {
    setupRussian();

    int choice;                   

    do {
        std::cout << "М Е Н Ю\n";
        std::cout << "1 – операции с числами\n";
        std::cout << "2 – площадь треугольника\n";
        std::cout << "0 – выход\n";
        std::cout << "\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;

        switch (choice) {         
        case 1: {
        std::cout << "\nВведите 3 целых числа (0 = пропуск, например: 5 0 10):\n";
        int x, y, z;
        
        // 1. ДОБАВЛЕН ВВОД ДАННЫХ
        if (!(std::cin >> x >> y >> z)) {
            std::cout << "Ошибка ввода!\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            break;
        }

        // 2. УПРОЩЕННАЯ ЛОГИКА ОТБОРА (исправляет баг с одинаковыми числами)
        // Собираем все ненулевые числа в массив
        int raw[3] = {x, y, z};
        int valid[3]; 
        int count = 0;

        for (int i = 0; i < 3; ++i) {
            if (raw[i] != 0) {
                valid[count] = raw[i];
                count++;
            }
        }

        // 3. ВЫЗОВ ФУНКЦИЙ
        if (count == 0) {
            std::cout << "Никаких ненулевых чисел не введено.\n";
        }
        else if (count == 1) {
            // Передаем первое найденное число
            std::cout << "Результат: " << operateNumbers(valid[0]) << "\n";
        }
        else if (count == 2) {
            // Передаем первое и второе найденное число
            std::cout << "Результат: " << operateNumbers(valid[0], valid[1]) << "\n";
        }
        else {
            std::cout << "ОШИБКА: введено ТРИ ненулевых числа!\n";
            std::cout << "Функция работает только с 1 или 2 числами.\n";
        }
        break;
    }

        case 2: {
            std::cout << "\nВведите три целые стороны треугольника:\n";
            int a, b, c;
            if (!(std::cin >> a >> b >> c)) {
                std::cout << "Ошибка ввода!\n";
                std::cin.clear(); std::cin.ignore(10000, '\n');
                break;
            }
            calculateTriangleArea(a, b, c);
            break;
        }

        case 0:
            std::cout << "Программа завершена.\n";
            break;

        default:
            std::cout << "Нет такого пункта. Выберите 0, 1 или 2.\n";
        }
    } while (choice != 0);

    return 0;
}