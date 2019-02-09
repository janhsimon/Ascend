#pragma once

#include "Application.hpp"

namespace asc
{
	namespace internal
	{
		void setLogApplicationInfo(const ApplicationInfo* _applicationInfo);
	}

	enum LogSeverity
	{
		Information,
		Warning,
		Error,
		Fatal
	};

	ASC_EXPORT void Log(const std::string& message, const LogSeverity severity = LogSeverity::Information);
}
