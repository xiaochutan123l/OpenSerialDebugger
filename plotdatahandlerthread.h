#ifndef PLOTDATAHANDLERTHREAD_H
#define PLOTDATAHANDLERTHREAD_H

#include <QObject>
#include "qcustomplot.h"

#include <deque>

#define RING_BUFFER_SIZE 50000
#define PLOT_BUFFER_SIZE 500 // 1000

using PlotDataPtr = QSharedPointer<QCPDataContainer<QCPGraphData>>;
using PlotDataPtrList = QVector<PlotDataPtr>;
Q_DECLARE_METATYPE(PlotDataPtrList)

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
            if (buffer[i].size() >= maxSize) {
                buffer[i].pop_front();
            }
            buffer[i].push_back(yValues[i]);
        }
        if (buffer[0].size() < maxSize) {
            xData.push_back(++xCount);
        }
    }

    QCPRange getPlotValues(PlotDataPtrList &plot_data, size_t start, size_t end) {
        if (plot_data.size() == 0 || start == end) {return QCPRange();}
        if (start >= buffer[0].size() || end > buffer[0].size() || start > end) {
            qDebug() << "start: " << start << ", end: " << end << "buffer size: " << buffer[0].size();
            throw std::out_of_range("Invalid range");
        }
        plotData.clear();
        QCPRange range;
        range.lower = buffer[0][start];
        range.upper = buffer[0][start];
        for (int i = 0; i < buffer.size(); i++) {
            plotData.append(QVector<QCPGraphData>());
            for (int j = start; j < end; j++) {
                double value = buffer[i][j];
                plotData[i].emplace_back(j, buffer[i][j]);

                if (value < range.lower) {
                    range.lower = value;
                }
                if (value > range.upper) {
                    range.upper = value;
                }
            }
             //qDebug() << "plotData ok";
            plot_data[i]->set(plotData[i], true);
            //qDebug() << "auto "  << i << ", range: " << range;
        }
        //qDebug() << "getPlotValues ok";
        // 稍微缩小一点让顶部和底部留有空间
        double mid = (range.upper + range.lower) / 2;
        double distance = (range.upper - range.lower) * 1.25;
        range.upper = mid + distance / 2;
        range.lower = mid - distance / 2;
        //qDebug() << "auto range: " << range;
        return range;
    }

    void getCompressedPlotValues(PlotDataPtrList &plot_data, size_t start, size_t end) {
        if (plot_data.size() == 0 || start == end) {return;}
        if (start >= buffer[0].size() || end > buffer[0].size() || start > end) {
            // qDebug() << "start: " << start << ", end: " << end << "buffer size: " << buffer[0].size();
            throw std::out_of_range("Invalid range");
        }
        plotData.clear();
        int step = (end - start) / PLOT_BUFFER_SIZE;
        //step = 1;
        //qDebug() << "step: " << step;
        for (int i = 0; i < buffer.size(); i++) {
            plotData.append(QVector<QCPGraphData>());
            for (int j = start; j < end; j += step) {
                plotData[i].emplace_back(j, buffer[i][j]);
            }
            //qDebug() << "plotData ok, data number: " << plot_data[i]->size();
            plot_data[i]->set(plotData[i], true);
        }
        //qDebug() << "getCompressed PlotValues ok";
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

    std::deque<double>& getXData() {return xData;}
    QVector<std::deque<double>>& getGraphData() {return buffer;}

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
    void readyForPlot(PlotDataPtrList data, QCPRange xRange, QCPRange yRange, bool auto_mode);
    void curveNumChanged(int new_num);
public slots:
    void onNewDataReceived(const QStringList lines, QCPRange xRange, bool auto_mode = true);
    void onAxisChanged(QCPRange range);
    void onClearPlotData();
    void onSavePlotDataToCSV(const QString &fileName);
private:
    void handleDataAuto(const QStringList &lines);
    void handleData(const QStringList &lines, QCPRange xRange);
    QVector<double> extractNumbers(const QString &input);
    void updateDisplayPlotData(const QVector<double> &yValues);
    void extractPlotData();
    void resetData();

    RingBuffer m_graphData = RingBuffer(RING_BUFFER_SIZE);

    // represent the begin and end position of plot data in the big ring buffer.
    PlotDataPtrList m_plot_data;

    QVector<QCPDataContainer<QCPGraphData>> m_plot_data_content;

    //QCPRange m_axis_range;
    int m_curve_num = 0;
};

#endif // PLOTDATAHANDLERTHREAD_H
