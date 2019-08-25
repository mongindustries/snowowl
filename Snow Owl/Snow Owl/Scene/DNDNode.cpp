#include <string>
#include <utility>
#include "DNDNode.hxx"

using namespace std;

using namespace snowowl;
using namespace scene;

IMPLEMENT_ENCODABLE(Node, Serializable) {

  Serializable make;

  make.write(input->_fName  );
  make.write(input->_fNodes );

  return make;
}
IMPLEMENT_DECODABLE(Node, Serializable) {

  target._fName = input.value<string>("name");
}


Node::Node(std::string name):
  _fName    (std::move(name)),
  _fNodes   ({ }) {
}


std::string
  Node::get_name            () const {
  return _fName;
}

void
  Node::set_name            (std::string value) {
  _fName = move(value);
}


vector<Node*>
  Node::get_children        () const {
  return _fNodes;
}


Node*
  Node::operator<<          (Node* rhs) {
  _fNodes.push_back(rhs);

  return this;
}

void
  Node::remove_from_parent  () {
  
}
