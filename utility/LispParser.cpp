#include "utility/LispExpressions.hpp"

namespace lisp {

      bool isWhitespace(char c) {
         return c == ' ' || c == '\t' || c == '\r' || c == '\n';
      }

      char resolveEscape(char c) {
         switch (c) {
         case 'n': return '\n';
         case 't': return '\t';
         case 'r': return '\r';
         }
         return c;
      }

      enum WordType {
         Literal,//double quotes string literal
         Comment,//semiclon'd comment block         
         ListPrefix,//Value ending with open-parens.  Needs to start a new list with the value pushed in
         Value//Not any of the othr types
      };

      bool charIn(char c, const char *chars) {
         while (char c2 = *chars++) {
            if (c == c2) {
               return true;
            }
         }
         return false;
      }

      //just need to determine if this is a float, an int, or a symbol
      Expr parseWord(char *buffer, int len) {
         if (buffer[len - 1] == 'f' || buffer[len - 1] == 'F' || strchr(buffer, '.')) {
            float f;
            if (sscanf(buffer, "%f", &f) > 0) {
               return f;
            }
         }

         int i;
         if (sscanf(buffer, "%i", &i) > 0) {
            return i;
         }

         return internSym(buffer);
      }

      WordType readWord(const char *&expr, char *&buffer, int &len) {
         len = 0;

         //skip whitespace
         while (char c = *expr) {
            if (!isWhitespace(c)) { break; }
            ++expr;
         }

         if (char c = *expr) {
            //string literal
            if (c == '\"') {
               while (char c = *++expr) {
                  if (c == '\\') {
                     if (char cnext = *++expr) {
                        buffer[len++] = resolveEscape(cnext);
                     }
                  }
                  else if (c == '\"') {
                     ++expr;//dont forget to move the ptr up to skip the last quote
                     break;
                  }
                  else {
                     buffer[len++] = c;
                  }
               }
               buffer[len] = 0;
               return Literal;
            }

            //comments go till end of lines
            if (c == ';') {
               while (char c = *++expr) {
                  if (c == '\n') {
                     break;
                  }
                  buffer[len++] = c;
               }
               buffer[len] = 0;
               return Comment;
            }

            //normal value
            while (char c = *expr++) {
               if (isWhitespace(c)) {
                  break;
               }

               if (c == '(' && len) {
                  //special case.  token ends with an open parens
                  //we want to handle this specially by pretending its the first itme in a new list
                  buffer[len] = 0;//the parens then gets skipped
                  return ListPrefix;
               }

               //we need to check for hitting delims
               if (charIn(c, "\":();")) {
                  if (len) {//we're mid-go so roll the ptr back and break out
                     --expr;
                  }
                  else {//first character so push it in and break out
                     buffer[len++] = c;
                  }
                  break;
               }

               //everythings good, add it!
               buffer[len++] = c;
            }
         }

         buffer[len] = 0;
         return Value;
      }

      Expr createSymDecl(Expr &e) {
         static Sym symName = internSym("dont-eval");
         return List{ symName , e };
      }

      void readList(List &list, const char *&expr) {
         static char buff[255] = { 0 };
         char *buffer = &buff[0];
         int len = 0;
         bool delayEvaluation = false;

         while (*expr) {
            auto wordType = readWord(expr, buffer, len);

            if (wordType == Literal) {//literal string could still be empty
               list.push_back(buffer);
               continue;
            }

            if (!len) {
               continue;//word is empty for some reason..skip it
            }

            if (wordType == Comment) {
               continue; //drop comments
            }

            if (buffer[0] == ':') {
               delayEvaluation = true; //flag the next expr to have delayed eval
               continue;
            }

            //start a new list
            if (buffer[0] == '(' || wordType == ListPrefix) {

               Expr newList = List();

               if (wordType == ListPrefix) {
                  newList.list()->push_back(parseWord(buffer, len));
               }

               readList(*newList.list(), expr);

               if (delayEvaluation) {
                  newList = createSymDecl(newList);
                  delayEvaluation = false;
               }

               list.push_back(newList);
               continue;
            }

            if (buffer[0] == ')') {
               return;//end our current list
            }

            //if we're here it means we're inside one big happy list
            auto newItem = parseWord(buffer, len);
            if (delayEvaluation) {
               newItem = createSymDecl(newItem);
               delayEvaluation = false;
            }
            list.push_back(newItem);
            
         }
      }

      Expr parse(const char *expr) {
         lisp::List out; //back() should always be either nil or a list         

         if (!expr) {
            return Expr();
         }

         readList(out, expr);


         if (out.size() == 0) {
            return Expr();
         }

         return out;
      }

}

