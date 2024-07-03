#include "serialplotter.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

QVector<double> extractNumbers(const QString &input);

serialPlotter::serialPlotter(QObject *parent,
                             QPushButton *clear,
                             QPushButton *save,
                             QPushButton *stop,
                             QCustomPlot *display_plot,
                             QScrollBar *display_verticalScrollBar,
                             QScrollBar *display_horizontalScrollBar)
    : QObject(parent),
    m_button_clear(clear),
    m_button_save(save),
    m_button_stop(stop),
    m_display_plot(display_plot),
    m_display_verticalScrollBar(display_verticalScrollBar),
    m_display_horizontalScrollBar(display_horizontalScrollBar)
{
    //connect(m_display_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_horzScrollBarChanged(int)));
    //connect(m_display_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_vertScrollBarChanged(int)));
    connect(m_display_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(m_display_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    connect(m_button_clear, &QPushButton::clicked, this, &serialPlotter::onClearButtonClicked);
    connect(m_button_save, &QPushButton::clicked, this, &serialPlotter::onSaveButtonClicked);
    connect(m_button_stop, &QPushButton::clicked, this, &serialPlotter::onStopButtonClicked);

    connect(this, &serialPlotter::newLinesReceived, &m_plot_thread, &plotDataHandlerThread::onNewDataReceived);
    connect(&m_plot_thread, &plotDataHandlerThread::curveNumChanged, this, &serialPlotter::onCurveNumChanged);
    connect(&m_plot_thread, &plotDataHandlerThread::readyForPlot, this, &serialPlotter::onReadyForPlot);
    connect(this, &serialPlotter::clearPlotData, &m_plot_thread, &plotDataHandlerThread::onClearPlotData);

    //setupDisplayPlot(MAX_GRAPH_NUM);

    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
    // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
    // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
    // just increase the minimum/maximum values of the scroll bars as needed.
    //m_display_horizontalScrollBar->setRange(-1000, 1000);
    //m_display_verticalScrollBar->setRange(-1000, 1000);
    //m_display_horizontalScrollBar->setDisabled(true);
    //m_display_verticalScrollBar->setDisabled(true);

    m_display_horizontalScrollBar->setHidden(true);
    m_display_verticalScrollBar->setHidden(true);

    m_button_stop->setText("Run");
}

serialPlotter::~serialPlotter() {
    qDebug() <<  "deconstruct serial plotter";
}

void serialPlotter::onNewLinesReceived(const QStringList &lines) {
    if (m_stop) {
        return;
    }

    m_x_axis_range_temp = getXAxis();
    m_y_axis_range_temp = getYAxis();
    emit newLinesReceived(lines, getXAxis(), getYAxis());
}

void serialPlotter::onCurveNumChanged(int new_num) {
    setupDisplayPlot(new_num);
}

void serialPlotter::onReadyForPlot(PlotDataPtrList &data) {
    qDebug() << "plot: set data";
    for (int i = 0; i < data.size(); i++) {
        m_display_plot->graph(i)->setData(data[i]);
    }
    qDebug() << "plot: set axis";
    // int left = data[0]->at(0)->key;
    // int right = data[0]->at(data[0]->size()-1)->key;
    // m_display_plot->xAxis->setRange(left, right);
    m_display_plot->xAxis->setRange(m_x_axis_range_temp);
    m_display_plot->yAxis->setRange(m_y_axis_range_temp);

    qDebug() << "plot";
    m_display_plot->replot();
}

void serialPlotter::xAxisChanged(QCPRange range)
{
  setXAxis(range);
}

void serialPlotter::yAxisChanged(QCPRange range)
{
  qDebug() << "y axis change: " << range;
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
    }

    m_display_plot->axisRect()->setupFullAxesBox(true);
    m_display_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

/*
Auto Mode:
xAxis 最新的1000组数据 range: (n-1000, n)
yAxis range: (min, max) of y(n-1000, n)

*/

void serialPlotter::onSaveButtonClicked() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Plot Data", "", "CSV files (*.csv)");
    if (!fileName.isEmpty()) {
        savePlotDataToCSV(fileName);
    }
}

void serialPlotter::onClearButtonClicked() {
    m_display_plot->clearGraphs();
    emit clearPlotData();
    m_display_plot->replot();
}

void serialPlotter::onStopButtonClicked() {
    m_stop = !m_stop;
    if (m_stop) {
        m_button_stop->setText("Run");
    }
    else {
        m_button_stop->setText("Stop");
    }
}

void serialPlotter::savePlotDataToCSV(const QString &fileName) {
    // QFile file(fileName);
    // if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //     QTextStream out(&file);

    //     // 写入标题行
    //     out << "X";
    //     for (int i = 0; i < m_graphData.size(); ++i) {
    //         out << ",Y" << i + 1;
    //     }
    //     out << "\n";

    //     // 写入数据行
    //     for (int j = 0; j < m_xData.size(); ++j) {
    //         out << m_xData[j];
    //         for (int i = 0; i < m_graphData.size(); ++i) {
    //             if (j < m_graphData[i].size()) {
    //                 out << "," << m_graphData[i][j];
    //             } else {
    //                 out << ",";
    //             }
    //         }
    //         out << "\n";
    //     }

    //     file.close();
    //     qDebug() << "Save succeed: " << fileName;
    // } else {
    //     qDebug() << "Failed to open file for writing: " << fileName;
    // }
}
