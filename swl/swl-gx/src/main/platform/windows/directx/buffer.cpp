#include "directx/context.h"

#include "directx/buffer.h"
#include "directx/buffer_allocator.h"

#include "directx/transfer_block.h"

#include "directx/resource_reference.h"

SNOW_OWL_NAMESPACE(gx::dx)

buffer_data::buffer_data  ()
  : buffer < pipeline::buffer_type::typeData >  () { }

cx::exp::ptr < gx::transfer_block >
  buffer_data::set_data   (std::array < pipeline::upload_desc, 8 > const &modifications) {

  /*
  
    We are modifying the upload buffer here. The data will be transferred by a queue's transfer
    method. We are doing this so that we can, as much as possible, delay the buffer issuance to
    the GPU, and afford the copy engine to fully utilize transfer bandwidth of the PCIe bus.

   */

  char *mapped_data = nullptr;
  resource_upload->Map(0, nullptr, reinterpret_cast < void ** >(&mapped_data));

  size_t min_m{0};
  size_t max_m{buffer_size};

  for (const auto &item : modifications) {
    if (item.size == item.start) { continue; }

    memcpy(mapped_data + item.start, item.data, item.size);

    min_m = std::min < size_t >(item.start, min_m);
    max_m = std::max < size_t >(item.start + item.size, max_m);
  }

  D3D12_RANGE touched{min_m, max_m};
  resource_upload->Unmap(0, &touched);

  dx::buffer_data_transfer_block *staging = new dx::buffer_data_transfer_block();

  /*
  D3D12_RESOURCE_STATES source_state = D3D12_RESOURCE_STATE_COMMON;
  D3D12_RESOURCE_STATES dest_state   = D3D12_RESOURCE_STATE_COMMON;

  switch (view_type) {
  case viewTypeConstant:
    source_state  = D3D12_RESOURCE_STATE_INDEX_BUFFER | D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    dest_state    = source_state;
    break;
  case viewTypeShader:
    source_state  = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    dest_state    = source_state;
    break;
  case viewTypeTexture:
  case viewTypeRenderTarget:
    source_state  = D3D12_RESOURCE_STATE_COMMON;
    dest_state    = source_state;
  }

  if (!data_initialized) { source_state = D3D12_RESOURCE_STATE_COPY_DEST; }
   */
   
  staging->buffer         = resource;
  staging->buffer_staging = resource_upload;

  staging->size           = buffer_size;
  staging->ref            = cx::exp::ptr_ref{this};

  return cx::exp::ptr< gx::transfer_block, dx::buffer_data_transfer_block >{ staging }.abstract();
}

cx::exp::ptr < gx::transfer_block >
  buffer_data::set_dirty  () { return cx::exp::ptr < gx::transfer_block >{nullptr}; }

cx::exp::ptr_ref < gx::resource_reference >
  buffer_data::reference  (pipeline::resource_reference_desc const &desc) {

  resource_reference *reference = new resource_reference();

  reference->resource = this->resource;
  reference->format   = DXGI_FORMAT_UNKNOWN;

  winrt::com_ptr < ID3D12Device > device;
  this->resource->GetDevice(__uuidof(ID3D12Device), device.put_void());

  switch (desc.usage) {
  case pipeline::buffer_usage_type::usageTypeShader: {
    D3D12_SHADER_RESOURCE_VIEW_DESC desc{};

    desc.ViewDimension           = D3D12_SRV_DIMENSION_BUFFER;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    desc.Buffer.FirstElement        = 0;
    desc.Buffer.NumElements         = buffer_size / buffer_stride;
    desc.Buffer.StructureByteStride = buffer_stride;
    desc.Buffer.Flags               = D3D12_BUFFER_SRV_FLAG_NONE;

    reference->created_state = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    reference->resource_type = resource_type::typeSRV;
  }
  break;
  case pipeline::buffer_usage_type::usageTypeConstant: {
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};

    desc.BufferLocation = resource->GetGPUVirtualAddress();
    desc.SizeInBytes    = buffer_size;

    reference->created_state = D3D12_RESOURCE_STATE_INDEX_BUFFER | D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    reference->resource_type = resource_type::typeCBV;
  }
  break;
  default:
    break;
  }

  if (!data_initialized) { reference->created_state = D3D12_RESOURCE_STATE_COPY_DEST; }

  return cx::exp::ptr_ref < resource_reference >{reference}.cast < gx::resource_reference >();
}

SNOW_OWL_NAMESPACE_END
