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

struct render_pass_context {
  cx::exp::ptr_ref<resource_reference> reference;

  render_pass_load_op     action_load;
  render_pass_store_op    action_store;

  std::array<float, 4>    load_clear;

  render_pass_transition  transition_before;
  render_pass_transition  transition_during;
  render_pass_transition  transition_after;
};

enum render_pass_stage_binding {
  bindingGraphicsVertex,
  bindingGraphicsFragment,
};

struct render_pass_draw_range {
  unsigned int begin;
  unsigned int size;
};

struct render_pass { SWL_POLYMORPHIC(render_pass)

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

template<buffer_type Type>
struct buffer_boundary {

  typedef std::vector<std::tuple<buffer<Type>&, buffer_view_type, buffer_transition>> input;

  buffer_boundary(render_pass& pass, input const& buffers) : pass(pass) {

    references    .reserve(buffers.size());
    this->buffers .reserve(buffers.size());

    for (auto& buffer : buffers) {
      references    .emplace_back( get<0>(buffer).reference(get<1>(buffer), get<2>(buffer)));
      this->buffers .emplace_back(&get<0>(buffer));
    }
  }

  ~buffer_boundary() {
    for (auto buffer : buffers) {
      buffer->dereference();
    }
  }

  operator std::vector<cx::exp::ptr_ref<resource_reference>>() {
    return references;
  }

private:

  render_pass& pass;

  std::vector<cx::exp::ptr_ref<buffer<Type>>> buffers;

  std::vector<cx::exp::ptr_ref<resource_reference>> references;
};

SNOW_OWL_NAMESPACE_END
