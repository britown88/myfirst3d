#pragma once

namespace utl {
   template<typename T>
   class ObjectHash
   {
   public:
      size_t operator()(T const &obj) const { return obj.hash(); }
   };
}
