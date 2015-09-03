#pragma once

#include "DLL_API.hpp"
#include "Singleton.hpp"

namespace utl {

   class IRTTI {
   public:
      virtual void destroy(void *self) = 0;
      virtual void move(void *lhs, void *rhs) = 0;
      virtual void copy(void *lhs, void *rhs) = 0;
      virtual size_t size() const = 0;
   };

   template<typename T>
   class RTTI : public IRTTI {
   public:
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

   typedef IRTTI* TypeID;

   template<typename T>
   TypeID GetTypeID() {
      return &Singleton<RTTI<T>>::Instance();
   }

}
