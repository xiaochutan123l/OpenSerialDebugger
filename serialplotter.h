#ifndef SERIALPLOTTER_H
#define SERIALPLOTTER_H

#include <QObject>
#include "qcustomplot.h"
#include <QDebug>

#define MAX_GRAPH_NUM 2 // plot max 5 curve

class serialPlotter : public QObject
{
    Q_OBJECT
public:
    serialPlotter(QObject *parent,
                  QPushButton *clear,
                  QPushButton *save,
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

    void onSaveButtonClicked();
    void onClearButtonClicked();
private:
    bool isValidFormat(const QString &line);

    void setupDisplayPlot(int numGraphs);
    void updateDisplayPlot(const QVector<double> &yValues);

    QCustomPlot *m_display_plot;
    QScrollBar *m_display_verticalScrollBar;
    QScrollBar *m_display_horizontalScrollBar;

    double m_x_count = 0;
    QVector<double> m_xData;
    QVector<QVector<double>> m_graphData;

    QPushButton *m_button_clear;
    QPushButton *m_button_save;
};

#endif // SERIALPLOTTER_H
