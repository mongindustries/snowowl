//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership.hpp>

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
  SWL_REFERENCEABLE(buffer_staging)
  SWL_POLYMORPHIC(buffer_staging)
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
    transitionConstantView,

    transitionRenderTargetView,
  };

  pipeline::shader_stage stage{0xff};

  transition_type during{transitionInherit};

  transition_type before{transitionInherit};
  transition_type after{transitionInherit};
};

/**
 * A graphics resource construct.
 *
 * Instances of this object are created from a <code>swl::gx::buffer_allocator</code>.
 *
 */
template < buffer_type Type >
struct buffer {
  SWL_REFERENCEABLE(buffer)
  SWL_POLYMORPHIC(buffer)

  struct upload_desc {

    size_t start;
    size_t size;

    std::vector < char >::pointer data; // fancier version of char* :P
  };

  template < typename EntryType >
  cx::exp::ptr < buffer_staging >
    set_data(size_t start, std::vector < EntryType > &items) { 
    return set_data(upload_desc{start, sizeof(EntryType) * items.size(), reinterpret_cast < char * >(items.data())});
  }

  /**
   * Method to encode fresh data to the GPU. Returns an instruction
   * on how a <code>queue</code> will synchronize the data.
   */
  cx::exp::ptr < buffer_staging >
    set_data(upload_desc const &upload) { return set_data(std::array < upload_desc, 8 >{upload}); }

  virtual cx::exp::ptr < buffer_staging >
    set_data(std::array < upload_desc, 8 > const &modifications) { return cx::exp::ptr < buffer_staging >{nullptr}; }

  /**
   * Method to obtain fresh data from the GPU. Returns an instruction
   * on how a <code>queue</code> will synchronize the data.
   */
  virtual cx::exp::ptr < buffer_staging >
    set_dirty() { return cx::exp::ptr < buffer_staging >{nullptr}; }

  /**
   *
   * Obtains a resource reference for this buffer.
   *
   */
  virtual cx::exp::ptr_ref < resource_reference >
    reference() { return cx::exp::ptr_ref < resource_reference >{nullptr}; }
};

template < buffer_type Type >
buffer < Type >::buffer() noexcept = default;

template < buffer_type Type >
buffer < Type >::~buffer() = default;

SNOW_OWL_NAMESPACE_END
