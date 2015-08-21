#pragma once

namespace gfx {
   class INativeWindow {
   protected:
      virtual ~INativeWindow() {}
   public:
      virtual size_t getWidth() = 0;
      virtual size_t getHeight() = 0;

      virtual void swapBuffers() = 0;

      virtual void destroy() = 0;
   };
}
