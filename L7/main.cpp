#include <iostream>
#include <vector>
#include <array>
#include <ctime> // Для time(0)
#include <cstdlib> // Для rand(), srand() - генерация чисел и их настройка

using namespace std;

// Помощник для вывода массива (чтобы код был чище)
void printArr(array<int, 10> arr) {
    cout << "[ ";
    for (int x : arr) cout << x << " ";
    cout << "]" << endl;
}

// 1. По значению (Копия)
void sortValAsc(array<int, 10> arr) {
    for (int i = 0; i < 10; i++) 
        for (int j = 0; j < 9 - i; j++) 
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
    
    cout << "   ВНУТРИ функции (Копия): "; printArr(arr);
}

// 2. По ссылке (Оригинал)
void sortRefAsc(array<int, 10> &arr) {
    for (int i = 0; i < 10; i++) 
        for (int j = 0; j < 9 - i; j++) 
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);

    cout << "   ВНУТРИ функции (Оригинал): "; printArr(arr);
}

// 3. По указателю (Адрес)
void sortPtrAsc(array<int, 10> *arr) {
    for (int i = 0; i < 10; i++) 
        for (int j = 0; j < 9 - i; j++) 
            if ((*arr)[j] > (*arr)[j + 1]) swap((*arr)[j], (*arr)[j + 1]);

    cout << "   ВНУТРИ функции (Оригинал): "; printArr(*arr);
}

// 4. По значению (Копия) - Убывание
void sortValDesc(array<int, 10> arr) {
    for (int i = 0; i < 10; i++) 
        for (int j = 0; j < 9 - i; j++) 
            if (arr[j] < arr[j + 1]) swap(arr[j], arr[j + 1]); // Знак <
    
    cout << "   ВНУТРИ функции (Копия): "; printArr(arr);
}

// 5. По ссылке (Оригинал) - Убывание
void sortRefDesc(array<int, 10> &arr) {
    for (int i = 0; i < 10; i++) 
        for (int j = 0; j < 9 - i; j++) 
            if (arr[j] < arr[j + 1]) swap(arr[j], arr[j + 1]); // Знак <

    cout << "   ВНУТРИ функции (Оригинал): "; printArr(arr);
}

// 6. По указателю (Адрес) - Убывание
void sortPtrDesc(array<int, 10> *arr) {
    for (int i = 0; i < 10; i++) 
        for (int j = 0; j < 9 - i; j++) 
            if ((*arr)[j] < (*arr)[j + 1]) swap((*arr)[j], (*arr)[j + 1]); // Знак <

    cout << "   ВНУТРИ функции (Оригинал): "; printArr(*arr);
}


