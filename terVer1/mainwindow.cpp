#include "mainwindow.h"

#include "kmath.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //так же легче да?
    pixmp.append(QPixmap(":/new/prefix1/formulas/C.png"));
    pixmp.append(QPixmap(":/new/prefix1/formulas/A.png"));
    pixmp.append(QPixmap(":/new/prefix1/formulas/P.png"));

    pixmp.append(QPixmap(":/new/prefix1/formulas/2019-02-27_20-38-54.png"));
    pixmp.append(QPixmap(":/new/prefix1/formulas/2019-02-27_20-38-38.png"));
    pixmp.append(QPixmap(":/new/prefix1/formulas/2019-02-27_20-39-25.png"));



    prefix.append("C");
    prefix.append("Ã");
    prefix.append("P");

    prefix.append("A");
    prefix.append("P");
    prefix.append("Ĉ");

    func.append(combinationsWithoutRepetition);
    func.append(placementWithRepetitions);
    func.append(permutationsWithRepetitions);
    func.append(permutationsWithoutRepetitions);
    func.append(permutationsWithoutRepetitionschs);
    func.append(combinationsWithRepetition);

    currentFormul=0;
    ui->spinBox_K->setMaximum(currentFormul == 0 ? 0 : 100);


    ui->img->setPixmap(pixmp.front());
    connect(ui->spinBox_N, SIGNAL(valueChanged(int)), this, SLOT(computing()));

    ui->labelInputNk->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateBtn_clicked() //устранить, хочу автоматику
{

}

void MainWindow::checking() {
    //QVector <kypluk::unlimInt> Nki;
    int32_t sum=0;
    for (auto item : listSpinbox) {
        sum += item->value();
    }
    if (currentFormul==2)
        ui->spinBox_N->setValue(sum);
}
//fix me
void MainWindow::on_spinBox_K_valueChanged(int arg1)
{
    static int tempArg1 = 0;
    if (currentFormul==2) { //тут нужны динамические элементы
    //создать k спинбоксов
        if (tempArg1 < arg1) {
            for (int i = 0; i < arg1 - tempArg1; i++) {
                QSpinBox *spinbox = new QSpinBox;
                listSpinbox.push_back(spinbox);
                ui->n_layout->addWidget(listSpinbox.back().get());
                connect(listSpinbox.back().get(), SIGNAL(valueChanged(int)), this, SLOT(computing()));
            }
        }
        else if(tempArg1>arg1 && !listSpinbox.isEmpty()) {
        //если число уменьшается, удалить последний спинбокс
            //deletelistSpinbox.back();
            for (int i = 0; i < tempArg1 - arg1; i++) listSpinbox.pop_back();
        }
    } else {
        listSpinbox.clear();
    }
    tempArg1 = arg1;

    computing();
}


void MainWindow::computing() {
    checking();
    //how we can track app state omagad
    kypluk::unlimInt result;

    QVector <kypluk::unlimInt> Nki;
    Nki.append(ui->spinBox_N->value());

    if (currentFormul != 2)
        Nki.append(ui->spinBox_K->value());

    //собираем все энки с интерфейса

    for (auto item : listSpinbox) {
        Nki.append(item->value());
    }


    result = func[currentFormul](Nki);

    //ui->result->setText( &(*kypluk::unlimInt::to_vstring(result).begin()) ); //ебать
    //по просьбе младенцев было сделано упрощение
    ui->result->setText( kypluk::unlimInt::to_vstring(result).data() );


}


void MainWindow::on_formul_currentIndexChanged(int index)
{
    currentFormul = index;
    ui->img->setPixmap(pixmp[currentFormul]);
    ui->prefix->setText(prefix[currentFormul]);
    //мне пока лень думать на этим
    //ui->img->setToolTip("Если среди n элементов есть n1 элементов одного вида, n2 элементов другого вида и т.д. (т.е. n=n1+n2+ ... + nk), то можно юзнуть эту формулу");
    ui->spinBox_K->setValue(0);
    ui->spinBox_N->setValue(0);
    //если сменили текущую формулу с 3 на любую другую
    //удалить все созданные спинбоксы
    bool boolValue = currentFormul==2;
    ui->spinBox_N->setDisabled(boolValue);
    ui->labelInputNk->setVisible(boolValue);

    on_spinBox_N_valueChanged(0);
    //не пали это
    if (currentFormul == 1 or currentFormul == 5) {
        ui->chark->setText("m: ");
    }
    else {
        ui->chark->setText("K: ");
    }
}


void MainWindow::on_spinBox_N_valueChanged(int arg1)
{
    //Говнокод нужно сделать по нормальному
    ui->spinBox_K->setMaximum(currentFormul == 0 || currentFormul == 3 ? arg1 : currentFormul == 2 ? 30 : 100);
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    emit hided();
}

void MainWindow::showing() {
    this->show();
}
