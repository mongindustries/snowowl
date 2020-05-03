//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <rect.hpp>

#include "render_pipeline.hpp"
#include "buffer.hpp"
#include "sampler.hpp"
#include "resource_reference.hpp"

SNOW_OWL_NAMESPACE(gx)

struct context;

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
  transitionShaderResource
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

enum class render_pass_stage_binding {
  bindingGraphicsVertex,
  bindingGraphicsFragment,
};

struct render_pass_draw_range {
  unsigned int begin;
  unsigned int size;
};

struct render_pass { SWL_POLYMORPHIC(render_pass)

  render_pass               (context& context, render_block& render_block);


  virtual void
    set_viewport            (const cx::size_2d &value);

  virtual void
    set_scissor             (const cx::rect &value);


  virtual void
    bind_buffer             (render_pass_stage_binding binding, unsigned int index, const cx::exp::ptr_ref<buffer<buffer_type::typeData>> &buffer);


  virtual void
    bind_fragment_texture   (unsigned int index, const cx::exp::ptr_ref<buffer<buffer_type::typeTexture2d>> &texture);

  virtual void
    bind_fragment_texture   (unsigned int index, const cx::exp::ptr_ref<buffer<buffer_type::typeTexture3d>> &texture);

  virtual void
    bind_fragment_sampler   (unsigned int index, const cx::exp::ptr_ref<sampler> &sampler);


  virtual void
    draw                    (const render_pass_draw_range &vertex_range);

  virtual void
    draw                    (const render_pass_draw_range &index_range, const cx::exp::ptr_ref<buffer<buffer_type::typeData>> &buffer);
};

SNOW_OWL_NAMESPACE_END
