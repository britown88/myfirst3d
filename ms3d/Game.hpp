#pragma once

#include <memory>

#include "graphics/Renderer.hpp"
#include "platform/Window.hpp"

namespace app {
   class Game {
      class Impl;
      std::unique_ptr<Impl> pImpl;
   public:
      Game(gfx::Renderer &r, plat::Window &w);
      ~Game();

      void start();
      void step();

      bool shouldStop();
   };
}
