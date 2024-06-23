#include "serialsendmonitor.h"

serialSendMonitor::serialSendMonitor(QPushButton *send_button,
                                     QLineEdit *line_input,
                                     QTextBrowser *text_browser,
                                     QObject *parent)
    : m_send_message(send_button),
    m_message_input(line_input),
    m_send_history(text_browser),
    QObject(parent) {}


void serialSendMonitor::onSendCommand(QByteArray &command) {
    emit sendCommand(command);
}

void serialSendMonitor::onSendSuccess() {

}

void serialSendMonitor::onSendButtonClicked() {
    QString message = m_send_message->text();
    if (message != "") {
        emit sendMessage(message);
    }
}

void serialSendMonitor::connect_widgets() {
    connect(m_send_message, &QPushButton::clicked, this, &serialSendMonitor::onSendButtonClicked);
}

void serialSendMonitor::updateHistory() {

}

void serialSendMonitor::clearHistory() {
    m_send_history->clear();
}
