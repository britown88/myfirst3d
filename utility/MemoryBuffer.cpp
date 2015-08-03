#include "utility/MemoryBuffer.hpp"

#include <malloc.h>

namespace utl{
   class MemoryBufferPrivate : public ObjectPrivate{
   public:
      DECLARE_UTILITY_PRIVATE(MemoryBuffer)

      void *data;
      size_t size, capacity;

      MemoryBufferPrivate():size(0), capacity(0){
         data = nullptr;
      }

      ~MemoryBufferPrivate(){
         reset();
      }

      void reset(){
         if (data){
            free(data);
            data = nullptr;
         }
      }

      void setSize(size_t newSize){
         if (newSize > capacity){
            reset();
            data = malloc(newSize);
            size = capacity = newSize;
         }
         else{
            size = newSize;
         }         
      }
   };

   MemoryBuffer::MemoryBuffer():Object(new MemoryBufferPrivate){
   }

   void MemoryBuffer::setSize(size_t newSize){
      self()->setSize(newSize);
   }
   void *MemoryBuffer::data(){
      return self()->data;
   }

   size_t MemoryBuffer::getSize(){
      return self()->size;
   }

   size_t MemoryBuffer::getCapacity(){
      return self()->capacity;

   }

   MemoryBuffer::operator bool() {
	   return pImpl && self()->size > 0;
   }
}

