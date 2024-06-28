#include "fakeserialport.h"

FakeSerialPort::FakeSerialPort(QObject *parent)
    : QObject(parent), m_isOpen(false), m_motor(this) // m_timer(this)
{
    //connect(&m_timer, &QTimer::timeout, this, &FakeSerialPort::onTimeout);
    //m_timer.setInterval(100); // 设置定时器的时间间隔
    qDebug() <<  "init fake serial port";
    connect(&m_motor, &fakeMotor::sendData, this, &FakeSerialPort::onReceivedSerialData);
}

FakeSerialPort::~FakeSerialPort()
{
    close();
}

bool FakeSerialPort::open(QTextStream::OpenMode mode)
{
    if (!m_isOpen) {
        m_isOpen = true;
        //m_timer.start();
        emit portOpened();
    }
    m_motor.start();
    return true;
}

bool FakeSerialPort::close()
{
    if (m_isOpen) {
        m_isOpen = false;
        //m_timer.stop();
        emit portClosed();
    }
    m_motor.stop();
    return true;
}

bool FakeSerialPort::isWritable() const
{
    return m_isOpen;
}

bool FakeSerialPort::isOpen() const
{
    return m_isOpen;
}

void FakeSerialPort::write(const QByteArray &data)
{
    if (m_isOpen) {
//        // 模拟接收到的数据
//        m_buffer.append(data);
//        m_buffer.append("\n"); // 添加换行符以模拟数据行
        qDebug() << "send to motor received data" << data;
        m_motor.receiveData(data);
    } else {
        emit errorOccurred(QSerialPort::SerialPortError::NotOpenError);
    }
}

QStringList FakeSerialPort::availablePorts() const
{
    // 返回模拟的可用串口列表
    return {"COM1", "COM2", "COM3", "FakePort"};
}

qint64 FakeSerialPort::bytesAvailable() {
    return m_recv_buffer.length();
}

QByteArray FakeSerialPort::readAll() {
    QByteArray data = std::move(m_recv_buffer);
    m_recv_buffer.clear();
    return data;
}

void FakeSerialPort::onReceivedSerialData(const QByteArray &data) {
    //qDebug() <<  "data received";
    m_recv_buffer.append(data);
}

//void FakeSerialPort::onTimeout()
//{
//    if (!m_recv_buffer.isEmpty()) {
//        emit readyRead(); // 触发 readyRead 信号
//    }
//}
