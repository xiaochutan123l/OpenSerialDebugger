#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QTextBrowser>
#include <QPushButton>
#include <QDebug>
#include "serialdatacontainer.h"

class serialMonitor : public QObject {
    Q_OBJECT
signals:
    void plotData();
    void newLinesReceived(const QStringList &lines);
    void updateGetParameter(const QByteArray &line);
public slots:
    void onSerialDataReceived(const QByteArray &data);
    void onClearBufferClicked();
public:
    serialMonitor(QObject *parent, QTextBrowser *textBrowser, QPushButton *button);
    ~serialMonitor();

private:
    void writeText(const QString &data);
    void clearText();
    QTextBrowser *m_textBrowser;
    QPushButton *m_clear_button;
    serialDataContainer m_dataContainer;
};

#endif // SERIALMONITOR_H
