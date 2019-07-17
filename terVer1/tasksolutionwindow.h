#ifndef TASKSOLUTIONWINDOW_H
#define TASKSOLUTIONWINDOW_H

#include <QMainWindow>
#include <kyplukSmartPtr.h>
#include <QSpinBox>
#include <QLabel>

namespace Ui {
class taskSolutionWindow;
}

class taskSolutionWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit taskSolutionWindow(QWidget *parent = nullptr);
        ~taskSolutionWindow();
    signals:
        void hided();
    public slots:
        void showing();
        void computing();

    private slots:
        void on_pushButton_clicked();

        void on_changeTask_currentIndexChanged(int index);

        void on_spinBox_N_valueChanged(int arg1);

        void on_spinBox_M_valueChanged(int arg1);

        void enableEnteringLabel(int arg1, QLabel *enter);

        void addDynamicSpinbox(int staticVariable, int arg1, QLayout* layout, QVector<SharedPtr<QSpinBox> > &vect);

    private:
        Ui::taskSolutionWindow *ui;
        QVector<QString> TaskText;
        QVector<QPixmap> formuls;

        QVector<SharedPtr<QSpinBox>> mSpinbox;
        QVector<SharedPtr<QSpinBox>> nSpinbox;

        int currentFormul;
};

#endif // TASKSOLUTIONWINDOW_H
