#include "parametermanager.h"

/* ----------------------- getParameterComboWidget --------------------------*/

getParameterComboWidget::getParameterComboWidget(QPushButton *get_button,
                                 QLineEdit *line_input,
                                 QComboBox *combo_box,
                                 QMap<QString, Command> *commands,
                                 QList<QString> *commandNames,
                                 QObject *parent)
    : m_get_button(get_button),
    m_line_input(line_input),
    m_combo_box(combo_box),
    m_commands(commands),
    m_commandNames(commandNames),
    QObject(parent)
{

}

// Copy constructor
getParameterComboWidget::getParameterComboWidget(const getParameterComboWidget &other_widget)
    : m_get_button(other_widget.m_get_button),
    m_line_input(other_widget.m_line_input),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{

}

// Move constructor
getParameterComboWidget::getParameterComboWidget(getParameterComboWidget &&other_widget)
    : m_get_button(other_widget.m_get_button),
    m_line_input(other_widget.m_line_input),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{
    other_widget.m_get_button = nullptr;
    other_widget.m_line_input = nullptr;
    other_widget.m_combo_box = nullptr;
    other_widget.m_commands = nullptr;
    other_widget.m_commandNames = nullptr;
    other_widget.d_ptr->parent = nullptr;
}

// Assignment constructor
getParameterComboWidget& getParameterComboWidget::operator=(const getParameterComboWidget &other_widget) {
    if (&other_widget != this) {
        m_get_button = other_widget.m_get_button;
        m_line_input = other_widget.m_line_input;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;
    }
    return *this;
}

// Move assignment constructor
getParameterComboWidget& getParameterComboWidget::operator=(getParameterComboWidget &&other_widget) {
    if (&other_widget != this) {
        m_get_button = other_widget.m_get_button;
        m_line_input = other_widget.m_line_input;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;

        other_widget.m_get_button = nullptr;
        other_widget.m_line_input = nullptr;
        other_widget.m_combo_box = nullptr;
        other_widget.m_commands = nullptr;
        other_widget.m_commandNames = nullptr;
        other_widget.d_ptr->parent = nullptr;
    }
    return *this;
}

void getParameterComboWidget::onCmdSelected(const QString & name) {
    m_command = &((*m_commands)[name]);
}

void getParameterComboWidget::onDataUpdated() {

}
void getParameterComboWidget::onGetButtonPushed() {

}

void getParameterComboWidget::addComboBoxList(QList<QString> &names) {
    m_combo_box->addItems(names);
    m_combo_box->setCurrentIndex(-1);
}

void getParameterComboWidget::clearComboBoxList() {
    m_combo_box->clear();
}

/* ----------------------- setParameterComboWidget --------------------------*/

setParameterComboWidget::setParameterComboWidget(QPushButton *send_button,
                                                 QLineEdit *line_input,
                                                 QComboBox *combo_box,
                                                 QMap<QString, Command> *commands,
                                                 QList<QString> *commandNames,
                                                 QObject *parent)
    : m_send_button(send_button),
    m_line_input(line_input),
    m_combo_box(combo_box),
    m_commands(commands),
    m_commandNames(commandNames),
    QObject(parent)
{

}

// Copy constructor
setParameterComboWidget::setParameterComboWidget(const setParameterComboWidget &other_widget)
    : m_send_button(other_widget.m_send_button),
    m_line_input(other_widget.m_line_input),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{

}

// Move constructor
setParameterComboWidget::setParameterComboWidget(setParameterComboWidget &&other_widget)
    : m_send_button(other_widget.m_send_button),
    m_line_input(other_widget.m_line_input),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{
    other_widget.m_send_button = nullptr;
    other_widget.m_line_input = nullptr;
    other_widget.m_combo_box = nullptr;
    other_widget.m_commands = nullptr;
    other_widget.m_commandNames = nullptr;
    other_widget.d_ptr->parent = nullptr;
}

// Assignment constructor
setParameterComboWidget& setParameterComboWidget::operator=(const setParameterComboWidget &other_widget) {
    if (&other_widget != this) {
        m_send_button = other_widget.m_send_button;
        m_line_input = other_widget.m_line_input;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;
    }
    return *this;
}

// Move assignment constructor
setParameterComboWidget& setParameterComboWidget::operator=(setParameterComboWidget &&other_widget) {
    if (&other_widget != this) {
        m_send_button = other_widget.m_send_button;
        m_line_input = other_widget.m_line_input;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;

        other_widget.m_send_button = nullptr;
        other_widget.m_line_input = nullptr;
        other_widget.m_combo_box = nullptr;
        other_widget.m_commands = nullptr;
        other_widget.m_commandNames = nullptr;
        other_widget.d_ptr->parent = nullptr;
    }
    return *this;
}

void setParameterComboWidget::onCmdSelected(const QString & name) {
    m_command = &(*m_commands)[name];
}
void setParameterComboWidget::onDataStringChanged() {

}
void setParameterComboWidget::onSendButtonPushed() {

}

