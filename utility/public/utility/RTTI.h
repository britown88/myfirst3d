#pragma once

#include "DLL_API.hpp"
#include "Singleton.hpp"

namespace utl {
   static size_t __RTTI_COUNT = 0;

   class IRTTI {
   public:
      virtual size_t getid() = 0;
      virtual void destroy(void *self) = 0;
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
      
      void destroy(void *self) {
         delete (T*)self;
      }
   };

   

}
