#ifndef FAKEMOTOR_H
#define FAKEMOTOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
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

    QTimer m_timer;
};

#endif // FAKEMOTOR_H
