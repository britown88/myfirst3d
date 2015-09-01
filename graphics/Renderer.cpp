#include "GL/glew.h"
#include "graphics/Renderer.hpp"
#include "DrawQueue.hpp"
#include "ModelPrivate.hpp"
#include "ShaderPrivate.hpp"
#include "TexturePrivate.hpp"

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
      ShaderFactory m_shaderFactory;
      TextureManager m_textureManager;

      Shader *m_activeShader;
      Model *m_activeModel;

   public:
      INativeWindow *wnd;

      RendererPrivate(INativeWindow *wnd) :
         wnd(wnd),
         m_workingQueue(new DrawQueue), 
         m_drawQueue(new DrawQueue),
         m_activeShader(nullptr),
         m_activeModel(nullptr){}

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
      ModelFactory const &getModelFactory() const { return m_modelFactory;  }
      ShaderFactory const &getShaderFactory() const { return m_shaderFactory; }
      TextureManager const &getTextureManager() const { return m_textureManager; }

      void clear(utl::ColorRGBAf const &c) {
         draw([=]() {
            glClearColor(c.r, c.g, c.b, c.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         });
      }
      void viewport(utl::Recti const &r) {
         int winHeight = wnd->getHeight();
         utl::Recti bounds = {
            r.top.x,
            winHeight - r.top.y - utl::height(r),
            utl::width(r),
            utl::height(r)
         };

         draw([=]() {
            glViewport(bounds.top.x, bounds.top.y, bounds.bot.x, bounds.bot.y);
         });
      }
      void setShader(Shader *s) {
         draw([=]() {
            if (s != m_activeShader) {
               shaderSetActive(s);
               m_activeShader = s;
            }
         });
      }

      void setFloat2(utl::StringView u, utl::Float2 const &value) { 
         draw([=]() {
            if (m_activeShader) {
               auto uni = shaderGetUniform(m_activeShader, u);
               shaderSetFloat2(m_activeShader, uni, value);
            }
            
         });
      }
      void setMatrix(utl::StringView u, utl::Matrix const &value) { 
         draw([=]() {
            if (m_activeShader) {
               auto uni = shaderGetUniform(m_activeShader, u);
               shaderSetMatrix(m_activeShader, uni, value);
            }
         });
      }
      void setColor(utl::StringView u, utl::ColorRGBAf const &value) { 
         draw([=]() {
            if (m_activeShader) {
               auto uni = shaderGetUniform(m_activeShader, u);
               shaderSetColor(m_activeShader, uni, value);
            }
         });
      }

      void setTextureSlot(utl::StringView u, TextureSlot const &value) {
         draw([=]() {
            if (m_activeShader) {
               auto uni = shaderGetUniform(m_activeShader, u);
               shaderSetTextureSlot(m_activeShader, uni, value);
            }
         });
      }

      void bindTexture(Texture *t, TextureSlot slot) {
         draw([=]() {
            textureBind(t, slot);
         });
      }

      void renderModel(Model *m) { 
         draw([=]() {
            if (m != m_activeModel) {
               modelBind(m);
               m_activeModel = m;
            }
               
            modelDraw(m);
         });
      }

      DECLARE_GRAPHICS_PRIVATE(Renderer)
   };
      
   Renderer::Renderer(INativeWindow *wnd) :Object(new RendererPrivate(wnd)){
   }
   
   void Renderer::beginRender() const {
      self()->wnd->beginRender();
      glewInit();
   }
   void Renderer::finish() {
      self()->swapQueues();
   }
   size_t Renderer::getWidth() const {
      return self()->wnd->getWidth();
   }
   size_t Renderer::getHeight() const {
      return self()->wnd->getHeight();
   }
   void Renderer::flush() const {
      self()->getQueue()->draw();
      self()->wnd->swapBuffers();
   }
   ModelFactory const &Renderer::getModelFactory() const { return self()->getModelFactory(); }
   ShaderFactory const &Renderer::getShaderFactory() const { return self()->getShaderFactory(); }
   TextureManager const &Renderer::getTextureManager() const { return self()->getTextureManager(); }

   void Renderer::clear(utl::ColorRGBAf const &c) { self()->clear(c); }
   void Renderer::viewport(utl::Recti const &r) { self()->viewport(r); }

   void Renderer::setShader(Shader *s) { self()->setShader(s); }   
   void Renderer::setFloat2(utl::StringView u, utl::Float2 const &value) { self()->setFloat2(u, value); }
   void Renderer::setMatrix(utl::StringView u, utl::Matrix const &value) { self()->setMatrix(u, value); }
   void Renderer::setColor(utl::StringView u, utl::ColorRGBAf const &value) { self()->setColor(u, value); }   
   void Renderer::setTextureSlot(utl::StringView u, TextureSlot const &value) { self()->setTextureSlot(u, value); }

   void Renderer::bindTexture(Texture *t, TextureSlot slot) { self()->bindTexture(t, slot); }
   void Renderer::renderModel(Model *m) { self()->renderModel(m); }

   
}