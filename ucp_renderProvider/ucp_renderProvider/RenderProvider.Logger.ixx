module;

#include <ucp3.h>

export module RenderProvider.Logger;

export enum class LogLevel : int
{
  LOG_NONE = 99, // for control stuff in the dll
  LOG_FATAL = ucp_NamedVerbosity::Verbosity_FATAL,
  LOG_ERROR = ucp_NamedVerbosity::Verbosity_ERROR,
  LOG_WARNING = ucp_NamedVerbosity::Verbosity_WARNING,
  LOG_INFO = ucp_NamedVerbosity::Verbosity_INFO,
  LOG_DEBUG = ucp_NamedVerbosity::Verbosity_1,
};


export void Log(const LogLevel level, const char* message);
export LogLevel GetCurrentLogLevel();

module :private;

void Log(const LogLevel level, const char* message)
{
  ucp_log(static_cast<ucp_NamedVerbosity>(level), message);
}

LogLevel GetCurrentLogLevel()
{
  return static_cast<LogLevel>(ucp_logLevel());
}
