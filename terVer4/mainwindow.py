#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5 import QtWidgets
from build import mainwindow
from PyQt5.QtWidgets import QApplication, QMainWindow, QGridLayout, QWidget, QTableWidget, QTableWidgetItem, QMessageBox
import math as mt
#import matplotlib as mp
import matplotlib.pyplot as plt
import numpy as np
from PyQt5.QtGui import QPixmap



class terver(QtWidgets.QMainWindow, mainwindow.Ui_MainWindow):
    
    def getXandY(self):
        x = []; y = []
        for i in range(0, self.countSpinBox.value()): #первый фановый забег по таблице
            if self.tableWidget.item(0,i) == None or self.tableWidget.item(1,i) == None:
                QMessageBox.question(self, 'PyQt5 message', "Не все ячейки заполнены", QMessageBox.Yes)
                return [],[]

        for i in range(0, self.countSpinBox.value()): #мы же вроде уже взяли значение из таблицы
            x.append(float(self.tableWidget.item(0, i).text()))
            y.append(float(self.tableWidget.item(1, i).text()))

        print("take values from table: ", x, y)
        return x,y

    def change(self, index):
        xyu = index != 0
        
        self.dSpinBox.setVisible(xyu)
        self.NSpinBox.setVisible(xyu)
        self.pSpinBox.setVisible(not xyu)
        #str = "n должен быть строго меньше N. "
        if (self.comboBox.currentIndex() == 0):
            self.formul.setPixmap(QPixmap("./binom.jpg"))
            self.guide.setText("")
        else:
            self.formul.setPixmap(QPixmap("./hyper.jpg"))
            self.guide.setText("N - количество всех элементов, n - объем выборки, D - элементы с нужным свойством," + " где n <= N.") 




    def __init__(self, ): #конструктор 
        super().__init__()
        self.setupUi(self)  

        self.countSpinBox.valueChanged.connect(self.setValuesToTable)
        self.graphic.clicked.connect(self.takeGraphicRasp)
        self.graphic2.clicked.connect(self.squareRasp)
        self.comboBox.currentIndexChanged.connect(self.change)
        self.resoltingButton.clicked.connect(self.resolting)

        self.dSpinBox.setVisible(False)
        self.NSpinBox.setVisible(False)
        self.nSpinBox.setVisible(True)  
        self.formul.setPixmap(QPixmap("./binom.jpg")) 
    

    def takeGraphicRasp(self): 
        x,y = self.getXandY()
        print(x)
        print(y)
        
        createGraphicRasp(x,y)


    def squareRasp(self):
        x,y = self.getXandY()
        createSquareRasp(x,y)

    def calculate(self):
        mx, dx = " ", " "; tempMx, tempMxSq = 0, 0
        n = self.nSpinBox.value()
        p = self.pSpinBox.value()
        mx="M(X)=n*p="+str(n*p)

        dx="D(X)=n*p*q="+str(n*p*(1-p))
        
        x,y = self.getXandY()
        for i in range(0, len(x)):
            tempMx += x[i]*y[i] 
            tempMxSq += pow(x[i],2) * y[i] 
        dx = str(tempMxSq- tempMx*tempMx )
        mx = str(tempMx)

        self.Mx.setText(mx)
        self.Dx.setText(dx)

    def resolting(self):
        count = self.countSpinBox.value()
        for i in range(0, count):
            value = float(self.tableWidget.item(0, i).text())
            item = None
            if self.comboBox.currentIndex() == 0:  
                item = binom(count-1,i,self.pSpinBox.value()) 
            else:
                item = hyperPheometry(self.NSpinBox.value(), self.nSpinBox.value()-value, self.nSpinBox.value(), self.NSpinBox.value()-self.dSpinBox.value()) #N,k,n, D
            print(item)
            self.tableWidget.setItem(1, i, QTableWidgetItem("%.7f" % item))
        self.tableWidget.resizeColumnsToContents()
        self.calculate()
            
    def setValuesToTable(self):
        self.tableWidget.setRowCount(2) #
        count = self.countSpinBox.value()
        self.tableWidget.setColumnCount(count) #
        
        for i in range(0, count):
            self.tableWidget.setHorizontalHeaderItem(i, QTableWidgetItem(" ")) #
        
            

def createSquareRasp(x,y):
    plt.ylim ([-0.01, max(y)+(max(y)/10)])
    plt.xlim([min(x)-1, max(x)+2])
    plt.plot(x,y)
    plt.grid()
    plt.show()

def createGraphicRasp(x,y):
    arrowstyle = {
        'arrowstyle': "->"
    }

    plt.ylim ([-0.01, 1.1])
    plt.xlim([min(x)-1, max(x)+2])
    for i in range(1, len(y)):
        y[i] += y[i-1]
    # лыл
    head_w = 0.01
    plt.hlines(y=0, xmin=-56465, xmax=min(x), linewidth=1, color='000000')
    for i in range(0, len(y)-1):
        plt.arrow (x[i+1], y[i], -(x[i+1]-x[i]), 0,
            head_width = head_w)
    # в идеале здесь должна быть линия уходящая в бесконечность
    dxinf = 10000
    plt.arrow ( max(x)+dxinf, 1, -dxinf, 0,
        head_width = head_w)
    plt.grid()
    plt.show()



def binom(n,k,p):
    C= mt.factorial(n) / ( mt.factorial( n-k ) * mt.factorial(k) )
    p1 = mt.pow(p,k)
    q = mt.pow((1 - p), (n-k))
    return C*p1*q


def hyperPheometry(N,k,n, D):
    print(N,k,n,D)
    return (fnC(k,D) * fnC (n-k, N-D))/fnC(n, N)


def fnC(k,n):
    print(n, k, n-k)
    if (n-k < 0):
        print("Отрицательные значения 1")
        return 0
    
    C = mt.factorial(n) / ( mt.factorial( n-k ) * mt.factorial(k) )
    return C

