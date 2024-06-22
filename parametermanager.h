#ifndef PARAMETERMANAGER_H
#define PARAMETERMANAGER_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include "command.h"
#include "protocolparser.h"
/* ----------------------- getParameterComboWidget --------------------------*/

class getParameterComboWidget: public QObject {
    Q_OBJECT

public:
    explicit getParameterComboWidget(QPushButton *get_button,
                                    QLineEdit *line_input,
                                    QComboBox *combo_box,
                                    QMap<QString, Command> *commands,
                                    QList<QString> *commandNames,
                                    QObject *parent = nullptr);
    // Copy constructor
    explicit getParameterComboWidget(const getParameterComboWidget &other_widget);

    // Move constructor
    explicit getParameterComboWidget(getParameterComboWidget &&other_widget);

    // Assignment constructor
    getParameterComboWidget& operator=(const getParameterComboWidget &other_widget);

    // Move assignment constructor
    getParameterComboWidget& operator=(getParameterComboWidget &&other_widget);

public slots:
    void onCmdSelected(const QString & name);
    void onDataUpdated();
    void onGetButtonPushed();
    void addComboBoxList(QList<QString> &names);
    void clearComboBoxList();
private:
    QPushButton *m_get_button;
    QLineEdit *m_line_input;
    QComboBox *m_combo_box;
    Command *m_command;
    QMap<QString, Command> *m_commands;
    QList<QString> *m_commandNames;
};

/* ----------------------- setParameterComboWidget --------------------------*/

class setParameterComboWidget: public QObject {
    Q_OBJECT

public:
    explicit setParameterComboWidget(QPushButton *send_button,
                                    QLineEdit *line_input,
                                    QComboBox *combo_box,
                                    QMap<QString, Command> *commands,
                                    QList<QString> *commandNames,
                                    QObject *parent = nullptr);

    // Copy constructor
    explicit setParameterComboWidget(const setParameterComboWidget &other_widget);

    // Move constructor
    explicit setParameterComboWidget(setParameterComboWidget &&other_widget);

    // Assignment constructor
    setParameterComboWidget& operator=(const setParameterComboWidget &other_widget);

    // Move assignment constructor
    setParameterComboWidget& operator=(setParameterComboWidget &&other_widget);

public slots:
    void onCmdSelected(const QString & name);
    void onDataStringChanged();
    void onSendButtonPushed();
    void addComboBoxList(QList<QString> &names);
    void clearComboBoxList();
private:
    QPushButton *m_send_button;
    QLineEdit *m_line_input;
    QComboBox *m_combo_box;
    Command *m_command;
    QMap<QString, Command> *m_commands;
    QList<QString> *m_commandNames;
};

/* ----------------------- switchParameterComboWidget --------------------------*/

class switchParameterComboWidget: public QObject {
    Q_OBJECT

public:
    explicit switchParameterComboWidget(QPushButton *action_button,
                                        QComboBox *combo_box,
                                        QMap<QString, Command> *commands,
                                        QList<QString> *commandNames,
                                        QObject *parent = nullptr);

    // Copy constructor
    explicit switchParameterComboWidget(const switchParameterComboWidget &other_widget);

    // Move constructor
    explicit switchParameterComboWidget(switchParameterComboWidget &&other_widget);

    // Assignment constructor
    switchParameterComboWidget& operator=(const switchParameterComboWidget &other_widget);

    // Move assignment constructor
    switchParameterComboWidget& operator=(switchParameterComboWidget &&other_widget);
public slots:
    void onCmdSelected(const QString & name);
    void onActionButtonPushed();
    void addComboBoxList(QList<QString> &names);
    void clearComboBoxList();
private:
    QPushButton *m_action_button;
    QComboBox *m_combo_box;
    Command *m_command;
    QMap<QString, Command> *m_commands;
    QList<QString> *m_commandNames;
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

public slots:
    void onNewFileLoaded();
    void onNewFileParsed();

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
