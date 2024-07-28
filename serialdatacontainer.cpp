#include "serialdatacontainer.h"
#include "packet.h"

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

        if (line.endsWith('\r')) {
            line.removeLast();
        }

        int offset = extractPackets(line);
        if (offset > 0) {
            // raw text with \n \r
            m_text.append(QString::fromUtf8(m_buffer.mid(offset, index+1 - offset)));
        }
        else {
            m_text.append(QString::fromUtf8(m_buffer.left(index+1)));
        }
        m_plot_lines.append(QString::fromUtf8(line));
        // 从缓冲区中移除该行（包括换行符）
        m_buffer.remove(0, index + 1);
    }
    // Packet with magic number has no '\n', extract packet again if a single packet left in buffer.
    extractPackets(m_buffer);
}

QList<QByteArray> serialDataContainer::getPackets() const {
    return m_packets;
}

QString serialDataContainer::getText() const
{
    return m_text;
}

QStringList serialDataContainer::getPlotLines() const
{
    return m_plot_lines;
}

void serialDataContainer::clearBuffer() {
    m_text.clear();
    m_plot_lines.clear();
    m_packets.clear();
}

int serialDataContainer::extractPackets(QByteArray& data) {
    // TODO: struct packet变成c++ struct， 支持初始化，提取chunk_num，计算各种len的函数,
    // 并支持packet byte字符串，将十六进制的数据包，变成用户可读格式的字符串.. 这个可能会和plotter那边的字符串冲突，先不考虑这个功能了
    uint8_t *packet = reinterpret_cast<uint8_t*>(data.data());
    int offset = 0;
    while (IS_VALID_PACKET(packet)) {
        m_packets.append(QByteArray(data.mid(0, PACKET_LEN)));
        //m_lines.append(QString::fromUtf8(data.mid(0, PACKET_LEN)));
        QString packet_str = PACKET_ID_STR + data.mid(0, PACKET_LEN).toHex(' ') + '\n';
        m_text.append(packet_str);
        data.remove(0, PACKET_LEN);
        packet+= PACKET_LEN;
        offset+= PACKET_LEN;
    }
    return offset;
    //int index = 0;
    // while (IS_VALID_PACKET(&packet[index])) {
    //     //uint8_t chunk_num = data[index+1];
    //     int packet_len = GET_CHUNK_NUM(&packet[index]) * CHUNK_LEN + 2;
    //     m_packets.append(QByteArray(data.mid(index, packet_len)));
    //     // m_lines 里面传packet生成的string
    //     //m_lines.append(QString::fromUtf8(data.mid(index, packet_len)));
    //     index += packet_len;
    // }
    //data.remove(0, index);
}
