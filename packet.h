#ifndef PACKET_H
#define PACKET_H

#ifdef __cplusplus
extern "C"{
#endif

#define MAGIC_NUM 0xAB // 1byte
#define PACKET_LEN 8 // bytes
#define CHUNK_LEN 6 // bytes

#include <stdint.h>
//#include <string.h>

#define IS_VALID_PACKET(packet) ((packet)[0] == MAGIC_NUM)
#define GET_MAGIC_NUM(packet) ((packet)[0])
#define GET_CHUNK_NUM(packet) ((packet)[1])
#define GET_CMD_NUM(packet) (*(uint16_t*)&(packet)[2])

#define GET_RAW_DATA_UINT(packet) (*(uint32_t*)&(packet)[4])
#define GET_RAW_DATA_BOOL(packet) (*(uint32_t*)&(packet)[4])
#define GET_RAW_DATA_INT(packet) (*(int32_t*)&(packet)[4])
#define GET_RAW_DATA_FLOAT(packet) (*(float*)&(packet)[4])

#define DATA_TO_UINT(data) (*(uint32_t*)(data))
#define DATA_TO_BOOL(data) (*(uint32_t*)(data))
#define DATA_TO_INT(data) (*(int32_t*)(data))
#define DATA_TO_FLOAT(data) (*(float*)(data))

// TODO: should be adapted if multiple chunk enabled.

struct Packet {
    uint8_t magicNum;
    uint8_t chunkNum;
    uint16_t cmdNum;
    uint32_t data;
};

// bool isValidPacket(uint8_t *packet);
// uint8_t getChunkNum(uint8_t *packet);
// uint16_t getCmdNum(uint8_t *packet);
// uint32_t getRawData(uint8_t *packet);

// bool isValidPacket(uint8_t *packet) {
//     return packet[0] == MAGIC_NUM;
// }

// uint8_t getChunkNum(uint8_t *packet) {
//     return packet[1];
// }

// uint16_t getCmdNum(uint8_t *packet) {
//     uint16_t cmd;
//     memcpy(&cmd, &packet[2], sizeof(uint16_t));
//     return cmd;
// }

// uint32_t getRawData(uint8_t *packet) {
//     uint32_t data;
//     memcpy(&data, &packet[4], sizeof(uint32_t));
//     return data;
// }

#ifdef __cplusplus
}
#endif

#endif // PACKET_H
