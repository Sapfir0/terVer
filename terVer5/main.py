
import json
import math as mt
import matplotlib.pyplot as plt
import numpy as np

# возвращает массив данных из файла


def readDataFromFile(filename):
    file = open(filename, 'r')
    arr = json.loads(file.read())
    file.close()
    return arr


def showVariationRow(xi, ni, wi):
    print('Вариационный ряд:')
    print('\t|\tXi\t|\tni\t|\twi\t|')
    for i in range(len(xi)):
        print('', xi[i], ni[i], "%.4f" % wi[i], '', sep='\t|\t')

# расчет повторов


def counter(array):
    result = {i: array.count(i) for i in array}
    return result

#


def probability(kortez):
    arr = np.array([kortez[i] for i in kortez])
    arr = arr / arr.sum()
    return arr.tolist()


def createrRow(variant):
    rrr = counter(variant)
    wi = probability(rrr)
    xi = [i for i in rrr]
    ni = [rrr[i] for i in rrr]
    return xi, ni, wi

# Poligon


def showPoligon(x, y, title = 'Полигон'):
    plt.ylim([-0.01, max(y)+(max(y)/10)])
    plt.xlim([min(x)-1, max(x)+2])
    plt.plot(x, y)
    plt.title(title)
    # plt.title(r'$p(x)=\frac{1}{\sqrt{2\pi\sigma^{2}}}e^{-\frac{(x-\mu)^{2}}{2\sigma^{2}}}$',
    #          fontsize = 20,    #  увеличиваем размер
    #          pad = 20)    #  приподнимаем над "Axes"
    plt.grid()
    plt.show()


def showRaspGraph(x, y, title = 'Эмпирическая функция распределения'):
    arrowstyle = {
        'arrowstyle': "->"
    }

    plt.ylim([-0.01, 1.1])
    plt.xlim([min(x)-1, max(x)+2])
    # лыл
    head_w = 0.01
    plt.hlines(y=0, xmin=-56465, xmax=min(x), linewidth=1.5, color='000000')
    buff = 0.0
    for i in range(0, len(y)-1):
        plt.arrow(x[i+1], y[i]+buff, -(x[i+1]-x[i]), 0, head_width=head_w)
        buff += y[i]
    # в идеале здесь должна быть линия уходящая в бесконечность
    dxinf = 10000
    plt.arrow(max(x)+dxinf, 1, -dxinf, 0, head_width=head_w)
    plt.title(title)
    plt.grid()
    plt.show()


def showFunctionRaspr(x, y):
    print("Эмпирическая функция распределения: ")
    buff=y[0]
    print("F*(x) = ", "%.4f" % 0.0, " При", "x <", x[0])
    for i in range(1, len(y)):
        print("F*(x) = ", "%.4f" % buff, " При", x[i - 1], "< x <=", x[i])
        buff += y[i]
    print("F*(x) = ", "%.4f" % buff, " При", "x >", x[len(y) - 1])
    showRaspGraph(x, y)

def firstTask():
    filename = str(input('Введите название файла: '))
    variant = readDataFromFile(filename)
    variant = sorted(variant)
    xi, ni, wi = createrRow(variant)
    showVariationRow(xi, ni, wi)
    
    # text = colored('Hello, World!', 'red', attrs=['reverse', 'blink'])
    # print(text)

    # show graphs
    showPoligon(xi, ni)
    showPoligon(xi, wi)
    showFunctionRaspr(xi, wi)

    xi = np.array(xi)
    ni = np.array(ni)
    wi = np.array(wi)

    selectiveAverage = (xi * ni).sum() / ni.sum()
    print("Выборочное среднее, найденное по формуле xв = (x1*n1 + x2*n2 + ... + xk*nk)/n :", selectiveAverage)

    # find moda
    moda = xi[wi.tolist().index(max(wi))]
    print("Мода выборки (варианта с наибольшей частотой появления) : ", moda)

    # find dispersion
    dispersion = (np.power(xi - selectiveAverage, 2) * ni).sum()/ni.sum()
    print("Выборочная дисперсия Dв - это среднее арифметическое квадратов отклонений всех вариант выборки от её средней")
    print("Найдена по формуле ((x1 - xв)^2 *n1 + (x2 - xв)^2 *n2 + ... + (xk - xв)^2 *nk)/n : ", dispersion)
    S = mt.sqrt(dispersion * ni.sum() / (ni.sum() - 1))
    print("Исправленное выборочное среднеквадратичное отклонение S по формуле sqrt(n*Dв/(n-1)) : ", S)

def thirdTask():
    intervals = [[], []]
    variant = []
    #             ---
    # етот чел сверху смотрит прямо в душу

    count = int(input('Число разбиений интервального ряда:'))

    # ввод интервалов
    for i in range(count):
        buff = [float(input('вводи начало интервала: ')),
                float(input('вводи конец интервала: '))]

        intervals[0].append(buff[0])
        intervals[1].append(buff[1])

        variant.append((intervals[0][i]+intervals[1][i])/2)
    ni = []
    for i in range(count):
        ni.append(int(input('Введите число появлений значений из ' +
                            str(i+1) + ' интервального ряда: ')))

    xi = np.array(variant)
    ni = np.array(ni)
    wi = ni / ni.sum()

    showVariationRow(xi, ni, wi)
    showPoligon(xi, wi)
    showFunctionRaspr(xi, wi)

    # хызы что дальше
    F = np.full(count+3, 0, dtype=float)
    variantsForFunc = np.full(count+3, 0, dtype=float)

    variantsForFunc[1] = intervals[0][0]
    for i in range(0, count):
        variantsForFunc[i + 2] = intervals[1][i]
        for j in range(i):
            F[i + 1] += wi[j]
    F[count + 1] = 1
    F[count + 2] = 1

    variantsForFunc[count + 2]=intervals[1][count - 1] + 5
    # for i in range(0, count+3):
    #     print(variantsForFunc[i], F[i])

    showPoligon(variantsForFunc, F)

    print("Размах интервального ряда:",
          intervals[1][count - 1] - intervals[0][0])

    selectiveAverage = (xi * ni).sum()/ni.sum()

    print("Выборочное среднее, найденное по формуле xв = (x1*n1 + x2*n2 + ... + xk*nk)/n :", selectiveAverage)

    dispersion = (np.power(xi - selectiveAverage, 2) * ni).sum() / ni.sum()

    print("Выборочная дисперсия, найденное по формуле xв = ((x1 - xср)*n1 + (x2 - xср)*n2 + ... + (xk - xср)*nk)/n :", dispersion)

def main():
    request = int(input('Задание: '))
    if request == 1 or request == 2:
        firstTask()
    elif request == 3:
        thirdTask()
    else:
        print('Задание not found')

main()
input(' жмяк ')