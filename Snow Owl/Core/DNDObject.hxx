#pragma once

#include <string>
#include <typeinfo>

#include "DNDDecoder.hxx"
#include "DNDEncoder.hxx"

namespace snowowl {

template <class DefiningType,
          typename SerializationType,
          typename Encoder = Encoder<DefiningType, SerializationType>,
          typename Decoder = Decoder<DefiningType, SerializationType>
>
class Object {

public:

  static std::string
  reflectable_name() {

    auto& info = typeid(DefiningType);
    return info.name();
  }

  static size_t
  reflectable_hash() {
    auto& info = typeid(DefiningType);
    return info.hash_code();
  }

  Object() {
    
  }

  Object(const typename Decoder::Input& input) {
    Decoder::Decode(input, *static_cast<DefiningType*>(this));
  }

  [[nodiscard]] typename Encoder::Output encode() const {
    return Encoder::Encode(static_cast<const DefiningType*>(this));
  }
};
}

#define DEFINE_CLASS(targetType, serializationType) \
public: \
  friend class snowowl::Encoder<targetType, serializationType>; \
  friend class snowowl::Decoder<targetType, serializationType>; \
  targetType(const serializationType &input): snowowl::Object<targetType, serializationType>(input)
