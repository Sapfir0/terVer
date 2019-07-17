#include "mainwindow.h"
#include <QApplication>
#include "tasksolutionwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    taskSolutionWindow w2;

    w.show();

    w.connect(&w, SIGNAL(hided()), &w2, SLOT(showing()));
    w2.connect(&w2, SIGNAL(hided()), &w, SLOT(showing()));

    return a.exec();
}
