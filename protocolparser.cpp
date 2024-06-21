#include "protocolparser.h"

ProtocolParser::ProtocolParser(QObject *parent) : QObject(parent) {}

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
    while (!in.atEnd()) {
        QString line = in.readLine();
        parseLine(line);
    }

    file.close();
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
    if (typeStr == "GET") {
        type = Command::GET;
    } else if (typeStr == "SET") {
        type = Command::SET;
    } else if (typeStr == "SWITCH") {
        type = Command::SWITCH;
    } else {
        // Invalid command type
        return;
    }

    Command command(name, type, number, dataType);

    switch (type) {
    case Command::GET:
        m_getCommands.append(command);
        break;
    case Command::SET:
        m_setCommands.append(command);
        break;
    case Command::SWITCH:
        m_switchCommands.append(command);
        break;
    }
}

// const QList<Command>& ProtocolParser::getCommands(Command::CommandType type) const {
//     switch (type) {
//     case Command::GET:
//         return m_getCommands;
//     case Command::SET:
//         return m_setCommands;
//     case Command::SWITCH:
//         return m_switchCommands;
//     }

//     // To handle compiler warnings
//     static const QList<Command> emptyList;
//     return emptyList;
// }
