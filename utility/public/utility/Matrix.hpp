#pragma once

#include "DLL_API.hpp"
#include "Geom.hpp"

namespace utl {
   struct UTILITY_API Matrix {
      

      static Matrix identity();
      static Matrix ortho(float left, float right, float bottom, float top, float near, float far);
      static Matrix scale(Float2 const &v);
      static Matrix translate(Float2 const &v);

      Matrix &operator*=(Matrix const &rhs);
      Matrix operator*(Matrix const &rhs) const; 

      float &operator[](size_t index);
      float const &operator[](size_t index) const;

   private:
      float data[16];
   };

}
