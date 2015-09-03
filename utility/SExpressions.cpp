#include "utility/SExpressions.hpp"
#include "utility/Defs.hpp"

namespace utl {
   class SExprPrivate : public ObjectPrivate {
      static const unsigned int PtrSize = sizeof(void*);
      byte m_data[PtrSize * 2];
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

         memcpy(m_data + PtrSize, &rtti, PtrSize);
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

         memcpy(m_data + PtrSize, &rtti, PtrSize);
      }

      void _copyFrom(SExprPrivate const &rhs) {
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
      }

      void _destroy() {
          _destroyObject();
          _clear();
      }

      TypeID getRTTI() const { return *(TypeID*)(m_data + PtrSize); }
      void *getObject() const { return getRTTI()->size() > PtrSize ? *(void**)m_data : (void*)m_data; }

   public:
      SExprPrivate() : m_eval(true) { _clear(); }
      SExprPrivate(void *data, TypeID rtti) :m_eval(true) {
         _clear();
         _copyObjectFrom(data, rtti);
      }

      SExprPrivate(void *data, TypeID rtti, int move) :m_eval(true) {
         _clear();
         _moveObjectFrom(data, rtti);
      }

      SExprPrivate(SExprPrivate const &rhs): m_eval(rhs.m_eval){
         _clear();
         _copyFrom(rhs);
      }

      SExprPrivate &operator=(SExprPrivate const &rhs) {
         _destroy();

         m_eval = rhs.m_eval;

         _copyFrom(rhs);

         return *this;
      }

      ~SExprPrivate() {
         _destroy();
      }

      template<typename T>
      T *conditionReturn() {
         return GetTypeID<T>() == getRTTI() ? (T*)getObject() : nullptr;
      }

      int *getInt() { return conditionReturn<int>(); }
      float *getFloat() { return conditionReturn<float>(); }
      Sublist *getList() { return conditionReturn<Sublist>(); }
      String *getStr() { return conditionReturn<String>(); }
      Symbol *getSymb() { return conditionReturn<Symbol>(); }

      void *getObj(TypeID rtti) { return rtti == getRTTI() ? getObject() : nullptr; }

      //returns if expr should be evaluated
      bool &eval() { return m_eval; }

      //returns false if sexpr is nil
      explicit operator bool() { return getRTTI() != nullptr; }

      DECLARE_UTILITY_PRIVATE(SExpr)
   };

   SExpr::SExpr(void *data, TypeID rtti) :Object(new SExprPrivate(data, rtti)) {}
   SExpr::SExpr(void *data, TypeID rtti, int move) : Object(new SExprPrivate(data, rtti, move)) {}
   void *SExpr::_getObjImpl(TypeID rtti) { return self()->getObj(rtti); }

   SExpr::SExpr() : Object(new SExprPrivate()) {}

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