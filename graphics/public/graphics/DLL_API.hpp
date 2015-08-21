#pragma once

#ifdef CompilingGraphicsDLL
#define GRAPHICS_API   __declspec( dllexport ) 
#else 
#define GRAPHICS_API   __declspec( dllimport ) 
#endif