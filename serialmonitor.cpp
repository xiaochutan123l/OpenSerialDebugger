#include "serialmonitor.h"
#include <QElapsedTimer>
//#include "packethandler.h"

serialMonitor::serialMonitor(QObject *parent, QTextBrowser *textBrowser, QPushButton *button)
    : QObject(parent), m_textBrowser(textBrowser), m_clear_button(button)
{
    connect(m_clear_button, &QPushButton::clicked, this, &serialMonitor::onClearBufferClicked);
    m_textBrowser->setReadOnly(true); // 确保文本浏览器为只读模式
}

serialMonitor::~serialMonitor()
{
    qDebug() <<  "deconstruct serial monitor";
}

void serialMonitor::onSerialDataReceived(const QByteArray &data) {
    m_dataContainer.feedData(data);

    const QString text = m_dataContainer.getText();
    const QStringList lines = m_dataContainer.getPlotLines();
    QList<QByteArray> packets = m_dataContainer.getPackets();
    m_dataContainer.clearBuffer();

    //QElapsedTimer timer;
    //timer.start();
    emit newLinesReceived(lines);
    //qDebug() << "plot time: " << timer.elapsed() << "ms";

    for (const QByteArray &packet : packets) {
        //writeText(QString::fromUtf8(packet));
        emit updateGetParameter(packet);
    }

    writeText(text);
}

void serialMonitor::onClearBufferClicked() {
    clearText();
}

void serialMonitor::writeText(const QString &data) {
    //m_textBrowser->append(data);
    m_textBrowser->insertPlainText(data);
    m_textBrowser->moveCursor(QTextCursor::End);
}

void serialMonitor::clearText() {
    m_textBrowser->clear();
}

