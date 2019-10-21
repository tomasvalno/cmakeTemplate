#ifndef DBUS_NODE_LIST_H
#define DBUS_NODE_LIST_H

#include <QtDBus>

namespace cuxs::dbusifc
{
	// move to header with common node types definition
	struct NodeInfo
	{
		uint32_t id;
		uint32_t type;
	};
}
Q_DECLARE_METATYPE(cuxs::dbusifc::NodeInfo);

// Registers complex dbus data types to QT
void registerQtMetatypes();

// Serialization and deserialization for complex dbus arguments
QDBusArgument &operator<<(QDBusArgument &argument, const cuxs::dbusifc::NodeInfo &mystruct);
const QDBusArgument &operator>>(const QDBusArgument &argument, cuxs::dbusifc::NodeInfo &mystruct);

#endif // DBUS_NODE_LIST_H
