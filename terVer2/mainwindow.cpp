#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <elements.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto elementOne = virtualElement(virtualElement::TypeSerial).addElement(realElement("q1")).addElement(virtualElement(virtualElement::TypeParallel).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement("q2")).addElement(virtualElement(virtualElement::TypeParallel).addElement(realElement("q3")).addElement(realElement("q4")))).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement("q5")).addElement(realElement("q6"))));
    auto elemTwoLineOne = virtualElement(virtualElement::TypeSerial).addElement(realElement("q1")).addElement(realElement("q2")).addElement(realElement("q3")),
             elemTwoLineTwo = virtualElement(virtualElement::TypeSerial).addElement(realElement("q4")).addElement(realElement("q5")),
             elemTwoLineThree = virtualElement(virtualElement::TypeSerial).addElement(realElement("q6")).addElement(virtualElement(virtualElement::TypeParallel).addElement(realElement("q7")).addElement(realElement("q8")));
    auto elementTwo = virtualElement(virtualElement::TypeParallel).addElement(realElement("P(A)")).addElement(realElement("P(B)")).addElement(realElement("P(C)"));

    //for sanya
    functions.append([](QVector<decimal> values){
        decimal q1 = decimal(1) - values[0],
                q2 = decimal(1) - values[1],
                q3 = decimal(1) - values[2],
                q4 = decimal(1) - values[3],
                q5 = decimal(1) - values[4],
                q6 = decimal(1) - values[5];

        auto res = decimal(1) - virtualElement(virtualElement::TypeSerial).addElement(realElement(q1, "q1")).addElement(virtualElement(virtualElement::TypeParallel).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement(q2, "q2")).addElement(virtualElement(virtualElement::TypeParallel).addElement(realElement(q3, "q3")).addElement(realElement(q4, "q4")))).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement(q5, "q5")).addElement(realElement(q6, "q6")))).calculate_breaks();
        return "Результат: " + QString(decimal::to_vstring(res).data());
    });
    //FIXME
    functions.append([](QVector<decimal> values) {
        decimal q1 = values[0],
                q2 = values[1],
                q3 = values[2],
                q4 = values[3],
                q5 = values[4],
                q6 = values[5],
                q7 = values[6],
                q8 = values[7];

        auto res = virtualElement(virtualElement::TypeParallel).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement(q1, "q1")).addElement(realElement(q2, "q2")).addElement(realElement(q3, "q3"))).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement(q4, "q4")).addElement(realElement(q5, "q5"))).addElement(virtualElement(virtualElement::TypeSerial).addElement(realElement(q6, "q6")).addElement(virtualElement(virtualElement::TypeParallel).addElement(realElement(q7, "q7")).addElement(realElement(q8, "q8")))).calculate_breaks();
        return "Результат: "+QString(res.to_vstring(res).data());
    });
    functions.append([](QVector<decimal> values) {
        decimal a1 = values[0];
        decimal a2 = values[1];
        decimal a3 = values[2];
        decimal a4 = values[3];
        decimal na1 = decimal(1)-a1;
        decimal na2 = decimal(1)-a2;
        decimal na3 = decimal(1)-a3;
        decimal na4 = decimal(1)-a4;
        decimal a = (a1*na2*na3*na4)+(na1*a2*na3*na4)+(na1*na2*a3*na4)+(na1*na2*na3*a4);
        decimal b = (na1*na2*a3*na4);
        decimal c = (a1*a2*na3*na4)+(a1*na2*a3*na4)+(a1*na2*na3*a4)+(na1*a2*a3*na4)+(na1*a2*na3*a4)+(na1*na2*a3*a4);
        decimal d = decimal(1)-(na1*na2*na3*na4);
        QString res;
        res += "Результат а: " + QString(decimal::to_vstring(a).data()) + "\n";
        res += "Результат b: " + QString(decimal::to_vstring(b).data()) + "\n";
        res += "Результат c: " + QString(decimal::to_vstring(c).data()) + "\n";
        res += "Результат d: " + QString(decimal::to_vstring(d).data()) + "\n";
        return res;
    });

    functions.append([](QVector<decimal> values) { //полная вероятность
        decimal res = decimal(0);
        for (int i = 0; i < values.size()-1; i+=2) {
            res += values[i]*values[i+1];
        }
        return "Результат: "+QString(res.to_vstring(res).data());
    });

    functions.append([&](QVector<decimal> values) { //полная вероятность
        decimal res = decimal(0);
        for (int i = 0; i < values.size()-1; i+=2) {
            res += values[i]*values[i+1];
        }
        if (res != decimal(0) && !values.isEmpty()) {
            int gipoteza = (ui->gipoteza->value()-1)*2;
            decimal tmp = values[gipoteza]*values[gipoteza+1];
            res = tmp / res;
            return "Результат: "+QString(res.to_vstring(res).data());
        } else {
            return QString("Нет значения");
        }
    });
    pixmp.append(QPixmap(":/img/2019-03-13_15-48-55.png")); //их придется менять, но пока поставим первое изначально
    pixmp.append(QPixmap(":/img/Bn0ZtZ_7qL8.png"));
    pixmp.append(QPixmap());
    pixmp.append(QPixmap());
    pixmp.append(QPixmap());

