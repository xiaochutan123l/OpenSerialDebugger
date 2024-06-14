#include "serialplotter.h"

serialPlotter::serialPlotter(QObject *parent,
                             QCustomPlot *display_plot,
                             QScrollBar *display_verticalScrollBar,
                             QScrollBar *display_horizontalScrollBar)
    : QObject(parent),
      m_display_plot(display_plot),
      m_display_verticalScrollBar(display_verticalScrollBar),
      m_display_horizontalScrollBar(display_horizontalScrollBar)
{
    connect(m_display_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_horzScrollBarChanged(int)));
    connect(m_display_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(display_vertScrollBarChanged(int)));
    connect(m_display_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(m_display_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
}

serialPlotter::~serialPlotter() {
    qDebug() <<  "deconstruct serial plotter";
}

void serialPlotter::onNewLineReceived(const QString &line) {

}

bool serialPlotter::isValidFormat(const QString &line)
{
    // 查找冒号的位置
    int colonIndex = line.indexOf(':');
    if (colonIndex == -1) {
        return false; // 没有冒号
    }

    // 提取冒号后的部分
    QString dataPart = line.mid(colonIndex + 1).trimmed();
    if (dataPart.isEmpty()) {
        return false; // 冒号后没有数据
    }

    // 检查是否由逗号分隔的数字组成
    QStringList numbers = dataPart.split(',');
    if (numbers.isEmpty()) {
        return false;
    }

    for (const QString &number : numbers) {
        bool ok;
        number.toDouble(&ok);
        if (!ok) {
            return false;
        }
    }

    return true;
}

void serialPlotter::display_horzScrollBarChanged(int value)
{
  if (qAbs(m_display_plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    m_display_plot->xAxis->setRange(value/100.0, m_display_plot->xAxis->range().size(), Qt::AlignCenter);
    m_display_plot->replot();
  }
}

void serialPlotter::display_vertScrollBarChanged(int value)
{
  if (qAbs(m_display_plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    m_display_plot->yAxis->setRange(-value/100.0, m_display_plot->yAxis->range().size(), Qt::AlignCenter);
    m_display_plot->replot();
  }
}

void serialPlotter::xAxisChanged(QCPRange range)
{
  m_display_horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  m_display_horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void serialPlotter::yAxisChanged(QCPRange range)
{
  m_display_verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  m_display_verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