void setParameterComboWidget::addComboBoxList(QList<QString> &names) {
    m_combo_box->addItems(names);
    m_combo_box->setCurrentIndex(-1);
}

void setParameterComboWidget::clearComboBoxList() {
    m_combo_box->clear();
}

/* ----------------------- switchParameterComboWidget --------------------------*/

switchParameterComboWidget::switchParameterComboWidget(QPushButton *action_button,
                                                 QComboBox *combo_box,
                                                 QMap<QString, Command> *commands,
                                                 QList<QString> *commandNames,
                                                 QObject *parent)
    : m_action_button(action_button),
    m_combo_box(combo_box),
    m_commands(commands),
    m_commandNames(commandNames),
    QObject(parent)
{

}

// Copy constructor
switchParameterComboWidget::switchParameterComboWidget(const switchParameterComboWidget &other_widget)
    : m_action_button(other_widget.m_action_button),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{

}

// Move constructor
switchParameterComboWidget::switchParameterComboWidget(switchParameterComboWidget &&other_widget)
    : m_action_button(other_widget.m_action_button),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{
    other_widget.m_action_button = nullptr;
    other_widget.m_combo_box = nullptr;
    other_widget.m_commands = nullptr;
    other_widget.m_commandNames = nullptr;
    other_widget.d_ptr->parent = nullptr;
}

// Assignment constructor
switchParameterComboWidget& switchParameterComboWidget::operator=(const switchParameterComboWidget &other_widget) {
    if (&other_widget != this) {
        m_action_button = other_widget.m_action_button;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;
    }
    return *this;
}

// Move assignment constructor
switchParameterComboWidget& switchParameterComboWidget::operator=(switchParameterComboWidget &&other_widget) {
    if (&other_widget != this) {
        m_action_button = other_widget.m_action_button;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;

        other_widget.m_action_button = nullptr;
        other_widget.m_combo_box = nullptr;
        other_widget.m_commands = nullptr;
        other_widget.m_commandNames = nullptr;
        other_widget.d_ptr->parent = nullptr;
    }
    return *this;
}

void switchParameterComboWidget::onCmdSelected(const QString & name) {
    m_command = &(*m_commands)[name];
}

void switchParameterComboWidget::onActionButtonPushed() {

}

void switchParameterComboWidget::addComboBoxList(QList<QString> &names) {
    m_combo_box->addItems(names);
    m_combo_box->setCurrentIndex(-1);
}

void switchParameterComboWidget::clearComboBoxList() {
    m_combo_box->clear();
}

/* ----------------------- parameterManager --------------------------*/

parameterManager::parameterManager(QPushButton *load_button,
                                   QObject *parent)
    : QObject(parent) , m_load_button(load_button){

    /* ----------------------- protocol parser --------------------------*/
    m_parser = new ProtocolParser(&m_getCommands, &m_setCommands, &m_switchCommands, this);
    m_parser->bindButton(m_load_button);
    connect(m_parser, &ProtocolParser::newFileLoaded, this, &parameterManager::onNewFileLoaded);
    connect(m_parser, &ProtocolParser::newFileParsed, this, &parameterManager::onNewFileParsed);

}

// TODO: change append to emplace?
void parameterManager::addGetComboWidget(QPushButton *get_button,
                                           QLineEdit *line_input,
                                           QComboBox *combo_box)
{
    m_getList.emplace_back(get_button,
                         line_input,
                         combo_box,
                         &m_getCommands,
                         &m_getCommandNames,
                         this);
}

void parameterManager::addSetComboWidget(QPushButton *send_button,
                                           QLineEdit *line_input,
                                           QComboBox *combo_box)
{
    m_setList.emplace_back(send_button,
                         line_input,
                         combo_box,
                         &m_setCommands,
                         &m_setCommandNames,
                         this);
}

void parameterManager::addSwitchComboWidget(QPushButton *action_button,
                                            QComboBox *combo_box)
{
    m_switchList.emplace_back(action_button,
                           combo_box,
                           &m_switchCommands,
                           &m_switchCommandNames,
                           this);
}

void parameterManager::onNewFileLoaded() {
    // clean maps before loading new data.
    m_getCommands.clear();
    m_setCommands.clear();
    m_switchCommands.clear();
    m_getCommandNames.clear();
    m_setCommandNames.clear();
    m_switchCommandNames.clear();
}

void parameterManager::onNewFileParsed() {
    // update commands combo list.
    m_getCommandNames = m_getCommands.keys();
    m_setCommandNames = m_setCommands.keys();
    m_switchCommandNames = m_switchCommands.keys();

    for (auto it = m_getList.begin(); it != m_getList.end(); ++it) {
        it->addComboBoxList(m_getCommandNames);
    }
    for (auto it = m_setList.begin(); it != m_setList.end(); ++it) {
        it->addComboBoxList(m_setCommandNames);
    }
    for (auto it = m_switchList.begin(); it != m_switchList.end(); ++it) {
        it->addComboBoxList(m_switchCommandNames);
    }
}
