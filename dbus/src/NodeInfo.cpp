#include "NodeInfo.h"
#include "CuxsQDbusTypes.h"

void registerQtMetatypes()
{
	qDBusRegisterMetaType<VectU32>();
	qDBusRegisterMetaType<VectNodeInfo>();
}

const QDBusArgument &operator>>(const QDBusArgument & argument, cuxs::dbusifc::NodeInfo & mystruct)
{
	argument.beginStructure();
	argument >> mystruct.id >> mystruct.type;
	argument.endStructure();
	return argument;
}

QDBusArgument &operator<<(QDBusArgument & argument, const cuxs::dbusifc::NodeInfo & mystruct)
{
	argument.beginStructure();
	argument << mystruct.id << mystruct.type;
	argument.endStructure();
	return argument;
}
