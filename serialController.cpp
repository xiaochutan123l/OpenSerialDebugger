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
SerialController::SerialController(QObject *parent, QLabel *connectStatus)
    : QObject(parent), m_connectStatus(connectStatus) {

#ifdef USE_FAKE_SERIAL
    m_port = new FakeSerialPort(this);
#else
    m_port = new QSerialPort(this);
#endif

    m_portInfo = new QSerialPortInfo();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerialController::onTimerTimeout);

    updateConnectionStatus(false);

    m_baudrate = BaudRateList[BAUDRATE_DEFUALT_INDEX];
    m_parity = ParityList[0];
    m_databits = DataBitsList[0];
    m_stopbits = StopBitsList[0];
    m_flowcontrol = FlowControlList[0];

    m_timer->setInterval(ReadDataTickTime);
   connect(m_port, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
       if (error != QSerialPort::NoError) {
           //emit errorOccurred(serialPort->errorString());
           qDebug() << "port error: " << m_port->errorString();
       }
   });
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
    emit portListUpdate(m_port_name_list);
}

void SerialController::onComSelected(int index)
{
    m_com_name = m_port_name_list[index];
}

void SerialController::onBaudRateSelected(int index)
{
    m_baudrate = BaudRateList[index];
}

void SerialController::onParitySelected(int index)
{
    m_parity = ParityList[index];
}

void SerialController::onDataBitsSelected(int index)
{
    m_databits = DataBitsList[index];
}

void SerialController::onStopBitsSelected(int index)
{
    m_stopbits = StopBitsList[index];
}

void SerialController::onFlowControlSelected(int index)
{
    m_flowcontrol = FlowControlList[index];
}

void SerialController::onConnectClicked() {
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
    updateConnectionStatus(false);
    m_timer->stop();
}

void SerialController::closePort() {
    m_port->close();
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
        emit dataReceived(data);
    }
//    else {
//        emit dataReceived("test =====");
//    }
}

void SerialController::updateConnectionStatus(bool connected)
{
    if (connected) {
        m_connectStatus->setPixmap(QPixmap(":/icons/icons/icon_connect.png"));
    } else {
        m_connectStatus->setPixmap(QPixmap(":/icons/icons/icon_disconnect.png"));
    }
}
