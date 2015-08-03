#pragma once

#include "MemoryBuffer.hpp"

namespace utl{
   template<typename T>
   class Vector{
      MemoryBuffer m_buffer;
      size_t m_size, m_capacity;

      void grow(){
         //vector was empty
         if (!m_capacity){
            m_capacity = 8;
            m_buffer.setSize(m_capacity * sizeof(T));
            return;
         }

         //create the new buffer and set its head
         MemoryBuffer newBuffer;
         newBuffer.setSize(m_capacity * sizeof(T) * 2);
         T *newBegin = (T*)newBuffer.data();

         //move our shit over there
         for (auto && obj : *this){
            new(newBegin++) T(std::move(obj));
         }

         //clear ourselves
         auto size = m_size;
         clear();

         //move back
         m_size = size;
         m_capacity *= 2;
         m_buffer = std::move(newBuffer);
      }

   public:
      Vector():m_size(0), m_capacity(0){
      }
      Vector(MemoryBuffer && buffer)
         :m_buffer(std::move(buffer)){
         m_size = m_buffer.getSize() / sizeof(T);
         m_capacity = m_buffer.getCapacity() / sizeof(T);
      }
      ~Vector(){
         clear();
      }

      T *begin(){
         return (T*)m_buffer.data();
      }

      T *end(){
         return begin() + size();
      }

      MemoryBuffer &&getBuffer(){
         m_size = 0;
         m_capacity = 0;
         return std::move(m_buffer);
      }

      void push_back(T && obj){
         if (m_size == m_capacity){
            grow();
         }

         //construct obj at the correct place in our buffer
         new(begin() + (m_size++)) T(std::move(obj));
         m_buffer.setSize(m_size * sizeof(T));
      }

      void clear(){
         if (m_size){
            while (m_size != 0){
               begin()[--m_size].~T();
            }
            m_buffer.setSize(0);
         }
      }

      size_t size(){
         return m_size;
      }

   };
}