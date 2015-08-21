#pragma once

#ifdef CompilingPlatformDLL
#define PLATFORM_API   __declspec( dllexport ) 
#else 
#define PLATFORM_API   __declspec( dllimport ) 
#endif