#pragma once

#include "utility/StringView.hpp"
#include <unordered_set>

namespace utl {
   struct StringHash {
      size_t operator()(StringView view) const;
   };

   struct StringComp {
      bool operator()(StringView lhs, StringView rhs) const;
   };

   class StringTable {
      std::unordered_set<StringView, StringHash, StringComp> m_table;
   public:
      ~StringTable();

      StringView get(const char* str);
   };
}
