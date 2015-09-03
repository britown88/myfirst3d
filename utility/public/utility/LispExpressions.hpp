#pragma once

#include "Object.hpp"
#include "RTTI.h"
#include "String.hpp"
#include "StringView.hpp"
#include "Vector.hpp"
#include "Singleton.hpp"
#include "Closure.hpp"

namespace lisp {
   class Expr;

   typedef utl::Vector<Expr> List;
   typedef utl::StringView Sym;
   typedef utl::String Str;

   class ExprPrivate;

   class UTILITY_API Expr : public utl::Object{
      Expr(void *data, utl::TypeID rtti, int move);
      Expr(void *data, utl::TypeID rtti);
      void *_getObjImpl(utl::TypeID id);
   public:
      Expr(Expr const &rhs);
      Expr(Expr &rhs) :Expr((Expr const &)rhs) {}
      Expr &operator=(Expr const &rhs);

      template<typename T>
      Expr(T &obj) :Expr(&obj, utl::GetTypeID<T>()) {}

      //we need to differentiate the move constructor over the copy SO PASS AN INT SHORE
      template<typename T>
      Expr(T &&obj) : Expr(&obj, utl::GetTypeID<T>(), 0) {}

      Expr();
      Expr(int i) : Expr(&i, utl::GetTypeID<int>()) {}
      Expr(float f) : Expr(&f, utl::GetTypeID<float>()) {}
      Expr(List &list) : Expr(&list, utl::GetTypeID<List>()) {}
      Expr(List &&list) : Expr(&list, utl::GetTypeID<List>(), 0) {}
      Expr(Sym &symb) : Expr(&symb, utl::GetTypeID<Sym>()) {}
      Expr(Sym &&symb) : Expr(&symb, utl::GetTypeID<Sym>(), 0) {}
      Expr(Str &str) : Expr(&str, utl::GetTypeID<Str>()) {}
      Expr(Str &&str) : Expr(&str, utl::GetTypeID<Str>(), 0) {}
      Expr(const char *str) : Expr(Str(str)) {}

      int *i32();
      float *f32();
      List *list();
      Str *str();
      Sym *sym();

      template<typename T>
      T *obj() { return (T*)_getObjImpl(utl::GetTypeID<T>()); }

      //returns if expr should be evaluated, defaults to true
      bool &eval();

      //returns false if Expr is nil
      explicit operator bool();      

      DECLARE_UTILITY_PUBLIC(Expr)
   };

   UTILITY_API List parseFile(const char *path);
}