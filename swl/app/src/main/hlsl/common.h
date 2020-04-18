//
// Created by Michael Ong on 13/4/20.
//

#ifndef SNOW_OWL_COMMON_H
#define SNOW_OWL_COMMON_H

struct vertex_input {
		[[vk::location(0)]] float4 pos : POSITION;
		[[vk::location(1)]] float4 col : COLOR;
};

struct vertex_output {
		[[vk::location(0)]] float4 pos : SV_POSITION;
		[[vk::location(1)]] float4 col : COLOR;
};

struct test_buffer {
		float4  desc;
		int2    something;
};

#endif //SNOW_OWL_COMMON_H
