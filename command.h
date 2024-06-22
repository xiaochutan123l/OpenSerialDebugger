#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QVariant>

class Command {
public:
    enum CommandType {
        GET,
        SET,
        SWITCH,
        NONE
    };

    enum DataType {
        UINT,
        INT,
        FLOAT,
        BOOL,
        UNKNOWN
    };
    Command() : m_name(""), m_type(NONE), m_number(0), m_dataType(UNKNOWN) {};
    Command(const QString &name, CommandType type, uint16_t number, const QString &dataType)
        : m_name(name), m_type(type), m_number(number) {
        m_dataType = stringToDataType(dataType);
    }

    QString name() const { return m_name; }
    CommandType type() const { return m_type; }
    uint16_t number() const { return m_number; }
    DataType dataType() const { return m_dataType; }

    void setData(const QVariant &data) { m_data = data; }
    QVariant data() const { return m_data; }

    void initData(const DataType type);

private:
    QString m_name;
    CommandType m_type;
    uint16_t m_number;
    DataType m_dataType;
    QVariant m_data;

    DataType stringToDataType(const QString &dataTypeStr) const;
};

#endif // COMMAND_H
