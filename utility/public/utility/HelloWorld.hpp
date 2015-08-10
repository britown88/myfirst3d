#pragma once

#include "Object.hpp"
#include "Vector.hpp"
#include "Closure.hpp"

namespace utl {
   class HelloWorldPrivate;

   class UTILITY_API HelloWorld : public Object {

      MemoryBuffer getShitImpl();

   public:
      HelloWorld(const char *toPrint);
      void print();
      Vector<int> getShit() {
         return getShitImpl();
      }

      Closure<int()> createNumberPrinter(int numberToPrint);

      DECLARE_UTILITY_PUBLIC(HelloWorld)
   };
}

