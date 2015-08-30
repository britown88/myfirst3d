#include "graphics/Model.hpp"
#include "ModelPrivate.hpp"

#include "utility/Defs.hpp"

#include <memory>

namespace gfx {
   class Model {
      std::unique_ptr<byte[]> m_data;
      std::unique_ptr<int[]> m_indices;
      size_t m_vertexSize;
      size_t m_vertexCount;  
      size_t m_indexCount;

      std::vector<VertexAttribute> m_attrs;
   public:
      Model(void *data, size_t size, size_t vCount, int *indices, int iCount, VertexAttribute *attrs, int attrCount)
      :  m_vertexSize(size), 
         m_vertexCount(vCount),
         m_indexCount(iCount),
         m_attrs(attrs, attrs + attrCount),
         m_data(new byte[size * vCount]),
         m_indices(new int[iCount]){
      }

      ~Model() {
      }

      void bind() {

      }

      void render() {

      }
   };

   Model *ModelFactory::_create(void *data, size_t size, size_t vCount, int *indices, int iCount, VertexAttribute *attrs, int attrCount) const {
      return new Model(data, size, vCount, indices, iCount, attrs, attrCount);
   }

   void modelDestroy(Model *self) {
      delete self;
   }

   void modelBind(Model *self) {
      self->bind();
   }
   void modelDraw(Model *self) {
      self->render();
   }
}