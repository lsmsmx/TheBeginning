#include <iostream>
#include <cstdlib>

using namespace std;

// Функция 1: Ищет нули и возвращает массив индексов строк на удаление
int* findZeroRows(int** arr, int rows, int cols, int* cnt) {
    // Временный массив меток (0 - ок, 1 - удаляем)
    int* flag = (int*)calloc(rows, sizeof(int));
    int foundCount = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (arr[i][j] == 0) {
                if (flag[i] == 0) {
                    flag[i] = 1;
                    foundCount++;
                }
                break; 
            }
        }
    }

    if (foundCount == 0) {
        free(flag);
        *cnt = 0;
        return NULL;
    }

    // Создаем одномерный массив с индексами
    int* resultIndices = (int*)malloc(foundCount * sizeof(int));
    int curIdx = 0;
    for (int i = 0; i < rows; i++) {
        if (flag[i] == 1) {
            resultIndices[curIdx] = i;
            curIdx++;
        }
    }

    free(flag);
    *cnt = foundCount;
    return resultIndices;
}

//Функция 2: Печатаем матрицу
void printMatrix(int** arr, int rows, int cols) {
    if (rows == 0) {
        cout << "Матрица пуста." << endl;
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << arr[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "-------------------" << endl;
}

// _____ main _____
int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // ================= ПУНКТ 1 =================
    cout << "=== ПУНКТ 1 ===" << endl;

    // 1. Создаем начальную матрицу 2x2
    int rows = 2;
    int cols = 2;
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    cout << "Введите 4 числа для матрицы 2x2:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> matrix[i][j];
        }
    }

    int A, B, C, D;
    while (true) {
        cout << "Введите A (строк сверху) и B (столбцов слева): " << endl;
        cin >> A >> B;
        if (A >= 0 && B >= 0) break;
        cout << "Ошибка!" << endl;
    }
    cout << "Введите коэффициенты C и D: " << endl;
    cin >> C >> D;

    int newRows = rows + A;
    int newCols = cols + B;

    // Шаг 1: Увеличиваем массив указателей на строки
    matrix = (int**)realloc(matrix, newRows * sizeof(int*));

    // Шаг 2: Сдвигаем старые указатели ВНИЗ (освобождаем место сверху)
    if (A > 0) {
        for (int i = rows - 1; i >= 0; i--) {
            matrix[i + A] = matrix[i]; 
        }
    }

    // Шаг 3: Выделяем память под НОВЫЕ строки (те, что сверху)
    for (int i = 0; i < A; i++) {
        matrix[i] = (int*)malloc(newCols * sizeof(int));
    }

    // Шаг 4: Расширяем СТАРЫЕ строки (теперь они внизу)
    for (int i = A; i < newRows; i++) {
        // Расширяем строку
        matrix[i] = (int*)realloc(matrix[i], newCols * sizeof(int));
        
        // Сдвигаем числа ВПРАВО (освобождаем место слева)
        if (B > 0) {
            for (int j = cols - 1; j >= 0; j--) {
                matrix[i][j + B] = matrix[i][j];
            }
        }
    }

    // Шаг 5: Заполняем пустые места по формуле
    for (int i = 0; i < newRows; i++) {
        for (int j = 0; j < newCols; j++) {
            if (i < A || j < B) {
                matrix[i][j] = i * C + j * D;
            }
        }
    }

    // Обновляем переменные размера
    rows = newRows;
    cols = newCols;

    cout << "\nРасширенная матрица:" << endl;
    printMatrix(matrix, rows, cols);

    // ЭТАП УДАЛЕНИЯ (Сжатие массива)
    int zerosCount = 0;
    int* rowsToDel = findZeroRows(matrix, rows, cols, &zerosCount);

    if (zerosCount > 0) {
        
        int wIdx = 0; // Куда записываем хорошую строку
        
        for (int rIdx = 0; rIdx < rows; rIdx++) {
            // Проверяем, есть ли rIdx в списке на удаление
            bool shouldDelete = false;
            for (int k = 0; k < zerosCount; k++) {
                if (rowsToDel[k] == rIdx) {
                    shouldDelete = true;
                    break;
                }
            }

            if (shouldDelete) {
                // Если строка плохая - освобождаем её память
                free(matrix[rIdx]);
            } else {
                // Если строка хорошая - переносим указатель на позицию wIdx
                if (wIdx != rIdx) {
                    matrix[wIdx] = matrix[rIdx];
                }
                wIdx++;
            }
        }

        // Уменьшаем основной массив указателей
        rows = wIdx; // Новый размер
        if (rows > 0) {
            matrix = (int**)realloc(matrix, rows * sizeof(int*));
        } else {
            free(matrix);
            matrix = NULL;
        }
        
        // Массив индексов больше не нужен
        free(rowsToDel);
    }

    cout << "Матрица после удаления строк:" << endl;
    printMatrix(matrix, rows, cols);

    // Очистка оставшейся памяти
    if (matrix != NULL) {
        for (int i = 0; i < rows; i++) free(matrix[i]);
        free(matrix);
    }

    // ================= ПУНКТ 2 =================
    cout << "\n=== ПУНКТ 2 ===" << endl;

    int* ptrA = new int;
    int* ptrB = new int;

    cout << "Введите два целых числа (a и b): " << endl;
    cin >> *ptrA >> *ptrB;

    *ptrA = (*ptrA) * 2;

    int temp = *ptrA;
    *ptrA = *ptrB;
    *ptrB = temp;

    cout << "После обмена: a = " << *ptrA << ", b = " << *ptrB << endl;

    delete ptrA;
    delete ptrB;

    return 0;
}