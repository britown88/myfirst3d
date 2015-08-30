#include "Game.hpp"

#include "graphics/Model.hpp"

namespace app {
   class Game::Impl {
      gfx::Renderer &m_renderer;
      plat::Window &m_window;

      gfx::Model *buildTestModel() {

         std::vector<gfx::FVF_Pos2_Col4> vertices = {
            { { 0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } },
            { { 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } },
            { { 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } },
            { { 0.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }
         };

         std::vector<int> indices = { 0, 1, 2, 3 };

         return m_renderer.getModelFactory().create(
            vertices.data(), 
            vertices.size(), 
            indices.data(), 
            indices.size());
      }

   public:
      Impl(gfx::Renderer &r, plat::Window &w):m_renderer(r), m_window(w) {}
      ~Impl() {}

      void start() {
         auto model = buildTestModel();
      }
      
      void update() {

      }

      void render() {
         m_renderer.viewport({0, 0, (int)m_renderer.getWidth(), (int)m_renderer.getHeight()});
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