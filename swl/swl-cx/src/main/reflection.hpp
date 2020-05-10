#pragma once

#include "header.hpp"
#include "ownership.hpp"

#include <string>
#include <vector>

SNOW_OWL_NAMESPACE(cx)

struct reflection_configurator {

  enum type {
    typeString,
    typeNumber,

    typeObject,
    typeObjectArray,

    typeNothing,
    typeCallback
  };

  enum property_option {
    optionReadOnly
  };

  void register_property(type type, std::string const& name, property_option option);

  void register_method  (type return_type, std::string const& name, std::vector<int> const& parameters);
};

template<typename ObjectType>
struct reflection_object {

  static ObjectType deserialize () { }

  static void       serialize   (ObjectType& object) { }


  static void       initialize  () { }

  static void       destroy     () { }
};

struct reflection_callback {
};

struct reflection_variable {
};

struct reflection {

  static cx::exp::ptr<reflection> instance;

  template<typename ObjectType>
  void register_object    (std::string const& package, reflection_object<ObjectType> const& add);

  void register_variable  (std::string const& package, reflection_variable const& add);
  
  void register_callback  (reflection_callback const& add);

};

SNOW_OWL_NAMESPACE_END
