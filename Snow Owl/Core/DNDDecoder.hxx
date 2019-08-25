#pragma once

namespace snowowl {

template <class TargetType, typename SerializationType>
class Decoder {
public:

  typedef SerializationType Input;

  static void Decode(const Input &input, TargetType &target) { }
};
}

#define DEFINE_DECODABLE(targetType, serializationType) \
namespace snowowl { \
template<> \
class Decoder<targetType, serializationType> { \
public: \
  typedef serializationType Input; \
  static void Decode(const serializationType &input, targetType &target); }; \
}

#define IMPLEMENT_DECODABLE(targetType, serializationType) \
void snowowl::Decoder<targetType, serializationType>::Decode(const serializationType &input, targetType &target)
