#ifndef SerialController_H
#define SerialController_H

//#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListWidgetItem>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QComboBox>
#include <clickablecombobox.h>

#ifdef USE_FAKE_SERIAL
#include "fakeserialport.h"
#endif

#define ReadDataTickTime 100 // ms

#define BAUDRATE_DEFUALT_INDEX 7 // 115200

class SerialController : public QObject{
    Q_OBJECT

public:
    SerialController(QObject *parent,
                     QLabel *connectStatus,
                     ClickableComboBox *com,
                     QComboBox *baudrate,
                     QComboBox *parity,
                     QComboBox *databits,
                     QComboBox *stopbits,
                     QComboBox *flowcontrol);
    ~SerialController();

signals:
    // add found port item name
    void portListUpdate(QList<QString> &port_name_list);
    void dataReceived(const QByteArray &data);

private slots:
    // set port parameter when user Selecteds.
    // user expands list view.
    void onComClicked();

    void onComSelected(int index);
    void onBaudRateSelected(int index);
    void onParitySelected(int index);
    void onDataBitsSelected(int index);
    void onStopBitsSelected(int index);
    void onFlowControlSelected(int index);

    void onTimerTimeout();
public slots:
    void onSendMessage(QString &message);
    void onSendCommand(QByteArray &command);
    void onConnectClicked();
    void onDisconnectClicked();

public:
    QStringList m_Bauderate_list_str = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200"};
    QStringList m_Parity_list_str = {"None", "Even", "Odd", "Mark", "Space"};
    QStringList m_Databits_list_str = {"8", "7", "6", "5"};
    QStringList m_Stopbits_list_str = {"1", "1.5", "2"};
    QStringList m_Flowctr_list_str = {"None", "RTS/CTS", "XON/XOFF"};

private:
    void openPort();
    void closePort();
    void updateConnectionStatus(bool connected);

    QSerialPortInfo *m_portInfo;

#ifdef USE_FAKE_SERIAL
    FakeSerialPort *m_port;
#else
    QSerialPort *m_port;
#endif

    QList<QString> m_port_name_list;

    QString m_com_name;
    qint32 m_baudrate;
    QSerialPort::DataBits m_databits;
    QSerialPort::StopBits m_stopbits;
    QSerialPort::Parity m_parity;
    QSerialPort::FlowControl m_flowcontrol;

    QLabel *m_connectStatus;
    ClickableComboBox *m_combobox_com;
    QComboBox *m_combobox_baudrate;
    QComboBox *m_combobox_parity;
    QComboBox *m_combobox_databits;
    QComboBox *m_combobox_stopbits;
    QComboBox *m_combobox_flowcontrol;

    QTimer *m_timer;


};

#endif // SerialController_H
