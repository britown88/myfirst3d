#include "utility/LispExpressions.hpp"
#include "utility/Defs.hpp"

using namespace utl;

namespace lisp {
   class ExprPrivate : public ObjectPrivate {
      static const unsigned int PtrSize = sizeof(void*);
      TypeID m_rtti;
      byte m_data[PtrSize];
      bool m_eval;

      void _moveObjectFrom(void *data, TypeID rtti) {
         if (!rtti) {
            return;
         }

         auto s = rtti->size();
         if (s >PtrSize) {
            byte *buff = new byte[s];
            rtti->move(buff, data);
            memcpy(m_data, &buff, PtrSize);
         }
         else {
            rtti->move(m_data, data);
         }

         m_rtti = rtti;
      }

      void _copyObjectFrom(void *data, TypeID rtti) {
         if (!rtti) {
            return;
         }

         auto s = rtti->size();
         if (s > PtrSize) {
            byte *buff = new byte[s];
            rtti->copy(buff, data);
            memcpy(m_data, &buff, PtrSize);
         }
         else {
            rtti->copy(m_data, data);
         }

         m_rtti = rtti;
      }

      void _copyFrom(ExprPrivate const &rhs) {
         if (auto rtti = rhs.getRTTI()) {
            _copyObjectFrom((void*)rhs.getObject(), rtti);
         }
      }

      void _destroyObject() {
         if (auto rtti = getRTTI()) {
            rtti->destroy(getObject());
            if (rtti->size() > PtrSize) {
               //we alloc'd extra...delete it
               delete getObject();
            }
         }
      }

      void _clear() {
         memset((byte*)m_data, 0, sizeof(m_data));
         m_rtti = nullptr;
      }

      void _destroy() {
          _destroyObject();
          _clear();
      }

      TypeID getRTTI() const { return m_rtti; }
      void *getObject() const { return getRTTI()->size() > PtrSize ? *(void**)m_data : (void*)m_data; }

   public:
      ExprPrivate() : m_eval(true) { _clear(); }
      ExprPrivate(void *data, TypeID rtti) :m_eval(true) {
         _clear();
         _copyObjectFrom(data, rtti);
      }

      ExprPrivate(void *data, TypeID rtti, int move) :m_eval(true) {
         _clear();
         _moveObjectFrom(data, rtti);
      }

      ExprPrivate(ExprPrivate const &rhs): m_eval(rhs.m_eval){
         _clear();
         _copyFrom(rhs);
      }

      ExprPrivate &operator=(ExprPrivate const &rhs) {
         _destroy();
         m_eval = rhs.m_eval;
         _copyFrom(rhs);

         return *this;
      }

      ~ExprPrivate() {
         _destroy();
      }

      template<typename T>
      T *conditionReturn() {
         return GetTypeID<T>() == getRTTI() ? (T*)getObject() : nullptr;
      }

      int *getInt() { return conditionReturn<int>(); }
      float *getFloat() { return conditionReturn<float>(); }
      List *getList() { return conditionReturn<List>(); }
      Str *getStr() { return conditionReturn<Str>(); }
      Sym *getSymb() { return conditionReturn<Sym>(); }

      void *getObj(TypeID rtti) { return rtti == getRTTI() ? getObject() : nullptr; }

      //returns if expr should be evaluated
      bool &eval() { return m_eval; }

      //returns false if Expr is nil
      explicit operator bool() { return getRTTI() != nullptr; }

      DECLARE_UTILITY_PRIVATE(Expr)
   };

   Expr::Expr(void *data, TypeID rtti) :Object(new ExprPrivate(data, rtti)) {}
   Expr::Expr(void *data, TypeID rtti, int move) : Object(new ExprPrivate(data, rtti, move)) {}
   void *Expr::_getObjImpl(TypeID rtti) { return self()->getObj(rtti); }

   Expr::Expr() : Object(new ExprPrivate()) {}

   Expr::Expr(Expr const &rhs): Object(new ExprPrivate(*rhs.self())) {}
   Expr &Expr::operator=(Expr const &rhs) {
      *self() = *rhs.self();
      return *this;
   }

   int *Expr::i32() { return self()->getInt(); }
   float *Expr::f32() { return self()->getFloat(); }
   List *Expr::list() { return self()->getList(); }
   Str *Expr::str() { return self()->getStr(); }
   Sym *Expr::sym() { return self()->getSymb(); }

   bool &Expr::eval() { return self()->eval(); }

   Expr::operator bool() { return (bool)*self(); }
}