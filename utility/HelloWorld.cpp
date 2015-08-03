#include "utility/HelloWorld.hpp"

#include <string>

namespace utl{
   class HelloWorldPrivate : public ObjectPrivate{
   public:
      std::string data;

      DECLARE_UTILITY_PRIVATE(HelloWorld)

      
   };

   MemoryBuffer HelloWorld::getShitImpl(){
      Vector<int> foo;
      foo.push_back(1);
      foo.push_back(2);
      foo.push_back(3);
      foo.push_back(4);
      foo.push_back(5);
      foo.push_back(6);
      foo.push_back(7);
      foo.push_back(8);
      foo.push_back(9);
      foo.push_back(10);
      foo.push_back(11);
      foo.push_back(12);
      
      return foo.getBuffer();
   }


   Closure<int()> HelloWorld::createNumberPrinter(int numberToPrint) {
	   Closure<int()> out([=]() {
		   printf("Hey here's your number: %d", numberToPrint);
		   return 0;
	   });


	   return std::move(out);
   }

   HelloWorld::HelloWorld(const char *toPrint):Object(new HelloWorldPrivate()){
      self()->data = toPrint;
   }

   void HelloWorld::print(){
      printf(self()->data.c_str());
   }

}