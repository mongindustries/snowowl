//
// Created by Michael Ong on 26/4/20.
//
#pragma once

#include <vector>
#include <functional>

#include <header.hpp>
#include <ownership.hpp>

#include "graphics_context.hpp"
#include "graphics_render_pipeline.hpp"
#include "graphics_resource_reference.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_render_pass;

enum graphics_render_pass_load {
  load,
  loadClear,
  loadDoNotCare,
  loadNoAccess
};

enum graphics_render_pass_store {
  store,
  storeDoNotCare,
  storeResolve,
  storeNoAccess
};

enum graphics_render_pass_transition {
  transitionInherit,
  transitionRenderTargetView,
  transitionShaderResource
};

struct graphics_render_pass_context {
  cx::exp::ptr_ref<graphics_resource_reference> reference;

  graphics_render_pass_load                     action_load;
  graphics_render_pass_store                    action_store;

  std::array<float, 4>                          load_clear;

  graphics_render_pass_transition               transition_before;
  graphics_render_pass_transition               transition_during;
  graphics_render_pass_transition               transition_after;
};

struct graphics_render_block {

  graphics_render_block (cx::exp::ptr_ref<graphics_queue> const& context,
                         cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline);

  virtual void
      render_pass       (const std::vector<graphics_render_pass_context>& context, const std::function<void(graphics_render_pass&)> &configure);

  friend struct block_boundary;

protected:

  virtual void
      reset             (cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline);

  virtual void
      close             ();

};

struct block_boundary {

  block_boundary  (cx::exp::ptr_ref<graphics_render_block> const& block, cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline);

  ~block_boundary ();

private:

  cx::exp::ptr_ref<graphics_render_block> block;
};


SNOW_OWL_NAMESPACE_END
