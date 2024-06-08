#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
