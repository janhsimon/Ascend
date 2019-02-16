#pragma once

#include "Application.hpp"

namespace asc
{
  enum LogSeverity
  {
    Information,
    Warning,
    Error,
    Fatal
  };

  void SetLogLambda(std::function<void(const std::string&, const LogSeverity)> logLambda);
  void Log(const std::string& message, const LogSeverity severity = LogSeverity::Information);
}
