#pragma once

#include <string>
#include <vector>

namespace snowowl {

struct Serializable {

  template<class OutputType>
  OutputType value(std::string key) const {
    
  }

  template<class InputType>
  void write(InputType value) {
    
  }

  template<class ItemInputType>
  void write(std::vector<ItemInputType> value) {
    
  }
};

template<>
[[nodiscard]] std::string Serializable::value (std::string key) const;

template<>
[[nodiscard]] long        Serializable::value (std::string key) const;

template<>
void                      Serializable::write (std::string value);
}
