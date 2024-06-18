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
SerialController::SerialController(QObject *parent) : QObject(parent){

#ifdef USE_FAKE_SERIAL
    m_port = new FakeSerialPort(this);
#else
    m_port = new QSerialPort(this);
#endif

    m_portInfo = new QSerialPortInfo();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SerialController::onTimerTimeout);

    m_baudrate = BaudRateList[BAUDRATE_DEFUALT_INDEX];
    m_timer->setInterval(ReadDataTickTime);
//    connect(serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
//        if (error != QSerialPort::NoError) {
//            emit errorOccurred(serialPort->errorString());
//        }
//    });
}

//void SerialController::resetMotors() {
//    if(m_port->isWritable()){
//        for (int i = 1; i < 7; i++) {
//            QString data = QString::number(i) + "," + QString::number(1500) + "," + QString::number(2000) + '\n';
//            m_port->write(data.toUtf8());
//            m_port->waitForBytesWritten();
//            m_motor_mgr->setMotorPosition(i, 1500);
//        }
//    }
//}

//void SerialController::sendMsg(int motorId, int toPosition, int time) {
//    int t = m_motor_mgr->calc_move_time(motorId, toPosition);

//    if(m_port->isWritable()){
//        QString data = QString::number(motorId) + "," + QString::number(toPosition) + "," + QString::number(t) + '\n';
//        std::cout << data.toStdString() << std::endl;
//        m_port->write(data.toUtf8());
//        m_port->waitForBytesWritten();
//        std::cout <<  "it should sent" << std::endl;
//        m_motor_mgr->setMotorPosition(motorId, toPosition);
//    }
//    else{
//        std::cout <<  "device open failed" << std::endl;
//        // if serial port not writable, new position should not be Selectedd.
//        //m_motor_mgr->setMotorPosition(motorId, toPosition);
//    }
//}

// realtime find available ports.
void SerialController::onComClicked() {
    m_port_name_list.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        const QString label = info.portName();
        std::cout << label.toStdString() << std::endl;
        m_port_name_list.append(info.portName());
        emit portListUpdate(m_port_name_list);
    }
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
    if (m_port->open(QIODevice::ReadWrite)){
        qDebug() <<  "device open success";
    }
    else {
        qDebug() <<  "device open failed";
    }
    m_timer->start();
}
void SerialController::onDisconnectClicked() {
    closePort();
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
