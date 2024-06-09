#ifndef SerialController_H
#define SerialController_H

//#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListWidgetItem>
#include <QDebug>

class SerialController : public QObject{
    Q_OBJECT

public:
    SerialController();
    ~SerialController();

signals:
    // add found port item name
    void portListUpdate(QList<QString> &port_name_list);

public slots:
    // set port parameter when user Selecteds.
    // user expands list view.
    void onComClicked();

    void onComSelected(int index);
    void onBaudRateSelected(int index);
    void onParitySelected(int index);
    void onDataBitsSelected(int index);
    void onStopBitsSelected(int index);
    void onFlowControlSelected(int index);

    void onConnectClicked();
    void onDisconnectClicked();

public:
    QStringList m_Bauderate_list_str = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
    QStringList m_Parity_list_str = {"None", "Even", "Odd", "Mark", "Space"};
    QStringList m_Databits_list_str = {"5", "6", "7", "8"};
    QStringList m_Stopbits_list_str = {"1", "1.5", "2"};
    QStringList m_Flowctr_list_str = {"None", "RTS/CTS", "XON/XOFF"};

private:
    void openPort();
    void closePort();

    QSerialPortInfo *m_portInfo;
    QSerialPort *m_port;
    QList<QString> m_port_name_list;

    QString m_com_name;
    qint32 m_baudrate;
    QSerialPort::DataBits m_databits;
    QSerialPort::StopBits m_stopbits;
    QSerialPort::Parity m_parity;
    QSerialPort::FlowControl m_flowcontrol;
};

#endif // SerialController_H
