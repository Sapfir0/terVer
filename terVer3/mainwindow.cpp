#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "kmath.h"
#include "formuls.h"
#include <kyplukMath.h>
#include <QDebug>
#include <cmath>

using namespace kypluk;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showM2();
    connect(ui->oneBernulli, SIGNAL(clicked()), this, SLOT(showM2()));
    connect(ui->twoBernylli, SIGNAL(clicked()), this, SLOT(showM2())); //я не понял почему тут нельзя аргумент передать
    connect(ui->threeBernulli, SIGNAL(clicked()), this, SLOT(showM2()));
    connect(ui->fourBernulli, SIGNAL(clicked()), this, SLOT(showM2()));

    ui->formul->setPixmap(QPixmap(":/new/prefix1/img/2019-04-30_12-16-00.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showM2() {

    if (ui->fourBernulli->isChecked() && ui->comboBox->currentIndex() == 0)
        ui->m2Spinbox->setVisible(true);
    else {
        ui->m2Spinbox->setVisible(false);
    }
}

void MainWindow::showRadioBtn(bool boo) {
    ui->oneBernulli->setVisible(boo);
    ui->twoBernylli->setVisible(boo);
    ui->threeBernulli->setVisible(boo);
    ui->fourBernulli->setVisible(boo);

    ui->m2Spinbox->setVisible(!boo);
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
    ui->result->setText("");
    if (index==1) {
        showRadioBtn(false);
        ui->formul->setPixmap(QPixmap(":/new/prefix1/img/2019-04-30_12-15-19.png"));
    }
    else {
        showRadioBtn(true);
        ui->formul->setPixmap(QPixmap(":/new/prefix1/img/2019-04-30_12-16-00.png"));
    }
}

typeOFBernulli MainWindow::getTypeOFBernulli() {
    typeOFBernulli res;
    std::vector<QRadioButton*> radio = {ui->oneBernulli, ui->twoBernylli, ui->threeBernulli, ui->fourBernulli};
    for (int i = 0; i < radio.size(); i++) {
        if (radio[i]->isChecked()) {
            res = typeOFBernulli(i);
        }
    }
    return res;
}

const double pi = 3.1415926535;
double f(double x) {
    return (1.0/std::sqrt(2.0*pi)) *
            integral(0, x, [](double t) {
        return std::exp(-sqr(t)/2);
    }
    );
}

double integralLapplas(double n, double p, double m1, double m2) {
    double q = 1.0 - p;
    double one = (m2-n*p)/(std::sqrt(n*p*q));
    double two = (m1-n*p)/(std::sqrt(n*p*q));
    double res = f(one) - f(two);
    return res;

}

double bernulli(int, int, double);
double typedBernulli(int n, double p, int m1, int m2=0, typeOFBernulli type = typeOFBernulli::equal) {
    double res = 0;
    switch(type) {
        case typeOFBernulli::equal: {
            res = bernulli(n, m1, p);
            break;
        }
        case typeOFBernulli::menshe: {
            for (int i = 0; i < m1; i++) {
                res += bernulli(n, i, p);
            }
            break;
        }
        case typeOFBernulli::bolshe: {
            for (int i = m1; i <= n; i++) {
                res += bernulli(n, i, p);
            }
            break;
        }
        case typeOFBernulli::interval: {
            for (int i = m1; i <= (m2 > n ? n : m2); i++) {
                res += bernulli(n, i, p);
            }
            break;
        }
    }
    return res;
}

double bernulli(int n, int m, double p) {
    double q = 1- p;
    double res=0;
    double combination = QString(unlimInt::to_vstring(combinationsWithoutRepetition(n,m)).data()).toDouble();
    qDebug() << "combinations: " << combination;
    res = combination * pow(p,m) * pow(q,(n-m));
    return res;
}

void MainWindow::calculate() {
    QString res;
    if(ui->comboBox->currentIndex() == 0) {
        res= QString::number(typedBernulli(results[0], results[1], results[2], results[3], getTypeOFBernulli()), 'g', 10);
    }
    else {
        res= QString::number(integralLapplas( results[0], results[1], results[2], results[3]), 'g', 10);
        qDebug() << "считаем интегральную";
    }
    ui->result->setText( res );
}

void MainWindow::takeValues() {
    results.clear();

    results.append(ui->nSpinbox->value());
    results.append(ui->pSpinbox->value());
    results.append(ui->m1Spinbox->value());
    results.append(ui->m2Spinbox->value());

    //    for(int i=0; i<4; i++) {
    //        if (results.at(i) == 0 )
    //          return;
    //        qDebug() << results[i];
    //    }
    calculate();
}

void MainWindow::on_nSpinbox_valueChanged(double arg1)
{
    takeValues();
}

void MainWindow::on_pSpinbox_valueChanged(double arg1)
{
    takeValues();
}

void MainWindow::on_m1Spinbox_valueChanged(double arg1)
{
    takeValues();

}

void MainWindow::on_m2Spinbox_valueChanged(double arg1)
{
    takeValues();

}
