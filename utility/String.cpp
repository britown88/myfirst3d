#include "utility/String.hpp"
#include <string>

namespace utl {
   class StringPrivate : public ObjectPrivate {
      
   public:
      StringPrivate() {}
      StringPrivate(const char *str):m_intern(str){}

      const char *c_str() const { return m_intern.c_str(); }
      size_t size() const { return m_intern.size(); }

      mutable std::string m_intern;

      DECLARE_UTILITY_PRIVATE(String)
   };

   String::String() :Object(new StringPrivate()) {}
   String::String(const char *str) :Object(new StringPrivate(str)) {}
   String::String(String const &rhs) :Object(new StringPrivate(rhs.c_str())) {}

   String &String::operator=(String const &rhs) { self()->m_intern = rhs.self()->m_intern; return *this; }

   const char *String::c_str() const { return self()->c_str(); }
   size_t String::size() const { return self()->size(); }
}