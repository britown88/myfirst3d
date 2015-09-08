#pragma once

#include <memory>
#include <vector>

namespace ent {
   static const int MAX_ENTITIES = (1024 * 128);

   template<typename T>
   struct Component {
      int parent;
      T comp;
   };

   class IComponentList {
   public:
      virtual ~IComponentList() {}

      virtual size_t count() = 0;

      virtual void *get(int index) = 0;
      virtual void add(void *data, int index) = 0;
      virtual void remove(int index) = 0;
   };

   template<typename T>
   class ComponentList : public IComponentList {
      std::vector<Component<T>> m_components;
      int m_lookup[MAX_ENTITIES];
   public:
      ComponentList() {
         for (int i = 0; i < MAX_ENTITIES; ++i) {
            m_lookup[i] = -1;
         }
      }

      size_t count() { return m_components.size(); }

      void *get(int index) {
         int i = m_lookup[index];
         return i != -1 ? &m_components[i].comp : nullptr;
      }

      void add(void *data, int index) {
         int i = m_lookup[index];
         if (i == -1) {
            m_lookup[index] = count();
            Component<T> newComp = { index, T(*(T*)data) };
            m_components.push_back(std::move(newComp));
         }
         else {
            new(&m_components[i].comp) T(*(T*)data);
         }
      }

      void remove(int index) {
         if (m_lookup[index] == -1) {
            return;
         }

         auto &toSwap = m_components[size() - 1];
         m_lookup[toSwap.parent] = index;
         std::swap(m_components[size() - 1], m_components[m_lookup[index]]);
         m_components.pop_back();
      }
   };

   class Entity;

   class EntitySystem {
      size_t foo;
   public:
      EntitySystem();
      ~EntitySystem();

      Entity *createEntity();
   };

   class Entity {
      int ID;
      EntitySystem *m_system;
   public:
      template<typename T>
      T *get() {
      }

      template<typename T>
      void add(T const &comp) {
      }

      template<typename T>
      void remove() {
      }
   };
}