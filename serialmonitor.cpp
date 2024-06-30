#include "serialmonitor.h"
//#include "packethandler.h"

serialMonitor::serialMonitor(QObject *parent, QTextBrowser *textBrowser, QPushButton *button)
    : QObject(parent), m_textBrowser(textBrowser), m_clear_button(button)
{
    connect(m_clear_button, &QPushButton::clicked, this, &serialMonitor::onClearBufferClicked);
}

serialMonitor::~serialMonitor()
{
    qDebug() <<  "deconstruct serial monitor";
}

void serialMonitor::onSerialDataReceived(const QByteArray &data) {
    m_dataContainer.feedData(data);
    QStringList lines = m_dataContainer.getLines();
    QList<QByteArray> packets = m_dataContainer.getPackets();
    m_dataContainer.clearBuffer();

    for (const QByteArray &packet : packets) {
        //writeText(QString::fromUtf8(packet));
        emit updateGetParameter(packet);
    }

    for (const QString &line : lines) {
        writeText(line);// 输出到 QTextEdit
        // TODO: packet struct完善后修改这里
        if (!line.startsWith("--Packet--")) {
            //qDebug() << line;
            emit newLineReceived(line); // 发出信号通知 SerialPlotter
        }
    }
}

void serialMonitor::onClearBufferClicked() {
    clearText();
}

void serialMonitor::writeText(const QString &data) {
    m_textBrowser->append(data);
    //m_textBrowser->insertPlainText(data);
}

void serialMonitor::clearText() {
    m_textBrowser->clear();
}

