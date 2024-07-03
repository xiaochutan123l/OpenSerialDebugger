#include "plotdatahandlerthread.h"

void plotDataHandlerThread::onNewDataReceived(const QStringList &lines) {
    handleData(lines);
}

void plotDataHandlerThread::onAxisChanged(QCPRange range) {
    // TODO: add lock
    m_axis_range = range;
}

void plotDataHandlerThread::onClearPlotData() {
    m_plot_data.clear();
    m_graphData.clear();
    m_plot_data_content.clear();
    m_curve_num = 0;
    qDebug() << "cleared plot data";
}

void plotDataHandlerThread::onAutoModeChanged(bool mode) {
    m_auto_mode = mode;
}

void plotDataHandlerThread::handleData(const QStringList &lines) {
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
    if (size <= PLOT_BUFFER_SIZE ) {
        m_graphData.getPlotValues(m_plot_data, 0, size);
    }
    else {
        m_graphData.getPlotValues(m_plot_data, size - PLOT_BUFFER_SIZE, size);
    }
    qDebug() << "send ready for plot";
    emit readyForPlot(m_plot_data);

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

void plotDataHandlerThread::extractPlotData() {

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
