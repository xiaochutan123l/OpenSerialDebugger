#include "serialController.h"
#include <iostream>

QList<qint32> BaudRateList = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
QList<QSerialPort::Parity> ParityList = {
    QSerialPort::NoParity, QSerialPort::EvenParity, QSerialPort::OddParity,
    QSerialPort::MarkParity, QSerialPort::SpaceParity
};
QList<QSerialPort::DataBits> DataBitsList = {
    QSerialPort::Data8, QSerialPort::Data7, QSerialPort::Data6, QSerialPort::Data5
};
QList<QSerialPort::StopBits> StopBitsList = {
    QSerialPort::OneStop, QSerialPort::OneAndHalfStop, QSerialPort::TwoStop
};
QList<QSerialPort::FlowControl> FlowControlList = {
    QSerialPort::NoFlowControl, QSerialPort::HardwareControl, QSerialPort::SoftwareControl
};


/* ----------------- member functions --------------------*/
SerialController::SerialController(QObject *parent,
                                   QLabel *connectStatus,
                                   ClickableComboBox *com,
                                   QComboBox *baudrate,
                                   QComboBox *parity,
                                   QComboBox *databits,
                                   QComboBox *stopbits,
                                   QComboBox *flowcontrol)
    : QObject(parent),
    m_connectStatus(connectStatus),
    m_combobox_com(com),
    m_combobox_baudrate(baudrate),
    m_combobox_parity(parity),
    m_combobox_databits(databits),
    m_combobox_stopbits(stopbits),
    m_combobox_flowcontrol(flowcontrol)
{

    // create serial port.
#ifdef USE_FAKE_SERIAL
    m_port = new FakeSerialPort(this);
#else
    m_port = new QSerialPort(this);
#endif

    // serial info
    m_portInfo = new QSerialPortInfo();

    // init connection status
    updateConnectionStatus(false);

    // set parameter default value
    m_combobox_baudrate->addItems(m_Bauderate_list_str);
    m_combobox_parity->addItems(m_Parity_list_str);
    m_combobox_databits->addItems(m_Databits_list_str);
    m_combobox_stopbits->addItems(m_Stopbits_list_str);
    m_combobox_flowcontrol->addItems(m_Flowctr_list_str);

    m_combobox_com->addItem("COM1");
    m_com_name = "COM1";
    m_combobox_baudrate->setCurrentText(m_Bauderate_list_str[BAUDRATE_DEFUALT_INDEX]);
    m_baudrate = BaudRateList[BAUDRATE_DEFUALT_INDEX];
    m_parity = ParityList[0];
    m_databits = DataBitsList[0];
    m_stopbits = StopBitsList[0];
    m_flowcontrol = FlowControlList[0];

    connect(m_combobox_baudrate, &QComboBox::activated, this, &SerialController::onBaudRateSelected);
    connect(m_combobox_parity, &QComboBox::activated, this, &SerialController::onParitySelected);
    connect(m_combobox_databits, &QComboBox::activated, this, &SerialController::onDataBitsSelected);
    connect(m_combobox_stopbits, &QComboBox::activated, this, &SerialController::onStopBitsSelected);
    connect(m_combobox_flowcontrol, &QComboBox::activated, this, &SerialController::onFlowControlSelected);
    connect(m_combobox_com, &ClickableComboBox::activated, this, &SerialController::onComSelected);
    connect(m_combobox_com, &ClickableComboBox::clicked, this, &SerialController::onComClicked);

    // init timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerialController::onTimerTimeout);
    m_timer->setInterval(ReadDataTickTime);


#ifdef USE_FAKE_SERIAL
    connect(m_port, &FakeSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if (error != QSerialPort::NoError) {
            //qDebug() << "port error: " << m_port->errorString();
            closePort();
        }
    });
#else
    // error handling
    connect(m_port, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if (error != QSerialPort::NoError) {
            //qDebug() << "port error: " << m_port->errorString();
            closePort();
        }
    });
#endif

}

void SerialController::onSendMessage(QString &message) {
    if(m_port->isWritable()){
       m_port->write(message.toUtf8());
       //m_port->waitForBytesWritten();
    }
    else {
        qDebug() << "serial port not writable";
    }
}

