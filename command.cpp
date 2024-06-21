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
