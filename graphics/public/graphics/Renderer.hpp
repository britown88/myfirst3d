#pragma once

#include "Object.hpp"
#include "INativeWindow.hpp"

#include "utility/Color.hpp"
#include "utility/Geom.hpp"


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

      //render functions
      void clear(utl::ColorRGBAf const &c);
      void viewport(utl::Recti const &r);

      

      DECLARE_GRAPHICS_PUBLIC(Renderer)
   };
}
