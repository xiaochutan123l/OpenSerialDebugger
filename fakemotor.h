#ifndef FAKEMOTOR_H
#define FAKEMOTOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#define MotorTimerInterval 50 // ms

class fakeMotor : public QObject
{
    Q_OBJECT

public slots:
    void onTimeout();
signals:
    void sendData(const QByteArray &data);
public:
    fakeMotor(QObject *parent = nullptr);
    void receiveData(const QByteArray &data); 
    void start();
    void stop();

    QTimer m_timer;
private:
    QByteArray generateNumberString();
    //QByteArray generateString();
};

#endif // FAKEMOTOR_H
