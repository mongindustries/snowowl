//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <vector>
#include <functional>

#include <header.hpp>
#include <ownership.hpp>

#include "context.hpp"
#include "render_pipeline.hpp"
#include "resource_reference.hpp"

SNOW_OWL_NAMESPACE(gx)

struct queue;

struct render_block { SWL_REFERENCEABLE(render_block) SWL_POLYMORPHIC(render_block)

  render_block (queue& queue, render_pipeline& pipeline);

  friend struct block_boundary;

protected:

  virtual void
    reset (render_pipeline& pipeline);

  virtual void
    close ();
};

struct block_boundary { SWL_BLOCK_CONTEXT(block_boundary)

  block_boundary  (render_block& block, render_pipeline& pipeline);

  ~block_boundary ();

private:

  cx::exp::ptr_ref<render_block> block;
};


SNOW_OWL_NAMESPACE_END
