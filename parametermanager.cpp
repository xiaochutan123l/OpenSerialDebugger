#include "parametermanager.h"

/* ----------------------- getParameterComboWidget --------------------------*/

ParameterComboWidget::ParameterComboWidget(QPushButton *button,
                                 QLineEdit *line_input,
                                 QComboBox *combo_box,
                                 QMap<QString, Command> *commands,
                                 QList<QString> *commandNames,
                                 QObject *parent)
    : m_button(button),
    m_line_input(line_input),
    m_combo_box(combo_box),
    m_commands(commands),
    m_commandNames(commandNames),
    QObject(parent) {}

// Copy constructor
ParameterComboWidget::ParameterComboWidget(const ParameterComboWidget &other_widget)
    : m_button(other_widget.m_button),
    m_line_input(other_widget.m_line_input),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{

}

// Move constructor
ParameterComboWidget::ParameterComboWidget(ParameterComboWidget &&other_widget)
    : m_button(other_widget.m_button),
    m_line_input(other_widget.m_line_input),
    m_combo_box(other_widget.m_combo_box),
    m_commands(other_widget.m_commands),
    m_commandNames(other_widget.m_commandNames),
    QObject(other_widget.parent())
{
    other_widget.m_button = nullptr;
    other_widget.m_line_input = nullptr;
    other_widget.m_combo_box = nullptr;
    other_widget.m_commands = nullptr;
    other_widget.m_commandNames = nullptr;
    other_widget.d_ptr->parent = nullptr;
}

// Assignment constructor
ParameterComboWidget& ParameterComboWidget::operator=(const ParameterComboWidget &other_widget) {
    if (&other_widget != this) {
        m_button = other_widget.m_button;
        m_line_input = other_widget.m_line_input;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;
    }
    return *this;
}

// Move assignment constructor
ParameterComboWidget& ParameterComboWidget::operator=(ParameterComboWidget &&other_widget) {
    if (&other_widget != this) {
        m_button = other_widget.m_button;
        m_line_input = other_widget.m_line_input;
        m_combo_box = other_widget.m_combo_box;
        m_commands = other_widget.m_commands;
        m_commandNames = other_widget.m_commandNames;
        d_ptr->parent = other_widget.d_ptr->parent;

        other_widget.m_button = nullptr;
        other_widget.m_line_input = nullptr;
        other_widget.m_combo_box = nullptr;
        other_widget.m_commands = nullptr;
        other_widget.m_commandNames = nullptr;
        other_widget.d_ptr->parent = nullptr;
    }
    return *this;
}

void ParameterComboWidget::onCmdSelected(const QString & name) {

    m_command = &((*m_commands)[name]);
    if (m_line_input != nullptr) {
        m_command->setData(m_line_input->text());
    }
    qDebug() << "command selected: " << m_command->name();
}

void ParameterComboWidget::setComboBoxList(QList<QString> &names) {
    m_combo_box->addItems(names);
    m_combo_box->setCurrentIndex(-1);
}

void ParameterComboWidget::clearWidget() {
    m_combo_box->clear();
    if(m_line_input != nullptr) {
        m_line_input->clear();
    }
    m_command = nullptr;
}

/* ----------------------- getParameterComboWidget --------------------------*/

getParameterComboWidget::getParameterComboWidget(QPushButton *get_button,
                        QLineEdit *line_input,
                        QComboBox *combo_box,
                        QMap<QString, Command> *commands,
                        QList<QString> *commandNames,
                        QObject *parent)
    : ParameterComboWidget(get_button,
                           line_input,
                           combo_box,
                           commands,
                           commandNames,
                           parent) {}

