#include "plotdatahandlerthread.h"

void plotDataHandlerThread::onNewDataReceived(const QStringList lines, QCPRange xRange, bool auto_mode) {
    //qDebug() << "on new data received";
    //qDebug() << "Worker thread ID:" << QThread::currentThreadId();
    if (auto_mode) {
        handleDataAuto(lines);
    }
    else {
        handleData(lines, xRange);
    }
}

void plotDataHandlerThread::onAxisChanged(QCPRange range) {
    // TODO: add lock
    size_t size = m_graphData.size();
    //qDebug() << "lower: " << xRange.lower << ", upper: " << xRange.upper;
    if (range.lower > size || range.upper <= 0) {
        // out of range, return nothing
        //m_graphData.clear();
        emit readyForPlot(m_plot_data, QCPRange(), QCPRange(), false);
        qDebug() << "wrong range: return" << range;
        return;
    }
    else {
        if (range.lower < 0) {
            range.lower = 0;
        }
        if (range.upper >= size) {
            range.upper = size;
        }
    }
    //m_axis_range = range;
    //qDebug() << range << "range";
    m_graphData.getPlotValues(m_plot_data, range.lower, range.upper);
    //qDebug() << "axis update, send ready for plot";
    emit readyForPlot(m_plot_data, range, QCPRange(), false);
}

void plotDataHandlerThread::onClearPlotData() {
    m_plot_data.clear();
    m_graphData.clear();
    m_plot_data_content.clear();
    m_curve_num = 0;
    //qDebug() << "cleared plot data";
}

void plotDataHandlerThread::handleDataAuto(const QStringList &lines) {
    if (lines.size() == 0) {
        return;
    }
    for (auto &line : lines) {
        updateDisplayPlotData(extractNumbers(line));
    }

    size_t size = m_graphData.size();
    if (size == 0) {
        // do nothing if no data
        return;
    }
    QCPRange xRange;
    QCPRange yRange;
    if (size <= PLOT_BUFFER_SIZE ) {
        xRange.lower = 0;
        xRange.upper = size;
        yRange = m_graphData.getPlotValues(m_plot_data, 0, size);
    }
    else {
        xRange.lower = size - PLOT_BUFFER_SIZE;
        xRange.upper = size;
        yRange = m_graphData.getPlotValues(m_plot_data, xRange.lower, xRange.upper);
    }
    //qDebug() << "send ready for plot";
    emit readyForPlot(m_plot_data, xRange, yRange, true);

}

void plotDataHandlerThread::handleData(const QStringList &lines, QCPRange xRange) {
    if (lines.size() == 0) {
        return;
    }
    for (auto &line : lines) {
        updateDisplayPlotData(extractNumbers(line));
    }

    size_t size = m_graphData.size();
    //qDebug() << "lower: " << xRange.lower << ", upper: " << xRange.upper;
    if (xRange.lower > size || xRange.upper <= 0) {
        // out of range, return nothing
        //m_graphData.clear();
        emit readyForPlot(m_plot_data, QCPRange(), QCPRange(), false);
        qDebug() << "wrong range: return" << xRange;
        return;
    }
    else {
        if (xRange.lower < 0) {
            xRange.lower = 0;
        }
        if (xRange.upper >= size) {
            xRange.upper = size;
        }
    }
    //qDebug() << "new range: " << xRange;
    // 当用户手动设置x轴范围超过两倍最大plotbuffer，就采样。
    // 小于两倍不好采样。
    if (xRange.size() <= PLOT_BUFFER_SIZE * 2) {
        m_graphData.getPlotValues(m_plot_data, xRange.lower, xRange.upper);
    }
    // 大于两倍的时候采样输出，保证画图最大两倍plotbuffer数据，保证流畅性
    else {
        m_graphData.getCompressedPlotValues(m_plot_data, xRange.lower, xRange.upper);
    }
    //qDebug() << "send ready for plot";
    emit readyForPlot(m_plot_data, QCPRange(), QCPRange(), false);

}

void plotDataHandlerThread::updateDisplayPlotData(const QVector<double> &yValues)
{
    if (yValues.isEmpty()) {
        return;
    }
    // 限制参数数量必须一致，不然重新绘制
    // 确保 yValues 数量与图表数量匹配
    if (m_curve_num != yValues.size()) {
        m_curve_num = yValues.size();
        resetData();
        emit curveNumChanged(m_curve_num);
    }
    m_graphData.add(yValues);

}

void plotDataHandlerThread::resetData() {
    m_plot_data.clear();
    m_graphData.reset(m_curve_num);
    m_plot_data_content.clear();
    for (int i = 0; i < m_curve_num; i++) {
        m_plot_data_content.append(QCPDataContainer<QCPGraphData>());
        PlotDataPtr ptr = QSharedPointer<QCPDataContainer<QCPGraphData>>::create();
        *ptr = m_plot_data_content[i];
        m_plot_data.append(ptr);
    }
}

// 读取字符串中的数字，包含数字有效性检查
QVector<double> plotDataHandlerThread::extractNumbers(const QString &input)
{
    QVector<double> numbers;

    // 找到冒号后的部分
    int colonIndex = input.indexOf(':');
    if (colonIndex == -1) {
        return numbers; // 未找到冒号，返回空列表
    }

    // 获取冒号后的子字符串，并去除空格
    QString dataPart = input.mid(colonIndex + 1).trimmed();

    // 分割字符串，提取数字
    QStringList numberStrings = dataPart.split(',', Qt::SplitBehaviorFlags::SkipEmptyParts);

    // 转换为数字并添加到列表
    for (const QString &numberStr : numberStrings) {
        bool ok;
        double number = numberStr.toDouble(&ok);
        if (ok) {
            numbers.append(number);
        }
    }

    return numbers;
}

void plotDataHandlerThread::onSavePlotDataToCSV(const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        std::deque<double> &x_data = m_graphData.getXData();
        QVector<std::deque<double>> &graph_data =  m_graphData.getGraphData();
        // 写入标题行
        out << "X";
        for (int i = 0; i < graph_data.size(); ++i) {
            out << ",Y" << i + 1;
        }
        out << "\n";

        // 写入数据行
        for (int j = 0; j < x_data.size(); ++j) {
            out << x_data[j];
            for (int i = 0; i < graph_data.size(); ++i) {
                if (j < graph_data[i].size()) {
                    out << "," << graph_data[i][j];
                } else {
                    out << ",";
                }
            }
            out << "\n";
        }

        file.close();
        qDebug() << "Save succeed: " << fileName;
    } else {
        qDebug() << "Failed to open file for writing: " << fileName;
    }
}
