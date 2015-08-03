#pragma once

#include "Object.hpp"

namespace utl{
   class MemoryBufferPrivate;
   class UTILITY_API MemoryBuffer : public Object{
   public:
      MemoryBuffer();

      void setSize(size_t newSize);
      size_t getSize();
      size_t getCapacity();
      void *data();

	  operator bool();

      DECLARE_UTILITY_PUBLIC(MemoryBuffer)
   };
}