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

struct buffer { SWL_REFERENCEABLE(buffer) SWL_POLYMORPHIC(buffer)

  union size {
    size_t        _data;
    cx::size_2d   _2d;
    cx::size_3d   _3d;
  };

  struct staging { SWL_REFERENCEABLE(staging) SWL_POLYMORPHIC(staging)

  };

  cx::exp::ptr<staging>
    update_data (size_t start, size_t size, std::vector<char> const& data);

  size
    get_size    () const;

  buffer_type type;
};

SNOW_OWL_NAMESPACE_END
