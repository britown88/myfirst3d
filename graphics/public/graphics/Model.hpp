#pragma once

#include "DLL_API.hpp"

#include "utility/Geom.hpp"
#include "utility/Color.hpp"

#include <vector>

namespace gfx {
   class Model;

   enum class VertexAttribute : unsigned int {
      Pos2 = 0,
      Tex2,
      Col4,
      COUNT
   };

   class FVF_Pos2_Tex2_Col4 {
   public:
      static std::vector<VertexAttribute> &attrs() {
         static std::vector<VertexAttribute> out;
         if (out.empty()) {
            out = { VertexAttribute::Pos2,
                     VertexAttribute::Tex2,
                     VertexAttribute::Col4 };
         }
         return out;
      }

      utl::Float2 pos2, tex2;
      utl::ColorRGBAf col4;
   };

   class FVF_Pos2_Col4 {
   public:
      static std::vector<VertexAttribute> &attrs() {
         static std::vector<VertexAttribute> out;
         if (out.empty()) {
            out = { VertexAttribute::Pos2,
               VertexAttribute::Col4 };
         }
         return out;
      }

      utl::Float2 pos2;
      utl::ColorRGBAf col4;
   };

   class GRAPHICS_API ModelFactory {
      Model *_create(void *data, size_t size, size_t vCount, int *indices, int iCount, VertexAttribute *attrs, int attrCount) const;
   public:
      template<typename FVF>
      Model *create(FVF *data, size_t vCount, int *indices, size_t iCount) const {
         return _create(data, sizeof(FVF), vCount, indices, iCount, FVF::attrs().data(), FVF::attrs().size());
      }
   };

   void modelDestroy(Model *self);
}
