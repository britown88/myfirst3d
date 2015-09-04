#include "utility/LispParser.hpp"
#include "utility/LispExpressions.hpp"


namespace lisp {
   bool isWhitespace(char c) {
      return c == ' ' || c == '\t' || c == '\r' || c == '\n';
   }

   char resolveEscape(char c) {
      switch (c) {
      case 'n': return '\n';
      case 't': return '\t';
      }
      return c;
   }

   //bool isAlpha(char c) {
   //   return c < '0' || c >
   //}

   Expr parse(const char *c) {

      while (c++) {

      }
      return Expr();
   }
}