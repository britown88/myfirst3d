#pragma once

#include "Object.hpp"
#include "RTTI.h"
#include "String.hpp"
#include "StringView.hpp"
#include "Vector.hpp"
#include "Singleton.hpp"
#include "Closure.hpp"

namespace utl {
   class SExpr;
   class LispContext;

   typedef Vector<SExpr> Sublist;
   typedef StringView Symbol;
   typedef Closure<SExpr(SExpr &, LispContext &)> Evaluator;

   class SExprPrivate;

   class UTILITY_API SExpr : public Object{
      SExpr(void *data, IRTTI* rtti, int move);
      SExpr(void *data, IRTTI* rtti);
      void *_getObjImpl(size_t id);
   public:
      SExpr(SExpr const &rhs);
      SExpr(SExpr &rhs) :SExpr((SExpr const &)rhs) {}
      SExpr &operator=(SExpr const &rhs);

      template<typename T>
      SExpr(T &obj) :SExpr(&obj, &Singleton<RTTI<T>>::Instance()) {}

      //we need to differentiate the move constructor over the copy SO PASS AN INT SHORE
      template<typename T>
      SExpr(T &&obj) : SExpr(&obj, &Singleton<RTTI<T>>::Instance(), 0) {}

      SExpr();
      SExpr(int i);
      SExpr(float f);
      SExpr(Sublist &list);
      SExpr(Sublist &&list);
      SExpr(String &str);
      SExpr(String &&str);
      SExpr(Symbol symb);

      int *getInt();
      float *getFloat();
      Sublist *getList();
      String *getStr();
      Symbol *getSymb();

      template<typename T>
      T *getObj() { return (T*)_getObjImpl(RTTI<T>::id()); }

      //returns if expr should be evaluated, defaults to true
      bool &eval();

      //returns false if sexpr is nil
      explicit operator bool();      

      DECLARE_UTILITY_PUBLIC(SExpr)
   };

   class LispContextPrivate;
   class UTILITY_API LispContext : public Object {
      //std::vector<unordered_map<Symbol, sexpr>>>
   public:
      LispContext();

      void push();
      void pop();

      void store();

      DECLARE_UTILITY_PUBLIC(LispContext)
   };

   UTILITY_API Sublist parseFile();
}