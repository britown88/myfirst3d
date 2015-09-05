#include "utility/LispContext.hpp"

#include <vector>
#include <unordered_map>

using namespace utl;

namespace lisp {

   void _printExpr(Expr &exp) {

   }

   void storeCommonFunctions(Context &c) {
      c.storeEvaluator("SYM", [=](lisp::Expr &expr, lisp::Context &c) {
         return (*expr.list())[1];
      });

      c.storeEvaluator("print", [=](lisp::Expr &expr, lisp::Context &c) {
         auto l = expr.list();

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
               printf(":%s", (char*)*sym);
            }
            else if (auto list = e.list()) {
               for (auto && subitem : *list) {
                  printFunc(subitem);
               }
            }
         };

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
         m_table.back().insert(std::make_pair(key, value));
      }
      void store(Sym key, Expr &&value) {
         m_table.back().insert(std::make_pair(key, std::move(value)));
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
               if (auto name = list->front().sym()) {
                  auto found = load(*name);
                  if (found) {
                     if (auto eval = found.obj<Evaluator>()) {
                        //if input is a list, that list isnt empty, the first item is a symbol, 
                        //that symbol is stored in the context, and the expr stored is an evaluator,
                        //return the result fo the evaulator, passing the input into it
                        return (**eval)(input, *parent());
                     }
                  }                  
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