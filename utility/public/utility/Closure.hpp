#pragma once

#include "MemoryBuffer.hpp"
#include "Singleton.hpp"

namespace utl {

   class IClosureDestroy {
   public:
      ~IClosureDestroy() {}
      virtual void operator()(MemoryBuffer &buffer) = 0;
   };

   template<typename T>
   class ClosureDestroy : public IClosureDestroy {
   public:
      void operator()(MemoryBuffer &buffer) {
         if (buffer) {
            ((T*)buffer.data())->~T();
            buffer.clear();
         }
      }
   };

   

   template<typename T> class Closure {};

   template<typename R, typename... Args>
   class Closure<R(Args...)> {
      typedef Closure<R(Args...)> Type;

      MemoryBuffer m_buffer;
      R(*m_execute)(MemoryBuffer &, Args...);
      IClosureDestroy *m_destroy;

      template<typename L>
      void _copyFromLambda(L const &lambda) {

         m_buffer.setSize(sizeof(L));
         new(m_buffer.data()) L(lambda);

         m_execute = [](MemoryBuffer &buff, Args... args) {
            return ((L*)buff.data())->operator()(args...);
         };
      }

      void _destroy() {
         if (m_destroy) {
            (*m_destroy)(m_buffer);
         }
      }

      //copy constructors here are prvate... no copyrino!
      Closure(Type const &other);
      Type &operator=(Type const &other);

   public:

      //default
      Closure() : m_execute(nullptr), m_destroy(nullptr) {}

      //move
      Closure(Type && other) :
         m_buffer(std::move(other.m_buffer)),
         m_execute(other.m_execute),
         m_destroy(other.m_destroy) {
         other.m_destroy = nullptr;
      }

      Type &operator=(Type && other) {
         if (this == &other) {
            return *this;
         }

         _destroy();
         m_buffer = std::move(other.m_buffer);
         m_destroy = other.m_destroy;
         m_execute = other.m_execute;
         return *this;
      }

      //init from lambda
      template<typename L>
      Closure(L const &lambda) :m_destroy(&Singleton<ClosureDestroy<L>>::Instance()) {
         _copyFromLambda(lambda);
      }

      template<typename L>
      Type &operator=(L const &lambda) {
         _destroy();
         m_destroy = &Singleton<ClosureDestroy<L>>::Instance();
         _copyFromLambda(lambda);
         return *this;
      }

      void clear() {
         std::swap(*this, Type());
      }

      //destruct
      ~Closure() {
         _destroy();
      }

      explicit operator bool() {
         return m_buffer;
      }

      R operator()(Args... args) {
         return m_execute(m_buffer, args...);
      }
   };
}