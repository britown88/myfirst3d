#include "graphics/Renderer.hpp"

#include <Windows.h> //TODO: kill this fucking shit with fire later fuck you cortana
#include <gl/GL.h>

namespace gfx {
   class RendererPrivate : public ObjectPrivate {
   public:
      INativeWindow *m_wnd;
   
      RendererPrivate(INativeWindow *wnd) :m_wnd(wnd) {}
      DECLARE_GRAPHICS_PRIVATE(Renderer)
   };

   
   Renderer::Renderer(INativeWindow *wnd) :Object(new RendererPrivate(wnd)){
   }

   void Renderer::clear(utl::ColorRGBAf const &c) {
      glClearColor(c.r, c.g, c.b, c.a);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   void Renderer::finish() {
      self()->m_wnd->swapBuffers();
   }
}