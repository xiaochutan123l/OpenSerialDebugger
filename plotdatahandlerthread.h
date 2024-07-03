#ifndef PLOTDATAHANDLERTHREAD_H
#define PLOTDATAHANDLERTHREAD_H

#include <QObject>
#include "qcustomplot.h"

#include <deque>

#define RING_BUFFER_SIZE 50000
#define PLOT_BUFFER_SIZE 1000

using PlotDataPtr = QSharedPointer<QCPDataContainer<QCPGraphData>>;
using PlotDataPtrList = QVector<PlotDataPtr>;

class RingBuffer {
public:
    RingBuffer(size_t size) : maxSize(size) {}

    // Reset deques for initializing with empty elements.
    void reset(int curve_num) {
        buffer.clear();
        xData.clear();
        xCount = 0;
        for (int i = 0; i < curve_num; i++) {
            buffer.append(std::deque<double>());
        }
    }

    void add(const QVector<double> &yValues) {
        for(int i = 0; i < yValues.size(); i++) {
            if (buffer[i].size() == maxSize) {
                buffer[i].pop_front();
            }
            buffer[i].push_back(yValues[i]);
        }
        if (buffer[0].size() < maxSize) {
            xData.push_back(++xCount);
        }
    }

    void getPlotValues(PlotDataPtrList &plot_data, size_t start, size_t end) {
        if (start >= buffer[0].size() || end > buffer[0].size() || start > end) {
            qDebug() << "start: " << start << ", end: " << end << "buffer size: " << buffer[0].size();
            throw std::out_of_range("Invalid range");
        }
        plotData.clear();
        // plot_data.clear();
        // for (int i = 0; i < end - start; i++) {
        //     for (int j = 0; i < buffer.size(); j++) {
        //         plot_data.emplace_back(buffer[j])
        //     }
        // }
        qDebug() << "start: " << start << ", end: " << end << "buffer size: " << buffer[0].size();
        qDebug() << "start processing data";
        for (int i = 0; i < buffer.size(); i++) {
            plotData.append(QVector<QCPGraphData>());
            for (int j = start; j < end; j++) {
                plotData[i].emplace_back(j, buffer[i][j]);
            }
            qDebug() << "plotData ok";
            plot_data[i]->set(plotData[i], true);
        }
        qDebug() << "getPlotValues ok";
    }

    size_t size() {
        if (buffer.size() > 0) {
            return buffer[0].size();
        }
        return 0;
    }

    void clear() {
        buffer.clear();
        xData.clear();
        xCount = 0;
    }

private:
    QVector<std::deque<double>> buffer;
    size_t maxSize;
    std::deque<double> xData;
    size_t xCount = 0;
    QVector<QVector<QCPGraphData>> plotData;
};



class plotDataHandlerThread : public QObject {
    Q_OBJECT
public:
    plotDataHandlerThread(){};
    ~plotDataHandlerThread(){};
signals:
    void readyForPlot(PlotDataPtrList &data);
    void curveNumChanged(int new_num);
public slots:
    void onNewDataReceived(const QStringList &lines);
    void onAxisChanged(QCPRange range);
    void onClearPlotData();
    void onAutoModeChanged(bool mode);
private:
    void handleData(const QStringList &lines);
    QVector<double> extractNumbers(const QString &input);
    void updateDisplayPlotData(const QVector<double> &yValues);
    void extractPlotData();
    void resetData();

    RingBuffer m_graphData = RingBuffer(RING_BUFFER_SIZE);

    // represent the begin and end position of plot data in the big ring buffer.
    PlotDataPtrList m_plot_data;

    QVector<QCPDataContainer<QCPGraphData>> m_plot_data_content;

    QCPRange m_axis_range;
    int m_curve_num = 0;

    bool m_auto_mode = false;
};

#endif // PLOTDATAHANDLERTHREAD_H
