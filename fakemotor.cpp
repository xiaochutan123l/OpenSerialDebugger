#include "fakemotor.h"

fakeMotor::fakeMotor(QObject *parent)
    : QObject(parent), m_timer(this)
{
    connect(&m_timer, &QTimer::timeout, this, &fakeMotor::onTimeout);
    m_timer.setInterval(5); // 设置定时器的时间间隔
}

void fakeMotor::start() {
    m_timer.start();
}

void fakeMotor::onTimeout()
{
    qDebug() <<  "send data";
    emit sendData("hello: 123,2.5\n");
}

void fakeMotor::receiveData(const QByteArray &data) {

}
