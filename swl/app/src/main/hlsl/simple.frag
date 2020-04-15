#include "common"

float4 main(vertex_output input): SV_TARGET {
    return input.col;
}
