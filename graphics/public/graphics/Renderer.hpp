#pragma once

#include "Object.hpp"
#include "INativeWindow.hpp"

#include "utility/Color.hpp"

namespace gfx {

   class RendererPrivate;

   class GRAPHICS_API Renderer : public Object{
   public:
      Renderer(INativeWindow *wnd);
      
      void clear(utl::ColorRGBAf const &c);
      void finish();


      DECLARE_GRAPHICS_PUBLIC(Renderer)
   };
}
