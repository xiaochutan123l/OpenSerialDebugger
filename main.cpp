#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QIcon icon(":/icons/icons/logo.ico");
    w.setWindowIcon(icon);
    w.setWindowTitle("OpenSerialDebugger");
    w.show();
    return a.exec();
}


/*
TODO List:
- serial monitor text browser max buffer size
- command add print function
- add data range verification (lineedit validator?)
- send history update after got success sent out
- check command ptr if is null and also like type, name ... validation
- add save pdf, png... function
- add clear button for send history
*/
