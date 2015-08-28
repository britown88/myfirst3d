#include "GLFW/glfw3.h"

#include "platform/Window.hpp"

#include <cstdlib>

namespace plat {
   class GLFWWindow : public gfx::INativeWindow {
      GLFWwindow *m_window, *m_threadWin;
      bool m_success;
      size_t m_width, m_height;
   public:
      GLFWWindow(size_t width, size_t height, const char *title, int flags):m_success(false) {
         GLFWmonitor *monitor = nullptr;
         
         if (!glfwInit()) {
            return;
         }

         if (flags&Window::FULLSCREEN) {
            monitor = glfwGetPrimaryMonitor();
         }

         glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
         m_window = glfwCreateWindow(1, 1, "", NULL, NULL);
         
         glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
         m_threadWin = glfwCreateWindow(width, height, title, monitor, m_window);

         if (!m_window || !m_threadWin) {
            glfwTerminate();
            return;
         }

         //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
         glfwGetFramebufferSize(m_threadWin, (int*)&m_width, (int*)&m_height);
         glfwMakeContextCurrent(m_window);

         m_success = true;
      }

      bool success() { return m_success; }

      size_t getWidth() {
         return m_width;
      }

      size_t getHeight() {
         return m_height;
      }

      void beginRender() {
         glfwMakeContextCurrent(m_threadWin);
      }

      void swapBuffers() {
         glfwSwapBuffers(m_threadWin);
      }

      void destroy() {
         if (m_window) {
            glfwDestroyWindow(m_window);
            glfwDestroyWindow(m_threadWin);
            glfwTerminate();
         }         
         delete this;
      }

      bool shouldClose() {
         return glfwWindowShouldClose(m_threadWin) != 0;
      }
   };


   class WindowPrivate : public ObjectPrivate {      
   public:
      GLFWWindow *wnd;

      WindowPrivate() :wnd(nullptr) {}
      ~WindowPrivate() {
         if (wnd) {
            wnd->destroy();
         }
      }

      DECLARE_PLATFORM_PRIVATE(Window)
   };

   Window::Window() :Object(new WindowPrivate()) {}

   Window Window::create(size_t width, size_t height, const char *title, int flags) {
      Window out;
      auto wnd = new GLFWWindow(width, height, title, flags);

      if (!wnd->success()) {
         wnd->destroy();
         wnd = nullptr;
      }

      out.self()->wnd = wnd;     
      return out;
   }

   gfx::INativeWindow *Window::getNative() {
      return self()->wnd;
   }

   void Window::pollEvents() {
      glfwPollEvents();
   }

   bool Window::shouldClose() {
      if (self()->wnd) {
         return self()->wnd->shouldClose();
      }
      else {
         return true;
      }
   }
}
