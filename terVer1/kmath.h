#ifndef FACTORIAL_H
#define FACTORIAL_H
#include <QVector>

#include <kyplukMath.h>

//батя решил что тут будут расчеты формул
//ты же не хотел пихать их в класс окна?
//да

template<class puk>
puk combinationsWithoutRepetition(QVector<puk> nki) {
    puk& n = nki.front(), k = nki.back();
    //return fact(n)/(fact((n-k))*fact(k));
    puk sb = n - k + 1;
    puk mult = 1;
    for (; sb <= n; ++sb) {
        mult *= sb;
    }
    return mult / fact(k);
}

template<class puk>
puk placementWithRepetitions(QVector<puk> nki) { //название не верное, требуется уточнение
    puk n = nki.front(), k = nki.back();
    return pow(n, k);
}
//считаем что общая Н является просто суммой всех Н
template<class puk>
puk permutationsWithRepetitions(QVector<puk> nki) { //я туплю, нам нужен массив энок?
    puk sum = 1, res = fact(nki.front());
    for (auto item = nki.begin() + 1; item != nki.end(); ++item) {
        sum *= fact(*item);
    }
    res = res / sum;
    return res;
}

template<class puk>
puk permutationsWithoutRepetitions(QVector<puk> nki) {
    puk n = nki.front(), k = nki.back();
    //return fact(n)/(fact((n-k))*fact(k));
    puk sb = n - k + 1;
    puk mult = 1;
    for (; sb <= n; ++sb) {
        mult *= sb;
    }
    return mult;
}

template<class puk>
puk permutationsWithoutRepetitionschs(QVector<puk> nki) {
    return fact(nki.front());
}

template<class puk>
puk combinationsWithRepetition(QVector<puk> nki) {
    QVector<puk> temp;
    puk& n = nki.front(), k = nki.back();
    temp.append(n+k-1);
    temp.append(k);
    return combinationsWithoutRepetition(temp);
}

//все мы знаем кто написал убогий код выше хехехехехеех
//template<class puk>
//bool operator!(puk val) { //можно сделать для рофла, но учитываем, что знак факториала будет ставится

//}

#endif // FACTORIAL_H
