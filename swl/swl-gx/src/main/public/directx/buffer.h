#pragma once

#include "buffer.hpp"

#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_data;

struct buffer_allocator;

struct buffer_data_staging final : buffer_staging {

  ~buffer_data_staging() override;

  winrt::com_ptr < ID3D12Resource > buffer;
  winrt::com_ptr < ID3D12Resource > buffer_staging;

  cx::exp::ptr_ref < buffer_data > ref;

  D3D12_RESOURCE_STATES source_state;
  D3D12_RESOURCE_STATES target_state;

  size_t alloc_pos;
  size_t size;
};

struct buffer_data final : buffer < typeData > {

  buffer_data();

  /**
   * Method to update a CPU buffer. Returned data is an instruction on how
   * this buffer will be updated to the GPU.
   */
  cx::exp::ptr < buffer_staging >
    set_data(std::array < upload_desc, 8 > const &modifications) override;

  cx::exp::ptr < buffer_staging >
    set_dirty() override;

  cx::exp::ptr_ref < gx::resource_reference >
    reference() override;


  winrt::com_ptr < ID3D12Resource > resource;
  winrt::com_ptr < ID3D12Resource > resource_upload;

  friend struct buffer_allocator;
  friend struct queue;

private:

  buffer_view_type view_type;

  char *mapped_data;

  size_t alloc_pos{};

  size_t buffer_size{};
  size_t buffer_stride{};

  bool data_initialized{};
};

SNOW_OWL_NAMESPACE_END
