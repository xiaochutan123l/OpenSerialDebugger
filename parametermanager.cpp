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

void ParameterComboWidget::setWidgetsVisible(bool visible) {
    QSizePolicy sp_retain;
    if (m_line_input != nullptr) {
        //m_line_input->setVisible(visible);
        sp_retain = m_line_input->sizePolicy();
        sp_retain.setRetainSizeWhenHidden(true);
        m_line_input->setSizePolicy(sp_retain);
        m_line_input->setVisible(visible);
    }
    // m_button->setVisible(visible);
    // m_combo_box->setVisible(visible);
    sp_retain = m_button->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    m_button->setSizePolicy(sp_retain);
    m_button->setVisible(visible);

    sp_retain = m_combo_box->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    m_combo_box->setSizePolicy(sp_retain);
    m_combo_box->setVisible(visible);
}

uint16_t ParameterComboWidget::commandNumber() {
    if (m_command != nullptr) {
        return m_command->number();
    }
    return 0;
}

Command::DataType ParameterComboWidget::commandDataType() {
    if (m_command != nullptr) {
        return m_command->dataType();
    }
    return Command::DataType::UNKNOWN;
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

void ParameterComboWidget::onCmdSelected(const QString & name) {

    m_command = &((*m_commands)[name]);
    if (m_line_input != nullptr) {
        m_command->setData(m_line_input->text());
    }
    qDebug() << "command selected: " << m_command->name();
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
    connect(pManager, &parameterManager::updateGetParameter, this, &getParameterComboWidget::onDataUpdated);
}

void getParameterComboWidget::disconnect_widgets(parameterManager *pManager) {
    //connect(m_line_input, &QLineEdit::editingFinished, this, &getParameterComboWidget::onDataUpdated);
    disconnect(m_combo_box, &QComboBox::currentTextChanged, this, &getParameterComboWidget::onCmdSelected);
    disconnect(m_button, &QPushButton::clicked, this, &getParameterComboWidget::onGetButtonPushed);
    disconnect(this, &getParameterComboWidget::sendCommand, pManager, &parameterManager::sendCommandBytes);
    disconnect(pManager, &parameterManager::updateGetParameter, this, &getParameterComboWidget::onDataUpdated);
}

void getParameterComboWidget::onDataUpdated(const QByteArray &data, Command::DataType type) {
    // TODO get number from struct packet itself.
    QString strValue;
    switch(type) {
        case Command::DataType::INT: {
            int32_t intNum = DATA_TO_INT(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())));
            strValue = QString::number(intNum);
            break;
        }
        case Command::DataType::UINT:
        case Command::DataType::BOOL: {
            uint32_t uintNum = DATA_TO_UINT(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())));
            strValue = QString::number(uintNum);
            break;
        }
        case Command::DataType::FLOAT: {
            float floatNum;
            floatNum = DATA_TO_FLOAT(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data())));
            strValue = QString::number(floatNum, 'f', 2); // 'f' for floating-point, 2 decimal places
            break;
        }
        case Command::DataType::UNKNOWN:
            break;
    }
    m_line_input->setText(strValue);
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

void parameterManager::onUpdateGetParameter(const QByteArray &packet) {
    // TODO
    // currently only support single chunk
    uint16_t cmd = GET_CMD_NUM(reinterpret_cast<uint8_t*>(const_cast<char*>(packet.data())));
    for (getParameterComboWidget& widget : m_getList) {
        if (widget.commandNumber() == cmd) {
            QByteArray data_byte = QByteArray::fromRawData(packet.data() + 4, sizeof(uint32_t));
            widget.onDataUpdated(data_byte, static_cast<Command::DataType>(widget.commandDataType()));
        }
    }
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

    int count = 0;
    int size = m_getCommandNames.size();

    for (auto it = m_getList.begin(); it != m_getList.end(); ++it) {
        if (!first_load) {
            it->clearWidget();
            it->disconnect_widgets(this);
        }
        it->setComboBoxList(m_getCommandNames);
        it->connect_widgets(this);

        if (count < size) {
            it->setWidgetsVisible(true);
        }
        else {
            it->setWidgetsVisible(false);
        }
        count++;
    }

    count = 0;
    size = m_setCommandNames.size();

    for (auto it = m_setList.begin(); it != m_setList.end(); ++it) {
        if (!first_load) {
            it->clearWidget();
            it->disconnect_widgets(this);
        }
        it->setComboBoxList(m_setCommandNames);
        it->connect_widgets(this);

        if (count < size) {
            it->setWidgetsVisible(true);
        }
        else {
            it->setWidgetsVisible(false);
        }
        count++;
    }

    count = 0;
    size = m_switchCommandNames.size();

    for (auto it = m_switchList.begin(); it != m_switchList.end(); ++it) {
        if (!first_load) {
            it->clearWidget();
            it->disconnect_widgets(this);
        }
        it->setComboBoxList(m_switchCommandNames);
        it->connect_widgets(this);

        if (count < size) {
            it->setWidgetsVisible(true);
        }
        else {
            it->setWidgetsVisible(false);
        }
        count++;
    }
}
