#include "utility/Assert.hpp"

#include <Windows.h>


void utl::debug::__assert(const char *file, int line, const char *message) {
   char assertBuff[1024];
   sprintf_s(assertBuff, "%s : %d -> FAILED ASSERTION (%s)", file, line, message);

   auto id = MessageBoxA(nullptr, assertBuff, "ASSERTION FAILED!", MB_ABORTRETRYIGNORE | MB_TOPMOST);
   if (id == IDABORT) {
      exit(1);
   }
   if (id == IDRETRY) {
      DebugBreak();
   }
}