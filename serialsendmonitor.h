#ifndef SERIALSENDMONITOR_H
#define SERIALSENDMONITOR_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QTextBrowser>
#include <QDebug>

class serialSendMonitor: public QObject {
    Q_OBJECT
public:
    serialSendMonitor(QPushButton *send_button,
                      QPushButton *clear_button,
                      QLineEdit *line_input,
                      QTextBrowser *text_browser,
                      QObject *parent = nullptr);

signals:
    void sendMessage(QString &message);
    void sendCommand(QByteArray &command);

public slots:
    void onSendCommand(QByteArray &command);
    void onSendButtonClicked();
    void onClearButtonClicked();
    void onSendSuccess();

public:
    void connect_widgets();

private:
    void updateHistory(QString &message);
    void clearHistory();


    QPushButton *m_send_message;
    QPushButton *m_clear_button;
    QLineEdit *m_message_input;
    QTextBrowser *m_send_history;
};

#endif // SERIALSENDMONITOR_H
