#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QTextBrowser>
#include <QPushButton>
#include <QDebug>

class serialMonitor : public QObject {
    Q_OBJECT
signals:
    void plotData();

public slots:
    void onSerialDataReceived(const QByteArray &data);
    void onClearBufferClicked();
public:
    serialMonitor(QObject *parent, QTextBrowser *textBrowser, QPushButton *button);


private:
    void writeText(const QByteArray &data);
    void clearText();
    QTextBrowser *m_textBrowser;
    QPushButton *m_clear_button;
};

#endif // SERIALMONITOR_H
