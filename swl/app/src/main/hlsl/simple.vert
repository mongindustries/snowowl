#include "common.h"

StructuredBuffer<vertex_input>  vertices  : register(t0);
StructuredBuffer<uint>          indices   : register(t1);

vertex_output main(unsigned int index : SV_VertexID) {

  vertex_output output;

  vertex_input input = vertices[indices[index]];

  output.pos = input.pos;
  output.col = input.col;

  return output;
}
