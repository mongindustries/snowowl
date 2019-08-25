#pragma once

namespace snowowl {

template<class TargetType, typename SerializationType>
class Encoder {
public:

    typedef SerializationType Output;

    static Output Encode(const TargetType* input) { return nullptr; }
};

}


#define DEFINE_ENCODABLE(targetType, serializationType) \
namespace snowowl {\
template<> \
class Encoder<targetType, serializationType> { \
public: \
  typedef serializationType Output; \
  static Output Encode(const targetType *input); \
};\
}

#define IMPLEMENT_ENCODABLE(targetType, serializationType) \
serializationType snowowl::Encoder<targetType, serializationType>::Encode(const targetType* input)
