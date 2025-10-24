//Вар. 6
#include <iostream>
#include <cmath>
int main() {
	//no.1
	{
		const int N = 9;
		int array1[N];
		std::cout << "Введите числа массива" << std::endl;
		for (int i = 0; i<N; ++i) {
			std::cin >> array1[i];
		}
		bool startsw3 = false; // флаг для проверки наличия числа начинающегося или нет на три
		for (int i = 0; i < N; ++i) { // сама проверка
			int abs_num = std::abs(array1[i]);
				if (abs_num == 3) {
					startsw3 = true;
					break;
				}
				else {
					while (abs_num>=10) {
						abs_num = abs_num / 10;
					}
					if (abs_num == 3) {
						startsw3 = true;
						break;
					}
				}
		}
		if (startsw3) { 
			for (int i = 0; i < N-1;++i){ //метод пузырька
				for (int j = 0; j< N-1-i; ++j){
					if (array1[j]>array1[j+1]){
						int temp = array1[j];
						array1[j]=array1[j+1];
						array1[j+1] = temp;
					}
				}
			}
			std::cout << "Массив после обработки: " << std::endl;
			for (int i = 0; i<N;i++){
				std::cout<<array1[i]<<" "; 
			}
		std::cout << std::endl;
	}
	else{
		std::cout<<"Не было найдено чисел удовлетворяющих условию."<< std::endl;
		for (int i = 0; i<N;i++){
				std::cout<<array1[i]<<" "; 
			}
		std::cout << std::endl;
	}
	}

	//no.2
	{
		const int rows = 3;
		const int columns = 4;
		int matrix[rows][columns];
		std::cout << "Введите " << rows*columns << " целых чисел в матрицу" << std::endl;
		for (int i = 0; i < rows; ++i){ //  строки матрицы
			for (int j = 0; j < columns; ++j){ //столбцы матрицы
				std::cin >> matrix[i][j]; 
			}
		}
		int maxAbsSumm = -1; // отдаленная от abs суммы
		int row_i = -1; // для запоминания номера строки
		for (int i = 0; i < rows; ++i){
			int currSumm = 0; // для накопления суммы
			for (int j = 0; j < columns; j++){
				currSumm += matrix[i][j]; // считаем эллемент в сумму
			}
			int currAbsSumm = std::abs(currSumm); //присваиваем абсолютное значение
			if (currAbsSumm > maxAbsSumm){ 
				maxAbsSumm = currAbsSumm;
				row_i = i; // запоминаем индекс
			}
		}
		if (row_i != -1){
			for (int j = 0; j<columns;++j){
				matrix[row_i][j]=999; // меняем все значения на 999
			}
		}
		else{
			std::cout <<"Все суммы равны нулю или матрица некоректна"<< std::endl;
		}
		std::cout << "Матрица после обработки: " << std::endl;
		for (int i = 0; i<rows;++i){ // вывод матрицы
			for (int j = 0; j<columns;++j){
				std::cout << matrix[i][j] << "\t";
			}
			std::cout << std::endl;
		}
	}
	return 0;
}