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

      Expr &load(Sym key);

      DECLARE_UTILITY_PUBLIC(Context)
   };

   typedef utl::Closure<Expr(Expr &, Context &)> EvalClosure;
   typedef std::shared_ptr<EvalClosure> Evaluator;
   
   template<typename L>
   Evaluator createEvaluator(L && lambda) { return std::make_shared<EvalClosure>(lambda); }

   Expr evaluate(Expr &eval, Expr &input, Context &context) {
      if (auto e = eval.obj<Evaluator>()) {
         return (**e)(input, context);
      }

      return Expr();
   }
}
