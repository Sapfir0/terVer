#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <kyplukDecimal.h>

namespace Ui {
class MainWindow;
}

enum class typeOFBernulli {
    equal = 0,
    menshe,
    bolshe,
    interval
};

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_comboBox_currentIndexChanged(int index);

        void on_nSpinbox_valueChanged(double arg1);

        void on_pSpinbox_valueChanged(double arg1);

        void on_m1Spinbox_valueChanged(double arg1);

        void on_m2Spinbox_valueChanged(double arg1);

        typeOFBernulli getTypeOFBernulli();

        void takeValues();
        void calculate();

        void showRadioBtn(bool boo);
        void showM2();

    private:

        Ui::MainWindow *ui;
        QVector<double> results;
};

#endif // MAINWINDOW_H
