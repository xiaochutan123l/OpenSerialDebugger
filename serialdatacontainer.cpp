#include "serialdatacontainer.h"

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
        // 将提取的行转换为 QString 并添加到 lines 列表中
        m_lines.append(QString::fromUtf8(line));
    }
}

QStringList serialDataContainer::getLines() const
{
    return m_lines;
}
