#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QPushButton>
#include <QFileDialog>

#include "command.h"

class ProtocolParser : public QObject {
    Q_OBJECT

public:
    explicit ProtocolParser(QObject *parent = nullptr);

    // const QList<Command>& getCommands(Command::CommandType type) const;


    void bindButton(QPushButton *button);

private slots:
    void onParseButtonClicked();

private:
    void parseFile(const QString &filePath);
    void parseLine(const QString &line);

    QList<Command> m_getCommands;
    QList<Command> m_setCommands;
    QList<Command> m_switchCommands;
};

#endif // PROTOCOLPARSER_H
