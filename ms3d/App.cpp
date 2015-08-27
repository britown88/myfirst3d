#include "App.hpp"

#include "platform/Window.hpp"
#include "graphics/Renderer.hpp"

#define WIN_X 800
#define WIN_Y 600
#define WIN_TITLE "ms3d"
#define WIN_FULLSCREEN 0

namespace app {
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
         while (true) {
            r.clear({0.0f, 0.0f, 0.0f, 1.0f});
            r.finish();
            w.pollEvents();

            if (w.shouldClose()) {
               break;
            }
         }
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