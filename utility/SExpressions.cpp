#include "utility/SExpressions.hpp"

namespace utl {
   class SExprPrivate : public ObjectPrivate {
      enum Type{ Nil, Int, Float, List, Literal, Symb, Obj };
      Type m_type;

      int m_int;
      float m_float;
      Sublist m_list;
      String m_string;
      StringView m_symbol;
      void *m_obj;

      IRTTI *m_objRtti;
   public:
      SExprPrivate():m_type(Nil) {}
      SExprPrivate(int i) :m_type(Int), m_int(i) {}
      SExprPrivate(float f) :m_type(Float), m_float(f) {}
      SExprPrivate(Sublist &list) :m_type(List), m_list(list) {}
      SExprPrivate(String &str) :m_type(Literal), m_string(str) {}
      SExprPrivate(Symbol symb) :m_type(Symb), m_symbol(symb) {}
      SExprPrivate(void *data, IRTTI *rtti) :m_type(Obj), m_obj(data), m_objRtti(rtti){}

      SExprPrivate(SExprPrivate const &rhs):m_type(rhs.m_type) { 
         switch (m_type) {
         case Int: m_int = rhs.m_int; break;
         case Float: m_float = rhs.m_float; break;
         case List: m_list = rhs.m_list; break;
         case Literal: m_string = rhs.m_string; break;
         case Symb: m_symbol = rhs.m_symbol; break;
         case Obj: m_int = rhs.m_int; m_objRtti = rhs.m_objRtti; break;
         }
      }

      ~SExprPrivate() {
         if (m_type == Obj) {
            m_objRtti->destroy(m_obj);
         }
      }

      int *getInt() { return m_type == Int ? &m_int : nullptr; }
      float *getFloat() { return m_type == Float ? &m_float : nullptr; }
      Sublist *getList() { return m_type == List ? &m_list : nullptr; }
      String *getStr() { return m_type == Literal ? &m_string : nullptr; }
      Symbol *getSymb() { return m_type == Symb ? &m_symbol : nullptr; }

      void *getObj(size_t rtti) { 
         return m_type == Obj && rtti == m_objRtti->getid() ? m_obj : nullptr;
      }

      //returns if expr should be evaluated
      bool eval() { return false; }

      //returns false if sexpr is nil
      operator bool() { return m_type != Nil; }

      DECLARE_UTILITY_PRIVATE(SExpr)
   };

   SExpr::SExpr(void *data, IRTTI *rtti) :Object(new SExprPrivate(data, rtti)) {}
   void *SExpr::_getObjImpl(size_t rtti) { return self()->getObj(rtti); }

   SExpr::SExpr() : Object(new SExprPrivate()) {}
   SExpr::SExpr(int i):Object(new SExprPrivate(i)) {}
   SExpr::SExpr(float f) : Object(new SExprPrivate(f)) {}
   SExpr::SExpr(Sublist &list) : Object(new SExprPrivate(list)) {}
   SExpr::SExpr(String &str) : Object(new SExprPrivate(str)) {}
   SExpr::SExpr(Symbol symb) : Object(new SExprPrivate(symb)) {}

   SExpr::SExpr(SExpr const &rhs): Object(new SExprPrivate(*rhs.self())) {}

   int *SExpr::getInt() { return self()->getInt(); }
   float *SExpr::getFloat() { return self()->getFloat(); }
   Sublist *SExpr::getList() { return self()->getList(); }
   String *SExpr::getStr() { return self()->getStr(); }
   Symbol *SExpr::getSymb() { return self()->getSymb(); }

   bool SExpr::eval() { return self()->eval(); }

   SExpr::operator bool() { return *self(); }
}