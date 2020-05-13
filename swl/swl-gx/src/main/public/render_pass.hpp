//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <rect.hpp>

#include "context.hpp"

#include "buffer.hpp"
#include "sampler.hpp"
#include "resource_reference.hpp"
#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

struct render_block;

enum render_pass_load_op {
  loadOpLoad,
  loadOpClear,
  loadOpDoNotCare,
  loadOpNoAccess
};

enum render_pass_store_op {
  storeOpStore,
  storeOpDoNotCare,
  storeOpResolve,
  storeOpNoAccess
};

enum render_pass_transition {
  transitionInherit,

  transitionRenderTargetView,
  transitionShaderResource,
  transitionSwapChainPresent,

  transitionCopySource,
  transitionCopyDestination,
};

enum render_pass_stage_binding {
  bindingGraphicsVertex,
  bindingGraphicsFragment,
};

struct render_pass_context {
  cx::exp::ptr_ref<resource_reference> reference;

  render_pass_load_op     action_load;
  render_pass_store_op    action_store;

  std::array<float, 4>    load_clear;

  render_pass_transition  transition_before;
  render_pass_transition  transition_during;
  render_pass_transition  transition_after;
};

struct render_pass_draw_range {
  unsigned int begin;
  unsigned int size;
};

struct transition_handle { SWL_POLYMORPHIC(transition_handle)

  void release();
};

struct render_pass { SWL_REFERENCEABLE(render_pass) SWL_POLYMORPHIC(render_pass)

  enum topology_type {
    typeTriangleList,
    typeTriganleStrip,

    typeLineList,
    typeLineStrip,

    typePointList
  };

  render_pass               (context& context, render_block& render_block);


  virtual void
    set_viewport            (const cx::size_2d &value);

  virtual void
    set_scissor             (const cx::rect &value);

  virtual void
    set_topology            (topology_type type);


  virtual cx::exp::ptr_ref<transition_handle>
    buffer_boundary         (std::vector<std::pair<cx::exp::ptr_ref<resource_reference>, buffer_transition>> const& transitions);


  virtual void
    bind_sampler            (render_pass_stage_binding binding, int slot, cx::exp::ptr_ref<sampler> const& sampler);


  virtual void
    bind_samplers           (render_pass_stage_binding binding, std::vector<cx::exp::ptr_ref<sampler>> const& samplers);


  virtual void
    bind_buffer             (render_pass_stage_binding binding, int slot, cx::exp::ptr_ref<resource_reference> const& reference);

  virtual void
    bind_buffers            (render_pass_stage_binding binding, std::vector<cx::exp::ptr_ref<resource_reference>> const& references);


  virtual void
    draw                    (const render_pass_draw_range &vertex_range);
};


struct buffer_usage_block {

  buffer_usage_block  (cx::exp::ptr_ref<render_pass> const& pass, std::vector<std::pair<cx::exp::ptr_ref<resource_reference>, buffer_transition>> const& transitions);

  ~buffer_usage_block ();


  cx::exp::ptr_ref<transition_handle> handle;
};


SNOW_OWL_NAMESPACE_END
