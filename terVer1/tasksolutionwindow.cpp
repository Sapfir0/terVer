#include "tasksolutionwindow.h"
#include "ui_tasksolutionwindow.h"
#include <QDebug>

#include "kmath.h"
#include <QSpinBox>
#include <kyplukBigInteger.h>
#include <kyplukDecimal.h>

using big_int = kypluk::unlimInt;
using fixed_float = kypluk::Decimal<30>;
//думаю о том, что нужно сделать кнопку просчета, так как нам нужна проерка введеных данных
//типа нам нужны тестовые N и M для каждой задачи
//чтобы юзер вводил неправильное значение у полей, жмякал на кнопку посчитать
//и получал на лицо
//псюх-псюх

taskSolutionWindow::taskSolutionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::taskSolutionWindow)
{
    ui->setupUi(this);


    TaskText.append("В отделение связи поступило m телеграмм, которые случайным образом распределяются по n каналам связи (n>m).Найти вероятность события A - на каждый канал придется не больше одной телеграммы");
    TaskText.append("В ящике имеется k ТЭЗ, из них k1 элементов 1-типа, ..., k1 элементов i-го типа, ..., km элементов m-ого типа. Из ящика выбирают наугад n ТЭЗ. Найти вероятность того, что серди них будет n1 ТЭЗ 1-го типа, ..., ni ТЭЗ i-го типа, ..., nm ТЭЗ m-го типа");
    TaskText.append("В кампанию по разработке программного обеспечения поступило 14 заказов проектов, они случайно распределяются между 25 сотрудниками. Найти вероятность события, при котором каждому сотруднику достанется не более одной задачи. ");
    TaskText.append("В лабораторию на фургоне завезли новое оборудование: 11 компьютеров, 18 мониторов, 12 клавиатур, 15 мышей для компьютера, что в сумме дает 56 единиц нового оборудования. Из фургона случайным образом достают 14 единиц оборудования. Найти вероятность того, что среди них будет 3 компьютера, 2 монитора, 5 клавиатур и 4 мыши.");

    formuls.append(QPixmap(":/tasks/task1.png"));
    formuls.append(QPixmap(":/tasks/task2NEW.png"));
    formuls.append(QPixmap(":/tasks/task1.png"));
    formuls.append(QPixmap(":/tasks/task2NEW.png"));



    ui->taskText->setWordWrap(true);
    currentFormul = 0;
    ui->spinBox_M->setMaximum(0);
    ui->spinBox_N->setMaximum(300); // Делай сосамбу //сам делай
    on_changeTask_currentIndexChanged(currentFormul);

    //connect(ui->spinBox_N, SIGNAL(valueChanged(int)), this, SLOT(computing()));

    ui->enterM->setVisible(false);
    ui->enterN->setVisible(false);
}

//бля вынеси этот мусор отсюда
//вынес епты

taskSolutionWindow::~taskSolutionWindow()
{
    delete ui;
}

void taskSolutionWindow::on_pushButton_clicked()
{
    this->hide();
    emit hided();
}

void taskSolutionWindow::showing() {
    this->show();
}

void taskSolutionWindow::computing() {
    fixed_float res;
    big_int N = ui->spinBox_N->value(), M = ui->spinBox_M->value();
    try {
        switch(currentFormul) {
            case 2:
            case 0: {
                QVector <big_int> Nki;
                Nki.append(N);
                Nki.append(M);
                fixed_float afirst = permutationsWithoutRepetitions<big_int>(Nki),
                        bsecond = placementWithRepetitions<big_int>(Nki);
                res = afirst / bsecond;

                break;
            }
            case 1:
            case 3: {
                QVector <big_int> multik;

                big_int sumN = 0, sumM = 0;

                for (int i = 0; i < mSpinbox.size(); i++) {
                    big_int M = mSpinbox[i]->value(), N = nSpinbox[i]->value();
                    sumN += N;
                    sumM += M;
                    QVector <big_int> puk;
                    puk.append(M); puk.append(N);
                    multik.append(combinationsWithoutRepetition(puk));
                }
                QVector <big_int> puks;
                puks.append(sumM); puks.append(sumN);
                big_int mult = 1;
                for (auto item : multik) {
                    mult *= item;
                }
                res = fixed_float(mult) / combinationsWithoutRepetition(puks);
            }
        }
        ui->resLabel->setText(fixed_float::to_vstring(res).data());
    } catch (big_int::division_by_zero puk) {
        ui->resLabel->setText("нет значений");
    }
}

void taskSolutionWindow::on_changeTask_currentIndexChanged(int index)
{
    currentFormul = index;

    ui->taskText->setText(TaskText[index]);
    ui->formul->setPixmap(formuls[currentFormul]);
    bool chek = currentFormul == 1 or currentFormul == 3;
    ui->enterM->setVisible(chek);
    ui->enterN->setVisible(chek);

    ui->spinBox_M->setValue(0);
    ui->spinBox_N->setValue(0);

    ui->spinBox_N->setVisible(!chek);
    ui->charn->setVisible(!chek);

    on_spinBox_N_valueChanged(0);

}

void taskSolutionWindow::enableEnteringLabel(int arg1, QLabel *enter) {
    if (arg1 >= 1) {
        enter->setVisible(true);
    }
    else if (arg1 <= 0) {
        enter->setVisible(false);
    }
}


void taskSolutionWindow::on_spinBox_N_valueChanged(int arg1)
{
    //qDebug() << currentFormul << arg1;
    if (currentFormul == 0 || currentFormul == 2) {
        ui->spinBox_M->setMaximum(arg1);
    } else {
        ui->spinBox_M->setMaximum(100);//fix
    }

    static int tempArg2 = 0;
    if (currentFormul==1 or currentFormul==3) { //тут нужны динамические элементы

        //enableEnteringLabel(arg1, ui->enterN);

        addDynamicSpinbox(tempArg2, arg1, ui->n_layout, nSpinbox); //нужно поправить функцию, я близко, но чет не робит

        tempArg2 = arg1;

    } else {
        nSpinbox.clear();
    }
    tempArg2 = arg1;
    computing();
}

void taskSolutionWindow::on_spinBox_M_valueChanged(int arg1)
{
    ////написать вместо этого говна нормальный код
    static int tempArg1 = 0;
    if (currentFormul==1 or currentFormul==3) { //тут нужны динамические элементы
        //enableEnteringLabel(arg1, ui->enterM);

        addDynamicSpinbox(tempArg1, arg1, ui->m_layout, mSpinbox); //нужно поправить функцию, я близко, но чет не робит
        addDynamicSpinbox(tempArg1,arg1, ui->n_layout, nSpinbox);
        tempArg1 = arg1;
    } else {
        mSpinbox.clear();
        nSpinbox.clear();

    }

    computing(); //какого черта это здесь
}
void taskSolutionWindow::addDynamicSpinbox(int staticVariable, int arg1, QLayout* layout, QVector<SharedPtr<QSpinBox>> &vect) {

    if (staticVariable < arg1) {
        for (int i = 0; i < arg1 - staticVariable; i++) {
            QSpinBox *spinbox = new QSpinBox; //возможен memory lick
            vect.push_back(spinbox);
            spinbox->setMaximum(200);
            connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(computing()));
            layout->addWidget(vect.back().get());
        }
    }
    else if(staticVariable>arg1 && !vect.isEmpty()) {
    //если число уменьшается, удалить последний спинбокс
        for (int i = 0; i < staticVariable - arg1; i++)
            vect.pop_back();
    }
}
