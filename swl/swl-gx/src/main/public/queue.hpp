//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership.hpp>

#include "context.hpp"

SNOW_OWL_NAMESPACE(gx)

struct render_block;

struct render_pipeline;

struct buffer_staging;


struct queue { SWL_REFERENCEABLE(queue) SWL_POLYMORPHIC(queue)

  explicit queue  (context& context);


  virtual void
        begin     (std::vector<cx::exp::ptr_ref<queue>> const& dependencies);

  virtual void
        submit    (std::vector<cx::exp::ptr_ref<render_block>> const& commands);

  virtual void
        transfer  (std::vector<cx::exp::ptr_ref<buffer_staging>> const& buffers);

protected:

  cx::exp::ptr_ref<context> context;
};

SNOW_OWL_NAMESPACE_END
