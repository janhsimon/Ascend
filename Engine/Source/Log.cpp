#include "Log.hpp"

namespace asc
{
	namespace internal
	{
		std::function<void(const std::string&, const LogSeverity)> logLambda = nullptr;
	}

	void SetLogLambda(std::function<void(const std::string&, const LogSeverity)> logLambda)
	{
		internal::logLambda = logLambda;
	}

	void Log(const std::string& message, const LogSeverity severity)
	{
		if (internal::logLambda)
		{
			internal::logLambda(message, severity);
		}
	}
}