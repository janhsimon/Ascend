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

	ASC_EXPORT void SetLogLambda(std::function<void(const std::string&, const LogSeverity)> logLambda);
	ASC_EXPORT void Log(const std::string& message, const LogSeverity severity = LogSeverity::Information);
}
