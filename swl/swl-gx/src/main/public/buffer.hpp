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

  enum class location {
    local,
    host,
    both
  };

  buffer  (const context& context, location locality, size_t size) { }

  buffer  (const context& context, location locality, const std::vector<char>& buffer) { }


  virtual void  update_data (unsigned long start, const std::vector<char>& data) { }

  virtual void  stage       (const queue& staging_queue) { }
};

template<>
struct buffer<typeTexture2d> { SWL_REFERENCEABLE(buffer<typeTexture2d>) SWL_POLYMORPHIC(buffer<typeTexture2d>)

  enum class location {
    local,
    host,
    both
  };

  buffer  (const context& context, location locality, cx::size_2d size, int format, const std::vector<char>& data) { }


  virtual void  update_data (cx::rect frame, const std::vector<char>& data) { }

  virtual void  stage       (const queue& staging_queue) { }
};

SNOW_OWL_NAMESPACE_END
