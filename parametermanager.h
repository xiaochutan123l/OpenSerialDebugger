#ifndef PARAMETERMANAGER_H
#define PARAMETERMANAGER_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include "command.h"
#include "protocolparser.h"

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

signals:
    void sendCommand(QByteArray &cmd);

public slots:
    void onCmdSelected(const QString & name);
    // void onDataUpdated();
    virtual void connect_widgets(parameterManager *pManager)=0;

    void addComboBoxList(QList<QString> &names);
    void clearComboBoxList();
//protected:
public:
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
    void onDataUpdated();
    void onGetButtonPushed();
public:
    void connect_widgets(parameterManager *pManager);
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
};

/* ----------------------- switchParameterComboWidget --------------------------*/

class switchParameterComboWidget: public ParameterComboWidget {
    Q_OBJECT

public:
    explicit switchParameterComboWidget(QPushButton *action_button,
                                        QComboBox *combo_box,
                                        QMap<QString, Command> *commands,
                                        QList<QString> *commandNames,
                                        QObject *parent = nullptr);
public slots:
    void onActionButtonPushed();
public:
    void connect_widgets(parameterManager *pManager);
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
    void addSwitchComboWidget(QPushButton *action_button,
                           QComboBox *combo_box);

signals:
    void sendCommandBytes(QByteArray &cmd);

public slots:
    void onNewFileLoaded();
    void onNewFileParsed();

    void onSendCommandBytes(QByteArray &cmd);
private:
    QList<setParameterComboWidget> m_setList;
    QList<getParameterComboWidget> m_getList;
    QList<switchParameterComboWidget> m_switchList;

    QMap<QString, Command> m_getCommands;
    QMap<QString, Command> m_setCommands;
    QMap<QString, Command> m_switchCommands;

    QList<QString> m_getCommandNames;
    QList<QString> m_setCommandNames;
    QList<QString> m_switchCommandNames;

    QPushButton *m_load_button;
    ProtocolParser *m_parser;
};

#endif // PARAMETERMANAGER_H
