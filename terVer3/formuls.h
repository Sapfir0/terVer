#ifndef FORMULS_H
#define FORMULS_H

#include <kmath.h>
#include <functional> // xexe

//еще не готово
double integral(double N, double dN, std::function<double(double)> func, double eps = 0.00000001) { // соре что до сих пор нужно писать ету поеботу
    double a = N, b = dN;
    double I=eps+1, I1=0; // I-предыдущее вычисленное значение интеграла, I1-новое, с большим N.
    for (int N=2; (abs(I1-I)>=eps); N*=2)
    {
        double h, sum2=0, sum4=0, sum=0;
        h=(b-a)/(2*N); // Шаг интегрирования.
        for (int i=1; i<=2*N-1; i+=2)
        {
            sum4+=func(a+h*i); // Значения с нечётными индексами, которые нужно умножить на 4.
            sum2+=func(a+h*(i+1)); // Значения с чётными индексами, которые нужно умножить на 2.
        }
        sum=func(a)+4*sum4+2*sum2-func(b); // Отнимаем значение f(b) так как ранее прибавили его дважды.
        I=I1;
        I1=(h/3)*sum;
    }
    return I1;
}

#endif // FORMULS_H
