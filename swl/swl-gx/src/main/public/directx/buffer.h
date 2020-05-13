#pragma once

#include "buffer.hpp"

#include "directx/context.h"

SNOW_OWL_NAMESPACE(gx::dx)

struct buffer_data : buffer<gx::typeData> {

  buffer_data   (dx::context& context);

  /**
   * Method to update a CPU buffer. Returned data is an instruction on how
   * this buffer will be updated to the GPU.
   */
  cx::exp::ptr<buffer_staging>
    set_data    (size_t start, size_t size, std::vector<char>::pointer data) override;

  cx::exp::ptr<buffer_staging>
    set_dirty   () override;

  cx::exp::ptr_ref<gx::resource_reference>
    reference   (buffer_view_type view_type) override;


  winrt::com_ptr<ID3D12Resource>        resource;
  winrt::com_ptr<ID3D12Resource>        resource_upload;

  winrt::com_ptr<ID3D12DescriptorHeap>  descriptor;


private:

  size_t                                buffer_size     {};
  size_t                                buffer_stride   {};

  bool                                  data_initialized{};
};

SNOW_OWL_NAMESPACE_END
