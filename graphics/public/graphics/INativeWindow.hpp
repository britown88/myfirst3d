#pragma once

#include "Object.hpp"

namespace gfx {


   class INativeWindow {
   protected:
      virtual ~INativeWindow() {}
   public:
      virtual size_t getWidth() = 0;
      virtual size_t getHeight() = 0;

      //call in the thread where rendering will take place
      virtual void beginRender() = 0;

      virtual void swapBuffers() = 0;
   };
}
