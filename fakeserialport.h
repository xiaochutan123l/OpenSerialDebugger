#ifndef FAKESERIALPORT_H
#define FAKESERIALPORT_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QStringList>
#include <QSerialPortInfo>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "fakemotor.h"

class FakeSerialPort : public QObject
{
    Q_OBJECT

public:
    explicit FakeSerialPort(QObject *parent = nullptr);
    ~FakeSerialPort();

    bool open(QTextStream::OpenMode mode);
    bool close();
    bool isOpen() const;

    bool isWritable() const;
    void write(const QByteArray &data);
    QStringList availablePorts() const;

    qint64 bytesAvailable();
    QByteArray readAll();

    void setPortName(const QString &name) {};
    void setPort(const QSerialPortInfo &info) {};
    bool setBaudRate(qint32 baudRate) {return true;};
    bool setDataBits(QSerialPort::DataBits dataBits) {return true;};
    bool setParity(QSerialPort::Parity parity) {return true;};
    bool setStopBits(QSerialPort::StopBits stopBits) {return true;};
    bool setFlowControl(QSerialPort::FlowControl flowControl) {return true;};

signals:
    void readyRead();
    //void errorOccurred(const QString &error);
    void portOpened();
    void portClosed();
    void errorOccurred(QSerialPort::SerialPortError error);

private slots:
    //void onTimeout();
    void onReceivedSerialData(const QByteArray &data);

private:
    bool m_isOpen;
    //QTimer m_timer;
    QByteArray m_recv_buffer;
    fakeMotor m_motor;
};

#endif // FAKESERIALPORT_H
