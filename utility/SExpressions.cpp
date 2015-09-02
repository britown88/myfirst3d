#include "utility/SExpressions.hpp"
#include "utility/Defs.hpp"

namespace utl {
   class SExprPrivate : public ObjectPrivate {
      enum Type{ Nil, Int, Float, List, Literal, Symb, Obj };
      Type m_type;
      byte m_data[sizeof(Vector<SExpr>)];
   public:
      SExprPrivate():m_type(Nil) {}
      SExprPrivate(int i) :m_type(Int) { new(m_data) int(i); }
      SExprPrivate(float f) :m_type(Float) { new(m_data) float(f);}
      SExprPrivate(Sublist &list) :m_type(List) { new(m_data) Sublist(list);}
      SExprPrivate(String &str) :m_type(Literal) { new(m_data) String(str);}
      SExprPrivate(Symbol symb) :m_type(Symb) { new(m_data) Symbol(symb);}
      SExprPrivate(void *data, IRTTI *rtti) :m_type(Obj) { 
         memcpy(m_data, &data, sizeof(void*)); 
         memcpy(m_data + sizeof(void*), &rtti, sizeof(void*));
      }

      SExprPrivate(SExprPrivate const &rhs):m_type(rhs.m_type) { 
         memcpy(m_data, rhs.m_data, sizeof(m_data));
      }

      ~SExprPrivate() {
         if (m_type == Obj) {
            getRTTI()->destroy(getObject());
         }
      }

      IRTTI *getRTTI() { return *(IRTTI**)(m_data + sizeof(void*)); }
      void *getObject() { return *(void**)m_data; }

      template<typename T>
      T *conditionReturn(Type t) {
         return m_type == t ? (T*)m_data : nullptr;
      }

      int *getInt() { return conditionReturn<int>(Int); }
      float *getFloat() { return conditionReturn<float>(Float); }
      Sublist *getList() { return conditionReturn<Sublist>(List); }
      String *getStr() { return conditionReturn<String>(Literal); }
      Symbol *getSymb() { return conditionReturn<Symbol>(Symb); }

      void *getObj(size_t rtti) { 
         if (m_type == Obj && rtti == getRTTI()->getid()) {
            return getObject();
         }

         return nullptr;
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