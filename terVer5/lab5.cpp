// lab5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <iomanip>

int readStatisticalDataFromFile(double* variants, int* n, double* w, char* fileName);
int findDoubleInArray(double* arr, double number);
void sortVariationRow(double* variants, int* n, double* w, int size);
void showVariationRow(double* variants, int* n, double* w, int variantsCount);
void distributionFunction(double x[], int size, double F[], int mode);
void setEnvironmentForGnu(double xMinPos, double xMaxPos);
void distributionPolygon(double x[], int size, double F[]);

using namespace std;

int main()
{
	ouble variants[20];//Варианты вариационного ряда
	int n[20];//Количество появлений каждой варианты
	double w[20];//Относительные частоты появления вариант
	double nFromInt[20];//Количество появлений каждой варианты переведённые к типу double 
	int nSum = 0;//Размер выборки
	int variantsCount; //Реальное число вариант
	double S; //Среднеквадратичное отклонение
	double selectiveAverage = 0;//Выборочное среднее
	double moda;//Мода выборки (варианта с наибольшей частотой появления) 
	double buff = 0; //буффер
	double dispersion; //Дисперсия
	switch (task) {
	case 1://case 1 отличается от второго лишь файлом откуда брать данные
		strcpy_s(fileName, "D:\\study\\2kurs\\4 sem\\TerVer\\lab5\\data.txt");
	case 2:
		variantsCount = readStatisticalDataFromFile(variants, n, w, fileName); //Чтение из файла статистических данных
		sortVariationRow(variants, n, w, variantsCount);//Сортировка вариационноно ряда (от меньшего к большему)
		showVariationRow(variants, n, w, variantsCount);//вывод в консоль вариационного ряда
		distributionPolygon(variants, variantsCount, w);//Вывод на экран полигона относительного ряда частот
		for (int i = 0; i < variantsCount; i++)	{
			nFromInt[i] = n[i];
		}
		distributionPolygon(variants, variantsCount, nFromInt);//Вывод на экран полигона статистического ряда частот
		distributionFunction(variants, variantsCount, w, 0);//Вывод на экран эмпирической функции распределения
		for (int i = 0; i < variantsCount; i++) {
			selectiveAverage += variants[i] * n[i];
			nSum += n[i];
		}
		selectiveAverage /= nSum;
		cout << "Выборочное среднее, найденное по формуле xв = (x1*n1 + x2*n2 + ... + xk*nk)/n :" << endl << selectiveAverage << endl;
		moda = 0;//Мода выборки (варианта с наибольшей частотой появления) 
		for (int i = 0; i < variantsCount; i++)
			if (w[i] > buff) {
				buff = w[i];
				moda = variants[i];
			}
		cout << "Мода выборки (варианта с наибольшей частотой появления) : " << endl << moda << endl;
		dispersion = 0;
		for (int i = 0; i < variantsCount; i++) {
			dispersion += pow(variants[i] - selectiveAverage, 2) * n[i];
		}
		dispersion /= nSum;
		cout << "Выборочная дисперсия Dв - это среднее арифметическое квадратов отклонений всех вариант выборки от её средней" << endl;
		cout << "Найдена по формуле ((x1 - xв)^2 *n1 + (x2 - xв)^2 *n2 + ... + (xk - xв)^2 *nk)/n : " << endl << dispersion << endl;
		S = sqrt(dispersion * nSum / (nSum - 1));
		cout << "Исправленное выборочное среднеквадратичное отклонение S по формуле sqrt(n*Dв/(n-1)) : " << endl << S << endl;
		break;
	case 3:
		cout << "Значения по заданию : " << endl << "|Xi; Xi+1 | 20;30 | 30;40 | 40;50 | 50;60 | 60;70 |" << endl << "|   ni    |   9   |  20   |  45   |  19   |   7   |" << endl;
		double intervals[20][2];
		do {
			cout << "Введите число разбиений интервального ряда" << endl;
			cin >> variantsCount;
		} while (variantsCount <= 0);
		for (int i = 0; i < variantsCount; i++) {
			cout << "введите начало интервала номер " << i + 1 << endl;
			cin >> intervals[i][0];
			variants[i] = intervals[i][0];
			do {
				cout << "введите конец интервала номер " << i + 1 << endl;
				cin >> intervals[i][1];
			} while (intervals[i][1] <= variants[i]);
			variants[i] += intervals[i][1];
			variants[i] /= 2;
		}
		for (int i = 0; i < variantsCount; i++) {
			do {
				cout << "Введите число появлений значений из интервального ряда номер " << i + 1 << endl;
				cin >> n[i];
			} while (n[i] <= 0);
			nSum += n[i];//Нахождение общего числа появлений
		}
		for (int i = 0; i < variantsCount; i++) {//нахождение относительных частот
			w[i] = n[i];
			w[i] /= nSum;
		}
		showVariationRow(variants, n, w, variantsCount);
		distributionPolygon(variants, variantsCount, w);
		double F[20];
		double variantsForFunc[20];
		F[0] = 0;
		variantsForFunc[0] = intervals[0][0] - 5;
		variantsForFunc[1] = intervals[0][0];
		for (int i = 0; i < variantsCount; i++) {//нахождение частот для функции распределения
			F[i + 1] = 0;
			variantsForFunc[i + 2] = intervals[i][1];
			for (int j = 0; j < i; j++) {
				F[i + 1] += w[j];
			}
		}
		F[variantsCount + 1] = 1;
		variantsForFunc[variantsCount + 2] = intervals[variantsCount - 1][1] + 5;
		F[variantsCount + 2] = 1;
		/*for (int i = 0; i < variantsCount + 3; i++)
		{
			cout << variantsForFunc[i] << "  " << F[i] << endl;
		}*/
		distributionPolygon(variantsForFunc, variantsCount + 3, F);
		cout << "Размах интервального ряда:" << endl << intervals[variantsCount - 1][1] - intervals[0][0] << endl;
		
		for (int i = 0; i < variantsCount; i++) {
			selectiveAverage += variants[i] * n[i];
		}
		selectiveAverage /= nSum;
		cout << "Выборочное среднее, найденное по формуле xв = (x1*n1 + x2*n2 + ... + xk*nk)/n :" << endl << selectiveAverage << endl;
		double selectiveDispersion = 0;
		for (int i = 0; i < variantsCount; i++) {
			selectiveDispersion += pow(variants[i] - selectiveAverage, 2) * n[i];
		}
		selectiveAverage /= nSum;
		cout << "Выборочная дисперсия, найденное по формуле xв = ((x1 - xср)*n1 + (x2 - xср)*n2 + ... + (xk - xср)*nk)/n :" << endl << selectiveAverage << endl;
		break;
	}


}

