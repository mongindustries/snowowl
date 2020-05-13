#include "directx/buffer_allocator.h"

SNOW_OWL_NAMESPACE(gx::dx)

buffer_allocator::buffer_allocator    (context& context, buffer_allocator_usage usage, size_t initial_size): gx::buffer_allocator(context, usage, initial_size) {

  D3D12_HEAP_DESC heap_desc;

  switch (usage) {
  case usagePrivate:

    // create two buffers (one is at buffer instance) one for upload heap and another for direct access

    heap_desc.SizeInBytes     = initial_size;
    heap_desc.Flags           = D3D12_HEAP_FLAG_NONE;
    heap_desc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;

    break;
  case usageShared:

    // create one upload buffer

    heap_desc.SizeInBytes     = initial_size;
    heap_desc.Flags           = D3D12_HEAP_FLAG_NONE;
    heap_desc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;

    break;
  }
  
  context.device->CreateHeap(&heap_desc, __uuidof(ID3D12Heap), heap.put_void());
}

buffer_allocator::~buffer_allocator   () {
  
}


cx::exp::ptr<buffer<typeData>>
  buffer_allocator::create_data       (size_t size) {
  
}

cx::exp::ptr<buffer<typeTexture2d>>
  buffer_allocator::create_texture2d  (cx::size_2d const& dimension, pipeline::format format) {
  
}

cx::exp::ptr<buffer<typeTexture3d>>
  buffer_allocator::create_texture3d  (cx::size_3d const& dimension, pipeline::format format) {
  
}

SNOW_OWL_NAMESPACE_END
