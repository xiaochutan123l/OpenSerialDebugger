#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    /* --------------------  Serial port  --------------------------*/

    m_serial_controller = new SerialController( this,
                                                ui->label_connectStatus,
                                               ui->comboBox_COM,
                                                ui->comboBox_Baudrate,
                                                ui->comboBox_PartBit,
                                                ui->comboBox_DataBits,
                                                ui->comboBox_StopBits,
                                               ui->comboBox_FlowControl);

    connect(ui->pushButton_serialConnect, &QPushButton::clicked, m_serial_controller, &SerialController::onConnectClicked);
    connect(ui->pushButton_serialDisconnect, &QPushButton::clicked, m_serial_controller, &SerialController::onDisconnectClicked);

   //onnect(m_serial_controller, &SerialController::portListUpdate, this, &MainWindow::portListUpdate);

    /* ----------------------- Serial monitor --------------------------*/
    m_serial_monitor = new serialMonitor(this, ui->textBrowser_serial_monitor, ui->pushButton_serialBufferClear);
    connect(m_serial_controller, &SerialController::dataReceived, m_serial_monitor, &serialMonitor::onSerialDataReceived);

    /* ----------------------- Serial plotter --------------------------*/
    // TODO: added two undifined buttons
    m_serial_plotter = new serialPlotter(this,
                                         ui->pushButton_plotter_clear,
                                         ui->pushButton_plotter_save,
                                         ui->pushButton_plotter_stop,
                                         ui->display_plot,
                                         ui->display_verticalScrollBar,
                                         ui->display_horizontalScrollBar);
    connect(m_serial_monitor, &serialMonitor::newLinesReceived, m_serial_plotter, &serialPlotter::onNewLinesReceived);

    ui->display_plot->xAxis->setRange(0, 6, Qt::AlignCenter);
    ui->display_plot->yAxis->setRange(0, 10, Qt::AlignCenter);

    /* ----------------------- Serial send monitor --------------------------*/
    m_serial_send_monitor = new serialSendMonitor(ui->pushButton_serialSend, ui->lineEdit_serialSend, ui->textBrowser_send_history, this);
    m_serial_send_monitor->connect_widgets();
    connect(m_serial_send_monitor, &serialSendMonitor::sendCommand, m_serial_controller, &SerialController::onSendCommand);
    connect(m_serial_send_monitor, &serialSendMonitor::sendMessage, m_serial_controller, &SerialController::onSendMessage);

    /* ----------------------- parameters --------------------------*/
    m_parameter_manager = new parameterManager(ui->pushButton_load_parameters, this);

    m_parameter_manager->addGetComboWidget(ui->pushButton_get1, ui->lineEdit_get1, ui->comboBox_get1);
    m_parameter_manager->addGetComboWidget(ui->pushButton_get2, ui->lineEdit_get2, ui->comboBox_get2);
    m_parameter_manager->addGetComboWidget(ui->pushButton_get3, ui->lineEdit_get3, ui->comboBox_get3);
    m_parameter_manager->addGetComboWidget(ui->pushButton_get4, ui->lineEdit_get4, ui->comboBox_get4);
    m_parameter_manager->addGetComboWidget(ui->pushButton_get5, ui->lineEdit_get5, ui->comboBox_get5);
    m_parameter_manager->addGetComboWidget(ui->pushButton_get6, ui->lineEdit_get6, ui->comboBox_get6);

    m_parameter_manager->addSetComboWidget(ui->pushButton_send1, ui->lineEdit_send1, ui->comboBox_send1);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send2, ui->lineEdit_send2, ui->comboBox_send2);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send3, ui->lineEdit_send3, ui->comboBox_send3);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send4, ui->lineEdit_send4, ui->comboBox_send4);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send5, ui->lineEdit_send5, ui->comboBox_send5);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send6, ui->lineEdit_send6, ui->comboBox_send6);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send7, ui->lineEdit_send7, ui->comboBox_send7);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send8, ui->lineEdit_send8, ui->comboBox_send8);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send9, ui->lineEdit_send9, ui->comboBox_send9);
    m_parameter_manager->addSetComboWidget(ui->pushButton_send10, ui->lineEdit_send10, ui->comboBox_send10);

    m_parameter_manager->addSwitchComboWidget(ui->pushButton_action1, ui->comboBox_action1);
    m_parameter_manager->addSwitchComboWidget(ui->pushButton_action2, ui->comboBox_action2);
    m_parameter_manager->addSwitchComboWidget(ui->pushButton_action3, ui->comboBox_action3);
    m_parameter_manager->addSwitchComboWidget(ui->pushButton_action4, ui->comboBox_action4);

    //connect(m_parameter_manager, &parameterManager::sendCommandBytes, m_serial_controller, &SerialController::onSendCommand);
    connect(m_parameter_manager, &parameterManager::sendCommandBytes, m_serial_send_monitor, &serialSendMonitor::onSendCommand);
    connect(m_serial_monitor, &serialMonitor::updateGetParameter, m_parameter_manager, &parameterManager::onUpdateGetParameter);

}

MainWindow::~MainWindow()
{
//    delete m_serial_controller;
//    delete m_serial_monitor;
//    delete m_serial_plotter;
    qDebug() <<  "start delete ui";
    delete ui;
    qDebug() <<  "delete ui done";

}
