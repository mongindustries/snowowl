//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <rect.hpp>

#include "context.hpp"
#include "queue.hpp"

SNOW_OWL_NAMESPACE(gx)

enum buffer_type {
  typeData,
  typeTexture2d,
  typeTexture3d
};

template<buffer_type BufferType>
struct buffer { };

template<>
struct buffer<typeData> { SWL_REFERENCEABLE(buffer<typeData>) SWL_POLYMORPHIC(buffer<typeData>)

};

template<>
struct buffer<typeTexture2d> { SWL_REFERENCEABLE(buffer<typeTexture2d>) SWL_POLYMORPHIC(buffer<typeTexture2d>)

};

SNOW_OWL_NAMESPACE_END
