#ifndef PLOTDATAHANDLERTHREAD_H
#define PLOTDATAHANDLERTHREAD_H

#include <QObject>
#include "qcustomplot.h"

#include <deque>
#include <iostream>
#include <vector>

#define RING_BUFFER_SIZE 50000

template <typename T>
class RingBuffer {
public:
    RingBuffer(size_t size) : maxSize(size) {}

    void add(const T& item) {
        if (buffer.size() == maxSize) {
            buffer.pop_front();
        }
        buffer.push_back(item);
    }

    std::vector<T> getRange(size_t start, size_t end) const {
        if (start >= buffer.size() || end >= buffer.size() || start > end) {
            throw std::out_of_range("Invalid range");
        }
        return std::vector<T>(buffer.begin() + start, buffer.begin() + end + 1);
    }

    void print() const {
        for (const auto& item : buffer) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

private:
    std::deque<T> buffer;
    size_t maxSize;
};



class plotDataHandlerThread : public QObject {
    Q_OBJECT
public:
    plotDataHandlerThread();
signals:
    void readyForPlot();
public slots:
    void onNewDataReceived(QStringList &lines);
    void onAxisChanged();
    void onClearData();
private:
    void handleData();

    // the big ring buffer recording all the data.
    QVector<double> m_xData;
    RingBuffer<QVector<double>> m_graphData = RingBuffer<QVector<double>>(RING_BUFFER_SIZE);

    // represent the begin and end position of plot data in the big ring buffer.
    size_t m_index_begin;
    size_t m_index_end;

    QCPDataContainer<QCPGraphData> m_plot_data;
};

#endif // PLOTDATAHANDLERTHREAD_H
