#include "utility/LispContext.hpp"

#include <vector>
#include <unordered_map>

namespace utl {
   class LispContextPrivate : public ObjectPrivate {
      SExpr None;
      std::vector<std::unordered_map<Symbol, SExpr>> m_table;
   public:
      LispContextPrivate() { m_table.push_back({}); }

      void push() { m_table.push_back({}); }
      void pop() { if (m_table.size() > 1) { m_table.pop_back(); } }

      void store(Symbol key, SExpr &value) {
         m_table.back().insert(std::make_pair(key, value));
      }
      void store(Symbol key, SExpr &&value) {
         m_table.back().insert(std::make_pair(key, std::move(value)));
      }

      SExpr &load(Symbol key) {

         for (auto iter = m_table.rbegin(); iter != m_table.rend(); ++iter) {
            auto found = iter->find(key);
            if (found != iter->end()) {
               return found->second;
            }
         }

         return None;
      }

      DECLARE_UTILITY_PRIVATE(LispContext)
   };

   LispContext::LispContext() :Object(new LispContextPrivate()) {}

   void LispContext::push() { self()->push(); }
   void LispContext::pop(){ self()->pop(); }

   void LispContext::store(Symbol key, SExpr &value) { self()->store(key, value); }
   void LispContext::store(Symbol key, SExpr &&value) { self()->store(key, std::move(value)); }

   SExpr &LispContext::load(Symbol key) { return self()->load(key); }
}