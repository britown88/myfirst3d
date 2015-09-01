#pragma once

#include "DLL_API.hpp"
#include <string>

namespace utl {
   typedef const char* StringView;

   UTILITY_API StringView internString(StringView str);
   UTILITY_API size_t stringViewHash(StringView str);
}
