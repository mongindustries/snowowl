#include "common.h"

[[vk::location(0)]] vertex_output main(vertex_input input) {

    vertex_output output;

    output.pos = input.pos;
    output.col = input.col;

    return output;
}
