#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QVector>
#include <functional> //мы не будем юзать функ стиль //поздно

#include <kyplukBigInteger.h>
#include <kyplukSmartPtr.h>

#include "tasksolutionwindow.h"


using big_int = kypluk::unlimInt;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    signals:
        void hided();
    public slots:
        void computing();
        void showing();
    private slots:
        void on_calculateBtn_clicked();

        void on_formul_currentIndexChanged(int index);

        //нужно спроектировать это ниже
        void on_spinBox_K_valueChanged(int arg1);

        void checking();

        void on_spinBox_N_valueChanged(int arg1);

        void on_pushButton_clicked();

private:
        QVector<SharedPtr<QSpinBox>> listSpinbox; //оставлю
        QVector<QPixmap> pixmp;
        QVector<QString> prefix;
        //Тип тут будет кусок функциональщины это будет выглядить сложно но на самом деле изи в плане меньшего написания кода (куча иф элсе)
        //callback => big_int function_name (big_int k, QVector<big_int>);
        QVector <big_int (*)(QVector<big_int>)> func;//ухх сложна ебат

        Ui::MainWindow *ui;
        int currentFormul; 



};

#endif // MAINWINDOW_H
