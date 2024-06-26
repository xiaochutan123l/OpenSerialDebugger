#include "fakemotor.h"

double generateSinPoint();
double generateCosPoint();

fakeMotor::fakeMotor(QObject *parent)
    : QObject(parent), m_timer(this)
{
    connect(&m_timer, &QTimer::timeout, this, &fakeMotor::onTimeout);
    m_timer.setInterval(MotorTimerInterval); // 设置定时器的时间间隔
}

void fakeMotor::start() {
    m_timer.start();
}

void fakeMotor::stop() {
    m_timer.stop();
}

void fakeMotor::onTimeout()
{
//    static int i = 0;
//    qDebug() <<  "send data: " << i++;
    emit sendData(generateNumberString());
}

void fakeMotor::receiveData(const QByteArray &data) {
    qDebug() << "motor received data" << data.toHex(' ');
    m_packethandler.hanler(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())));
}

QByteArray fakeMotor::generateNumberString() {
    QString numberString = "test data: " + QString::number(generateSinPoint()) + "," + QString::number(generateCosPoint()) + "\n";
    //QString numberString = "test data: " + QString::number(generateSinPoint()) + "\n";
    return numberString.toUtf8();
}

double generateSinPoint()
{
    static double angle = 0.0;
    const double step = 0.1; // 步长，可以调整
    const double twoPi = 2.0 * M_PI;

    // 计算 sin 值
    double value = std::sin(angle);
    // 更新角度，确保在 0 到 2π 之间循环
    angle += step;
    if (angle >= twoPi) {
        angle -= twoPi;
    }

    // 返回结果
    return value;
}

double generateCosPoint()
{
    static double angle = 0.0;
    const double step = 0.1; // 步长，可以调整
    const double twoPi = 2.0 * M_PI;

    // 计算 sin 值
    double value = std::cos(angle);
    // 更新角度，确保在 0 到 2π 之间循环
    angle += step;
    if (angle >= twoPi) {
        angle -= twoPi;
    }

    // 返回结果
    return value;
}
