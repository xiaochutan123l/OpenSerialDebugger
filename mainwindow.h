#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "qcustomplot.h"
#include "serialController.h"
#include "serialmonitor.h"
#include "serialplotter.h"
#include "serialsendmonitor.h"
#include "parametermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SerialController *m_serial_controller;
    serialMonitor *m_serial_monitor;
    serialPlotter *m_serial_plotter;
    parameterManager *m_parameter_manager;
    serialSendMonitor *m_serial_send_monitor;
};
#endif // MAINWINDOW_H
