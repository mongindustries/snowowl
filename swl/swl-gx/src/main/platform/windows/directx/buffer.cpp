#include "directx/context.h"

#include "directx/buffer.h"
#include "directx/buffer_allocator.h"

#include "directx/transfer_block.h"

#include "directx/resource_reference.h"
#include "directx/render_pipeline.h"

SNOW_OWL_NAMESPACE(gx::dx)

buffer_data::buffer_data  ()
  : buffer < gx::pipeline::buffer_type::typeData >  () { }

cx::exp::ptr < gx::transfer_block >
  buffer_data::set_data   (std::array < gx::pipeline::upload_desc, 8 > const &modifications, gx::pipeline::resource_state_desc const& target_type) {

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

  auto *staging = new buffer_data_transfer_block();

  D3D12_RESOURCE_STATES target_state{};

  switch (target_type.usage) {
  case pipeline::usageTypeConstant:
    target_state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    break;
  case pipeline::usageTypeRenderTarget:
    target_state = D3D12_RESOURCE_STATE_RENDER_TARGET;
    break;
  case pipeline::usageTypeTexture:
  case pipeline::usageTypeShader:
    switch (target_type.stage) {
    case 1:   target_state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      break;
    default:  target_state = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
      break;
    }
    break;
  default: ;
  }

  staging->source_state   = current_state;
  staging->target_state   = target_state;

  staging->buffer         = resource;
  staging->buffer_staging = resource_upload;

  staging->size           = buffer_size;
  staging->ref            = cx::exp::ptr_ref{this};

  return cx::exp::ptr< transfer_block, buffer_data_transfer_block >{ staging }.abstract();
}

cx::exp::ptr < gx::transfer_block >
  buffer_data::set_dirty  () { return cx::exp::ptr < gx::transfer_block >{nullptr}; }

cx::exp::ptr_ref < gx::resource_reference >
  buffer_data::reference  (cx::exp::ptr_ref<gx::render_pipeline> const &desc, gx::pipeline::shader_stage const &stage, int slot) {

  resource_reference *reference = new resource_reference();

  reference->resource = this->resource;
  reference->format   = DXGI_FORMAT_UNKNOWN;

  winrt::com_ptr < ID3D12Device > device;
  this->resource->GetDevice(__uuidof(ID3D12Device), device.put_void());

  D3D12_RESOURCE_STATES old_state = current_state;

  switch (desc->stages[stage].input.arguments[slot].type) {
  case gx::pipeline::shader_argument_type::typeBuffer: {

    D3D12_SHADER_RESOURCE_VIEW_DESC desc{};

    desc.ViewDimension           = D3D12_SRV_DIMENSION_BUFFER;
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    desc.Buffer.FirstElement        = 0;
    desc.Buffer.NumElements         = buffer_size / buffer_stride;
    desc.Buffer.StructureByteStride = buffer_stride;
    desc.Buffer.Flags               = D3D12_BUFFER_SRV_FLAG_NONE;

    reference->created_state = current_state;
    reference->resource_type = resource_type::typeSRV;
  } break;
  case gx::pipeline::shader_argument_type::typeConstant: {

    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};

    desc.BufferLocation = resource->GetGPUVirtualAddress();
    desc.SizeInBytes    = buffer_size;

    reference->created_state = current_state;
    reference->resource_type = resource_type::typeCBV;
  } break;
  default:
    break;
  }

  return cx::exp::ptr_ref < resource_reference >{reference}.cast < gx::resource_reference >();
}

SNOW_OWL_NAMESPACE_END
