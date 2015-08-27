#pragma once

#include "DLL_API.hpp"
#include "Object.hpp"

#include "graphics/INativeWindow.hpp"

namespace plat {

   class WindowPrivate;

   class PLATFORM_API Window : public Object {
      Window();
   public:
      enum : int{
         FULLSCREEN = 1 << 0
      };

      static Window create(size_t width, size_t height, const char *title, int flags);

      gfx::INativeWindow *getNative();

      bool shouldClose();
      void pollEvents();
      
      DECLARE_PLATFORM_PUBLIC(Window)
   };

    
}