//    QSize PicSize(100, 100);
//    for (auto& itempxmp : pixmp) {
//        itempxmp = itempxmp.scaled(PicSize,Qt::KeepAspectRatio);
//    }

    spiboxCount.append(6);
    spiboxCount.append(8);
    spiboxCount.append(4);
    spiboxCount.append(4);
    spiboxCount.append(8);

    formuls.append(elementOne.getFormul());
    formuls.append(QString("первая линия: P(A) = 1 - P(Ā1*Ā2*Ā3)") + "\nвторая линия: P(B) = P(A4) + P(A5) - P(A4)P(A5)" + "\nтретья линия: P(C) = P(A6) + P(A7)*P(A8) - P(A6)*P(A7)*P(A8)" + "\nОбщая формула: P(R) = P(A) * P(B) * P(C)" );
    formuls.append(QString(""));
    formuls.append(QString(""));
    formuls.append(QString(""));


    desclymerStr.append("");
    desclymerStr.append("");
    desclymerStr.append("Рабочий обслуживает 4 станка, работающих независимо друг от друга.\n"
                        "Вероятность того, что за смену i-ый станок потребует наладки, равна pi. Найти вероятность того, что за смену:\n"
                        "a) только один станок потребует наладки;\n"
                        "b) только третий станок потребует наладки;\n"
                        "c) только два станка потребуют наладки;\n"
                        "d)  хотя бы один станок потребует наладки;\n");

    desclymerStr.append("При производстве ноутбука Xiaomi на него возможна установка двух различных видов процессоров.\n"
                        "Известно, что в n% произведенных ноутбуков установлен процессор m3, а в оставшихся m% - i5.\n"
                        "Вероятность того, что в процессоре m3 не будет скрытых дефектов равна p1, а в процессоре i5 равна p2.\n"
                        "Алексей приобретает ноутбук, какова вероятность того, что купленное устройство не будет содержать\n"
                        "дефекта процессора?");

    desclymerStr.append("Команда разработчиков из 4-х человек работает над своим проектом.\n"
                        "Первый программист написал n% кода, второй написал m% кода, третий написал k% кода и четвертый l.\n"
                        "Вероятность того, что в коде была допущена ошибка равна p1 для первого, p2, для второго, p3 для третьего\n"
                        "и p4 для четвертого. При компиляции программы, собранной из функций, написанных разными программистами\n"
                        "возникла ошибка. Какова вероятность того, что ошибку допустил второй программист?");

    on_formulsType_currentIndexChanged(0);

    ui->count->setPrefix("Количество: ");
    ui->gipoteza->setPrefix("Гипотеза: ");
    ui->gipoteza->setMinimum(1);

    ui->gipoteza->setMaximumWidth(200);
    ui->count->setMaximumWidth(200);
    ui->count->setRange(1,10);

    ui->image->setScaledContents(true);

    connect(ui->gipoteza, SIGNAL(valueChanged(const QString &)), this, SLOT(calculate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_formulsType_currentIndexChanged(int index)
{
    currentFormul = index;

    ui->image->setPixmap(pixmp[currentFormul]);
    ui->image->setHidden(pixmp[currentFormul].isNull());

    spinboxList.clear();
    for (uint i = 0; i < spiboxCount[currentFormul]; ++i) {
        //есть варик поиграться так чтоб красиво выглядело
        auto spin = new QDoubleSpinBox();
        ui->spinbox_layout->addWidget(spin);
        spinboxList.push_back(spin);
        spin->setMaximum(1.0);
        spin->setMinimum(0.0);
        spin->setSingleStep(0.01);
        spin->setPrefix("Вероятность: ");
        spin->setDecimals(3);
        spin->setMaximumWidth(300);
        connect(spin, SIGNAL(valueChanged(const QString &)), this, SLOT(calculate()));
    }
    ui->gipoteza->setVisible(currentFormul == 4);
    ui->count->setHidden(currentFormul < 3);
    ui->problemDescr->setText(desclymerStr[currentFormul]);

    ui->formul->setText(formuls[currentFormul]);
    ui->formul->setHidden(formuls[currentFormul].isEmpty());
    ui->result->setText("Нет значения");
}


void MainWindow::calculate() {
    QVector <decimal> values;
    for (auto item : spinboxList) {
        values.push_back(decimal::from_string(item->cleanText().toStdString().c_str()));//я устал пока писал ету херню
    }
    auto res = functions[currentFormul](values);
    //вывод результата в гуи
    ui->result->setText(res);

}

void MainWindow::on_count_valueChanged(int arg1)
{
    spiboxCount[4] = spiboxCount[3] = arg1*2;
    ui->gipoteza->setMaximum(arg1 != 0 ? arg1 : 1);
    on_formulsType_currentIndexChanged(currentFormul);
}
