#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <stdint.h>
#include <string.h>
#include <QDebug>

enum DataType {
    UINT,
    INT,
    FLOAT,
    BOOL,
    UNKNOWN
};

enum CMDType {
    SetKp=1,
    SetKi,
    SetKd,
    GetPosition,
    GetVelocity,
    Stop,
    Enable
};
//#pragma pack(push, 1)
struct Packet {
    uint8_t magicNum;
    uint8_t chunkNum;
    uint16_t cmdNumber;
    uint32_t data;
};
//#pragma pack(pop)
class packetHandler
{
public:
    packetHandler();
    void hanler(uint8_t* packet) {
        memcpy(&m_packet, packet, sizeof(Packet));
        if (m_packet.magicNum != 0xAB) {
            qDebug() << "wrong magic number: " << m_packet.magicNum;
            return;
        }
        qDebug() << "packet size: " << sizeof(Packet);
        qDebug() << "chunk number: " << m_packet.chunkNum;
        qDebug() << "cmd number: " << m_packet.cmdNumber;
        switch(m_packet.cmdNumber) {
            case SetKp: {
                int32_t data = static_cast<int32_t>(m_packet.data);
                qDebug() << "set kp: " << data;
                break;
            }
            case SetKi: {
                float data;
                memcpy(&data, &m_packet.data, sizeof(float)); // Ensure proper conversion
                qDebug() << "set ki: " << data;
                break;
            }
            case SetKd: {
                int32_t data = static_cast<int32_t>(m_packet.data);
                qDebug() << "set kd: " << data;
                break;
            }
            case GetPosition: {
                int32_t data = static_cast<int32_t>(m_packet.data);
                qDebug() << "GetPosition: " << data;
                break;
            }
            case GetVelocity: {
                float data;
                memcpy(&data, &m_packet.data, sizeof(float)); // Ensure proper conversion
                qDebug() << "GetVelocity: " << data;
                break;
            }
            case Stop: {
                bool data = m_packet.data != 0;
                qDebug() << "Stop: " << data;
                break;
            }
            case Enable: {
                bool data = m_packet.data != 0;
                qDebug() << "Enable: " << data;
                break;
            }
            default:
                qDebug() << "Unknown command";
                break;
            }
    }

    Packet m_packet;
};

#endif // PACKETHANDLER_H
