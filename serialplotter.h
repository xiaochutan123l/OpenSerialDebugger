#ifndef SERIALPLOTTER_H
#define SERIALPLOTTER_H

#include <QObject>
#include "qcustomplot.h"
#include <QDebug>
#include "plotdatahandlerthread.h"

#define MAX_GRAPH_NUM 5 // plot max 5 curve
#define GRAPH_PEN_WIDTH 2

class serialPlotter : public QObject
{
    Q_OBJECT
public:    
    serialPlotter(QObject *parent,
                  QPushButton *clear,
                  QPushButton *save,
                  QPushButton *stop,
                  QCustomPlot *display_plot,
                  QScrollBar *display_verticalScrollBar,
                  QScrollBar *display_horizontalScrollBar);
    ~serialPlotter();

signals:
    void newLinesReceived(const QStringList &lines, QCPRange xRange, QCPRange yRange);
    void clearPlotData();
public slots:
    void onNewLinesReceived(const QStringList &lines);

    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);

    void onSaveButtonClicked();
    void onClearButtonClicked();
    void onStopButtonClicked();

    void onCurveNumChanged(int new_num);
    void onReadyForPlot(PlotDataPtrList &data);
private:
    void setupDisplayPlot(int numGraphs);
    void savePlotDataToCSV(const QString &fileName);

    void setXAxis(QCPRange range) {m_x_axis_range = range;};
    void setYAxis(QCPRange range) {m_y_axis_range = range;};
    QCPRange getXAxis() {return m_x_axis_range;};
    QCPRange getYAxis() {return m_y_axis_range;};

    // void setHorizontalScrollBar(QCPRange range);
    // void setVerticalScrollBar(QCPRange range);

    QList<QColor> m_pen_colors = {Qt::blue, Qt::red, Qt::green, Qt::black, Qt::gray};

    QCustomPlot *m_display_plot;
    QScrollBar *m_display_verticalScrollBar;
    QScrollBar *m_display_horizontalScrollBar;

    QPushButton *m_button_clear;
    QPushButton *m_button_save;
    QPushButton *m_button_stop;
    bool m_stop = true;

    QCPRange m_x_axis_range;
    QCPRange m_y_axis_range;

    QCPRange m_x_axis_range_temp;
    QCPRange m_y_axis_range_temp;
    // int m_hori_value = 0;
    // int m_verti_value = 0;

    plotDataHandlerThread m_plot_thread;
};

#endif // SERIALPLOTTER_H
