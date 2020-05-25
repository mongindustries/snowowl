#pragma once

#include "buffer.hpp"
#include "pipeline.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_data;

struct buffer_allocator;

struct buffer_data final : buffer < pipeline::buffer_type::typeData > {

  buffer_data ();

  /**
   * Method to update a CPU buffer. Returned data is an instruction on how
   * this buffer will be updated to the GPU.
   */
  cx::exp::ptr < gx::transfer_block >
    set_data  (std::array < pipeline::upload_desc, 8 > const &modifications) override;

  cx::exp::ptr < gx::transfer_block >
    set_dirty () override;

  cx::exp::ptr_ref < gx::resource_reference >
    reference (pipeline::resource_reference_desc const &desc) override;


  winrt::com_ptr < ID3D12Resource > resource;
  winrt::com_ptr < ID3D12Resource > resource_upload;

  friend struct buffer_allocator;
  friend struct queue;

private:

  D3D12_RESOURCE_STATES   current_state     {};

  size_t                  alloc_pos         {};

  size_t                  buffer_size       {};
  size_t                  buffer_stride     {};

  bool                    data_initialized  {};
};

SNOW_OWL_NAMESPACE_END
