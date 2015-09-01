#include "utility/StringView.hpp"
#include "utility/Singleton.hpp"

#include <unordered_set>
#include <string>
#include <string.h>
namespace utl {
   struct StringHash {
      size_t operator()(StringView str) const {
         size_t out = 5381;
         while (*str)
            out = (out << 5) + (out << 1) + (*str++);

         return out;
      }
   };

   struct StringComp {
      bool operator()(StringView lhs, StringView rhs) const {
         return strcmp(lhs, rhs) == 0;
      }
   };

   class StringTable {
      std::unordered_set<StringView, StringHash, StringComp> m_table;
   public:
      ~StringTable() {
         //destruction should be at porogram termination but let's class this joint up
         for (auto &&item : m_table) {
            free((void*)item);
         }
      }

      StringView get(StringView str) {
         auto found = m_table.find(str);
         if (found == m_table.end()) {
            auto len = strlen(str) + 1;
            StringView viewStorage = (StringView)malloc(len);
            memcpy((char*)viewStorage, str, len);
            found = m_table.insert(viewStorage).first;
         }

         return *found;
      }
   };

   StringView internString(StringView str) {
      return Singleton<StringTable>::Instance().get(str);
   }
}