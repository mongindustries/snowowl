//
// Created by Michael Ong on 13/4/20.
//

#ifndef SNOW_OWL_COMMON_H
#define SNOW_OWL_COMMON_H

#if defined(CPP_SHADER)

#include <point.hpp>
typedef swl::cx::vector<float, 4> float4;
#define SV_POSITION
#define COLOR0
#else
#define SV_POSITION : SV_Position
#define COLOR0 : COLOR0
#endif

struct vertex_input {
  float4 pos;
  float4 col;
};

struct vertex_output {
  float4 pos SV_POSITION;
  float4 col COLOR0;
};

#endif //SNOW_OWL_COMMON_H
