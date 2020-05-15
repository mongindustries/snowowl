## SnowOwl: graphics

Defines the rendering abstraction infrastructure.

### Contents

All objects are contained inside the `swl::gx` namespace.

- `gx::factory`
     Factory to create rendering objects given a rendering context (see below)
- `gx::context`
     The graphics context. This creates the instance and device for a given rendering API.
- `gx::queue`
     A wrapper for graphics submission operations.
- `gx::swap_chain`
     A wrapper for a window's swap chain (a list of display buffers that are presented/swapped incrementally).
- `gx::buffer<>`
     A wrapper for a graphics resource.
     - `gx::buffer<typeTexture2d>` a 2d texture resource.
     - `gx::buffer<typeTexture3d>` a 3d texture resource.
     - `gx::buffer<typeData>` a dat resource.
- `gx::buffer_allocator`
     A wrapper that creates `gx::buffer<>` resources. Specifies the memory locality for a set of resources and synchronization operations.
- `gx::resource_reference`
     A reference made from a `gx::buffer<>`. It defines a graphics API reference.
- `gx::render_pipeline`
     A wrapper for a particular graphics configuration (eg: raster, depth, stencil, shader binding, etc.)
- `gx::render_block`
     A wrapper for a particular graphics state.
- `gx::render_pass`
     A wrapper for graphics command execution.
- `gx::shader`
     A wrapper for a shader object.

### Motivation

Even if modern graphics API are mostly consistent on their goals, namely, treating the GPU as an independent execution machine, specifics for each platform sometimes does not directly translate from one API to another. Additionally, to support the full breadth of features available on a graphics API, one must create platform-specific code for that specific graphics API.. which is time consuming and maintenance prone. To solve this, this module is an opinionated take on how SnowOwl: would at large issue API commands to the GPU. There are some restrictions in terms of buffer submission, queue task scheduling, and resource binding that will be discussed below.

#### Buffer submission

To make the API as consistent as possible, `buffer_allocator`s have their memory locality set to the graphics device RAM. An additional buffer is created pro-bono on `create_*` calls for staging data from the GPU or reading back data from the GPU.

This ensures that when resources are used on draw calls, the data is not in a state that the GPU will cause cache flushing or stalls because a buffer region is dirty. Additionally, this adds a design thinking that updating any kind of resource on the GPU is expensive and should be done at the same time, as much as possible (see `gx::queue`'s `transfer` method).

#### Queue task scheduling

Some graphics API (Vulkan) uses an explicit type (`vkSemaphore`, `vkFence`) to denote GPU or CPU dependencies, while other graphics API (Direct3D 12, Metal) uses an event handling procedure to denote CPU dependencies, and an explicit type (`MTLEvent`, `ID3D12Fence`) for GPU dependencies with the later also applicable for CPU dependencies.

This makes it hard to reason execution graph without making GPU command submission in-order and deterministic. Therefore, on `gx::queue`'s `begin` method, the queue explicitly CPU-waits for its own execution completion before giving control back to the application.

In addition, a `dependencies` parameter exists in `begin` to let the queue know which `queue`s it will CPU-wait on. This might change in the future to have a flag whether it will CPU wait for GPU wait.

#### Resource binding

Resource binding of vulkan, d3d12, and metal are distantly similar. Direct3D and Vulkan has a concept of descriptor tables and descriptor heaps wherein any resource bound to the pipeline will be slotted into those descriptor information, while in Metal direct references to `MTLBuffer`s or `MTLTexture`s are used.

To alleviate this, an opaque object `gx::resource_reference` is used for getting a handle from a `buffer`. The actual implementation for `resource_reference`s varies from one API to another. In Direct3D, it contains the descriptor handle and a reference to its original state for resource barrier transition. In Metal, this just contains a `__weak` reference to a `MTLBuffer` or `MTLTexture`.

#### Shader authoring

`gx` will consistently use HLSL as its shading language by default, when the need arise, platform specific shading code will be used and be exported as a c/c++ header file containing the compiled DXIL, SPIR, or MetalIR code.
