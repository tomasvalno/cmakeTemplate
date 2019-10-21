#ifndef   LOG_LOG_H
#define	  LOG_LOG_H

#include <QDebug>

#ifndef RELEASE
#define TRACE logr::debug() << __PRETTY_FUNCTION__ << " +" << __LINE__;
#else
#define TRACE ;
#endif

namespace logr
{
	void init();

	inline QDebug debug()
	{ return QDebug(QtDebugMsg); };

	inline QDebug info()
	{ return QDebug(QtInfoMsg); };

	inline QDebug warning()
	{ return QDebug(QtWarningMsg); };

	inline QDebug error()
	{ return QDebug(QtCriticalMsg); };
}

#endif // LOG_LOG_H
