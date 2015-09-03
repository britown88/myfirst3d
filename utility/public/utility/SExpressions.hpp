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

   typedef Vector<SExpr> Sublist;
   typedef StringView Symbol;

   class SExprPrivate;

   class UTILITY_API SExpr : public Object{
      SExpr(void *data, TypeID rtti, int move);
      SExpr(void *data, TypeID rtti);
      void *_getObjImpl(TypeID id);
   public:
      SExpr(SExpr const &rhs);
      SExpr(SExpr &rhs) :SExpr((SExpr const &)rhs) {}
      SExpr &operator=(SExpr const &rhs);

      template<typename T>
      SExpr(T &obj) :SExpr(&obj, GetTypeID<T>()) {}

      //we need to differentiate the move constructor over the copy SO PASS AN INT SHORE
      template<typename T>
      SExpr(T &&obj) : SExpr(&obj, GetTypeID<T>(), 0) {}

      SExpr();
      SExpr(int i) : SExpr(&i, GetTypeID<int>()) {}
      SExpr(float f) : SExpr(&f, GetTypeID<float>()) {}
      SExpr(Sublist &list) : SExpr(&list, GetTypeID<Sublist>()) {}
      SExpr(Sublist &&list) : SExpr(&list, GetTypeID<Sublist>(), 0) {}
      SExpr(String &str) : SExpr(&str, GetTypeID<String>()) {}
      SExpr(String &&str) : SExpr(&str, GetTypeID<String>(), 0) {}
      SExpr(Symbol symb) : SExpr(&symb, GetTypeID<Symbol>()) {}

      int *getInt();
      float *getFloat();
      Sublist *getList();
      String *getStr();
      Symbol *getSymb();

      template<typename T>
      T *getObj() { return (T*)_getObjImpl(GetTypeID<T>()); }

      //returns if expr should be evaluated, defaults to true
      bool &eval();

      //returns false if sexpr is nil
      explicit operator bool();      

      DECLARE_UTILITY_PUBLIC(SExpr)
   };

   UTILITY_API Sublist parseFile(const char *path);
}