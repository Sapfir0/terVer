#ifndef FACTORIAL_H
#define FACTORIAL_H
#include <QVector>

#include <kyplukMath.h>
#include <kyplukBigNumbers.h> // тупо все большое

//батя решил что тут будут расчеты формул
//ты же не хотел пихать их в класс окна?
//да

// за такое и в бан можно
using kypluk::unlimInt;
using kypluk::decimal;

unlimInt combinationsWithoutRepetition(unlimInt n, unlimInt k) {
    //return fact(n)/(fact((n-k))*fact(k));
    unlimInt sb = n - k + 1;

    if (sb < k) { // Это ускорит вычисление в два раза
        swap(sb, k);
        sb+=1;
        k-=1;
    }

    unlimInt mult = 1;
    for (; sb <= n; ++sb) {
        mult *= sb;
    }
    return mult / fact(k);
}


unlimInt placementWithRepetitions(unlimInt n, unlimInt k) { //название не верное, требуется уточнение
    return kypluk::pow<unlimInt>(n, k);
}

//считаем что общая Н является просто суммой всех Н

unlimInt permutationsWithRepetitions(unlimInt n, QVector<unlimInt> nki) { //я туплю, нам нужен массив энок?
    unlimInt sum = 1, res = fact(n);
    for (auto item = nki.begin(); item != nki.end(); ++item) {
        sum *= fact(*item);
    }
    res = res / sum;
    return res;
}


unlimInt permutationsWithoutRepetitions(unlimInt n, unlimInt k) {
    unlimInt sb = n - k + 1;
    unlimInt mult = 1;
    for (; sb <= n; ++sb) {
        mult *= sb;
    }
    return mult;
}


unlimInt permutationsWithoutRepetitionschs(QVector<unlimInt> nki) {
    return fact(nki.front());
}


unlimInt combinationsWithRepetition(unlimInt n, unlimInt k) {
    return combinationsWithoutRepetition(n+k-1, k);
}

// все мы знаем кто написал убогий код выше хехехехехеех
// некит? только он мог написать такую хуйню
// bool operator!(unlimInt val) { //можно сделать для рофла, но учитываем, что знак факториала будет ставится

#endif // FACTORIAL_H
