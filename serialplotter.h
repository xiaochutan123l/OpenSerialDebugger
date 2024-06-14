#ifndef SERIALPLOTTER_H
#define SERIALPLOTTER_H

#include <QObject>
#include "qcustomplot.h"
#include <QDebug>

class serialPlotter : public QObject
{
    Q_OBJECT
public:
    serialPlotter(QObject *parent,
                  QCustomPlot *display_plot,
                  QScrollBar *display_verticalScrollBar,
                  QScrollBar *display_horizontalScrollBar);
    ~serialPlotter();

public slots:
    void onNewLineReceived(const QString &line);

    void display_horzScrollBarChanged(int value);
    void display_vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
private:
    bool isValidFormat(const QString &line);
    QCustomPlot *m_display_plot;
    QScrollBar *m_display_verticalScrollBar;
    QScrollBar *m_display_horizontalScrollBar;
};

#endif // SERIALPLOTTER_H
