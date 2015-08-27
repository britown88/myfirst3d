#pragma once

#include <memory>
#include <vector>

#include "utility/Defs.hpp"
#include "utility/Singleton.hpp"


class DrawQueue {
   const static size_t PageSize = 1024;
   struct Page {
      byte data[PageSize];
      size_t size;
   };

   class ICallDestroy {
   public:
      virtual void operator()(void *buffer) {}
   };

   template<typename T>
   class CallDestroy : public ICallDestroy {
   public:
      void operator()(void *buffer) {
         if (buffer) {
            ((T*)buffer)->~T();
         }
      }
   };

   struct Call {
      size_t size; //sizeof(L) + sizeof(Call)
      ICallDestroy *destroy;
      void(*execute)(void *);
   };

   std::vector<Page> m_pages;

public:
   DrawQueue() {
   }
   ~DrawQueue() {
      for (auto && p : m_pages) {
         size_t i = 0;
         while (i < p.size) {
            Call *c = (Call *)(p.data + i);
            if (c->size > sizeof(Call)) {
               c->destroy->operator()((void*)(p.data + i + sizeof(Call)));
            }
            i += c->size;
         }
      }
   }

   template<typename L>
   void push(L && lambda) {
      size_t callSize = sizeof(Call) + sizeof(L);

      if (callSize > PageSize) {
         return;
      }

      if (m_pages.empty() || m_pages.back().size + callSize > PageSize) {
         m_pages.push_back({ 0 });
      }

      auto &p = m_pages.back();
      Call *c = (Call *)(p.data + p.size);

      c->size = callSize;
      c->destroy = &utl::Singleton<CallDestroy<L>>::Instance();
      c->execute = [](void *data) { ((L*)data)->operator()(); };

      new(p.data + p.size + sizeof(Call)) L(lambda);

      p.size += callSize;
   }

   void draw() {
      for (auto && p : m_pages) {
         size_t i = 0;
         while (i < p.size) {
            Call *c = (Call *)(p.data + i);
            if (c->size > sizeof(Call)) {
               c->execute((void*)(p.data + i + sizeof(Call)));
            }
            i += c->size;
         }
      }
   }
};