void getParameterComboWidget::connect_widgets(parameterManager *pManager) {
    //connect(m_line_input, &QLineEdit::editingFinished, this, &getParameterComboWidget::onDataUpdated);
    m_line_input->setReadOnly(true);
    m_line_input->clear();
    connect(m_combo_box, &QComboBox::currentTextChanged, this, &getParameterComboWidget::onCmdSelected);
    connect(m_button, &QPushButton::clicked, this, &getParameterComboWidget::onGetButtonPushed);
    connect(this, &getParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
}

void getParameterComboWidget::disconnect_widgets(parameterManager *pManager) {
    //connect(m_line_input, &QLineEdit::editingFinished, this, &getParameterComboWidget::onDataUpdated);
    disconnect(m_combo_box, &QComboBox::currentTextChanged, this, &getParameterComboWidget::onCmdSelected);
    disconnect(m_button, &QPushButton::clicked, this, &getParameterComboWidget::onGetButtonPushed);
    disconnect(this, &getParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
}

void getParameterComboWidget::onDataUpdated(QString data) {
    qDebug() << "onDataUpdated";
    m_line_input->setText(data);
}
void getParameterComboWidget::onGetButtonPushed() {
    qDebug() << "onGetButtonPushed";
    if (m_command == nullptr) {
        qDebug() << "command not selected, send nothing";
        return;
    }
    QByteArray cmd = m_command->generateCommandBytes();
    emit sendCommand(cmd);
}

/* ----------------------- setParameterComboWidget --------------------------*/

setParameterComboWidget::setParameterComboWidget(QPushButton *send_button,
                        QLineEdit *line_input,
                        QComboBox *combo_box,
                        QMap<QString, Command> *commands,
                        QList<QString> *commandNames,
                        QObject *parent)
    : ParameterComboWidget(send_button,
                           line_input,
                           combo_box,
                           commands,
                           commandNames,
                           parent) {}

void setParameterComboWidget::connect_widgets(parameterManager *pManager) {
    connect(m_combo_box, &QComboBox::currentTextChanged, this, &setParameterComboWidget::onCmdSelected);
    connect(m_line_input, &QLineEdit::editingFinished, this, &setParameterComboWidget::onDataStringChanged);
    connect(m_button, &QPushButton::clicked, this, &setParameterComboWidget::onSendButtonPushed);
    connect(this, &setParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
}

void setParameterComboWidget::disconnect_widgets(parameterManager *pManager) {
    disconnect(m_combo_box, &QComboBox::currentTextChanged, this, &setParameterComboWidget::onCmdSelected);
    disconnect(m_line_input, &QLineEdit::editingFinished, this, &setParameterComboWidget::onDataStringChanged);
    disconnect(m_button, &QPushButton::clicked, this, &setParameterComboWidget::onSendButtonPushed);
    disconnect(this, &setParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
}

void setParameterComboWidget::onDataStringChanged() {
    qDebug() << "onDataStringChanged: " << m_line_input->text();
    m_command->setData(m_line_input->text());

}
void setParameterComboWidget::onSendButtonPushed() {
    qDebug() << "onSendButtonPushed";
    if (m_command == nullptr) {
        qDebug() << "command not selected, send nothing";
        return;
    }
    QByteArray cmd = m_command->generateCommandBytes();
    emit sendCommand(cmd);
}

/* ----------------------- switchParameterComboWidget --------------------------*/

switchParameterComboWidget::switchParameterComboWidget(QPushButton *action_button,
                           QComboBox *combo_box,
                           QMap<QString, Command> *commands,
                           QList<QString> *commandNames,
                           QObject *parent)
    : ParameterComboWidget(action_button,
                           nullptr,
                           combo_box,
                           commands,
                           commandNames,
                           parent) {}

void switchParameterComboWidget::connect_widgets(parameterManager *pManager) {
    connect(m_combo_box, &QComboBox::currentTextChanged, this, &switchParameterComboWidget::onCmdSelected);
    connect(m_button, &QPushButton::clicked, this, &switchParameterComboWidget::onActionButtonPushed);
    connect(this, &switchParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
}

void switchParameterComboWidget::disconnect_widgets(parameterManager *pManager) {
    disconnect(m_combo_box, &QComboBox::currentTextChanged, this, &switchParameterComboWidget::onCmdSelected);
    disconnect(m_button, &QPushButton::clicked, this, &switchParameterComboWidget::onActionButtonPushed);
    disconnect(this, &switchParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
}

void switchParameterComboWidget::onActionButtonPushed() {
    qDebug() << "onActionButtonPushed";
    if (m_command == nullptr) {
        qDebug() << "command not selected, send nothing";
        return;
    }
    m_command->setData("1");
    QByteArray cmd = m_command->generateCommandBytes();
    emit sendCommand(cmd);
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

void parameterManager::onSendCommandBytes(QByteArray &cmd) {
    qDebug() << "parameter manager: send command";
    emit sendCommandBytes(cmd);
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
    static bool first_load = true;
    if (first_load) {
        first_load = false;
    }
    // update commands combo list.
    m_getCommandNames = m_getCommands.keys();
    m_setCommandNames = m_setCommands.keys();
    m_switchCommandNames = m_switchCommands.keys();

    for (auto it = m_getList.begin(); it != m_getList.end(); ++it) {
        if (!first_load) {
            it->clearWidget();
            it->disconnect_widgets(this);
        }
        it->setComboBoxList(m_getCommandNames);
        it->connect_widgets(this);
    }
    for (auto it = m_setList.begin(); it != m_setList.end(); ++it) {
        if (!first_load) {
            it->clearWidget();
            it->disconnect_widgets(this);
        }
        it->setComboBoxList(m_setCommandNames);
        it->connect_widgets(this);
    }
    for (auto it = m_switchList.begin(); it != m_switchList.end(); ++it) {
        if (!first_load) {
            it->clearWidget();
            it->disconnect_widgets(this);
        }
        it->setComboBoxList(m_switchCommandNames);
        it->connect_widgets(this);
    }
}
