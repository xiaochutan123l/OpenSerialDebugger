#ifndef SERIALPLOTTER_H
#define SERIALPLOTTER_H

#include <QObject>
#include "qcustomplot.h"
#include <QDebug>
#include <QThread>

#include <atomic>
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
                  QPushButton *_auto,
                  QCustomPlot *display_plot,
                  QScrollBar *display_verticalScrollBar,
                  QScrollBar *display_horizontalScrollBar);
    ~serialPlotter();

signals:
    void newLinesReceived(const QStringList lines, QCPRange xRange, bool auto_mode = true);
    void axisRangeChanged(QCPRange xRange);
    void clearPlotData();
    void savePlotDataToCSV(const QString &fileName);
public slots:
    void onNewLinesReceived(const QStringList &lines);

    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);

    void onSaveButtonClicked();
    void onClearButtonClicked();
    void onStopButtonClicked();
    void onAutoButtonClicked();

    void onCurveNumChanged(int new_num);
    void onReadyForPlot(PlotDataPtrList data, QCPRange xRange, QCPRange yRange, bool auto_mode);
private:
    void setupDisplayPlot(int numGraphs);

    void setXAxis(QCPRange range) {m_x_axis_range = range;};
    void setYAxis(QCPRange range) {m_y_axis_range = range;};
    QCPRange getXAxis() {return m_x_axis_range;};
    QCPRange getYAxis() {return m_y_axis_range;};

    QList<QColor> m_pen_colors = {Qt::blue, Qt::red, Qt::green, Qt::black, Qt::gray};

    QCustomPlot *m_display_plot;
    QScrollBar *m_display_verticalScrollBar;
    QScrollBar *m_display_horizontalScrollBar;

    QPushButton *m_button_clear;
    QPushButton *m_button_save;
    QPushButton *m_button_stop;
    QPushButton *m_button_auto;
    bool m_stop = false;
    bool m_auto = true;

    QCPRange m_x_axis_range;
    QCPRange m_y_axis_range;

    QCPRange m_x_axis_range_temp;
    QCPRange m_y_axis_range_temp;

    //std::atomic_bool m_axis_changed = false;
    // int m_hori_value = 0;
    // int m_verti_value = 0;

    plotDataHandlerThread m_plot_thread;
    QThread *m_thread;
    std::atomic_bool m_plot_data_finished = true;
    QStringList m_qeued_lines;


};

#endif // SERIALPLOTTER_H
