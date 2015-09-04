#pragma once

#include "DLL_API.hpp"
#include <stdio.h>

#ifndef NDEBUG
#define ASSERT(condition, message, ...) do {if (!(condition)) { \
  char assertBuf__internal[512]; \
  sprintf_s(assertBuf__internal, message, __VA_ARGS__);\
  utl::debug::__assert(__FILE__, __LINE__, assertBuf__internal);\
}} while (0)

#else 

#define ASSERT(condition, message, ...)

#endif

namespace utl {
   namespace debug {
      UTILITY_API void __assert(const char *file, int line, const char *message);
   }
}

