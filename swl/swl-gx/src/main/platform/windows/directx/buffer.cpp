#include "directx/buffer.h"
#include "directx/render_block.h"
#include "directx/resource_reference.h"

SNOW_OWL_NAMESPACE(gx::dx)

buffer_data::buffer_data    (dx::context& context): gx::buffer<gx::typeData>() { }

cx::exp::ptr<buffer_staging>
  buffer_data::set_data(size_t start, size_t size, std::vector<char>::pointer data) {
  return cx::exp::ptr<buffer_staging>();
}

cx::exp::ptr<buffer_staging>
  buffer_data::set_dirty    () {
  return cx::exp::ptr<buffer_staging>();
}

cx::exp::ptr_ref<gx::resource_reference>
  buffer_data::reference    (buffer_view_type view_type) {

  auto reference = new dx::resource_reference();

  if (data_initialized) {
    reference->created_state    = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
  } else {
    reference->created_state    = D3D12_RESOURCE_STATE_COPY_DEST;
  }

  reference->resource           = this->resource;
  reference->format             = DXGI_FORMAT_UNKNOWN;

  reference->type               = dx::typeAll;

  reference->handle.cpu_handle  = descriptor->GetCPUDescriptorHandleForHeapStart();
  reference->handle.gpu_handle  = descriptor->GetGPUDescriptorHandleForHeapStart();

  winrt::com_ptr<ID3D12Device> device;
  this->resource->GetDevice(__uuidof(ID3D12Device), device.put_void());

  switch (view_type) {
  case viewTypeShader: {

    D3D12_SHADER_RESOURCE_VIEW_DESC desc{};

    desc.ViewDimension              = D3D12_SRV_DIMENSION_BUFFER;

    desc.Buffer.FirstElement        = 0;
    desc.Buffer.NumElements         = buffer_size;

    desc.Buffer.StructureByteStride = buffer_stride;

    desc.Buffer.Flags               = D3D12_BUFFER_SRV_FLAG_NONE;

    device->CreateShaderResourceView(this->resource.get(), &desc, reference->handle.cpu_handle);
  } break;
  case viewTypeConstant: {

    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};

    desc.BufferLocation = resource->GetGPUVirtualAddress();
    desc.SizeInBytes    = buffer_size;

    device->CreateConstantBufferView(&desc, reference->handle.cpu_handle);
  } break;
  default:
    break;
  }

  return cx::exp::ptr_ref<resource_reference>{ reference }.cast<gx::resource_reference>();
}

SNOW_OWL_NAMESPACE_END
