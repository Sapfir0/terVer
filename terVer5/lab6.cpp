// lab6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <iomanip>

int readStatisticalDataFromFile(double* variants, int* eachVariantCount, double* varFrequncies, int* countOfAllVariants, char* fileName);
int findDoubleInArray(double* arr, double number);
void sortVariationRow(double* variants, int* n, double* w, int size);
double findSelectiveAverage(double* variants, int* eachVariantCount, int arrSize);
double findSelectiveDispersion(double* variants, int* eachVariantCount, int arrSize);
void showVariationRow(double* variants, int* n, double* w, int variantsCount);
void distributionFunction(double x[], int size, double F[], int mode);
void setEnvironmentForGnu(double xMinPos, double xMaxPos);
void distributionPolygon(double x[], int size, double F[]);
double f(double x);
double laplasIntegral(double b);
double laplasFunc(double z);
double factorFunc(double a);


using namespace std;

int main()
{
	setlocale(0, "");
	int task;

	cout << "Выберите задание: \n1 - Проверка гипотезы о нормальном распределении на значениях заданных массивом\n";
	cout << "2 - Проверка гипотезы о нормальном распределении на значениях заданных интервалами\n";
	cout << "3 - Я ещё не придумал как описать это задание\n";
	while (!(cin >> task) || task > 3 || task < 1) {
		cin.clear();
		fflush(stdin);
	}

	char fileName[100] = "D:\\study\\2kurs\\4 sem\\TerVer\\lab5\\data1.txt";
	double variants[20];
	int eachVariantCount[20];
	double varFrequencies[20];
	int realVariantsArrSize;
	int countOfAllVariants = 0;
	double selectiveAverage;
	double selectiveDispersion;
	double selectiveStandardDeviation;
	double U[20];
	double P[22];
	double theoreticalFreq[20];
	double acceptanceСriterion = 0;
	double critycalСriterion;
	double sum = 0;
	double h = 0.1;
	cout << "Введите значения критической точки распределения Пирсона\nНапример 11,1; 12,6; 14,1 \n";
	while (!(cin >> critycalСriterion) || critycalСriterion < 0 || critycalСriterion > 50) {
		cin.clear();
		fflush(stdin);
	}

	switch (task) {
	case 1:
		strcpy_s(fileName, "D:\\study\\2kurs\\4 sem\\TerVer\\lab5\\data.txt");
		h = 1.0;
	case 2:
		realVariantsArrSize = readStatisticalDataFromFile(variants, eachVariantCount, varFrequencies, &countOfAllVariants, fileName);
		sortVariationRow(variants, eachVariantCount, varFrequencies, realVariantsArrSize);//Сортировка вариационноно ряда (от меньшего к большему)
		showVariationRow(variants, eachVariantCount, varFrequencies, realVariantsArrSize);
		selectiveAverage = findSelectiveAverage(variants, eachVariantCount, realVariantsArrSize);
		cout << "Выборочное среднее: " << selectiveAverage << endl;
		selectiveDispersion = findSelectiveDispersion(variants, eachVariantCount, realVariantsArrSize);
		cout << "Выборочная дисперсия: " << selectiveDispersion << endl;
		selectiveStandardDeviation = sqrt(selectiveDispersion);
		cout << "Выборочная среднеквадратическое отклонение: " << selectiveStandardDeviation << endl;
		cout << "Плотность распределения : f(x) = " << 1.0 / sqrt(2.0 * 3.14 * selectiveStandardDeviation) << " * exp( -( x - " << selectiveAverage << " )^2 / " << 2.0 * pow(selectiveStandardDeviation,2) << ") " << endl;
		cout << "Функция распределения : F(x) = 0,5 + Ф( (x - " << selectiveAverage << " ) / " << selectiveStandardDeviation << " ) "<< endl;

		cout << "Проверим гипотезу о нормальном законе распределения случайной величины Х по критерию согласия Пирсона X^2 \n";
		cout << "Для этого преобразуем границы частичных интервалов по формуле Ui = (Xi - Xв)/Sв \n";
		cout << "-------------------------------------------------------------------------------------------------------\n";
		cout << "|Ui    |";
		for (int i = 0; i < realVariantsArrSize; i++) {
			U[i] = (variants[i] - selectiveAverage) / selectiveStandardDeviation;
			cout << U[i] << "  |";
		}
		cout << "\n-------------------------------------------------------------------------------------------------------\n";
		cout << "Теоретические частоты:\n";
		cout << "|n*i   |";
		for (int i = 0; i < realVariantsArrSize; i++) {
			theoreticalFreq[i] = countOfAllVariants * h * laplasFunc(U[i]) / selectiveStandardDeviation;
			cout << theoreticalFreq[i] << "  |";
		}
		cout << "\n-------------------------------------------------------------------------------------------------------\n";
		cout << "Вычислим значения критерия принятия Кси квадрат по формуле (ni - Pi)^2 / Pi\n";
		for (int i = 0; i < realVariantsArrSize; i++) {
			P[i] = eachVariantCount[i];
			acceptanceСriterion += pow(theoreticalFreq[i] - eachVariantCount[i], 2)/ theoreticalFreq[i];
		}
		cout << "\nКритерий принятия Пирсона: " <<acceptanceСriterion << endl;

		if (acceptanceСriterion < critycalСriterion) {
			cout << "Критерий принятия Пирсона меньше критического, следовательно выборка распределена по нормальному закону" << endl;
		}
		else {
			cout << "Критерий принятия Пирсона больше критического, следовательно выборка не распределена по нормальному закону" << endl;
		}

		distributionPolygon(variants, realVariantsArrSize, varFrequencies);
		break;
	case 3:
		
		cout << "Введите количество интервалов :";
		while (!(cin >> realVariantsArrSize) || realVariantsArrSize < 0 || realVariantsArrSize > 20) {
			cin.clear();
			fflush(stdin);
		}
		for (int i = 0; i < realVariantsArrSize; i++) {
			cout << "\nВведите " << i << "-ую варианту\n";
			while (!(cin >> variants[i])) {
				cin.clear();
				fflush(stdin);
			}
			cout << "\nВведите частоту" << i << "-ой варианты\n";
			while (!(cin >> eachVariantCount[i]) || eachVariantCount[i] < 0) {
				cin.clear();
				fflush(stdin);
			}
			countOfAllVariants += eachVariantCount[i];
		}
		cout << "На основании наблюдаемых значений найдем точечную оценку параметра лямбда\n";
		selectiveAverage = 0;
		for (int i = 0; i < realVariantsArrSize; i++) {
			selectiveAverage += variants[i] * eachVariantCount[i];
		}
		selectiveAverage /= countOfAllVariants;
		cout << "лямбда = " << selectiveAverage;
		cout << "\nНайдём теоретические вероятности появления событий\n";
		for (int i = 0; i < realVariantsArrSize; i++) {
			P[i] = pow(selectiveAverage, variants[i]) * exp(-selectiveAverage) / factorFunc(variants[i]);
			//cout << "P" << i << " = " << P[i] << endl;
			acceptanceСriterion += pow(eachVariantCount[i] - countOfAllVariants * P[i], 2) / (countOfAllVariants * P[i]);
		}

		cout << "  xi   |mi | pi     |  n*pi   |(m-npi)^2|(m-npi)^2 / npi\n";
		for (int i = 0; i < realVariantsArrSize; i++) {
			cout << fixed << setprecision(4);
			cout << variants[i] << " | ";
			if (eachVariantCount[i] > 10) {
				cout << eachVariantCount[i] << "| ";
			}
			else {
				cout << eachVariantCount[i] << " | ";
			}
			cout << P[i] << " | ";
			cout << P[i] * countOfAllVariants << " | ";
			cout << pow(eachVariantCount[i] - countOfAllVariants * P[i], 2) << " | ";
			cout << pow(eachVariantCount[i] - countOfAllVariants * P[i], 2) / (countOfAllVariants* P[i]) << " | " << endl;
		}


		cout << "Критерий принятия :" << acceptanceСriterion;
		if (acceptanceСriterion < critycalСriterion) {
			cout << "\nКритерий принятия Пирсона меньше критического, следовательно выборка распределена по закону Пуассона\n" << endl;
		}
		else {
			cout << "Критерий принятия Пирсона больше критического, следовательно выборка не распределена по закону Пуассона\n" << endl;
		}

		break;
	}
}

