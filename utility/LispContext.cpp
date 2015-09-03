#include "utility/LispContext.hpp"

#include <vector>
#include <unordered_map>

using namespace utl;

namespace lisp {
   class ContextPrivate : public ObjectPrivate {
      Expr None;
      std::vector<std::unordered_map<Sym, Expr>> m_table;
   public:
      ContextPrivate() { m_table.push_back({}); }

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

      DECLARE_UTILITY_PRIVATE(Context)
   };

   Context::Context() :Object(new ContextPrivate()) {}

   void Context::push() { self()->push(); }
   void Context::pop(){ self()->pop(); }

   void Context::store(Sym key, Expr &value) { self()->store(key, value); }
   void Context::store(Sym key, Expr &&value) { self()->store(key, std::move(value)); }

   Expr &Context::load(Sym key) { return self()->load(key); }
}