int readStatisticalDataFromFile(double* variants, int* n, double* w, char* fileName) {
	int count = 0; //Число вариант в массиве 
	if (readDone) {//Если ReadFile вернул true (чтение завершилось успешно)
		//puts(buffer);
		char* token = NULL;		//Текущая варианта в виде char*
		char* nextToken = NULL;
		double var; //текущая варианта перевёдённая к int
		double var2;
		int index = -1; //Индекс варианты из массива вариант. -1 если такой варианты в массиве ещё нету

		token = strtok_s(buffer, "#", &nextToken);//Разбиение массива char'ов на куски разделённые # 
		while (token != NULL) {
			if (token != NULL) {
				//printf("%s\n", token);
				var = atof(token);
				index = findDoubleInArray(variants, var); //Проверка есть ли такая варианта уже в массиве. Если есть, то в index вернётся номер этой варианты в массиве
				if (index == -1) {//Если такой варианты ещё нет в массиве
					variants[count] = var;
					n[count] = 1;
					count++;
				}
				else {//Если такая варинта в массиве уже есть
					n[index]++;
				}
				token = strtok_s(NULL, "#", &nextToken);
			}
		}

		int nSum = 0;
		for (int i = 0; i < count; i++) nSum += n[i];
		for (int i = 0; i < count; i++) {
			w[i] = n[i];
			w[i] /= nSum;
		}
	}
	CloseHandle(dataFile);
	return count;
}