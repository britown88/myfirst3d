#pragma once

#include "Object.hpp"

namespace lisp {
   class LispParserPrivate;
   class UTILITY_API LispParser : public utl::Object{
   public:
      LispParser();
      DECLARE_UTILITY_PUBLIC(LispParser)
   };
}
