#pragma once

#include "MemoryBuffer.hpp"

namespace utl {
   template<typename T>
   class Vector {
      MemoryBuffer m_buffer;
      size_t m_size, m_capacity;

      void grow() {
         //vector was empty
         if (!m_capacity) {
            m_capacity = 8;
            m_buffer.setSize(m_capacity * sizeof(T));
            return;
         }

         //create the new buffer and set its head
         MemoryBuffer newBuffer;
         newBuffer.setSize(m_capacity * sizeof(T) * 2);
         T *newBegin = (T*)newBuffer.data();

         //move our shit over there
         for (auto && obj : *this) {
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
      Vector() :m_size(0), m_capacity(0) {
      }
      Vector(MemoryBuffer && buffer)
         :m_buffer(std::move(buffer)) {
         m_size = m_buffer.getSize() / sizeof(T);
         m_capacity = m_buffer.getCapacity() / sizeof(T);
      }
      ~Vector() {
         clear();
      }

      Vector(Vector<T> const &other) {
         for (auto && o : other) {
            push_back(o);
         }
      }

      Vector<T> &operator=(Vector<T> const &other) {
         if (this == &other) {
            return *this;
         }

         clear();
         for (auto &&o : other) {
            push_back(o);
         }
         return *this;
      }

      Vector(Vector<T> const &&other) {
         for (auto &&o : other) {
            push_back(std::move(o));
         }
         other.clear();
      }

      Vector<T> &operator=(Vector<T> const &&other) {
         if (this == &other) {
            return *this;
         }

         clear();
         for (auto &&o : other) {
            push_back(std::move(o));
         }
         other.clear();
         return *this;
      }

      T const *begin() const {
         return (T*)m_buffer.data();
      }

      T const *end() const {
         return begin() + size();
      }


      T *begin() {
         return (T*)m_buffer.data();
      }

      T *end() {
         return begin() + size();
      }

      MemoryBuffer &&getBuffer() {
         m_size = 0;
         m_capacity = 0;
         return std::move(m_buffer);
      }

      void push_back(T const &obj) {
         if (m_size == m_capacity) {
            grow();
         }

         //construct obj at the correct place in our buffer
         new(begin() + (m_size++)) T(obj);
         m_buffer.setSize(m_size * sizeof(T));
      }

      void push_back(T && obj) {
         if (m_size == m_capacity) {
            grow();
         }

         //construct obj at the correct place in our buffer
         new(begin() + (m_size++)) T(std::move(obj));
         m_buffer.setSize(m_size * sizeof(T));
      }

      void clear() {
         while (m_size > 0) {
            begin()[--m_size].~T();
         }            


         if (m_buffer) {
            m_buffer.clear();
         }
         
         m_capacity = 0;
      }

      size_t size() const {
         return m_size;
      }

   };
}