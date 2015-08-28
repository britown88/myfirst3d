#include "App.hpp"

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

         while (true) {
            r.clear({rand()/(float)RAND_MAX, 0.0f, 0.0f, 1.0f});
            r.finish();
            w.pollEvents();

            if (w.shouldClose()) {
               break;
            }
         }

         rThread.stop();
      }
   };

   static App *g_app = nullptr;

   App::App() :pImpl(new Impl()) { g_app = this; }
   App::~App() {}

   void App::start() {
      pImpl->start();
   }

   App const &getApp() {
      return *g_app;
   }
}