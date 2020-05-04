#include "common.h"

float4 main(vertex_output input): SV_TARGET {
  return input.col;
}
