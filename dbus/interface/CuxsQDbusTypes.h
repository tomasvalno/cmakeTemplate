#ifndef CUXS_QDBUS_TYPES_H
#define CUXS_QDBUS_TYPES_H

#include <QVector>
#include <QString>

#include "NodeInfo.h"

typedef QVector<uint32_t> VectU32;
Q_DECLARE_METATYPE(VectU32);

typedef QVector<cuxs::dbusifc::NodeInfo> VectNodeInfo;
Q_DECLARE_METATYPE(VectNodeInfo);

#endif // CUXS_QDBUS_TYPES_H
