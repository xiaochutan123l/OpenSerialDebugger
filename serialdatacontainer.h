 #ifndef SERIALDATACONTAINER_H
#define SERIALDATACONTAINER_H

#include <QObject>
#include <QDebug>

class serialDataContainer : public QObject
{
    Q_OBJECT
public:
    serialDataContainer();
    ~serialDataContainer();
    // serial monitor feed new serial data
    void feedData(const QByteArray &data);
    // return all split lines
    QString getText() const;
    QStringList getPlotLines() const;
    QList<QByteArray> getPackets() const;

    void clearBuffer();
private:
    // return the offset of real text(not packet)
    int extractPackets(QByteArray& data);

    QByteArray m_buffer;   // 用于存储未结束的片段
    QStringList m_plot_lines;   // 用于需要plot的字符串数据
    QString m_text;   // 用于存储完整的行
    QList<QByteArray> m_packets;   // 用于存储符合规定格式的数据包（不以换行符结尾）
};

#endif // SERIALDATACONTAINER_H
