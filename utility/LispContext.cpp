#include "utility/LispContext.hpp"

#include <vector>
#include <unordered_map>

using namespace utl;

namespace lisp {


   void storeCommonFunctions(Context &c) {
      c.storeEvaluator("dont-eval", [=](lisp::Expr &expr, lisp::Context &c) {
         return (*expr.list())[1];
      });

      c.storeEvaluator("range-from", [=](lisp::Expr &expr, lisp::Context &c) {
         static Sym symTo = internSym("to");
         List output;
         auto &list = *expr.list();

         //name, from, to, other
         if (list.size() != 4) {
            return Expr();
         }

         int start = 0;
         int end = 0;

         auto startIter = c.evaluate(list[1]);
         auto endIter = c.evaluate(list[3]);

         if (auto i = startIter.i32()) {
            start = *i;
         }
         else {
            return Expr();
         }

         if (auto to = list[2].sym()) {
            if (*to != symTo) {
               return Expr();
            }
         }
         else {
            return Expr();
         }

         if (auto i = endIter.i32()) {
            end = *i;
         }
         else {
            return Expr();
         }

         int inc = 1;
         if (start > end) {
            inc = -inc;
         }

         for (int i = start; ; i += inc) {
            output.push_back(i);
            if (i == end) {
               break;
            }
         }

         return Expr(output);
      });

      c.storeEvaluator("for-each", [=](lisp::Expr &expr, lisp::Context &c) {
         static Sym symIn = internSym("in");
         auto &list = *expr.list();
         Sym stored = nullptr;

         //name, iter, in, lsit, first expr
         if (list.size() < 5) {
            return Expr();
         }

         c.push();

         if (auto iter = list[1].sym()) {
            stored = *iter;            
         }
         else {
            return Expr();
         }

         if (auto in = list[2].sym()) {
            if (*in != symIn) {
               return Expr();
            }            
         }
         else {
            return Expr();
         }

         auto evaluatedList = c.evaluate(list[3]);
         if (auto iterList = evaluatedList.list()) {
            for (auto &i : *iterList) {
               c.store(stored, i);
               for (auto iter = list.begin() + 4; iter != list.end(); ++iter) {
                  c.evaluate(*iter);
               }
            }
         }

         c.pop();
         return Expr();
      });

      c.storeEvaluator("+", [=](lisp::Expr &expr, lisp::Context &c) {
         int outi = 0;
         float outf = 0.0f;
         bool fl = false;

         auto addFunc = [&](Expr*iter) {
            if (auto i = iter->i32()) {
               if (fl) {
                  outf += (float)*i;
               }
               else {
                  outi += *i;
               }
            }
            else if (auto f = iter->f32()) {
               if (!fl) {
                  outf = (float)outi;
                  fl = true;
               }

               outf += *f;
            }
         };
         
         auto l = expr.list();
         for (auto iter = l->begin() + 1; iter != l->end(); ++iter) {
            if (iter->sym() || iter->list()) {
               auto resolved = c.evaluate(*iter);
               addFunc(&resolved);
            }
            else {
               addFunc(iter);
            }
         }

         return fl ? Expr(outf) : Expr(outi);
      });

      c.storeEvaluator("print", [=](lisp::Expr &expr, lisp::Context &c) {         

         utl::Closure<void(Expr&)> printFunc = [&](Expr &e) {
            if (auto str = e.str()) {
               printf("%s", str->c_str());
            }
            else if (auto i = e.i32()) {
               printf("%i", *i);
            }
            else if (auto f = e.f32()) {
               printf("%f", *f);
            }
            else if (auto sym = e.sym()) {
               printf("<%s>", (char*)*sym);
            }
            else if (auto list = e.list()) {
               for (auto && subitem : *list) {
                  printFunc(subitem);
               }
            }
         };

         auto l = expr.list();
         for (auto iter = l->begin() + 1; iter != l->end(); ++iter) {  
            printFunc(c.evaluate(*iter));
         }
         return Expr();
      });
   }

   class ContextPrivate : public ObjectPrivate {
      Expr None;
      std::vector<std::unordered_map<Sym, Expr>> m_table;
   public:
      ContextPrivate() { 
         m_table.push_back({}); 
      }

      void push() { m_table.push_back({}); }
      void pop() { if (m_table.size() > 1) { m_table.pop_back(); } }

      void store(Sym key, Expr &value) {
         auto &found = load(key);
         if (found) {
            found = value;
         }
         else {
            m_table.back().insert(std::make_pair(key, value));
         }
      }
      void store(Sym key, Expr &&value) {
         auto &found = load(key);
         if (found) {
            found = std::move(value);
         }
         else {
            m_table.back().insert(std::make_pair(key, std::move(value)));
         }
      }

      Expr &load(Sym key) {
         for (auto iter = m_table.rbegin(); iter != m_table.rend(); ++iter) {
            auto found = iter->find(key);
            if (found != iter->end()) {
               return found->second;
            }
         }

         return None;
      }

      Expr evaluate(Expr &input) {
         if (auto list = input.list()) {
            if (list->size()) {
               auto first = evaluate(list->front());

               if (auto eval = first.obj<Evaluator>()) {
                  return (**eval)(input, *parent());
               }
            }
         }
         else if (auto name = input.sym()) {
            auto found = load(*name);
            if (found) {
               //if the input is a symbol and that symbol has a stored value, return it
               return evaluate(found);
            }
         }

         //didnt find anything useful, return yourself
         return input;
      }



      DECLARE_UTILITY_PRIVATE(Context)
   };

   Context::Context() :Object(new ContextPrivate()) {
      storeCommonFunctions(*this);
   }

   void Context::push() { self()->push(); }
   void Context::pop(){ self()->pop(); }

   void Context::store(Sym key, Expr &value) { self()->store(key, value); }
   void Context::store(Sym key, Expr &&value) { self()->store(key, std::move(value)); }

   Expr &Context::load(Sym key) { return self()->load(key); }

   Expr Context::evaluate(Expr &input) { return std::move(self()->evaluate(input)); }

   
}