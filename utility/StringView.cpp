#include "utility/StringView.hpp"
#include "utility/Singleton.hpp"

#include <unordered_set>
#include <string>
#include <string.h>

namespace utl {

   class StringTable {
      std::unordered_set<StringView> m_table;
   public:
      ~StringTable() {
         //destruction should be at porogram termination but let's class this joint up
         for (auto &&item : m_table) {
            free((void*)item);
         }
      }

      StringView get(const char* str) {
         auto found = m_table.find((StringView)str);
         if (found == m_table.end()) {
            auto len = strlen(str) + 1;
            StringView viewStorage = (StringView)malloc(len);
            memcpy((char*)viewStorage, str, len);
            found = m_table.insert(viewStorage).first;
         }

         return *found;
      }
   };

   StringView internString(const char* str) {
      return Singleton<StringTable>::Instance().get(str);
   }

   size_t stringViewHash(StringView str) {
      static std::hash<void*> hashFunc;
      return hashFunc((void*)str);
   }
}