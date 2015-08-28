#include "App.hpp"
#include "Game.hpp"

#include "platform/Window.hpp"
#include "graphics/Renderer.hpp"

#include <atomic>
#include <thread>

#define WIN_X 800
#define WIN_Y 600
#define WIN_TITLE "ms3d"
#define WIN_FULLSCREEN 0

namespace app {
   class RenderThread {
      gfx::Renderer const &m_renderer;
      std::atomic_bool m_isRunning;
      std::thread m_thread;

   public:
      RenderThread(gfx::Renderer const &r) :m_renderer(r), m_isRunning(false){}
      ~RenderThread() {
         stop();
      }

      void start() {
         if (!m_isRunning) {
            m_isRunning = true;
            m_thread = std::thread([=]() {
               m_renderer.beginRender();
               while (m_isRunning) {
                  m_renderer.flush();
               }
            });
         }
      }
      void stop() {
         if (m_isRunning) {
            m_isRunning = false;
            m_thread.join();
         }
      }
   };
   
   class App::Impl {
   public:
      Impl() {
      }
      ~Impl() {
      }

      void start() {
         plat::Window w = plat::Window::create(WIN_X, WIN_Y, WIN_TITLE, WIN_FULLSCREEN ? plat::Window::FULLSCREEN : 0);

         if (!w.getNative()) {
            return;
         }

         gfx::Renderer r(w.getNative());
         RenderThread rThread(r);
         rThread.start();

         Game g(r, w);

         g.start();

         while (true) {
            g.step();

            

            if (g.shouldStop()) {
               break;
            }
         }

         rThread.stop();
      }
   };

   App::App() :pImpl(new Impl()) {}
   App::~App() {}

   void App::start() {
      pImpl->start();
   }

}