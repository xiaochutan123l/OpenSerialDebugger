#include "command.h"

Command::DataType Command::stringToDataType(const QString &dataTypeStr) const {
    if (dataTypeStr == "uint" || dataTypeStr == "Uint" || dataTypeStr == "UINT") {
        return UINT;
    } else if (dataTypeStr == "int" || dataTypeStr == "Int" || dataTypeStr == "INT") {
        return INT;
    } else if (dataTypeStr == "float" || dataTypeStr == "Float" || dataTypeStr == "FLOAT") {
        return FLOAT;
    } else if (dataTypeStr == "bool" || dataTypeStr == "Bool" || dataTypeStr == "BOOL") {
        return BOOL;
    } else {
        return UNKNOWN;
    }
}

void Command::initData(const DataType type) {
    switch (type) {
    case UINT:
        m_data = QVariant::fromValue<uint>(0);
        break;
    case INT:
        m_data = QVariant::fromValue<int>(0);
        break;
    case FLOAT:
        m_data = QVariant::fromValue<float>(0.0);
        break;
    case BOOL:
        m_data = QVariant::fromValue<bool>(false);
        break;
    case UNKNOWN:
    default:
        // Handle unknown data type if necessary
        break;
    }
}

QByteArray Command::generateCommandBytes() {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QDataStream::WriteOnly);
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);

    // MagicNum
    uint16_t magicNum = 0xABCD;
    stream << magicNum;

    // ChunkNum
    uint16_t chunkNum = 1;
    stream << chunkNum;
    uint16_t num = number();
    qDebug() << "m_number: " << num;
    // CMD number
    //if (m_number) {
        stream << m_number;
    //}

    // data
    switch (m_dataType) {
    case UINT: {
        uint32_t value = m_data.toUInt();
        stream << value;
        break;
    }
    case INT: {
        int32_t value = m_data.toInt();
        stream << value;
        break;
    }
    case FLOAT: {
        float value = m_data.toFloat();
        stream.writeRawData(reinterpret_cast<const char*>(&value), sizeof(float));
        break;
    }
    case BOOL: {
        bool value = m_data.toBool();
        uint32_t boolValue = value ? 1 : 0;
        stream << boolValue;
        break;
    }
    case UNKNOWN:
    default:
        // Handle unknown data type if necessary
        break;
    }

    return byteArray;
}
