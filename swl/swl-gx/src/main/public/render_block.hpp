//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <header.hpp>
#include <ownership.hpp>

#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

struct queue;

/**
 * A render block construct.
 *
 * A render block is an abstraction of a graphics command buffer. A render block
 * builds commands from a <code>render_pass</code> and is executed by a <code>queue</code>.
 */
struct render_block { SWL_REFERENCEABLE(render_block) SWL_POLYMORPHIC(render_block)

  render_block(queue &queue, render_pipeline *pipeline);

  friend struct render_block_scope;

protected:

  virtual void
    reset (render_pipeline &pipeline);

  virtual void
    close ();
};

struct render_block_scope { SWL_BLOCK_CONTEXT(render_block_scope)

  render_block_scope  (render_block &block, render_pipeline &pipeline);

  ~render_block_scope ();

private:

  cx::exp::ptr_ref < render_block > block;
};


SNOW_OWL_NAMESPACE_END
