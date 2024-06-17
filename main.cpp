#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("OpenSerialDebugger");
    w.show();
    return a.exec();
}


/*
TODO List:
- main window appearing position
- serial default parameters
- serial monitor text browser max buffer size
- fake motor protocol
- what if plot number of data changed. 2 curves -> 3curve?
- serial plotter max buffer size? ring buffer? then how efficient (simple popfront and append?)
- add "save" and clear button for plotter
- dynamical x axis zoom
*/
