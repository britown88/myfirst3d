#pragma once

#include <memory>

namespace app {
   class App {
      class Impl;
      std::unique_ptr<Impl> pImpl;
   public:
      App();
      ~App();
      void start();
   };
}

