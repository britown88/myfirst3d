#pragma once

#include "DLL_API.hpp"
#include "Singleton.hpp"

namespace utl {
   static size_t __RTTI_COUNT = 0;

   class IRTTI {
   public:
      virtual size_t getid() = 0;
      virtual void destroy(void *self) = 0;
      virtual void move(void *lhs, void *rhs) = 0;
      virtual void copy(void *lhs, void *rhs) = 0;
      virtual size_t size() const = 0;
   };

   template<typename T>
   class RTTI : public IRTTI {
   public:
      static size_t id() {
         static size_t TYPEID = __RTTI_COUNT++;
         return TYPEID;
      }

      size_t getid() {
         return id();
      }

      size_t size() const { return sizeof(T); }
      
      void destroy(void *self) {
         ((T*)self)->~T();
      }

      void move(void *lhs, void *rhs) {
         new(lhs) T(std::move(*(T*)rhs));
      }

      void copy(void *lhs, void *rhs) {
         new(lhs) T(*(T*)rhs);
      }
   };

   

}
