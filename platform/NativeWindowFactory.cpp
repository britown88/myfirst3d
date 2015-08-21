#include "GLFW/glfw3.h"

#include "platform/NativeWindowFactory.hpp"

#include <cstdlib>

namespace plat {
   class GLFWWindow : public gfx::INativeWindow {
      GLFWwindow *m_window;
   public:
      GLFWWindow() {
         glfwInit();
         m_window = glfwCreateWindow(800, 600, "fuck", nullptr, NULL);

         if (!m_window) {
            glfwTerminate();
         }

         glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
         glfwMakeContextCurrent(m_window);
      }

      size_t getWidth() {
         return 800;
      }

      size_t getHeight() {
         return 600;
      }

      void swapBuffers() {
         glfwSwapBuffers(m_window);
         glfwPollEvents();

         if (glfwWindowShouldClose(m_window)) {
            destroy();
         }
      }

      void destroy() {
         glfwDestroyWindow(m_window);
         glfwTerminate();
         delete this;
         exit(0);
      }
   };

   gfx::INativeWindow *windowCreate() {
      return new GLFWWindow();
   }
}
