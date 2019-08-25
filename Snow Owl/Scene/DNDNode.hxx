#pragma once

#include <vector>

#include "../Core/DNDObject.hxx"
#include "../Core/DNDSerializable.hxx"

namespace snowowl::scene { class Node; }

DEFINE_ENCODABLE(snowowl::scene::Node, snowowl::Serializable)
DEFINE_DECODABLE(snowowl::scene::Node, snowowl::Serializable)

namespace snowowl::scene {

class Node : public Object<Node, Serializable> {

private:

  std::string        _fName;
  std::vector<Node*> _fNodes;

  DEFINE_CLASS(Node, Serializable) { }

public:

  explicit
    Node                (std::string name);


  [[nodiscard]] std::string
    get_name            () const;

  void
    set_name            (std::string value);

  [[nodiscard]] std::vector<Node*>
    get_children        () const;


  Node*
    operator <<         (Node* rhs); \

  void
    remove_from_parent  ();
};

}
