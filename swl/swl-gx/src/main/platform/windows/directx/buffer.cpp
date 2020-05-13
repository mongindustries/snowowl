#include "directx/buffer.h"
#include "directx/resource_reference.h"

SNOW_OWL_NAMESPACE(gx::dx)

buffer_data::buffer_data    (dx::context& context): gx::buffer<gx::typeData>() {

  D3D12_HEAP_PROPERTIES props{};

  props.Type = D3D12_HEAP_TYPE_UPLOAD;

  D3D12_RESOURCE_DESC resource{};

  resource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  resource.Width = 12312312;
  resource.Height = 1;
  resource.MipLevels = 1;
  resource.SampleDesc.Count = 1;
  resource.SampleDesc.Quality = 0;
  resource.Format = DXGI_FORMAT_UNKNOWN;
  resource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
  resource.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
  resource.Flags = {};

  context.device->CreateCommittedResource(
    &props, D3D12_HEAP_FLAG_NONE,
    &resource, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr,
    __uuidof(ID3D12Resource), this->resource.put_void());

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};

  heap_desc.NumDescriptors  = 1;
  heap_desc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  heap_desc.Flags           = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

  context.device->CreateDescriptorHeap(
    &heap_desc,
    __uuidof(ID3D12DescriptorHeap), descriptor.put_void());
}

cx::exp::ptr<buffer_staging>
  buffer_data::update_data  (size_t start, size_t size, std::vector<char>::pointer data) {
  return cx::exp::ptr<buffer_staging>();
}

cx::exp::ptr<buffer_staging>
  buffer_data::set_dirty    () {
  return cx::exp::ptr<buffer_staging>();
}

cx::exp::ptr_ref<gx::resource_reference>
  buffer_data::reference    (buffer_view_type view_type, buffer_transition const& transition_info) {

  dx::resource_reference* reference = new dx::resource_reference();

  reference->created_state      = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

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
    desc.Buffer.NumElements         = 10000000;

    desc.Buffer.StructureByteStride = 0;

    desc.Buffer.Flags               = D3D12_BUFFER_SRV_FLAG_NONE;

    device->CreateShaderResourceView(this->resource.get(), &desc, reference->handle.cpu_handle);
  } break;
  case viewTypeConstant: {

    D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};

    desc.BufferLocation = resource->GetGPUVirtualAddress();
    desc.SizeInBytes    = 10000000;

    device->CreateConstantBufferView(&desc, reference->handle.cpu_handle);
  } break;
  case viewTypeTexture: {
    device->CreateShaderResourceView(this->resource.get(), nullptr, reference->handle.cpu_handle);
  } break;
  case viewTypeRenderTarget: {
    device->CreateRenderTargetView(this->resource.get(), nullptr, reference->handle.cpu_handle);
  } break;
  }

  return cx::exp::ptr_ref<resource_reference>{ reference }.cast<gx::resource_reference>();
}

void 
  buffer_data::dereference  () {
}

SNOW_OWL_NAMESPACE_END
