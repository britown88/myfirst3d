#pragma once

#include "Object.hpp"
#include "INativeWindow.hpp"
#include "Model.hpp"
#include "Shader.hpp"

#include "utility/Color.hpp"
#include "utility/Geom.hpp"
#include "utility/Matrix.hpp"
#include "utility/StringView.hpp"

namespace gfx {

   

   class RendererPrivate;

   class GRAPHICS_API Renderer : public Object{
   public:
      Renderer(INativeWindow *wnd);

      //utility
      void finish();
      void flush() const;
      void beginRender() const;
      size_t getWidth() const;
      size_t getHeight() const; 
      ModelFactory const &getModelFactory() const;
      ShaderFactory const &getShaderFactory() const;

      //render functions
      void clear(utl::ColorRGBAf const &c);
      void viewport(utl::Recti const &r);
      
      void setShader(Shader *s);
      void setFloat2(utl::StringView u, utl::Float2 const &value);
      void setMatrix(utl::StringView u, utl::Matrix const &value);
      void setColor(utl::StringView u, utl::ColorRGBAf const &value);

      void renderModel(Model *m);      

      DECLARE_GRAPHICS_PUBLIC(Renderer)
   };
}
