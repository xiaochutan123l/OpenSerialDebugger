#include "serialmonitor.h"

serialMonitor::serialMonitor(QObject *parent, QTextBrowser *textBrowser, QPushButton *button)
    : QObject(parent), m_textBrowser(textBrowser), m_clear_button(button)
{
    connect(m_clear_button, &QPushButton::clicked, this, &serialMonitor::onClearBufferClicked);
}

void serialMonitor::onSerialDataReceived(const QByteArray &data) {
    writeText(data);
}

void serialMonitor::onClearBufferClicked() {
    clearText();
}

void serialMonitor::writeText(const QByteArray &data) {
    m_textBrowser->append(data);
}

void serialMonitor::clearText() {
    m_textBrowser->clear();
}

