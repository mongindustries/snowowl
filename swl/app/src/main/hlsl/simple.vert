#include "common.h"

static vertex_input vertex[3] = {
    { {  0, -1, 1, 1 }, { 1, 0, 0, 1 } },
    { {  1,  1, 1, 1 }, { 0, 1, 0, 1 } },
    { { -1,  1, 1, 1 }, { 0, 0, 1, 1 } }
};

vertex_output main(int index : SV_VertexID) {

    vertex_output output;

    output.pos = vertex[index].pos;
    output.col = vertex[index].col;

    return output;
}
