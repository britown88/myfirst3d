#pragma once

#include "Object.hpp"

namespace utl {
   class MemoryBufferPrivate;
   class UTILITY_API MemoryBuffer : public Object {

      MemoryBuffer(MemoryBuffer const &other);
      MemoryBuffer &operator=(MemoryBuffer const &other);

   public:
      MemoryBuffer();

      void clear();

      void setSize(size_t newSize);
      size_t getSize();
      size_t getCapacity();
      void *data();

      explicit operator bool();

      DECLARE_UTILITY_PUBLIC(MemoryBuffer)
   };
}