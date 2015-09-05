#pragma once

#include "Object.hpp"
#include "LispExpressions.hpp"
#include <memory>

namespace lisp {
   class ContextPrivate;
   class UTILITY_API Context : public utl::Object {
   public:
      Context();

      void push();
      void pop();

      void store(Sym key, Expr &value);
      void store(Sym key, Expr &&value);

      template<typename L>
      void storeEvaluator(const char *name, L && lambda) {
         store(internSym(name), std::move(createEvaluator(std::move(lambda))));
      }

      Expr &load(Sym key);

      Expr evaluate(Expr &input);

      DECLARE_UTILITY_PUBLIC(Context)
   };
   
   template<typename L>
   Evaluator createEvaluator(L && lambda) { return std::make_shared<EvalClosure>(std::move(lambda)); }

}
