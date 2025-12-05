#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

// Структура ВАГОН
struct Vag {
    int n;          // номер
    string gruz;    // груз
    int w;          // вес (weight)
    Vag* next;      // ссылка на след. вагон
};

// Структура ЛОКОМОТИВ
struct Loco {
    char kod;       // код поезда (A, B, C...)
    Vag* head;      // первый вагон
};

// Функция создания вагона (чтобы код был короче)
Vag* newV(int n, string g, int w) {
    Vag* v = new Vag{ n, g, w, nullptr}; // выделяем память
    return v;
}

// 1. Вывод состава + подсчет веса и количества
void show(Loco& l) { // По ссылке, принимает локомотив и называет его l
    cout << "\n--- Поезд [" << l.kod << "] ---\n";
    if (l.head == NULL) { // Сцепка со след.
        cout << "Поезд пустой.\n";
        return;
    }

    Vag* cur = l.head; // курсор для движения
    int k = 0;         // кол-во вагонов (счетчик)
    int sum = 0;       // общий вес

    while (cur != NULL) { // указывает на не пустоту
        cout << " [№" << cur->n << " | " << cur->gruz << " | " << cur->w << "т] -> "; // Выводим данные вагона
        k++;
        sum += cur->w; // Прибавляем вес текущего вагона к сумме
        cur = cur->next; // Прыжок к след вагону
    }
    cout << "КОНЕЦ\n";
    cout << "Всего вагонов: " << k << ", Общий вес: " << sum << "\n";
}

// Добавление вагона (нужно для меню)
void add(Loco& l) {
    int n, w;
    string g;
    cout << "Номер вагона: "; cin >> n;
    cout << "Название груза: "; cin >> g;
    cout << "Вес груза: "; cin >> w;

    Vag* v = newV(n, g, w); // Создаем новый вагон из памяти

    if (l.head == NULL) { // Если поезд был пуст,
        l.head = v; // то новый вагон становится head
    } else {
        Vag* cur = l.head; // используем курсор
        while (cur->next != NULL) { // идем в конец 
            cur = cur->next; // указываем на последний
        }
        cur->next = v; // цепляем к последнему
    }
    cout << "Вагон прицеплен!\n";
}

// 2. Отцепить последний вагон
void del(Loco& l) {
    if (l.head == NULL) {
        cout << "Нечего отцеплять.\n";
        return;
    }

    // Если вагон всего один
    if (l.head->next == NULL) {
        delete l.head;
        l.head = NULL;
        cout << "Вагон отцеплен (поезд пуст).\n";
        return;
    }

    // Ищем предпоследний
    Vag* cur = l.head;
    while (cur->next->next != NULL) {
        cur = cur->next;
    }
    // Удаляем последний
    delete cur->next;
    cur->next = NULL;
    cout << "Последний вагон отцеплен.\n";
}

int main() {
    setlocale(LC_ALL, "RU_ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Массив из 3 поездов
    Loco tr[3]; 
    tr[0].kod = 'A'; tr[0].head = NULL;
    tr[1].kod = 'B'; tr[1].head = NULL;
    tr[2].kod = 'C'; tr[2].head = NULL;

    // --- РУЧНАЯ СБОРКА (по заданию) ---
    // Поезд А (3 вагона)
    tr[0].head = newV(1, "Уголь", 50);
    tr[0].head->next = newV(2, "Уголь", 50);
    tr[0].head->next->next = newV(3, "Дрова", 30);

    // Поезд B (2 вагона)
    tr[1].head = newV(10, "Нефть", 100);
    tr[1].head->next = newV(11, "Бензин", 80);

    // Поезд C (3 вагона)
    tr[2].head = newV(77, "Зерно", 40);
    tr[2].head->next = newV(78, "Мука", 45);
    tr[2].head->next->next = newV(79, "Сахар", 20);
    // ----------------------------------

    int opt; // выбор меню
    int idx = 0; // индекс поезда

    while (true) {
        cout << "\n=== МЕНЮ ===\n";
        cout << "1. Показать все поезда\n";
        cout << "2. Добавить вагон\n";
        cout << "3. Отцепить последний\n";
        cout << "0. Выход\n";
        cout << "Выбор: ";
        cin >> opt;

        if (opt == 0) break;

        // ВАРИАНТ 1: Просто показать
        if (opt == 1) {
            for (int i = 0; i < 3; i++) show(tr[i]);
        }
        // ВАРИАНТ 2 и 3: Нужен выбор поезда
        else if (opt == 2 || opt == 3) {
            cout << "Выберите поезд (0-A, 1-B, 2-C): ";
            cin >> idx;
            if (idx >= 0 && idx <= 2) {
                // Выполняем действие только если индекс правильный
                if (opt == 2) add(tr[idx]);
                if (opt == 3) del(tr[idx]);
            }
            else {
                cout << "Ошибка! Нет такого номера поезда.\n";
            }
        }
        else {
            cout << "Неверный пункт меню.\n";
        }
    }
    return 0;
}
