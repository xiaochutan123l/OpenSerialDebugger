#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "serialController.h"
#include "serialmonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setup_display_plot();

private slots:
  void display_horzScrollBarChanged(int value);
  void display_vertScrollBarChanged(int value);
  void xAxisChanged(QCPRange range);
  void yAxisChanged(QCPRange range);

  void portListUpdate(QList<QString> &port_name_list);


private:
    Ui::MainWindow *ui;

    SerialController *m_serial_controller;
    serialMonitor *m_serial_monitor;
};
#endif // MAINWINDOW_H
