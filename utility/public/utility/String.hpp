#pragma once

#include "Object.hpp"

namespace utl {
   class StringPrivate;

   class UTILITY_API String : public Object {
   public:
      String();
      String(const char *str);
      String(String const &rhs);
      const char*c_str() const;

      String &operator=(String const &rhs);

      size_t size() const;
      size_t length() const { return size(); }

      DECLARE_UTILITY_PUBLIC(String)
   };
}