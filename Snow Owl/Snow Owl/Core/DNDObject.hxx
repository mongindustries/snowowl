#pragma once

#include <string>
#include <typeinfo>
#include <type_traits>

#include <concepts>

#include "DNDDecoder.hxx"
#include "DNDEncoder.hxx"

namespace snowowl {

template<class T>
concept snowowlObject = std::is_class<T>::value;

template <snowowlObject DefiningType,
          typename SerializationType,
          typename Encoder = Encoder<DefiningType, SerializationType>,
          typename Decoder = Decoder<DefiningType, SerializationType>>
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
    Decoder::Decode(input, *(DefiningType*) this);
  }

  [[nodiscard]] typename Encoder::Output encode() const {
    return Encoder::Encode(reinterpret_cast<const DefiningType*>(this));
  }
};
}

#define DEFINE_CLASS(targetType, serializationType) \
public: \
  friend class snowowl::Encoder<targetType, serializationType>; \
  friend class snowowl::Decoder<targetType, serializationType>; \
  targetType(const serializationType &input): snowowl::Object<targetType, serializationType>(input)
