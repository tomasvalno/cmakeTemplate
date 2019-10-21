#include <map>
#include <QDate>

#ifdef __linux__
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include "Log.h"

namespace logr
{

	void verboseMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	{
		static const char* typeStr[] = {"debug", "warning", "error", "fatal", "info", "trace"};
		QString level{typeStr[type]};
		QString message{msg};

		if (type <= QtFatalMsg)
		{
			if (msg.toUpper() == "TRACE")
			{
				QString contextString(QStringLiteral("%1 +%2")
									  .arg(context.function)
									  .arg(context.line));
				message = contextString;
				level = typeStr[5];
			}
		}

		QByteArray localMsg = message.toLocal8Bit();
		QString timeStr(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz"));

		std::string color = "\033[0m";
		switch (type)
		{
			case QtDebugMsg:
				color = "\033[34m";
				break;

			case QtInfoMsg:
				break;

			case QtWarningMsg:
				color = "\033[93m";
				break;

			case QtCriticalMsg:
				color = "\033[91m";
				break;
			case QtFatalMsg:
				color = "\033[91m";
				break;
		}

		QTextStream(stdout, QIODevice::WriteOnly)
				<< color.c_str()
				<< "[" << timeStr.toLocal8Bit().constData() << "] "
				<< "[cuxscored] "
				   // add padding here to make all the messages more uniform
		   #ifdef __linux__
		   # if defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 21)
		   #  define SYS_gettid __NR_gettid
		   # endif
				<< "[" << static_cast<size_t>(syscall(SYS_gettid)) << "] "
		   #else
				<< "[" << QThread::currentThreadId() << "] "
		   #endif
				<< "[" << level << "] "
				<< localMsg.constData()
				<< "\033[0m"
				<< "\n";

	}

	void init()
	{
		qInstallMessageHandler(verboseMessageHandler);
	}
}
