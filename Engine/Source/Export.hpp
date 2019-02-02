#pragma once

#ifdef ASC_BUILD_DLL
	#define ASC_EXPORT __declspec(dllexport)
#else
	#define ASC_EXPORT __declspec(dllimport)
#endif