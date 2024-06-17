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
    QStringList getLines() const;
    void clearBuffer();
private:
    QByteArray m_buffer;   // 用于存储未结束的片段
    QStringList m_lines;   // 用于存储完整的行

};

#endif // SERIALDATACONTAINER_H
