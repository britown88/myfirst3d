#pragma once

#include "Object.hpp"
#include "LispExpressions.hpp"

namespace lisp {
   class ContextPrivate;
   class UTILITY_API Context : public utl::Object {
   public:
      Context();

      void push();
      void pop();

      void store(Sym key, Expr &value);
      void store(Sym key, Expr &&value);

      Expr &load(Sym key);

      DECLARE_UTILITY_PUBLIC(Context)
   };

   typedef utl::Closure<Expr(Expr &, Context &)> Evaluator;
}
