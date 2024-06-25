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

QByteArray Command::generateCommandBytes() {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QDataStream::WriteOnly);
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);

    // MagicNum
    uint8_t magicNum = 0xAB;
    stream << magicNum;

    // ChunkNum
    uint8_t chunkNum = 1;
    stream << chunkNum;

    // CMD number
    stream << m_number;

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
         uint32_t boolValue = m_data.toUInt() ? 1 : 0;
         stream << boolValue;
         break;
     }
     case UNKNOWN:
     default:
         qDebug() << "wrong data type: ";
         // Handle unknown data type if necessary
         break;
     }

    return byteArray;
}
