#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSpinBox>

#include <kyplukDecimal.h>
#include <kyplukSmartPtr.h>
#include <functional>

using decimal = kypluk::Decimal<12>;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    void setVis(bool t);


    public slots:
        void calculate();

    private slots:
        void on_formulsType_currentIndexChanged(int index);

        void on_count_valueChanged(int arg1);

private:
        Ui::MainWindow *ui;
        int currentFormul;

        QVector <std::function<QString(QVector<decimal>)>> functions;
        kypluk::Vector <kypluk::SharedPtr<QDoubleSpinBox>> spinboxList;
        QVector <uint> spiboxCount;
        QVector <QPixmap> pixmp;

        QVector <QString> formuls;
        QVector <QString> desclymerStr;

};

#endif // MAINWINDOW_H
