//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <vector>
#include <functional>

#include <header.hpp>
#include <ownership_exp.hpp>

#include "graphics_context.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_render_pass;

struct graphics_render_pipeline;

struct graphics_render_block {

  graphics_render_block (cx::exp::ptr_ref<graphics_context> const& context,
                         cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline);

  virtual void
      reset             (cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline);

  virtual void
      close             ();

  virtual void
      render_pass       (const std::function<void(graphics_render_pass&)> &configure);
};

SNOW_OWL_NAMESPACE_END
