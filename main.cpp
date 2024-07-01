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
- serial monitor text browser max buffer size
- serial plotter max buffer size? ring buffer? then how efficient (simple popfront and append?)
- dynamical x axis zoom
- command add print function
- add data range verification (lineedit validator?)
- send history update after got success sent out
- check command ptr if is null and also like type, name ... validation
- optimize parameterComboWidget structure, virtual functions
- add save pdf, png... function
*/
