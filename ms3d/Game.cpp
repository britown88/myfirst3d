#include "Game.hpp"

namespace app {
   class Game::Impl {
      gfx::Renderer &m_renderer;
      plat::Window &m_window;
   public:
      Impl(gfx::Renderer &r, plat::Window &w):m_renderer(r), m_window(w) {}
      ~Impl() {}

      void start() {

      }
      
      void update() {

      }

      void render() {
         m_renderer.clear({ 0.0f, 0.0f, 0.0f, 1.0f });

         //draw!

         m_renderer.finish();
      }

      void processInput() {
         m_window.pollEvents();
      }

      void step() {
         update();
         render();
         processInput();
      }

      bool shouldStop() {
         return m_window.shouldClose();
      }
   };

   Game::Game(gfx::Renderer &r, plat::Window &w):pImpl(new Impl(r, w)) { }
   Game::~Game() { }

   void Game::start() { pImpl->start(); }
   void Game::step() { pImpl->step(); }
   bool Game::shouldStop() { return pImpl->shouldStop(); }
}