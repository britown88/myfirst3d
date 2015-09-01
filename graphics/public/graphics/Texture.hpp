#pragma once

#include "utility/StringView.hpp"
#include "DLL_API.hpp"
#include "Object.hpp"

#include <stdint.h>

namespace gfx {
   typedef uintptr_t TextureSlot;

   enum RepeatType : unsigned int {
      Repeat = 0,
      Clamp
   };

   enum FilterType : unsigned int {
      Linear = 0,
      Nearest
   };

   struct GRAPHICS_API TextureRequest {
      RepeatType repeatType;
      FilterType filterType;
      utl::StringView path;

      TextureRequest(utl::StringView path, RepeatType repeat = RepeatType::Clamp, FilterType filter = FilterType::Nearest);
      bool operator==(const TextureRequest &rhs)const;
      size_t hash() const;
   };

   class Texture;
   class TextureManagerPrivate;

   class GRAPHICS_API TextureManager : public Object{
   public:
      TextureManager();
      Texture *get(TextureRequest const &request) const;

      DECLARE_GRAPHICS_PUBLIC(TextureManager)
   };



}
