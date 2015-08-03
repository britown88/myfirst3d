#pragma once

#include "DLL_API.hpp"

#include <utility>

#define DECLARE_UTILITY_PUBLIC(ClassName) \
   ClassName##Private *self(){return reinterpret_cast<ClassName##Private*>(pImpl);} \
   ClassName##Private const *self() const{return reinterpret_cast<ClassName##Private const *>(pImpl);} \
   ClassName(ClassName && rhs):Object(std::move(rhs)){} \
   ClassName & operator=(ClassName && rhs){Object::operator=(std::move(rhs)); return *this;} \

#define DECLARE_UTILITY_PRIVATE(ClassName) \
   ClassName *parent(){return reinterpret_cast<ClassName*>(parentPtr);} \
   ClassName const *parent() const{return reinterpret_cast<ClassName const *>(parentPtr);}

namespace utl {

   class Object;

   class ObjectPrivate {      
   public:
      Object *parentPtr;

      void setParent(Object *p){
         parentPtr = p;
      }

      virtual ~ObjectPrivate(){}
   };

   class UTILITY_API Object {
   public:
      ObjectPrivate *pImpl;

      Object(ObjectPrivate *pImpl);
      Object(Object && rhs);
      Object &operator=(Object && rhs);
      ~Object();
   };

} // utl