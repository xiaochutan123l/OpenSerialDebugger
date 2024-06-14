#include "serialmonitor.h"

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
    for (const QString &line : lines) {
        writeText(data);// 输出到 QTextEdit
        emit newLineReceived(line); // 发出信号通知 SerialPlotter
    }

}

void serialMonitor::onClearBufferClicked() {
    clearText();
}

void serialMonitor::writeText(const QString &data) {
    m_textBrowser->append(data);
}

void serialMonitor::clearText() {
    m_textBrowser->clear();
}
