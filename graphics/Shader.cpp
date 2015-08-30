#include "graphics/Shader.hpp"

namespace gfx {
   class Shader {
   public:
      Shader(const char *file, int params) {}
      ~Shader() {}
   };

   Shader *ShaderFactory::create(const char *file, int params) {
      return new Shader(file, params);
   }

   void shaderDestroy(Shader *self) {
      delete self;
   }
}