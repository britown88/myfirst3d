#include "utility/Object.hpp"

namespace utl{

   Object::Object(ObjectPrivate *_pImpl):pImpl(_pImpl){
      if (pImpl){
         pImpl->setParent(this);
      }
   }
   Object::Object(Object && rhs){
      //steal the pimpl from rhs
      pImpl = rhs.pImpl;
      rhs.pImpl = nullptr;
      if (pImpl){
         pImpl->setParent(this);
      }
   }
   Object &Object::operator=(Object && rhs){
      //assigning self to self...
      if (this == &rhs){
         return *this;
      }

      //current data now invalid, delete
      if (pImpl){
         delete pImpl;
      }

      //same as move constructor
      pImpl = rhs.pImpl;
      rhs.pImpl = nullptr;
      if (pImpl){
         pImpl->setParent(this);
      }

      return *this;
   }
   Object::~Object(){
      if (pImpl){
         delete pImpl;
      }
   }
}