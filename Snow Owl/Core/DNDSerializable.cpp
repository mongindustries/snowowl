#include "DNDSerializable.hxx"

using namespace std;
using namespace snowowl;

template<>
string  Serializable::value
  (std::string key) const {
  return "";
}

template<>
long    Serializable::value(string key) const {
  return 100l;
}

template<>
void Serializable::write(std::string value) {
}
