#ifndef galileo_log_h
#define galileo_log_h

#include <math.h>
#include <sys/sysinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string>
#include <EthernetUdp.h>

typedef enum _LogLevel {
	LEVEL_DEBUG = 0,
	LEVEL_INFO = 1,
	LEVEL_ERROR = 2
} LogLevel;

class Log {
public:
	static void d(const char *, ...);
	static void i(const char *, ...);
	static void e(const char *, ...);

	static void setLogLevel(LogLevel);
	static void setSyslogServer(IPAddress);
	static void setLogFile(const char *);
	static void enableSerialDebug(bool);
	static void enableStdoutDebug(bool);
	static void setDeviceId(std::string);
	static void updateFromConfig();

private:
	static void log(LogLevel, const char *, va_list argptr);
	static std::string getDateTime();

	static IPAddress syslogServer;
	static bool syslogEnabled;
	static EthernetUDP syslogUdp;
	static const char *logFilePath;
	static FILE *logFile;
	static bool serialDebug;
	static LogLevel logLevel;
	static std::string deviceid;
	static bool stdoutDebug;
};

#endif