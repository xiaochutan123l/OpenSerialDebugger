#include "protocolparser.h"

ProtocolParser::ProtocolParser(QMap<QString, Command> *getCommands,
                               QMap<QString, Command> *setCommands,
                               QMap<QString, Command> *actionCommands,
                               QObject *parent)
    : m_getCommands(getCommands),
    m_setCommands(setCommands),
    m_actionCommands(actionCommands),
    QObject(parent) {}

void ProtocolParser::bindButton(QPushButton *button) {
    connect(button, &QPushButton::clicked, this, &ProtocolParser::onParseButtonClicked);
}

void ProtocolParser::onParseButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select File to Parse", "", "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        qDebug() << "Selected file:" << filePath;
        parseFile(filePath);
    } else {
        qDebug() << "No file selected.";
    }
}

void ProtocolParser::parseFile(const QString &filePath) {
    // Implement your file parsing logic here
    qDebug() << "Parsing file:" << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Parse file failed:" << filePath;
        return;
    }

    QTextStream in(&file);

    emit newFileLoaded();

    while (!in.atEnd()) {
        QString line = in.readLine();
        parseLine(line);
    }

    file.close();

    emit newFileParsed();
}

void ProtocolParser::parseLine(const QString &line) {
    if (line.isEmpty() || line.startsWith("#") || line.startsWith("//") || line.startsWith("*") ||line.startsWith("/*")) {
        // Skip comments and empty lines
        return;
    }

    QStringList parts = line.split(",");
    if (parts.size() < 4) {
        // Invalid format
        return;
    }

    QString name = parts[0].trimmed();
    QString typeStr = parts[1].trimmed();
    bool ok;
    uint16_t number = parts[2].trimmed().toUInt(&ok, 16);
    QString dataType = parts[3].trimmed();

    Command::CommandType type;
    if (typeStr == "GET" || typeStr == "Get" || typeStr == "get") {
        type = Command::GET;
    } else if (typeStr == "SET" || typeStr == "Set" || typeStr == "set") {
        type = Command::SET;
    } else if (typeStr == "ACTION" || typeStr == "Action" || typeStr == "action") {
        type = Command::ACTION;
    }
    // backward compatible for old version
    else if (typeStr == "SWITCH" || typeStr == "Switch" || typeStr == "switch") {
        type = Command::ACTION;
    } else {
        // Invalid command type
        return;
    }

    Command command(name, type, number, dataType);
    switch (type) {
    case Command::GET:
        if (!m_getCommands->contains(name)) {
            m_getCommands->insert(name, command);
        }
        else {
            qDebug() << "duplicated name: " << name;
        }
        break;
    case Command::SET:
        if (!m_setCommands->contains(name)) {
            m_setCommands->insert(name, command);
        }
        else {
            qDebug() << "duplicated name: " << name;
        }
        break;
    case Command::ACTION:
        if (!m_actionCommands->contains(name)) {
            m_actionCommands->insert(name, command);
        }
        else {
            qDebug() << "duplicated name: " << name;
        }
        break;
    case Command::NONE:
        break;
    }
}

// const QList<Command>& ProtocolParser::getCommands(Command::CommandType type) const {
//     switch (type) {
//     case Command::GET:
//         return m_getCommands;
//     case Command::SET:
//         return m_setCommands;
//     case Command::ACTION:
//         return m_actionCommands;
//     }

//     // To handle compiler warnings
//     static const QList<Command> emptyList;
//     return emptyList;
// }
