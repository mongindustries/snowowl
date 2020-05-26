#pragma once

#include <header.hpp>
#include <ownership.hpp>

#include "transfer_block.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_data;

struct buffer_data_transfer_block final : transfer_block {

  ~buffer_data_transfer_block() override;

  winrt::com_ptr < ID3D12Resource > buffer;
  winrt::com_ptr < ID3D12Resource > buffer_staging;

  cx::exp::ptr_ref < buffer_data > ref;

  D3D12_RESOURCE_STATES source_state;
  D3D12_RESOURCE_STATES target_state;

  size_t alloc_pos;
  size_t size;
};

SNOW_OWL_NAMESPACE_END
