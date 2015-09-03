#pragma once

#include "Object.hpp"
#include "SExpressions.hpp"

namespace utl {
   class LispContextPrivate;
   class UTILITY_API LispContext : public Object {
   public:
      LispContext();

      void push();
      void pop();

      void store(Symbol key, SExpr &value);
      void store(Symbol key, SExpr &&value);

      SExpr &load(Symbol key);

      DECLARE_UTILITY_PUBLIC(LispContext)
   };

   typedef Closure<SExpr(SExpr &, LispContext &)> Evaluator;
}
