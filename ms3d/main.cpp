#include "graphics/Renderer.hpp"
#include "platform/NativeWindowFactory.hpp"

#include <Windows.h>
#include <time.h>

int main() {

   auto wnd = plat::windowCreate();
   auto renderer = gfx::Renderer(wnd);

   srand(time(NULL));

   while (true) {
      renderer.clear({ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1.0 });
      renderer.finish();
      Sleep(0);
   }

   
   return 0;
}
