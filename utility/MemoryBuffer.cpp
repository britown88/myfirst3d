#include "utility/MemoryBuffer.hpp"

#include <malloc.h>

namespace utl {
   class MemoryBufferPrivate : public ObjectPrivate {
   public:
      DECLARE_UTILITY_PRIVATE(MemoryBuffer)

      //lifehacks: union your stack-buffers with a double to force alignment!
      union {
         char sbo[sizeof(double)];
         double align;
      };

      void *data;
      size_t size, capacity;

      MemoryBufferPrivate() :size(0), capacity(0), data(nullptr) {
      }

      ~MemoryBufferPrivate() {
         reset();
      }

      void reset() {
         if (data) {
            free(data);
            data = nullptr;
         }
      }

      void setSize(size_t newSize) {
         if (newSize > capacity) {
            if (newSize >= sizeof(sbo)) {
               reset();
               data = malloc(newSize);
            }
            size = capacity = newSize;
         }
         else {
            size = newSize;
         }
      }

      void *getData() const{
         if (size < sizeof(sbo)) {
            return (void*)sbo;
         }
         else {
            return data;
         }
      }
   };


   MemoryBuffer::MemoryBuffer() :Object(new MemoryBufferPrivate) {
   }

   void MemoryBuffer::clear() {
      std::swap(*this, MemoryBuffer());
   }

   void MemoryBuffer::setSize(size_t newSize) {
      self()->setSize(newSize);
   }

   void *MemoryBuffer::data() {
      return self()->getData();
   }

   const void *MemoryBuffer::data() const{
      return self()->getData();
   }

   size_t MemoryBuffer::getSize() {
      return self()->size;
   }

   size_t MemoryBuffer::getCapacity() {
      return self()->capacity;
   }

   MemoryBuffer::operator bool() {
      return pImpl && self()->size > 0;
   }
}

