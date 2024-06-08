#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H

//#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QListWidgetItem>

#include <map>

class Serial_controller : public QObject{
    Q_OBJECT

public:
    Serial_controller();
    ~Serial_controller();

signals:
    // add selected port name
    void portSelected(QString portName);
    // add found port item name
    void portFound(QString portName);

public slots:
    void portListClickHandler(QListWidgetItem *item);
    void listPorts();
    void portSelectClickHandler();

private:
    void openPort();
    void closePort();

    //Ui::MainWindow *ui;
    QSerialPortInfo *m_portInfo;
    QSerialPort *m_port;
    QList<QSerialPortInfo> m_port_list;
    std::map<QString, QSerialPort> m_port_list_map;
};

#endif // SERIAL_CONTROLLER_H
