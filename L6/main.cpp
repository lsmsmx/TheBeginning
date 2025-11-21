#include <iostream>
#include <cstdlib>

using namespace std;

// Функция: Ищет нули и возвращает массив индексов строк на удаление
int* findZeroRows(int** arr, int rows, int cols, int* cnt) {
    int foundCount = 0;
    int* tempFlags = (int*)calloc(rows, sizeof(int)); 

    for (int i = 0; i < rows; i++) {
        bool hasZero = false; // Логическая переменная для проверки
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] == 0) {
                hasZero = true;
                break; 
            }
        }
        if (hasZero) {
            tempFlags[i] = 1;
            foundCount++;
        }
    }

    if (foundCount == 0) {
        free(tempFlags);
        *cnt = 0;
        return NULL;
    }

    // Создаем итоговый массив индексов
    int* resultIndices = (int*)malloc(foundCount * sizeof(int));
    int curIdx = 0;
    for (int i = 0; i < rows; i++) {
        if (tempFlags[i] == 1) {
            resultIndices[curIdx] = i;
            curIdx++;
        }
    }

    free(tempFlags);
    *cnt = foundCount;
    return resultIndices;
}

// Вспомогательная функция: Печать матрицы
void printMatrix(int** arr, int rows, int cols) {
    if (rows == 0 || arr == NULL) {
        cout << "Матрица пуста." << endl;
        return;
    }
    cout << "--- Матрица (" << rows << "x" << cols << ") ---" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "-----------------------" << endl;
}
// _____ main _____
int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // ================= ПУНКТ 1 =================
    
    // 1. Создаем начальную матрицу 2x2 (динамически через malloc)
    int rows = 2;
    int cols = 2;
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    cout << "Заполните матрицу 2x2." << endl;

    // Ввод A (matrix[0][0]) с проверкой
    while (true) {
        cout << "Введите A: ";
        cin >> matrix[0][0];
        if (matrix[0][0] >= 0) break;
        cout << "Ошибка! A должно быть неотрицательным." << endl;
    }

    // Ввод B (matrix[0][1]) с проверкой
    while (true) {
        cout << "Введите B: ";
        cin >> matrix[0][1];
        if (matrix[0][1] >= 0) break;
        cout << "Ошибка! B должно быть неотрицательным." << endl;
    }

    cout << "Введите C: ";
    cin >> matrix[1][0];
    cout << "Введите D: ";
    cin >> matrix[1][1];

    // Теперь извлекаем значения для расчетов прямо из матрицы
    int A = matrix[0][0];
    int B = matrix[0][1];
    int C = matrix[1][0];
    int D = matrix[1][1];

    cout << "\nИсходная матрица:" << endl;
    printMatrix(matrix, rows, cols);

    // === ПРЕОБРАЗОВАНИЕ МАТРИЦЫ ===
    int newRows = rows + A;
    int newCols = cols + B;

    // 1. Увеличиваем массив указателей строк
    matrix = (int**)realloc(matrix, newRows * sizeof(int*));

    // 2. Сдвигаем СТАРЫЕ строки вниз (освобождаем место для A строк сверху)
    // Идем с конца, чтобы не перезаписать данные
    if (A > 0) {
        for (int i = rows - 1; i >= 0; i--) {
            matrix[i + A] = matrix[i];
        }
    }

    // 3. Выделяем память для НОВЫХ строк сверху
    for (int i = 0; i < A; i++) {
        matrix[i] = (int*)malloc(newCols * sizeof(int));
    }

    // 4. Расширяем СТАРЫЕ строки (которые теперь внизу) и сдвигаем данные вправо
    for (int i = A; i < newRows; i++) {
        // Сначала расширяем память строки
        matrix[i] = (int*)realloc(matrix[i], newCols * sizeof(int));
        // Сдвигаем старые 2 значения вправо (освобождаем B столбцов слева)
        if (B > 0) {
            for (int j = cols - 1; j >= 0; j--) {
                matrix[i][j + B] = matrix[i][j];
            }
        }
    }

    // 5. Заполняем освободившиеся места формулой (0*C + 0*D и т.д.)
    for (int i = 0; i < newRows; i++) {
        for (int j = 0; j < newCols; j++) {
            // Если мы в "новой" зоне (верхние строки ИЛИ левые столбцы)
            if (i < A || j < B) {
                matrix[i][j] = i * C + j * D;
            }
            // Иначе (если i >= A и j >= B) - там лежат наши старые числа, их не трогаем
        }
    }

    // Обновляем глобальные размеры
    rows = newRows;
    cols = newCols;

    cout << "Расширенная матрица:" << endl;
    printMatrix(matrix, rows, cols);

    // === УДАЛЕНИЕ СТРОК С НУЛЯМИ ===

    // 1. Передать матрицу в функцию
    int countToDelete = 0;
    // 2. Получить массив индексов
    int* indicesToDelete = findZeroRows(matrix, rows, cols, &countToDelete);

    if (countToDelete > 0) {
        // 3. Удалить соответствующие строки
        // Используем метод двух указателей для сжатия массива
        int writeIdx = 0; 
        
        for (int readIdx = 0; readIdx < rows; readIdx++) {
            bool toDelete = false;
            // Проверяем, есть ли текущий индекс в списке на удаление
            for (int k = 0; k < countToDelete; k++) {
                if (indicesToDelete[k] == readIdx) {
                    toDelete = true;
                    break;
                }
            }

            if (toDelete) {
                free(matrix[readIdx]); // Освобождаем память строки
            } else {
                if (writeIdx != readIdx) {
                    matrix[writeIdx] = matrix[readIdx]; // Перемещаем указатель
                }
                writeIdx++;
            }
        }

        // Обновляем размер массива указателей
        rows = writeIdx;
        if (rows > 0) {
            matrix = (int**)realloc(matrix, rows * sizeof(int*));
        } else {
            free(matrix);
            matrix = NULL;
        }
        
        free(indicesToDelete); // Очищаем массив индексов
    }

    // 4. Вывести полученную матрицу
    cout << "Финальная матрица:" << endl;
    printMatrix(matrix, rows, cols);

    // Очистка памяти в конце
    if (matrix) {
        for (int i = 0; i < rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }

    // ================= ПУНКТ 2 =================
    cout << "\n=== ПУНКТ 2 ===" << endl;
    cout << "Работа с числами a и b" << endl;
    // Создать динамические переменные
    int* ptrA = (int*)malloc(sizeof(int));
    int* ptrB = (int*)malloc(sizeof(int));

    if (ptrA && ptrB) {
        cout << "Введите число a: ";
        cin >> *ptrA;
        cout << "Введите число b: ";
        cin >> *ptrB;

        // Увеличить a в 2 раза
        *ptrA = (*ptrA) * 2;

        // Поменять местами значения
        int temp = *ptrA;
        *ptrA = *ptrB;
        *ptrB = temp;

        cout << "Результат: a = " << *ptrA << ", b = " << *ptrB << endl;
        
        free(ptrA);
        free(ptrB);
    }

    return 0;
}