#include "Game.hpp"

#include "graphics/Model.hpp"

#include "utility/StringView.hpp"
#include "utility/Matrix.hpp"
#include "utility/Color.hpp"
#include "utility/SExpressions.hpp"
#include "utility/String.hpp"

#include <assert.h>
#include "utility/Closure.hpp"
#include "utility/LispContext.hpp"

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


      void _testxpressions() {
         utl::Sublist list1;
         list1.push_back(utl::SExpr(45));

         class foo {
            int i[100];
         public:
            foo() { memset((int*)i, 0, 100 * sizeof(int)); }
            ~foo() {
               i[0] = 5;
            }
         };

         foo bar;

         utl::SExpr sexnil();
         utl::SExpr sexi(1);
         utl::SExpr sexf(2.0f);
         utl::SExpr sexstr(utl::String("lol"));
         utl::SExpr sexsymb(utl::internString("lolol"));
         utl::SExpr sexlist(std::move(list1));
         utl::SExpr sexfoo(bar);


         auto i = *sexi.getInt();
         auto f = *sexf.getFloat();
         auto str = *sexstr.getStr();
         auto symb = *sexsymb.getSymb();
         auto list = *sexlist.getList();
         auto bar2 = *sexfoo.getObj<foo>();


         int j = i;
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

         _testxpressions();

         utl::LispContext context;

         auto evalName = utl::internString("thisIsATest");
         auto eval = std::make_shared<utl::Evaluator>([=](utl::SExpr &sxp, utl::LispContext &context) {return utl::SExpr();});

         context.store(evalName, eval);

         if (auto evalExpr = context.load(evalName)) {
            if (auto e = evalExpr.getObj<std::shared_ptr<utl::Evaluator>>()) {
               auto out = (**e)(utl::SExpr(), context);
            }
         }

         utl::Vector<int> foo;
         foo.push_back(1);
         foo.push_back(2);

         for (auto &&f : foo) {
            int i = f;
            int j = 5;
         }

         utl::Sublist list1;
         list1.push_back(utl::SExpr(45));
         {
            utl::Sublist list2(std::move(list1));
            list2.push_back(utl::SExpr(186));

            for (auto && item : list2) {
               if (auto i = item.getInt()) {
                  int blah = *i;
                  int blahbla = 5;
               }

            }

            list1 = list2;
            auto list3 = std::move(list1);
            list1 = std::move(list3);
         }
         for (auto && item : list1) {
            if (auto i = item.getInt()) {
               int blah = *i;
               int blahbla = 5;
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