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

SNOW_OWL_NAMESPACE(gx)

struct resource_reference;

enum buffer_type {
  typeData,
  typeTexture2d,
  typeTexture3d
};

struct buffer_staging {
  SWL_REFERENCEABLE(buffer_staging) SWL_POLYMORPHIC(buffer_staging)
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
    update_data(size_t start, std::vector<EntryType>& items) { return update_data(start, sizeof(EntryType) * items.size(), reinterpret_cast<char*>(items.data())); }

  /**
   * Method to update a CPU buffer. Returned data is an instruction on how
   * this buffer will be updated to the GPU.
   */
  cx::exp::ptr<buffer_staging>
    update_data (size_t start, size_t size, std::vector<char>::pointer data) { return cx::exp::ptr<buffer_staging>{ nullptr }; }

  /**
   * Method to inform the data from CPU/GPU has changed. Returned data is an
   * instruction on how this buffer will be synchronized.
   */
  cx::exp::ptr<buffer_staging>
    set_dirty   () { }

  /**
   * Obtains a resource reference for this buffer. A resource reference configures
   * a buffer transition alongside a handle to the CPU/GPU memory location for the
   * buffer.
   *
   * This method also mark this buffer's transition information, calling this method
   * twice with dissimilar data will lead to undefined state.
   */
  cx::exp::ptr_ref<resource_reference>
    reference   (int transition_props) { return cx::exp::ptr_ref<resource_reference>{ nullptr }; }

  /**
   * Invalidates a resource reference for this buffer. This finishes the transition
   * for a buffer by reverting it into the state it was created.
   *
   * Any calls to <code>reference</code> should have balanced calls to <code>dereference</code>.
   */
  void
    dereference () { }
};

SNOW_OWL_NAMESPACE_END
