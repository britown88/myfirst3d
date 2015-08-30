#pragma once

#include "DLL_API.hpp"

namespace gfx {
   class Shader;

   enum ShaderParams : int{
      DiffuseTexture = 1 << 0
   };

   class GRAPHICS_API ShaderFactory {
   public:
      Shader *create(const char *file, int params);
   };

   void shaderDestroy(Shader *self);

}