int readStatisticalDataFromFile(double* variants, int* eachVariantCount, double* varFrequncies, int* countOfAllVariants, char* fileName) {
	HANDLE dataFile = CreateFileA(fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (dataFile == INVALID_HANDLE_VALUE) {
		cout << "No such file";
		CloseHandle(dataFile);
		exit(0);
	}
	char buffer[1001] = "0";
	bool readDone = ReadFile(dataFile, buffer, 1000, NULL, NULL);
	int count = 0; //Число вариант в массиве 
	if (readDone) {//Если ReadFile вернул true (чтение завершилось успешно)
		//puts(buffer);
		char* token = NULL;		//Текущая варианта в виде char*
		char* nextToken = NULL;
		double var; //текущая варианта перевёдённая к int
		int index = -1; //Индекс варианты из массива вариант. -1 если такой варианты в массиве ещё нету

		token = strtok_s(buffer, "#", &nextToken);//Разбиение массива char'ов на куски разделённые # 
		while (token != NULL) {
			if (token != NULL) {
				//printf("%s\n", token);
				var = atof(token);
				index = findDoubleInArray(variants, var); //Проверка есть ли такая варианта уже в массиве. Если есть, то в index вернётся номер этой варианты в массиве
				if (index == -1) {//Если такой варианты ещё нет в массиве
					variants[count] = var;
					eachVariantCount[count] = 1;
					count++;
				}
				else {//Если такая варинта в массиве уже есть
					eachVariantCount[index]++;
				}
				token = strtok_s(NULL, "#", &nextToken);
			}
		}

		int nSum = 0;
		for (int i = 0; i < count; i++) nSum += eachVariantCount[i];
		*countOfAllVariants = nSum;
		for (int i = 0; i < count; i++) {
			varFrequncies[i] = eachVariantCount[i];
			varFrequncies[i] /= nSum;
		}
	}
	CloseHandle(dataFile);
	return count;
}

int findDoubleInArray(double* arr, double number) {
	bool answer = false;
	for (int i = 0; i < 20; i++) {
		if (arr[i] == number) return i;
	}
	return -1;
}

void sortVariationRow(double* variants, int* n, double* w, int size) {
	double buff;
	double buffD;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (variants[i] < variants[j]) {
				buff = variants[i];
				variants[i] = variants[j];
				variants[j] = buff;
				buff = n[i];
				n[i] = n[j];
				n[j] = buff;
				buffD = w[i];
				w[i] = w[j];
				w[j] = buffD;
			}
		}
	}
}

