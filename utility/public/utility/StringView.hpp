#pragma once

#include "DLL_API.hpp"
#include <string>

namespace utl {
   struct StringViewSlot{
      const char *name;
   };
   typedef StringViewSlot const* StringView;

   UTILITY_API StringView internString(const char* str);
   UTILITY_API size_t stringViewHash(StringView str);
}
