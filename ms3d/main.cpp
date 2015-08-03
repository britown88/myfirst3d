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

	auto printer = foo.createNumberPrinter(17);

	printer();

	printer = std::move(foo.createNumberPrinter(24));

	printer();

	printer = std::move(utl::Closure<int()>([]() {printf("testestest\n"); return 0;}));

	printer();

	auto printer2 = utl::Closure<int()>([]() {printf("2nd printer\n"); return 0;});

	//printer = std::move(printer2);

	printer();
	printer2();


	//thasjdkl
	utl::Closure<int()> printer3;
	printer3 = []() {return 5;};

	printf("\n\nHere's a %d\n\n", printer3());
	




   return 0;
}
