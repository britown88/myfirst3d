#pragma once

#include "graphics/Shader.hpp"
#include "graphics/Texture.hpp"

#include "utility/Geom.hpp"
#include "utility/Matrix.hpp"
#include "utility/Color.hpp"
#include "utility/StringView.hpp"

#include <stdint.h>

namespace gfx {
   typedef uintptr_t Uniform;

   void shaderSetActive(Shader *self);

   Uniform shaderGetUniform(Shader *self, utl::StringView name);
   void shaderSetFloat2(Shader *self, Uniform u, utl::Float2 const &value);
   void shaderSetMatrix(Shader *self, Uniform u, utl::Matrix const &value);
   void shaderSetColor(Shader *self, Uniform u, utl::ColorRGBAf const &value);
   void shaderSetTextureSlot(Shader *self, Uniform u, TextureSlot const &slot);

}
