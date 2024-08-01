#include "serialsendmonitor.h"

serialSendMonitor::serialSendMonitor(QPushButton *send_button,
                                     QPushButton *clear_button,
                                     QLineEdit *line_input,
                                     QTextBrowser *text_browser,
                                     QObject *parent)
    : m_send_message(send_button),
    m_clear_button(clear_button),
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

void serialSendMonitor::onClearButtonClicked() {
    clearHistory();
}

void serialSendMonitor::connect_widgets() {
    connect(m_send_message, &QPushButton::clicked, this, &serialSendMonitor::onSendButtonClicked);
    connect(m_clear_button, &QPushButton::clicked, this, &serialSendMonitor::onClearButtonClicked);
}

void serialSendMonitor::updateHistory(QString &message) {
    m_send_history->append(message);
    m_send_history->moveCursor(QTextCursor::End);
}

void serialSendMonitor::clearHistory() {
    m_send_history->clear();
}
