//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>
#include <optional>

#include <header.hpp>
#include <ownership.hpp>

#include "pipeline.hpp"
#include "resource_reference.hpp"
#include "transfer_block.hpp"

SNOW_OWL_NAMESPACE(gx)

/**
 * A graphics resource construct.
 *
 * Instances of this object are created from a <code>swl::gx::buffer_allocator</code>.
 *
 */
template < pipeline::buffer_type Type >
struct buffer { SWL_REFERENCEABLE(buffer) SWL_POLYMORPHIC(buffer)

  /**
   * Method to encode fresh data to the GPU. Returns an instruction
   * on how a <code>queue</code> will synchronize the data.
   */
  template < typename EntryType >
  cx::exp::ptr < transfer_block >
    set_data    (size_t start, std::vector < EntryType > &items) { 
      return set_data(upload_desc{start, sizeof(EntryType) * items.size(), reinterpret_cast < char * >(items.data())});
    }

  /**
   * Method to encode fresh data to the GPU. Returns an instruction
   * on how a <code>queue</code> will synchronize the data.
   */
  cx::exp::ptr < transfer_block >
    set_data    (pipeline::upload_desc const &upload) {
      return set_data(std::array < upload_desc, 8 >{upload});
    }

  /**
   * Method to encode fresh data to the GPU. Returns an instruction
   * on how a <code>queue</code> will synchronize the data.
   */
  virtual cx::exp::ptr < transfer_block >
    set_data    (std::array < pipeline::upload_desc, 8 > const &modifications) {
      return cx::exp::ptr < transfer_block >{nullptr};
    }

  /**
   * Method to obtain fresh data from the GPU. Returns an instruction
   * on how a <code>queue</code> will synchronize the data.
   */
  virtual cx::exp::ptr < transfer_block >
    set_dirty   () {
      return cx::exp::ptr < transfer_block >{nullptr};
    }

  /**
   * Prepares buffer for resource binding in a render pass.
   */
  virtual cx::exp::ptr_ref < resource_reference >
    reference   (pipeline::resource_reference_desc const &desc) {
      return cx::exp::ptr_ref < resource_reference >{nullptr};
    }
};

template < pipeline::buffer_type Type >
buffer < Type >::buffer   () noexcept = default;

template < pipeline::buffer_type Type >
buffer < Type >::~buffer  () = default;

SNOW_OWL_NAMESPACE_END
