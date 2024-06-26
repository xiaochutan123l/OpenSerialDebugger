#ifndef CLICKABLECOMBOBOX_H
#define CLICKABLECOMBOBOX_H

#include <QComboBox>
#include <QMouseEvent>

class ClickableComboBox : public QComboBox
{
    Q_OBJECT

public:
    using QComboBox::QComboBox;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QComboBox::mousePressEvent(event);
    }
};

#endif // CLICKABLECOMBOBOX_H
