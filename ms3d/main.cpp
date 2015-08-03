#include <iostream>
#include <vector>
#include <functional>

#include "utility/HelloWorld.hpp"
#include "utility/Closure.hpp"

int main(){
   //utl::HelloWorld foo("foo\n");
   //utl::HelloWorld bar("bar\n");

   //foo.print();
   //std::swap(foo, bar);
   //foo.print();

	auto functor = [](int x) {printf("%d", x);};

	//

	//for (auto && shit : foo.getShit()){
	//   printf("%d", shit);
	//}

	utl::HelloWorld foo("sup");

	foo.createNumberPrinter(17)();




   return 0;
}