double findSelectiveAverage(double* variants, int* eachVariantCount, int arrSize) {
	double selectiveAverage = 0;
	int countOfAllVariants = 0;
	for (int i = 0; i < arrSize; i++) {
		selectiveAverage += variants[i] * eachVariantCount[i];
		countOfAllVariants+= eachVariantCount[i];
	}
	selectiveAverage /= countOfAllVariants;
	return selectiveAverage;
}

double findSelectiveDispersion(double* variants, int* eachVariantCount, int arrSize) {
	double selectiveAverage = findSelectiveAverage(variants, eachVariantCount, arrSize);
	int countOfAllVariants = 0;
	double selectiveDispersion = 0;
	for (int i = 0; i < arrSize; i++) {
		selectiveDispersion += eachVariantCount[i] * pow((variants[i] - selectiveAverage), 2);
		countOfAllVariants += eachVariantCount[i];
	}
	selectiveDispersion /= countOfAllVariants;
	return selectiveDispersion;
}

void showVariationRow(double* variants, int* n, double* w, int variantsCount) {
	int nSum = 0;
	for (int i = 0; i < variantsCount; i++) nSum += n[i];
	cout << "Вариационный ряд из файла: " << endl << "------------------------------" << endl;
	cout << "|  Xi      |  ni  |  wi      |" << endl;
	cout << fixed << setprecision(3);
	for (int i = 0; i < variantsCount; i++) {
		if (n[i] > 9) {
			cout << "|  " << variants[i] << "  |  " << n[i] << "  |  " << w[i] << "   |" << endl;
		}
		else {
			cout << "|  " << variants[i] << "  |  " << n[i] << "   |  " << fixed << setprecision(3) << w[i] << "   |" << endl;
		}
	}
	cout << "--------------------------" << endl;

}

