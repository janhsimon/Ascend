#pragma once

#pragma warning(disable: 4251) // no warnings for deliberately unexposed members of exposed classes

#ifdef ASC_BUILD_DLL
	#define ASC_EXPORT __declspec(dllexport)
#else
	#define ASC_EXPORT __declspec(dllimport)
#endif