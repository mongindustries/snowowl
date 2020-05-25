#include "directx/buffer_allocator.h"
#include "directx/buffer.h"

SNOW_OWL_NAMESPACE(gx::dx)

buffer_allocator::buffer_allocator    (context& context, size_t initial_size)
  : gx::buffer_allocator(context, initial_size) {

  D3D12_HEAP_DESC heap_desc{};

  // allocate alongside alignment props
  size_t final_size = initial_size;
  size_t alignment  = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

  if (final_size % alignment > 0)
    final_size = (static_cast<size_t>(ceil(static_cast<float>(initial_size) / alignment)) + 1) * alignment;

  // create a private GPU only heap, buffer instances will hold intermediate data for upload to this heap
  heap_desc.SizeInBytes     = final_size;
  heap_desc.Flags           = D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES;
  heap_desc.Alignment       = alignment;
  heap_desc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;

  context.device->CreateHeap(&heap_desc, __uuidof(ID3D12Heap), heap.put_void());
}

buffer_allocator::~buffer_allocator   () { }

cx::exp::ptr < buffer < pipeline::buffer_type::typeData > >
  buffer_allocator::create_data       (pipeline::buffer_visibility_type visibility, size_t allocator_offset, size_t size, size_t stride) {
  
    cx::exp::ptr<buffer< pipeline::buffer_type::typeData >, dx::buffer_data> instance;

    D3D12_RESOURCE_DESC resource_desc{};

    resource_desc.Dimension         = D3D12_RESOURCE_DIMENSION_BUFFER;
    resource_desc.Alignment         = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    resource_desc.DepthOrArraySize  = 1;

    resource_desc.Height            = 1;
    resource_desc.Width             = size;

    resource_desc.MipLevels         = 1;

    resource_desc.SampleDesc.Count  = 1;

    resource_desc.Layout            = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    winrt::com_ptr<ID3D12Device> device;
    heap->GetDevice(__uuidof(ID3D12Device), device.put_void());

    D3D12_HEAP_DESC heap_desc = heap->GetDesc();

    // buffer for actual data

    size_t final_offset = allocator_offset;
    size_t alignment    = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

    if (final_offset % alignment > 0)
      final_offset = size_t(std::ceil(static_cast<float>(final_offset) / static_cast<float>(alignment))) * alignment;

    device->CreatePlacedResource(
      heap.get(),
      final_offset,
      &resource_desc,
      D3D12_RESOURCE_STATE_COPY_DEST,
      nullptr,
      __uuidof(ID3D12Resource),
      instance->resource.put_void());

    D3D12_HEAP_PROPERTIES desc  {};
    D3D12_RESOURCE_STATES state {};

    switch (visibility) {
    case pipeline::buffer_visibility_type::dataUsagePrivate: {
      // create upload heap
      desc.Type = D3D12_HEAP_TYPE_UPLOAD;
      state     = D3D12_RESOURCE_STATE_GENERIC_READ;
    } break;
    case pipeline::buffer_visibility_type::dataUsageReadBack: {
      // create read-back heap
      desc.Type = D3D12_HEAP_TYPE_READBACK;
      state     = D3D12_RESOURCE_STATE_COPY_DEST;
    } break;
    }

    // buffer for modifying allocated buffer data~

    device->CreateCommittedResource(
     &desc,
     D3D12_HEAP_FLAG_NONE,
     &resource_desc,
     state,
     nullptr,
     __uuidof(ID3D12Resource),
     instance->resource_upload.put_void());

    instance->current_state     = D3D12_RESOURCE_STATE_COPY_DEST;

    instance->buffer_size       = size;
    instance->buffer_stride     = stride;

    instance->alloc_pos         = final_offset;

    return instance.abstract();
  }

cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture2d > >
  buffer_allocator::create_texture2d  (pipeline::buffer_visibility_type visibility, cx::size_2d const &dimension, pipeline::format) {
  
    D3D12_RESOURCE_DESC tex_desc{};

    tex_desc.Dimension  = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    tex_desc.Width      = dimension.x();
    tex_desc.Height     = dimension.y();

    tex_desc.MipLevels  = 1;
    tex_desc.Alignment  = 0;

    tex_desc.DepthOrArraySize   = 1;

    tex_desc.SampleDesc.Count   = 1;
    tex_desc.SampleDesc.Quality = 0;

    winrt::com_ptr<ID3D12Device> device;
    heap->GetDevice(__uuidof(ID3D12Device), device.put_void());

    return cx::exp::ptr<buffer< pipeline::buffer_type::typeTexture2d >>{nullptr};
  }

cx::exp::ptr < buffer < pipeline::buffer_type::typeTexture3d > >
  buffer_allocator::create_texture3d  (pipeline::buffer_visibility_type visibility, cx::size_3d const&, pipeline::format) {
    return cx::exp::ptr<buffer< pipeline::buffer_type::typeTexture3d >>{nullptr};
  }

SNOW_OWL_NAMESPACE_END
