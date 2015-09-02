#pragma once

#include "Object.hpp"
#include "RTTI.h"
#include "String.hpp"
#include "StringView.hpp"
#include "Vector.hpp"
#include "Singleton.hpp"

namespace utl {
   class SExpr;

   typedef Vector<SExpr> Sublist;
   typedef StringView Symbol;

   class SExprPrivate;

   class UTILITY_API SExpr : public Object{
      SExpr(void *data, IRTTI* rtti);
      void *_getObjImpl(size_t id);
   public:
      template<typename T>
      SExpr(T &obj) :SExpr(new T(obj), &Singleton<RTTI<T>>::Instance()) {}

      SExpr();
      SExpr(int i);
      SExpr(float f);
      SExpr(Sublist &list);
      SExpr(String &str);
      SExpr(Symbol symb);

      SExpr(SExpr const &rhs);

      int *getInt();
      float *getFloat();
      Sublist *getList();
      String *getStr();
      Symbol *getSymb();

      template<typename T>
      T *getObj() { return (T*)_getObjImpl(RTTI<T>::id()); }

      //returns if expr should be evaluated
      bool eval();

      //returns false if sexpr is nil
      operator bool();      

      DECLARE_UTILITY_PUBLIC(SExpr)
   };

   UTILITY_API Sublist parseFile();
}