void SerialController::onSendCommand(QByteArray &command) {
    if(m_port->isWritable()){
        m_port->write(command);
        //m_port->waitForBytesWritten();
    }
    else {
        qDebug() << "serial port not writable";
    }
}

// realtime find available ports.
void SerialController::onComClicked() {
    m_port_name_list.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        const QString label = info.portName();
        std::cout << label.toStdString() << std::endl;
        m_port_name_list.append(info.portName());
    }
    m_combobox_com->clear();
    m_combobox_com->addItems(m_port_name_list);
    qDebug() << "com list com name:" << m_com_name;
    m_combobox_com->setCurrentText(m_com_name);
}


void SerialController::onComSelected(int index)
{
    m_com_name = m_port_name_list[index];
    m_port->setPortName(m_com_name);
}

void SerialController::onBaudRateSelected(int index)
{
    m_baudrate = BaudRateList[index];
    m_port->setBaudRate(m_baudrate);
}

void SerialController::onParitySelected(int index)
{
    m_parity = ParityList[index];
    m_port->setParity(m_parity);
}

void SerialController::onDataBitsSelected(int index)
{
    m_databits = DataBitsList[index];
    m_port->setDataBits(m_databits);
}

void SerialController::onStopBitsSelected(int index)
{
    m_stopbits = StopBitsList[index];
    m_port->setStopBits(m_stopbits);
}

void SerialController::onFlowControlSelected(int index)
{
    m_flowcontrol = FlowControlList[index];
    m_port->setFlowControl(m_flowcontrol);
}

void SerialController::onConnectClicked() {
    if (m_port->isOpen()) {
        // is still open
        return;
    }
    m_port->setPortName(m_com_name);
    m_port->setBaudRate(m_baudrate);
    m_port->setParity(m_parity);
    m_port->setDataBits(m_databits);
    m_port->setStopBits(m_stopbits);
    m_port->setFlowControl(m_flowcontrol);
    qDebug() << "port name: " << m_com_name;
    qDebug() << "baudrate: " << m_baudrate;
    qDebug() << "parity: " << m_parity;
    qDebug() << "databits: " << m_databits;
    qDebug() << "stopbits: " << m_stopbits;
    qDebug() << "flowcontrol: " << m_flowcontrol;

    if (m_com_name == "" || m_com_name == nullptr) {
        qDebug() <<  "device open failed: com not selected";
        return;
    }
    //qDebug() << "port name: " << m_com_name;
    if (m_port->open(QSerialPort::ReadWrite)){
        qDebug() <<  "device open success";
        updateConnectionStatus(true);
        m_timer->start();
    }
    else {
        qDebug() <<  "device open failed: " << m_com_name;
    }
}
void SerialController::onDisconnectClicked() {
    closePort();
}

void SerialController::closePort() {
    if (m_port->isOpen()) {
        m_port->close();
        updateConnectionStatus(false);
        m_timer->stop();
    }
}

SerialController::~SerialController(){
    closePort();
//    delete m_port;
//    delete m_portInfo;
//    delete m_timer;
    qDebug() <<  "deconstructor serial controller";
}

void SerialController::onTimerTimeout() {
    if (m_port && m_port->bytesAvailable() > 0) {
        const QByteArray data = m_port->readAll();
        if(data.size() > 0) {
            emit dataReceived(data);
        }
    }
}

void SerialController::updateConnectionStatus(bool connected)
{
    if (connected) {
        m_connectStatus->setPixmap(QPixmap(":/icons/icons/icon_connect.png"));
        m_combobox_com->setEnabled(false);
        m_combobox_baudrate->setEnabled(false);
        m_combobox_parity->setEnabled(false);
        m_combobox_databits->setEnabled(false);
        m_combobox_stopbits->setEnabled(false);
        m_combobox_flowcontrol->setEnabled(false);
    } else {
        m_connectStatus->setPixmap(QPixmap(":/icons/icons/icon_disconnect.png"));
        m_combobox_com->setEnabled(true);
        m_combobox_baudrate->setEnabled(true);
        m_combobox_parity->setEnabled(true);
        m_combobox_databits->setEnabled(true);
        m_combobox_stopbits->setEnabled(true);
        m_combobox_flowcontrol->setEnabled(true);
    }
}
