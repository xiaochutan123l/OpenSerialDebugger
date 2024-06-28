#include "serialdatacontainer.h"
#include "packethandler.h"

serialDataContainer::serialDataContainer()
{

}

serialDataContainer::~serialDataContainer()
{
    qDebug() <<  "deconstruct serial data container";
}

void serialDataContainer::feedData(const QByteArray &data)
{
    // 将新数据追加到缓冲区
    m_buffer.append(data);

    int index;
    // 查找换行符的位置，并处理每一行
    while ((index = m_buffer.indexOf('\n')) != -1) {
        // 提取一行数据
        QByteArray line = m_buffer.left(index);
        // 从缓冲区中移除该行（包括换行符）
        m_buffer.remove(0, index + 1);
        extractPackets(line);
        // 将提取的行转换为 QString 并添加到 lines 列表中
        m_lines.append(QString::fromUtf8(line));
    }
}

QList<QByteArray> serialDataContainer::getPackets() const {
    return m_packets;
}

QStringList serialDataContainer::getLines() const
{
    return m_lines;
}

void serialDataContainer::clearBuffer() {
    m_lines.clear();
    m_packets.clear();
}

void serialDataContainer::extractPackets(QByteArray& data) {
    // TODO: struct packet变成c++ struct， 支持初始化，提取chunk_num，计算各种len的函数,
    // 并支持packet byte字符串，将十六进制的数据包，变成用户可读格式的字符串
    int index = 0;
    while (data[index] == static_cast<char>(MAGIC_NUM)) {
        uint8_t chunk_num = data[index+1];
        int packet_len = chunk_num * CHUNK_LEN + 2;
        m_packets.append(QByteArray(data.mid(index, packet_len)));
        // m_lines 里面传packet生成的string
        //m_lines.append(QString::fromUtf8(data.mid(index, packet_len)));
        index += packet_len;
    }
    data.remove(0, index);
}
