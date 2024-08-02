#ifndef PROTOCOLPARSER_H
#define PROTOCOLPARSER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QPushButton>
#include <QFileDialog>
#include <QMap>
#include "command.h"

class ProtocolParser : public QObject {
    Q_OBJECT

public:
    explicit ProtocolParser(QMap<QString, Command> *getCommands,
                            QMap<QString, Command> *setCommands,
                            QMap<QString, Command> *actionCommands,
                            QObject *parent = nullptr);

    void bindButton(QPushButton *button);

signals:
    void newFileLoaded();
    void newFileParsed();

private slots:
    void onParseButtonClicked();

private:
    void parseFile(const QString &filePath);
    void parseLine(const QString &line);

    QMap<QString, Command> *m_getCommands;
    QMap<QString, Command> *m_setCommands;
    QMap<QString, Command> *m_actionCommands;
};

#endif // PROTOCOLPARSER_H
