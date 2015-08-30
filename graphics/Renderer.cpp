#include "graphics/Renderer.hpp"
#include "DrawQueue.hpp"
#include "ModelPrivate.hpp"

#include <Windows.h> //TODO: kill this fucking shit with fire later fuck you cortana
#include <gl/GL.h>

#include <mutex>
#include <vector>
#include <memory>

namespace gfx {
   class RendererPrivate : public ObjectPrivate {
      std::shared_ptr<DrawQueue> m_workingQueue, m_drawQueue;
      mutable std::mutex m_mutex;
      ModelFactory m_modelFactory;

   public:
      INativeWindow *m_wnd;

      RendererPrivate(INativeWindow *wnd) :
         m_wnd(wnd), 
         m_workingQueue(new DrawQueue), 
         m_drawQueue(new DrawQueue) {}

      template <typename L>
      void draw(L && lambda){
         m_workingQueue->push(std::move(lambda));
      }

      void swapQueues() {
         m_mutex.lock();
         m_drawQueue = std::move(m_workingQueue);
         m_workingQueue.reset(new DrawQueue());
         m_mutex.unlock();
      }

      std::shared_ptr<DrawQueue> getQueue() const {
         m_mutex.lock();
         auto out = m_drawQueue;
         m_mutex.unlock();
         return out;
      }
      ModelFactory const &getModelFactory() const {
         return m_modelFactory;
      }

      DECLARE_GRAPHICS_PRIVATE(Renderer)
   };
      
   Renderer::Renderer(INativeWindow *wnd) :Object(new RendererPrivate(wnd)){
   }
   
   void Renderer::beginRender() const {
      self()->m_wnd->beginRender();
   }
   void Renderer::finish() {
      self()->swapQueues();
   }
   size_t Renderer::getWidth() const {
      return self()->m_wnd->getWidth();
   }
   size_t Renderer::getHeight() const {
      return self()->m_wnd->getHeight();
   }
   void Renderer::flush() const {
      self()->getQueue()->draw();
      self()->m_wnd->swapBuffers();
   }
   ModelFactory const &Renderer::getModelFactory() const {
      return self()->getModelFactory();
   }

   void Renderer::clear(utl::ColorRGBAf const &c) {
      self()->draw([=]() {
         glClearColor(c.r, c.g, c.b, c.a);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      });
   }
   void Renderer::viewport(utl::Recti const &r) {
      int winHeight = self()->m_wnd->getHeight();
      utl::Recti bounds = { 
         r.top.x, 
         winHeight - r.top.y - utl::height(r),
         utl::width(r), 
         utl::height(r)
      };

      self()->draw([=]() {
         glViewport(bounds.top.x, bounds.top.y, bounds.bot.x, bounds.bot.y);
      });
   }

   
}