int main() {
    setlocale(LC_ALL, "RU_ru"); 
    srand((unsigned int)time(0)); // Чтобы числа были разными при каждом запуске

    int choice;
    cout << "1. Пункт 1 (Вектор)\n";
    cout << "2. Пункт 2 (Массив: 6 функций)\n";
    cout << "Выбор: ";
    cin >> choice;

    if (choice == 1) {
        vector<int> v;
        int opt, x;
        while (true) { // Консольное меню
            cout << "\n0.Выход\n1.Просмотр\n2.В начало\n3.В конец\n4.Очистка\n5.Поиск\n6.Задание\nВвод: ";
            cin >> opt;
            if (opt == 0) break;
            if (opt == 1) { cout << "[ "; for (int val : v) cout << val << " "; cout << "]\n"; }  // Печатаем каждый элемет проходя по масииву циклом
            else if (opt == 2) { cout << "Число: "; cin >> x; v.insert(v.begin(), x); }// Добавляем в начало
            else if (opt == 3) { cout << "Число: "; cin >> x; v.push_back(x); } // Добавляем в конец
            else if (opt == 4) { v.clear(); cout << "Очищено.\n"; }
            else if (opt == 5) {
                cout << "Число: "; cin >> x; cout << "Индексы: ";
                // Если число есть то флаг меняем на true и выводим его индекс
                bool f = false; for(int i=0; i<(int)v.size(); i++) if(v[i]==x) {cout << i << ","; f = true; }
                if(!f) cout<<"[]"; cout<<endl;
            }
            else if (opt == 6) {
                cout << "До: "; for(int val : v) cout<<val<<" "; cout<<endl;
                bool ch = false; // Если происходит изменение, то флаг срабатывает
                // Идем с конца, чтобы вставка не сбила индексы
                for(int i=(int)v.size()-1; i>=0; i--) {
                    // Четная позиция и нечетное число
                    if(i%2==0 && v[i]%2!=0) { v.insert(v.begin()+i+1, 0); ch=true; } // Проверка условия и вставка нуля
                }
                if(!ch) { v.insert(v.begin(), -1); v.push_back(-1); }
                cout << "После: "; for(int val : v) cout<<val<<" "; cout<<endl;
            }
        }
    }
    else if (choice == 2) {
        array<int, 10> a;
        
        // --- ПО ВОЗРАСТАНИЮ ---
        cout << "\n=== СОРТИРОВКА ПО ВОЗРАСТАНИЮ ===\n";
        
        // По значению
        for(int i=0; i<10; i++) a[i] = rand()%21 - 10; // Создаем фиксированный массив... [-10; 10] ... при делении на 21 даст от 0 до 20 и еще вычет 10
        cout << "\n1. По значению:\n";
        cout << "   ДО функции:    "; printArr(a);
        sortValAsc(a);
        cout << "   ПОСЛЕ функции: "; printArr(a); // Не изменится

        // По ссылке
        for(int i=0; i<10; i++) a[i] = rand()%21 - 10;
        cout << "\n2. По ссылке:\n";
        cout << "   ДО функции:    "; printArr(a);
        sortRefAsc(a);
        cout << "   ПОСЛЕ функции: "; printArr(a); // Изменится

        // По указателю
        for(int i=0; i<10; i++) a[i] = rand()%21 - 10;
        cout << "\n3. По указателю:\n";
        cout << "   ДО функции:    "; printArr(a);
        sortPtrAsc(&a);
        cout << "   ПОСЛЕ функции: "; printArr(a); // Изменится


        // --- ПО УБЫВАНИЮ ---
        cout << "\n\n=== СОРТИРОВКА ПО УБЫВАНИЮ ===\n";

        // По значению
        for(int i=0; i<10; i++) a[i] = rand()%21 - 10;
        cout << "\n4. По значению:\n";
        cout << "   ДО функции:    "; printArr(a);
        sortValDesc(a);
        cout << "   ПОСЛЕ функции: "; printArr(a); 

        // По ссылке
        for(int i=0; i<10; i++) a[i] = rand()%21 - 10;
        cout << "\n5. По ссылке:\n";
        cout << "   ДО функции:    "; printArr(a);
        sortRefDesc(a);
        cout << "   ПОСЛЕ функции: "; printArr(a); 

        // По указателю
        for(int i=0; i<10; i++) a[i] = rand()%21 - 10;
        cout << "\n6. По указателю:\n";
        cout << "   ДО функции:    "; printArr(a);
        sortPtrDesc(&a);
        cout << "   ПОСЛЕ функции: "; printArr(a); 
    }

    return 0;
}
/*
ПУНКТ 3:

1. Почему в Пункте 1 выбрали std::vector?
   Ответ: Потому что по заданию нужно добавлять элементы в начало и в конец.
   Вектор — это динамический массив, он умеет менять свой размер.
   Обычный массив (или std::array) так не умеет.

2. Почему в Пункте 2 выбрали std::array?
   Ответ: Потому что по условию размер массива фиксирован (10 элементов)
   и менять его не нужно. std::array работает немного быстрее и занимает
   меньше памяти (хранится на стеке).

3. Когда они выполняют одинаковую роль?
   Ответ: Когда размер массива известен заранее и мы НЕ собираемся
   добавлять или удалять элементы, а только менять значения внутри.
*/