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
    m_send_history->append(command.toHex(' '));
    emit sendCommand(command);
}

void serialSendMonitor::onSendSuccess() {

}

void serialSendMonitor::onSendButtonClicked() {
    QString message = m_message_input->text();
    m_message_input->clear();
    if (message != "") {
        qDebug() << "send message: " << message;
        updateHistory(message);
        emit sendMessage(message);
    }
}

void serialSendMonitor::connect_widgets() {
    connect(m_send_message, &QPushButton::clicked, this, &serialSendMonitor::onSendButtonClicked);
}

void serialSendMonitor::updateHistory(QString &message) {
    m_send_history->append(message);
}

void serialSendMonitor::clearHistory() {
    m_send_history->clear();
}
