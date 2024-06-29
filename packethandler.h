#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <stdint.h>
#include <string.h>
#include <QDebug>
#include "packet.h"

class packetHandler
{
public:
    enum DataType {
        UINT,
        INT,
        FLOAT,
        BOOL,
        UNKNOWN
    };

    enum CMDType {
        CMD_UNKNOWN=0,
        SetKp=1,
        SetKi,
        SetKd,
        GetPosition,
        GetVelocity,
        Stop,
        Enable
    };

    packetHandler();

    CMDType hanler(uint8_t* packet) {
        memcpy(&m_packet, packet, sizeof(Packet));
        if (!IS_VALID_PACKET(packet)) {
            qDebug() << "wrong magic number: " << GET_MAGIC_NUM(packet);
            return CMD_UNKNOWN;
        }
        qDebug() << "packet size: " << PACKET_LEN;
        qDebug() << "chunk number: " << GET_CHUNK_NUM(packet);
        qDebug() << "cmd number: " << GET_CMD_NUM(packet);
        switch(GET_CMD_NUM(packet)) {
            case SetKp: {
                int32_t data = static_cast<int32_t>GET_RAW_DATA_INT(packet);
                qDebug() << "set kp: " << data;
                break;
            }
            case SetKi: {
                //float data;
                //memcpy(&data, &m_packet.data, sizeof(float)); // Ensure proper conversion
                qDebug() << "set ki: " << (float)GET_RAW_DATA_FLOAT(packet);
                break;
            }
            case SetKd: {
                uint32_t data = GET_RAW_DATA_INT(packet);
                qDebug() << "set kd: " << data;
                break;
            }
            case GetPosition: {
                int32_t data = static_cast<int32_t>GET_RAW_DATA_INT(packet);
                qDebug() << "GetPosition: " << data;
                return GetPosition;
                break;
            }
            case GetVelocity: {
                float data = (float)GET_RAW_DATA_FLOAT(packet);
                //memcpy(&data, &m_packet.data, sizeof(float)); // Ensure proper conversion
                qDebug() << "GetVelocity: " << data;
                return GetVelocity;
                break;
            }
            case Stop: {
                //bool data = m_packet.data != 0;
                qDebug() << "Stop: " << GET_RAW_DATA_UINT(packet);
                break;
            }
            case Enable: {
                //bool data = m_packet.data != 0;
                qDebug() << "Enable: " << GET_RAW_DATA_UINT(packet);
                break;
            }
            default:
                qDebug() << "Unknown command";
                break;
            }
            return CMD_UNKNOWN;
    }

    Packet m_packet;
};

#endif // PACKETHANDLER_H