void distributionPolygon(double x[], int size, double F[]) {
	HANDLE grafData = CreateFile2(TEXT("F:\\progs\\gnuplot\\bin\\grafData.txt"), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, NULL); //Создание файла 

	if (grafData == INVALID_HANDLE_VALUE) {//Если не удалось создать фалй
		cout << "Создать файл для отрисовки графика не удалось, программа закрывается";
		system("pause");
		exit(1); //Завершение программы с кодом 1
	}

	char spacer1 = ' ';							//Разделитель для текста в файле (пробел)
	char spacer2 = '\n';						//Разделитель для текста в файле (перенос строки)
	char XfromDouble[20];							//Значение координаты икс переведённой из инта в чар
	char FfromDouble[20];							//Значение координаты игрик переведённой из инта в чар

	for (int i = 0; i < size; i++) {
		sprintf_s(XfromDouble, "%f", x[i]);
		*(strchr(XfromDouble, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
		WriteFile(grafData, XfromDouble, sizeof(char) * strlen(XfromDouble), NULL, NULL);
		WriteFile(grafData, &spacer1, sizeof(char), NULL, NULL);
		sprintf_s(FfromDouble, "%f", F[i]);
		*(strchr(FfromDouble, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
		WriteFile(grafData, FfromDouble, sizeof(char) * strlen(FfromDouble), NULL, NULL);
		WriteFile(grafData, &spacer2, sizeof(char), NULL, NULL);
	}

	CloseHandle(grafData);	//Закрытие файла с данными графика 

	setEnvironmentForGnu(x[0] - 1, x[size - 1] + 1);

	STARTUPINFOA info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	char gnu[] = "F:\\progs\\gnuplot\\bin\\gnuplot.exe F:\\progs\\gnuplot\\bin\\environmentData.gnu";

	CreateProcessA(NULL, gnu, NULL, NULL, FALSE, NULL, NULL, NULL, &info, &processInfo);//Создание процесса с графиком

	system("PAUSE");
}

void distributionFunction(double x[], int size, double F[], int mode) {
	cout << "Эмпирическая функция распределения: " << endl;
	double buff = 0;
	buff += F[0];
	cout << "F*(x) = " << fixed << setprecision(4) << 0.0 << " При      x <  " << x[0] << endl;
	for (int i = 1; i < size; i++) {
		cout << "F*(x) = " << fixed << setprecision(4) << buff << " При " << x[i - 1] << " < x <= " << x[i] << endl;
		buff += F[i];
	}
	cout << "F*(x) = " << fixed << setprecision(4) << 1.0 << " При      x >  " << x[size - 1] << endl;

	HANDLE grafData = CreateFile2(TEXT("F:\\progs\\gnuplot\\bin\\grafData.txt"), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, NULL); //Создание файла 

	if (grafData == INVALID_HANDLE_VALUE) {//Если не удалось создать фалй
		cout << "Создать файл для отрисовки графика не удалось, программа закрывается";
		system("pause");
		exit(1); //Завершение программы с кодом 1
	}
	double xStartPos = x[0] - 5;					//Начальная позиция по координате икс
	double xEndPos = x[size - 1] + 5;				//Конечная позиция по координате икс
	char xStartPosChar[20];						//Начальная позиция по координате икс приведённая к типу char
	char xEndPosChar[20];						//Конечная позиция по координате икс приведённая к типу char
	sprintf_s(xStartPosChar, "%f", xStartPos);  //Перевод начальной позиции координаты икс из инта в чар
	*(strchr(xStartPosChar, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
	sprintf_s(xEndPosChar, "%f", xEndPos);		//Перевод конечной позиции координаты икс из инта в чар
	*(strchr(xEndPosChar, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
	char spacer1 = ' ';							//Разделитель для текста в файле (пробел)
	char spacer2 = '\n';						//Разделитель для текста в файле (перенос строки)
	char XfromDouble[20];							//Значение координаты икс переведённой из инта в чар
	char FfromDouble[20];							//Значение координаты игрик переведённой из инта в чар
	double bufferF = 0;						//Сумма значений вероятностей 

	WriteFile(grafData, &xStartPosChar, sizeof(char) * strlen(xStartPosChar), NULL, NULL); //Запись в текстовый файл начальной позиции икс 
	WriteFile(grafData, &spacer1, sizeof(char), NULL, NULL);
	sprintf_s(XfromDouble, "%f", 0.0);
	*(strchr(XfromDouble, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
	WriteFile(grafData, &XfromDouble, sizeof(char) * 3, NULL, NULL);
	WriteFile(grafData, &spacer2, sizeof(char), NULL, NULL);

	if (mode == 1) {//Если задана не таблица распределения, то фактический размер будет на 1 больше, потому прибавляю единицу
		size += 1;
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 2; j++) {
			sprintf_s(XfromDouble, "%f", x[i]);
			*(strchr(XfromDouble, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
			sprintf_s(FfromDouble, "%f", bufferF);
			*(strchr(FfromDouble, ',')) = '.'; //Нахожу указатель на запятую в переменной и заменяю на точку
			WriteFile(grafData, &XfromDouble, sizeof(char) * strlen(XfromDouble), NULL, NULL);
			WriteFile(grafData, &spacer1, sizeof(char), NULL, NULL);
			WriteFile(grafData, FfromDouble, sizeof(char) * strlen(FfromDouble), NULL, NULL);
			WriteFile(grafData, &spacer2, sizeof(char), NULL, NULL);
			if (j == 0) {
				bufferF += F[i];
			}
		}
	}

	WriteFile(grafData, &xEndPosChar, sizeof(char) * strlen(xEndPosChar), NULL, NULL); //Запись в текстовый файл конечной позиции позиции икс 
	WriteFile(grafData, &spacer1, sizeof(char), NULL, NULL);
	sprintf_s(XfromDouble, "%d", 1);
	WriteFile(grafData, &XfromDouble, sizeof(char), NULL, NULL);

	CloseHandle(grafData);	//Закрытие файла с данными графика 

	setEnvironmentForGnu(xStartPos, xEndPos);

	STARTUPINFOA info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	char gnu[] = "F:\\progs\\gnuplot\\bin\\gnuplot.exe F:\\progs\\gnuplot\\bin\\environmentData.gnu";
	CreateProcessA(NULL, gnu, NULL, NULL, FALSE, NULL, NULL, NULL, &info, &processInfo);//Создание процесса с графиком
}

void setEnvironmentForGnu(double xMinPos, double xMaxPos) {
	HANDLE environmentData = CreateFile2(TEXT("F:\\progs\\gnuplot\\bin\\environmentData.gnu"), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, NULL); //Создание файла 

	if (environmentData == INVALID_HANDLE_VALUE) {//Если не удалось создать файл
		cout << "Создать файл для отрисовки графика не удалось, программа закрывается";
		system("pause");
		exit(1); //Завершение программы с кодом 1
	}

	char environmentText[150];
	int a = xMinPos;//Костыль, простите, но у меня не хватает времени, чтобы сделать лучше
	int b = xMaxPos;//Костыль, простите, но у меня не хватает времени, чтобы сделать лучше
	sprintf_s(environmentText, "set xlabel \"x\"\nset ylabel \"F\"\nset xrange [%d:%d]\nset yrange [-0.05:1.05]\nplot \'F:\\progs\\gnuplot\\bin\\grafData.txt\' with line\npause -1", a, b);
	WriteFile(environmentData, environmentText, sizeof(char) * strlen(environmentText), NULL, NULL);

	CloseHandle(environmentData);		//Закрытие конфигурационного файла
}

double f(double x)
{
	return exp(-pow(x, 2) / 2);
}

double laplasIntegral(double b)
{
	if (b >= 5) {
		return 0.5;
	}
	else if (b < -5) {
		return -0.5;
	}
	bool bWasNegative = false;
	if (b < 0) {
		b = -b;
		bWasNegative = true;
	}
	int i; // счётчик
	double Integral; // здесь будет интеграл
	double a = 0.0;
	double h = 0.0001;// задаём шаг интегрирования

	double n; // задаём число разбиений n

	n = (b - a) / h;

	// вычисляем интеграл по формуле трапеций
	Integral = h * (f(a) + f(b)) / 2.0;
	for (i = 1; i <= n - 1; i++)
		Integral = Integral + h * f(a + h * i);
	Integral /= sqrt(2 * 3.141592653589793238463);
	//cout << "I2 = " << Integral << "\n";
	if (bWasNegative) {
		Integral = -Integral;
	}
	return Integral;
}

double laplasFunc(double z) {
	double F = exp(-(z * z / 2)) / sqrt(2.0 * 3.14159265359);
	return F;
}

double factorFunc(double a) {//Функция факториала
	if (a < 0) {
		return 0;
	}
	else if (a == 0) {
		return 1;
	}
	else {
		return a * factorFunc(a - 1);
	}
}