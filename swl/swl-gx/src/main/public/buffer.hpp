//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership.hpp>
#include <rect.hpp>

#include "context.hpp"
#include "queue.hpp"
#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

struct resource_reference;

struct render_pass;

enum buffer_type {
  typeData,
  typeTexture2d,
  typeTexture3d
};

struct buffer_staging {
  SWL_REFERENCEABLE(buffer_staging) SWL_POLYMORPHIC(buffer_staging)
};

enum buffer_view_type {
  viewTypeShader,
  viewTypeConstant,
  viewTypeTexture,
  viewTypeRenderTarget
};

struct buffer_transition {

  enum transition_type {
    transitionInherit,

    transitionShaderView,
    transitionRenderTargetView,

    transitionCopySource,
    transitionCopyDestination,
  };

  pipeline::shader_stage  stage;

  transition_type         during;

  transition_type         before  {transitionInherit};
  transition_type         after   {transitionInherit};
};

/**
 * A graphics resource construct.
 *
 * Instances of this object are created from a <code>swl::gx::buffer_allocator</code>.
 *
 */
template<buffer_type Type>
struct buffer { SWL_REFERENCEABLE(buffer) SWL_POLYMORPHIC(buffer)

  template<typename EntryType>
  cx::exp::ptr<buffer_staging>
    set_data    (size_t start, std::vector<EntryType>& items) { return update_data(start, sizeof(EntryType) * items.size(), reinterpret_cast<char*>(items.data())); }

  /**
   * Method to update a CPU buffer. Returned data is an instruction on how
   * this buffer will be updated to the GPU.
   */
  virtual cx::exp::ptr<buffer_staging>
    set_data    (size_t start, size_t size, std::vector<char>::pointer data) { return cx::exp::ptr<buffer_staging>{ nullptr }; }

  /**
   * Method to inform the data from CPU/GPU has changed. Returned data is an
   * instruction on how this buffer will be synchronized.
   */
  virtual cx::exp::ptr<buffer_staging>
    set_dirty   () { }

  /**
   *
   * Obtains a resource reference for this buffer.
   *
   */
  virtual cx::exp::ptr_ref<gx::resource_reference>
    reference   (buffer_view_type view_type) { return cx::exp::ptr_ref<resource_reference>{ nullptr }; }
};

SNOW_OWL_NAMESPACE_END
