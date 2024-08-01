#include "serialplotter.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

QVector<double> extractNumbers(const QString &input);

serialPlotter::serialPlotter(QObject *parent,
                             QPushButton *clear,
                             QPushButton *save,
                             QPushButton *stop,
                             QPushButton *_auto,
                             QCustomPlot *display_plot,
                             QScrollBar *display_verticalScrollBar,
                             QScrollBar *display_horizontalScrollBar)
    : QObject(parent),
    m_button_clear(clear),
    m_button_save(save),
    m_button_stop(stop),
    m_button_auto(_auto),
    m_display_plot(display_plot),
    m_display_verticalScrollBar(display_verticalScrollBar),
    m_display_horizontalScrollBar(display_horizontalScrollBar),
    m_thread(new QThread(this))
{
    //connect(m_display_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_horzScrollBarChanged(int)));
    //connect(m_display_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_vertScrollBarChanged(int)));
    connect(m_display_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(m_display_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    connect(m_button_clear, &QPushButton::clicked, this, &serialPlotter::onClearButtonClicked);
    connect(m_button_save, &QPushButton::clicked, this, &serialPlotter::onSaveButtonClicked);
    connect(m_button_stop, &QPushButton::clicked, this, &serialPlotter::onStopButtonClicked);
    connect(m_button_auto, &QPushButton::clicked, this, &serialPlotter::onAutoButtonClicked);


    qRegisterMetaType<PlotDataPtrList>("PlotDataPtrList");

    m_plot_thread.moveToThread(m_thread);

    connect(this, &serialPlotter::newLinesReceived, &m_plot_thread, &plotDataHandlerThread::onNewDataReceived);
    connect(this, &serialPlotter::axisRangeChanged, &m_plot_thread, &plotDataHandlerThread::onAxisChanged);
    connect(&m_plot_thread, &plotDataHandlerThread::curveNumChanged, this, &serialPlotter::onCurveNumChanged);
    connect(&m_plot_thread, &plotDataHandlerThread::readyForPlot, this, &serialPlotter::onReadyForPlot);
    connect(this, &serialPlotter::clearPlotData, &m_plot_thread, &plotDataHandlerThread::onClearPlotData);
    connect(this, &serialPlotter::savePlotDataToCSV, &m_plot_thread, &plotDataHandlerThread::onSavePlotDataToCSV);

    connect(m_thread, &QThread::finished, &m_plot_thread, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);

    m_thread->start();
    //setupDisplayPlot(MAX_GRAPH_NUM);

    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
    // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
    // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
    // just increase the minimum/maximum values of the scroll bars as needed.
    m_display_horizontalScrollBar->setHidden(true);
    m_display_verticalScrollBar->setHidden(true);

    m_button_stop->setText("Stop");
}

serialPlotter::~serialPlotter() {
    qDebug() <<  "deconstruct serial plotter";

    disconnect(m_thread, &QThread::finished, &m_plot_thread, &QObject::deleteLater);
    disconnect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);

    if (m_thread->isRunning()) {
        m_thread->quit();
        m_thread->wait();
    }
}

void serialPlotter::onNewLinesReceived(const QStringList &lines) {
    if (m_stop) {
        return;
    }
    // store lines when plot data not finished yet.
    m_qeued_lines.append(lines);
    if (m_plot_data_finished) {
        m_x_axis_range_temp = getXAxis();
        m_y_axis_range_temp = getYAxis();
        m_plot_data_finished = false;
        emit newLinesReceived(m_qeued_lines, m_x_axis_range_temp, m_auto);
        m_qeued_lines.clear();
    }
}

void serialPlotter::onCurveNumChanged(int new_num) {
    setupDisplayPlot(new_num);
}

void serialPlotter::onReadyForPlot(PlotDataPtrList data, QCPRange xRange, QCPRange yRange, bool auto_mode) {
    //qDebug() << "plot: set data";
    for (int i = 0; i < data.size(); i++) {
        m_display_plot->graph(i)->setData(data[i]);
    }
    //qDebug() << "plot: set axis";
    if (auto_mode) {
        m_display_plot->xAxis->setRange(xRange);
        m_display_plot->yAxis->setRange(yRange);
    }
    else {
        m_display_plot->xAxis->setRange(m_x_axis_range_temp);
        m_display_plot->yAxis->setRange(m_y_axis_range_temp);
    }
    //QElapsedTimer timer;
    //timer.start();
    m_display_plot->replot();
    //qDebug() << "raw plot timer: " << timer.elapsed() << "ms";
    m_plot_data_finished = true;
}

void serialPlotter::xAxisChanged(QCPRange range)
{
  setXAxis(range);
    // make zooming possible when plot stopped
  if (m_x_axis_range_temp != range && m_plot_data_finished && m_stop) {
      m_x_axis_range_temp = getXAxis();
      m_y_axis_range_temp = getYAxis();
      m_plot_data_finished = false;
      emit axisRangeChanged(m_x_axis_range);
  }
}

void serialPlotter::yAxisChanged(QCPRange range)
{
  //qDebug() << "y axis change: " << range;
  setYAxis(range);
}


void serialPlotter::setupDisplayPlot(int numGraphs)
{
    m_display_plot->clearGraphs(); // 清除现有的图表

    // 动态创建图表和数据容器
    for (int i = 0; i < numGraphs; ++i) {
        m_display_plot->addGraph();
        QPen pen(m_pen_colors[i]);
        pen.setWidth(GRAPH_PEN_WIDTH); // 设置线的粗细
        m_display_plot->graph()->setPen(pen);
        //m_display_plot->graph()->setPen(m_pen_colors[i]);
        m_display_plot->legend->setVisible(true);
        // 关闭抗锯齿
        m_display_plot->graph()->setAntialiased(false);
    }

    m_display_plot->axisRect()->setupFullAxesBox(true);
    m_display_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_display_plot->setNoAntialiasingOnDrag(true);
    //qDebug() << "set up display plot";
    // if (m_thread->isRunning()) {
    //     m_thread->quit();
    //     m_thread->wait();
    // }
    // start plot thread
    /*m_thread->start();
    m_plot_thread.moveToThread(m_thread);*/  // 确保 worker 对象仍然在正确的线程中
}

/*
Auto Mode:
xAxis 最新的1000组数据 range: (n-1000, n)
yAxis range: (min, max) of y(n-1000, n)

*/

void serialPlotter::onSaveButtonClicked() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Plot Data", "", "CSV files (*.csv)");
    if (!fileName.isEmpty()) {
        emit savePlotDataToCSV(fileName);
    }
}

void serialPlotter::onClearButtonClicked() {
    // quit plot thread
    m_display_plot->clearGraphs();
    emit clearPlotData();
    m_display_plot->replot();
}

void serialPlotter::onStopButtonClicked() {
    m_stop = !m_stop;
    if (m_stop) {
        m_button_stop->setText("Run");
        m_button_save->setEnabled(true);
    }
    else {
        m_button_stop->setText("Stop");
        m_button_save->setEnabled(false);
    }
}

void serialPlotter::onAutoButtonClicked() {
    m_auto = !m_auto;
}
