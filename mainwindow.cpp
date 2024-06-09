#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup_display_plot();
    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
    // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
    // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
    // just increase the minimum/maximum values of the scroll bars as needed.
    ui->display_horizontalScrollBar->setRange(-500, 500);
    ui->display_verticalScrollBar->setRange(-500, 500);

    // create connection between axes and scroll bars:
    connect(ui->display_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_horzScrollBarChanged(int)));
    connect(ui->display_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_vertScrollBarChanged(int)));
    connect(ui->display_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->display_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    // initialize axis range (and scroll bar positions via signals we just connected):
    ui->display_plot->xAxis->setRange(0, 6, Qt::AlignCenter);
    ui->display_plot->yAxis->setRange(0, 10, Qt::AlignCenter);

    // Set serial port combo boxes.
    ui->comboBox_Baudrate->addItems(m_serial_controller.m_Bauderate_list_str);
    ui->comboBox_PartBit->addItems(m_serial_controller.m_Parity_list_str);
    ui->comboBox_DataBits->addItems(m_serial_controller.m_Databits_list_str);
    ui->comboBox_StopBits->addItems(m_serial_controller.m_Stopbits_list_str);
    ui->comboBox_FlowControl->addItems(m_serial_controller.m_Flowctr_list_str);

    connect(ui->comboBox_Baudrate, &QComboBox::activated, &m_serial_controller, &SerialController::onBaudRateSelected);
    connect(ui->comboBox_PartBit, &QComboBox::activated, &m_serial_controller, &SerialController::onParitySelected);
    connect(ui->comboBox_DataBits, &QComboBox::activated, &m_serial_controller, &SerialController::onDataBitsSelected);
    connect(ui->comboBox_StopBits, &QComboBox::activated, &m_serial_controller, &SerialController::onStopBitsSelected);
    connect(ui->comboBox_FlowControl, &QComboBox::activated, &m_serial_controller, &SerialController::onFlowControlSelected);
    connect(ui->comboBox_COM, &ClickableComboBox::activated, &m_serial_controller, &SerialController::onComSelected);
    connect(ui->comboBox_COM, &ClickableComboBox::clicked, &m_serial_controller, &SerialController::onComClicked);
    connect(ui->pushButton_serialConnect, &QPushButton::clicked, &m_serial_controller, &SerialController::onConnectClicked);
    connect(ui->pushButton_serialDisconnect, &QPushButton::clicked, &m_serial_controller, &SerialController::onDisconnectClicked);

    connect(&m_serial_controller, &SerialController::portListUpdate, this, &MainWindow::portListUpdate);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::portListUpdate(QList<QString> &port_name_list){
    ui->comboBox_COM->clear();
    ui->comboBox_COM->addItems(port_name_list);
}

void MainWindow::setup_display_plot()
{
  // The following display_plot setup is mostly taken from the display_plot demos:
  ui->display_plot->addGraph();
  ui->display_plot->graph()->setPen(QPen(Qt::blue));
  ui->display_plot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
  ui->display_plot->addGraph();
  ui->display_plot->graph()->setPen(QPen(Qt::red));
  QVector<double> x(500), y0(500), y1(500);
  for (int i=0; i<500; ++i)
  {
    x[i] = (i/499.0-0.5)*10;
    y0[i] = qExp(-x[i]*x[i]*0.25)*qSin(x[i]*5)*5;
    y1[i] = qExp(-x[i]*x[i]*0.25)*5;
  }
  ui->display_plot->graph(0)->setData(x, y0);
  ui->display_plot->graph(1)->setData(x, y1);
  ui->display_plot->axisRect()->setupFullAxesBox(true);
  ui->display_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void MainWindow::display_horzScrollBarChanged(int value)
{
  if (qAbs(ui->display_plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->display_plot->xAxis->setRange(value/100.0, ui->display_plot->xAxis->range().size(), Qt::AlignCenter);
    ui->display_plot->replot();
  }
}

void MainWindow::display_vertScrollBarChanged(int value)
{
  if (qAbs(ui->display_plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->display_plot->yAxis->setRange(-value/100.0, ui->display_plot->yAxis->range().size(), Qt::AlignCenter);
    ui->display_plot->replot();
  }
}

void MainWindow::xAxisChanged(QCPRange range)
{
  ui->display_horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  ui->display_horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void MainWindow::yAxisChanged(QCPRange range)
{
  ui->display_verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  ui->display_verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}
