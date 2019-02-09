#include "Log.hpp"

namespace asc
{
	namespace internal
	{
		const ApplicationInfo* applicationInfo = nullptr;

		void setLogApplicationInfo(const ApplicationInfo* _applicationInfo)
		{
			applicationInfo = _applicationInfo;
		}
	}

	void Log(const std::string& message, const LogSeverity severity)
	{
		if (internal::applicationInfo)
		{
			internal::applicationInfo->logLambda(message, severity);
		}
	}
}