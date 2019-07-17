#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5 import QtWidgets
from build import mainwindow
from PyQt5.QtWidgets import QApplication, QMainWindow, QGridLayout, QWidget, QTableWidget, QTableWidgetItem, QMessageBox, QFileDialog
from PyQt5.QtGui import QPixmap
import math as mt
import matplotlib.pyplot as plt
import numpy as np

import json

def readDataFromFile(filename):
    file = open(filename, 'r')
    arr = json.loads(file.read())
    file.close()
    return arr

def calculateParametres(X, P):
    x = np.array(X)
    p = np.array(P)
    
    M = (x*p).sum()
    M2 = (np.square(x)*p).sum()

    D = M2 - mt.pow(M, 2)

    s = mt.sqrt(D)

    return M, D, s

class Interval:
    b = 0
    e = 0
    
    def __init__(self, start_value = 0, end_value = 0):
        if end_value >= start_value:
            self.b = start_value
            self.e = end_value
        else:
            self.b, self.e = start_value, start_value

    def c(self):
        return (self.e+self.b)/2


    def w(self):
        return self.e-self.b


class terver(QtWidgets.QMainWindow, mainwindow.Ui_mainWindowClass):
    cTable = [
        [6.6, 9.2, 11.3, 13.3, 15.1, 16.8, 18.5, 20.1, 21.7, 23.2],
		[5.0, 7.4, 9.4, 11.1, 12.8, 14.4, 16.0, 17.5, 19.0, 20.5],
		[3.8, 6.0, 7.8, 9.5, 11.1, 12.6, 14.1, 15.5, 16.9, 18.3]
    ]


    def __init__(self, ): #конструктор 
        super().__init__()
        self.setupUi(self)  

        self.radioButton_norm.clicked.connect(self.onRadioBtnClicked)
        self.radioButton_uniform.clicked.connect(self.onRadioBtnClicked)
        # чтоб не писать заново те три строчки кода
        self.onRadioBtnClicked()

        # connect (self.tableSize_1,		SIGNAL (valueChanged(int)),	this, SLOT (resizeTable1(int)))
        self.tableSize.valueChanged.connect(self.resizeTable)

        self.getFromFileButton.clicked.connect(self.onFileBtnClicked)

        self.calculateBtn.clicked.connect(self.calculate)

        # table
        self.CriticalTable.setRowCount(len(self.cTable[0]))
        for i in range(len(self.cTable)):
            for j in range(len(self.cTable[i])):
                item = QTableWidgetItem(str(self.cTable[i][j]))
                self.CriticalTable.setItem(j, i, item)

    def onRadioBtnClicked(self):
        imageName = "./img/" + ("formula_1.bmp" if (self.radioButton_norm.isChecked()) else "formula_2.bmp")
        pm = QPixmap(imageName)
        self.image.setPixmap(pm)

        indexSlackWidged = 0 if (self.radioButton_norm.isChecked()) else 1
        self.stackedWidget.setCurrentIndex(indexSlackWidged)

    def onFileBtnClicked(self):
        # filename = './data/test1.json'
        filename, _ = QFileDialog.getOpenFileName(self , "Открыть файл для занесения данных в таблицу", "", "json files(*.json)Text files(*.txt)All files(*.*)")
        
        if not filename:
            return
        arr = None
        try: 
            arr = readDataFromFile(filename)
        except:
            QMessageBox.warning(self, '', "Файл не верно заполнен", QMessageBox.Yes)
            return
        
        # Здесь еще что то (заполнение таблицы) но я неебумба
        
        uniqueValues = set(arr)

        # по формуле Стёрджеса рассчитываем число интервалов
        groupCount = round(1 + mt.log(float(len(uniqueValues)/mt.log(2.0))))

        if len(uniqueValues) < groupCount:
            groupCount = len(uniqueValues)

        if (groupCount < 4):
            QMessageBox.warning(self, '', "Разбитие производится на слишком малое кол-во интервалов.")
            return

        maximum = max(arr)
        minimum = min(arr)

        h = (maximum-minimum)/groupCount

        N = [0]*groupCount

        for i in range(len(arr)):
            group = int((arr[i]-minimum)/h)

            # без этого условия максимум будет помешаться за пределы массива N
            if (arr[i]==maximum):
                group = groupCount-1

            N[group] += 1
        
        self.resizeTable(groupCount)

        for i in range(groupCount):
            item = QTableWidgetItem(str((i*h)+minimum)+ ";" +str(((i+1)*h)+minimum))
            self.table.setItem(0, i, item)
            item = QTableWidgetItem(str(N[i]))
            self.table.setItem(1, i, item)

    def resizeTable(self, columnCount):
        self.table.setColumnCount(columnCount)
        self.k.setText("k = количество интервалов - 3 = "+str(columnCount-3))

    def calculate(self):
        X = [] # интервалы значений
        N = [] # соответствующие им частоты 
        W = [] # относительные частоты
        C = [] # середины интервалов

        for i in range(0, self.table.columnCount()):
            # Нужно чекать правильность данных, но нет
            # ок я проверю
            if (self.table.item(0, i) == None):
                QMessageBox.question(self, 'Ошибка', "Значения не верны", QMessageBox.Yes )
                return
            x_cur = self.table.item(0, i).text()
            n_cur = self.table.item(1, i).text()
            
            # парсим x_cur
            x_cur_list = x_cur.split(';')
            x1 = x_cur_list[0]
            x2 = x_cur_list[1]

            curx = Interval(float(x1), float(x2)) # це какая то структура хз для чего
            curn = int(n_cur)

            X.append(curx)
            N.append(curn)
            C.append(curx.c())

        N = np.array(N)
        C = np.array(C)

        W = N/N.sum()

        # заполнение таблички
        for i in range(len(W)):
            item = QTableWidgetItem("%.4f" % (W[i]))
            self.table.setItem(2, i, item)

        M, D, s = calculateParametres(C, W)

        self.Mx_1.setText("%.6f" % (M))
        self.Dx_1.setText("%.6f" % (D))
        self.Sx_1.setText("%.6f" % (s))

        a = M-1.7320508075*s
        b = M+1.7320508075*s

        self.a.setText("%.6f" % (M))
        self.Sx.setText("%.6f" % (s))

        self.a_2.setText("%.6f" % (a))
        self.b_2.setText("%.6f" % (b))

        self.table.resizeColumnsToContents() #добавим норм отображение

        p, hi, nexp, ncur = None, 0.0, None, None
        P = []
    	
        h = X[1].c() - X[0].c()
        summ = N.sum()
        for i in range(len(X)):
            if self.radioButton_norm.isChecked(): # формула: p=(h/s)*f(u)
                u = (X[i].c() - M)/s
                f = (1/2.5066282746)*mt.exp(-u*u/2)
                p = h*f/s
            else: # формула: p = (1/[b-1])*(x1-a) ||| (xi-xi-1) ||| (b-xn)
                if i == 0:				
                    p = (X[0].e - a)/(b-a)
                elif i == len(X)-1:	
                    p = (b - X[i].b)/(b-a)
                else:					
                    p = (X[i].e - X[i-1].e)/(b-a)
            nexp = summ*p
            ncur = int(self.table.item(1, i).text())
            hi += (nexp - ncur)*(nexp - ncur)/(nexp)
            P.append(p)
            item = QTableWidgetItem( "%.4f" % (p) ) 
            self.table.setItem(3, i, item)
    	
        self.hi_e.setText(str(hi))

        temp_str = self.k.text()
        temp_str = temp_str[temp_str.rindex('=')+1:]
        k = int(temp_str)
        index = self.alphaBox.currentIndex()

        informationStr = 'none'
        if k < 1:
            informationStr = "Слишком малое число степеней свободы."
        elif k > len(self.cTable[0]):
            informationStr = "Слишком большое число степеней свободы."
        elif self.cTable[index][k-1] > hi:
            informationStr = "Полученное значение критерия Пирсона не превышает критическое ( %s ), нет оснований отвергнуть гипотезу." % str(self.cTable[index][k-1])
        else: 
            informationStr = "Полученное значение критерия Пирсона превышает критическое ( %s ), есть смысл отвергнуть гипотезу." % str(self.cTable[index][k-1])
        self.VuvodText.setText(informationStr)

        # рисование графика
        w_max=0
        C_n=[]; W_n=[]

        if(self.radioButton_norm.isChecked()):
            P = []
            C_n = np.linspace(C[0], C[-1], 100)
            P = np.exp((-((C_n-M)*(C_n-M)))/(2*s*s))/(s*2.50662825325)
                  
            W_n = [W[i] / (X[i].e - X[i].b) for i in range(len(X))]
            
            w_max = max((max(P), max(W_n)))
            
            ymax = max([w_max,max(P)])
            ymax = ymax + (ymax/len(P))*100
            plt.xlim([X[0].b, X[len(X)-1].e])
            plt.ylim([0, ymax])

            for index, interval in enumerate(X):
                plt.hlines(W_n[index], interval.b, interval.e, colors='b', linestyles='dashed')
                plt.vlines(interval.b, 0, W_n[index], colors='b', linestyles='dashed')
                plt.vlines(interval.e, 0, W_n[index], colors='b', linestyles='dashed')

            plt.plot(C, W_n)
            
            if (self.radioButton_norm.isChecked()):
                print('puk true')
                plt.plot(C_n, P)
            else:
                print('puk')
                # self.g_1.scene().addLine(a*g.scaleX + g.offset, g.scaleY/(a+b), b*g.scaleX + g.offset, g.scaleY/(a+b), "r")
            
            plt.title('график')
            plt.grid()
            plt.show()