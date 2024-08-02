#ifndef PARAMETERMANAGER_H
#define PARAMETERMANAGER_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include "command.h"
#include "protocolparser.h"

/* 一个qt WidgetTree的坑:
比如在addActionComboWidget()里面
void parameterManager::addActionComboWidget(QPushButton *action_button,
                                            QComboBox *combo_box)
{
    m_actionList.emplace_back(action_button,
                           combo_box,
                           &m_actionCommands,
                           &m_actionCommandNames,
                           this);
}
这个actionList里面的元素会接收parameterManager指针作为元素的父类指针，
然后在多次emplace_back元素之后，list或者vector会动态扩容，导致会调用构造函数，
拷贝构造函数和移动构造函数，产生的临时变量都将this也就是parameterManager作为父类widget指针，
导致parameerManager的children里面会有很多额外的child，
其实我只emplace了4个元素，但是却给parameterManager带来了十几个children，由于临时变量会在作用域结束时析构，
这样导致最后析构的时候这些额外的child已经被释放了，所以qt在析构他们的时候就segmentfault了，
导致程序崩溃。

解决办法：
1. 在一开始指定vector大小，这样就可以避免动态扩容导致的额外children。
    (这个方案操作最简单，目前用的这个，之后如果需要变长的vector元素那么就得修改)
2. 使用智能指针
    使用 QSharedPointer 或 std::unique_ptr 可以解决因动态扩容导致的问题，也就是说vector元素是widget指针而不是class
    因为它们管理对象的生命周期并避免复制对象本身
        myVector.append(QSharedPointer<MyClass>::create(this));
        or
        myUniqueVector.append(std::make_unique<MyClass>(this));
3. 在所有元素添加完之后，手动设置parent.
*/
#define ACTION_COMBO_WIDGET_NUM 4
#define GET_COMBO_WIDGET_NUM 6
#define SET_COMBO_WIDGET_NUM 10

class parameterManager;

/* ----------------------- ParameterComboWidget --------------------------*/

class ParameterComboWidget: public QObject {
    Q_OBJECT

public:
    ParameterComboWidget(QPushButton *button,
                                    QLineEdit *line_input,
                                    QComboBox *combo_box,
                                    QMap<QString, Command> *commands,
                                    QList<QString> *commandNames,
                                     QObject *parent = nullptr);
    // Copy constructor
    ParameterComboWidget(const ParameterComboWidget &other_widget);

    // Move constructor
    ParameterComboWidget(ParameterComboWidget &&other_widget);

    // Assignment constructor
    ParameterComboWidget& operator=(const ParameterComboWidget &other_widget);

    // Move assignment constructor
    ParameterComboWidget& operator=(ParameterComboWidget &&other_widget);

    void setComboBoxList(QList<QString> &names);
    void clearWidget();
    void setWidgetsVisible(bool visible);

    uint16_t commandNumber();
    Command::DataType commandDataType();

signals:
    void sendCommand(QByteArray &cmd);

public slots:
    void onCmdSelected(const QString & name);
    virtual void connect_widgets(parameterManager *pManager)=0;

protected:

    QPushButton *m_button;
    QLineEdit *m_line_input;
    QComboBox *m_combo_box;
    Command *m_command;
    QMap<QString, Command> *m_commands;
    QList<QString> *m_commandNames;
};

/* ----------------------- getParameterComboWidget --------------------------*/

class getParameterComboWidget: public ParameterComboWidget {
    Q_OBJECT

public:
    explicit getParameterComboWidget(QPushButton *get_button,
                                     QLineEdit *line_input,
                                     QComboBox *combo_box,
                                     QMap<QString, Command> *commands,
                                     QList<QString> *commandNames,
                                     QObject *parent = nullptr);

public slots:
    void onDataUpdated(const QByteArray &data, Command::DataType type);
    void onGetButtonPushed();
public:
    void connect_widgets(parameterManager *pManager);
    void disconnect_widgets(parameterManager *pManager);
};

/* ----------------------- setParameterComboWidget --------------------------*/

class setParameterComboWidget: public ParameterComboWidget {
    Q_OBJECT

public:
    explicit setParameterComboWidget(QPushButton *send_button,
                                    QLineEdit *line_input,
                                    QComboBox *combo_box,
                                    QMap<QString, Command> *commands,
                                    QList<QString> *commandNames,
                                    QObject *parent = nullptr);

public slots:
    void onDataStringChanged();
    void onSendButtonPushed();
public:
    void connect_widgets(parameterManager *pManager);
    void disconnect_widgets(parameterManager *pManager);
};

/* ----------------------- actionParameterComboWidget --------------------------*/

class actionParameterComboWidget: public ParameterComboWidget {
    Q_OBJECT

public:
    explicit actionParameterComboWidget(QPushButton *action_button,
                                        QComboBox *combo_box,
                                        QMap<QString, Command> *commands,
                                        QList<QString> *commandNames,
                                        QObject *parent = nullptr);
public slots:
    void onActionButtonPushed();
public:
    void connect_widgets(parameterManager *pManager);
    void disconnect_widgets(parameterManager *pManager);
};

/* ----------------------- parameterManager --------------------------*/

class parameterManager: public QObject {
    Q_OBJECT

public:
    parameterManager(QPushButton *load_button, QObject *parent = nullptr);
    void addGetComboWidget(QPushButton *set_button,
                          QLineEdit *line_input,
                          QComboBox *combo_box);
    void addSetComboWidget(QPushButton *send_button,
                           QLineEdit *line_input,
                           QComboBox *combo_box);
    void addActionComboWidget(QPushButton *action_button,
                           QComboBox *combo_box);
    ~parameterManager() {
        qDebug() << "deconstruct parameter manager";
    };

signals:
    void sendCommandBytes(QByteArray &cmd);
    void updateGetParameter(const QByteArray &packet, Command::DataType type);
public slots:
    void onNewFileLoaded();
    void onNewFileParsed();

    void onUpdateGetParameter(const QByteArray &packet);

    void onSendCommandBytes(QByteArray &cmd);
private:
    QVector<setParameterComboWidget> m_setList;
    QVector<getParameterComboWidget> m_getList;
    QVector<actionParameterComboWidget> m_actionList;

    QMap<QString, Command> m_getCommands;
    QMap<QString, Command> m_setCommands;
    QMap<QString, Command> m_actionCommands;

    QList<QString> m_getCommandNames;
    QList<QString> m_setCommandNames;
    QList<QString> m_actionCommandNames;

    QPushButton *m_load_button;
    ProtocolParser *m_parser;
};

#endif // PARAMETERMANAGER_H
