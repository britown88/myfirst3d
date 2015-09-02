#include "Game.hpp"

#include "graphics/Model.hpp"

#include "utility/StringView.hpp"
#include "utility/Matrix.hpp"
#include "utility/Color.hpp"
#include "utility/SExpressions.hpp"
#include "utility/String.hpp"

#include <assert.h>
#include "utility/Closure.hpp"

namespace app {
   class Game::Impl {
      gfx::Renderer &m_renderer;
      plat::Window &m_window;

      gfx::Model *buildTestModel() {

         std::vector<gfx::FVF_Pos2_Tex2_Col4> vertices = {
            { { 0.0f, 0.0f },{ 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
            { { 1.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
            { { 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
            { { 0.0f, 1.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }
         };

         std::vector<int> indices = { 0, 1, 3, 1, 2, 3 };

         return m_renderer.getModelFactory().create(
            vertices.data(), 
            vertices.size(), 
            indices.data(), 
            indices.size());
      }

   public:
      Impl(gfx::Renderer &r, plat::Window &w):m_renderer(r), m_window(w) {}
      ~Impl() {}

      gfx::Shader *m_shader;
      gfx::Model *m_model;
      gfx::Texture *m_texture;

      void start() {
         auto &r = m_renderer;

         m_model = buildTestModel();
         m_shader = r.getShaderFactory().create("assets/shaders.glsl", gfx::DiffuseTexture);

         gfx::TextureRequest request(utl::internString("assets/00.png"));

         m_texture = r.getTextureManager().get(request);

         utl::String str1("lol");
         utl::String str2;

         auto asdjak = utl::SExpr(std::move(str1));

         utl::Sublist ajskdljakl;
         auto asdjasdaak = utl::SExpr(std::move(ajskdljakl));

         class foo {
            int i;
         public:
            foo() { i = 10; }
            ~foo() {
               i = 5;
            }
         };

         foo bar;

         auto exp = new utl::SExpr(bar);
         auto barptr = exp->getObj<foo>();
         delete exp;


         utl::SExpr modelexp(m_model);
         utl::SExpr intexp(1);
         utl::SExpr floatexp(2.0f);
         utl::SExpr stringexp(utl::String("hello"));
         utl::SExpr symbexp(utl::internString("hellohello"));
         utl::SExpr listexp(utl::Sublist{});

         
         auto i = intexp.getInt();
         auto m = modelexp.getObj<gfx::Model*>();
         auto f = floatexp.getFloat();
         auto str = stringexp.getStr();
         auto s = symbexp.getSymb();
         auto l = listexp.getList();


         typedef utl::Closure<utl::SExpr(utl::SExpr &)> EvalFunc;

         EvalFunc myclosure([=](utl::SExpr &in) ->utl::SExpr {
            if (auto i = in.getInt()) {
               return utl::SExpr(*i + 5);
            }
            return utl::SExpr();
         });

         auto ptr = std::make_shared<EvalFunc>(std::move(myclosure));
         auto ptr2 = std::move(ptr);

         utl::SExpr *cexpptr = new utl::SExpr(std::move(ptr2));

         auto ptrptr = ptr2.get();

         utl::SExpr cexp = *cexpptr;
         delete cexpptr;


         utl::SExpr integer(1);

         auto func = cexp.getObj<std::shared_ptr<EvalFunc>>();
         if (func) {
            auto result = (**func)(integer);
            if (auto resulti = result.getInt()) {
               auto thisShouldBeA6 = *resulti;
               int asd = 6;
               asd += 6;
            }
         }


      }
      
      void update() {

      }

      void render() {
         auto &r = m_renderer;

         auto uView = utl::internString("uViewMatrix");
         auto uModel = utl::internString("uModelMatrix");
         auto uColor = utl::internString("uColorTransform");
         auto uTexture = utl::internString("uTexMatrix");
         auto uTextureSlot = utl::internString("uTexture");
         

         r.viewport({ 0, 0, (int)r.getWidth(), (int)r.getHeight() });
         r.clear({ 1.0f, 0.0f, 0.0f, 1.0f });

         utl::Matrix modelTransform =
            utl::Matrix::translate({ 100.0f, 100.0f }) *
            utl::Matrix::scale({ 100.0f, 100.0f });

         utl::Matrix viewTransform =
            utl::Matrix::ortho(0.0f, (float)r.getWidth(), (float)r.getHeight(), 0.0f, 1.0f, -1.0f);

         utl::Matrix texTransform = utl::Matrix::identity();

         utl::ColorRGBAf colorTransform = { 1.0f, 1.0f, 1.0f, 1.0f };

         r.setShader(m_shader);
         r.setMatrix(uView, viewTransform);
         r.setMatrix(uModel, modelTransform);
         r.setMatrix(uTexture, texTransform);
         r.setColor(uColor, colorTransform);

         r.bindTexture(m_texture, 0);
         r.setTextureSlot(uTextureSlot, 0);

         r.renderModel(m_model);

         r.finish();
      }

      void processInput() {
         m_window.pollEvents();
      }

      void step() {
         update();
         render();
         processInput();
      }

      bool shouldStop() {
         return m_window.shouldClose();
      }
   };

   Game::Game(gfx::Renderer &r, plat::Window &w):pImpl(new Impl(r, w)) { }
   Game::~Game() { }

   void Game::start() { pImpl->start(); }
   void Game::step() { pImpl->step(); }
   bool Game::shouldStop() { return pImpl->shouldStop(); }
}