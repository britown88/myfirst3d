#include "utility/SExpressions.hpp"
#include "utility/Defs.hpp"

namespace utl {
   class SExprPrivate : public ObjectPrivate {
      enum Type{ Nil, Int, Float, List, Literal, Symb, Obj };
      Type m_type;
      byte m_data[sizeof(Sublist)];
      bool m_eval;

      void _moveObjectFrom(void *data, IRTTI *rtti) {
         auto s = rtti->size();
         if (s > sizeof(void*)) {
            byte *buff = new byte[s];
            rtti->move(buff, data);
            memcpy(m_data, &buff, sizeof(void*));
         }
         else {
            rtti->move(m_data, data);
         }

         memcpy(m_data + sizeof(void*), &rtti, sizeof(void*));
      }

      void _copyObjectFrom(void *data, IRTTI *rtti) {
         auto s = rtti->size();
         if (s > sizeof(void*)) {
            byte *buff = new byte[s];
            rtti->copy(buff, data);
            memcpy(m_data, &buff, sizeof(void*));
         }
         else {
            rtti->copy(m_data, data);
         }

         memcpy(m_data + sizeof(void*), &rtti, sizeof(void*));
      }

      void _copyFrom(SExprPrivate const &rhs) {
         switch (m_type) {
         case Literal: new(m_data) String(*(String*)rhs.m_data); break;
         case List: new(m_data) Sublist(*(Sublist*)rhs.m_data); break;
         case Obj: _copyObjectFrom((void*)rhs.getObject(), rhs.getRTTI()); break;
         default: memcpy(m_data, rhs.m_data, sizeof(m_data)); break;
         }
      }

      void _destroyObject() {
         getRTTI()->destroy(getObject());
         if (getRTTI()->size() > sizeof(void*)) {
            //we alloc'd extra...delete it
            delete getObject();
         }
      }

      void _destroy() {
         switch (m_type) {
         case Literal: ((String*)m_data)->~String(); break;
         case List: ((Sublist*)m_data)->~Sublist(); break;
         case Obj: _destroyObject(); break;
         default: break;
         }
      }

      IRTTI *getRTTI() const { return *(IRTTI**)(m_data + sizeof(void*)); }
      void *getObject() const { return getRTTI()->size() > sizeof(void*) ? *(void**)m_data : (void*)m_data; }

   public:
      SExprPrivate():m_type(Nil), m_eval(true) {}
      SExprPrivate(int i) :m_type(Int), m_eval(true) { new(m_data) int(i); }
      SExprPrivate(float f) :m_type(Float), m_eval(true) { new(m_data) float(f);}
      SExprPrivate(Sublist &list) :m_type(List), m_eval(true) { new(m_data) Sublist(list);}
      SExprPrivate(Sublist &&list) :m_type(List), m_eval(true) { new(m_data) Sublist(std::move(list)); }
      SExprPrivate(String &str) :m_type(Literal), m_eval(true) { new(m_data) String(str);}
      SExprPrivate(String &&str) :m_type(Literal), m_eval(true) { new(m_data) String(std::move(str)); }
      SExprPrivate(Symbol symb) :m_type(Symb), m_eval(true) { new(m_data) Symbol(symb);}
      SExprPrivate(void *data, IRTTI *rtti) :m_type(Obj), m_eval(true) {
         _copyObjectFrom(data, rtti);
      }

      SExprPrivate(void *data, IRTTI *rtti, int move) :m_type(Obj), m_eval(true) {
         _moveObjectFrom(data, rtti);
      }

      SExprPrivate(SExprPrivate const &rhs):m_type(rhs.m_type), m_eval(rhs.m_eval){
         _copyFrom(rhs);
      }

      SExprPrivate &operator=(SExprPrivate const &rhs) {
         _destroy();

         m_type = rhs.m_type;
         m_eval = rhs.m_eval;

         _copyFrom(rhs);

         return *this;
      }

      ~SExprPrivate() {
         _destroy();
      }

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
      bool &eval() { return m_eval; }

      //returns false if sexpr is nil
      explicit operator bool() { return m_type != Nil; }

      DECLARE_UTILITY_PRIVATE(SExpr)
   };

   SExpr::SExpr(void *data, IRTTI *rtti) :Object(new SExprPrivate(data, rtti)) {}
   SExpr::SExpr(void *data, IRTTI *rtti, int move) : Object(new SExprPrivate(data, rtti, move)) {}
   void *SExpr::_getObjImpl(size_t rtti) { return self()->getObj(rtti); }

   SExpr::SExpr() : Object(new SExprPrivate()) {}
   SExpr::SExpr(int i):Object(new SExprPrivate(i)) {}
   SExpr::SExpr(float f) : Object(new SExprPrivate(f)) {}
   SExpr::SExpr(Sublist &list) : Object(new SExprPrivate(list)) {}
   SExpr::SExpr(Sublist &&list) : Object(new SExprPrivate(std::move(list))) {}
   SExpr::SExpr(String &str) : Object(new SExprPrivate(str)) {}
   SExpr::SExpr(String &&str) : Object(new SExprPrivate(std::move(str))) {}
   SExpr::SExpr(Symbol symb) : Object(new SExprPrivate(symb)) {}

   SExpr::SExpr(SExpr const &rhs): Object(new SExprPrivate(*rhs.self())) {}
   SExpr &SExpr::operator=(SExpr const &rhs) {
      *self() = *rhs.self();
      return *this;
   }

   int *SExpr::getInt() { return self()->getInt(); }
   float *SExpr::getFloat() { return self()->getFloat(); }
   Sublist *SExpr::getList() { return self()->getList(); }
   String *SExpr::getStr() { return self()->getStr(); }
   Symbol *SExpr::getSymb() { return self()->getSymb(); }

   bool &SExpr::eval() { return self()->eval(); }

   SExpr::operator bool() { return (bool)*self(); }
}