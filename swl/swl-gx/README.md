## SnowOwl: graphics

Defines the rendering abstraction infrastructure.

### Contents

All objects are contained inside the `swl::gx` namespace.

#### Core Components

- `gx::factory`
     Factory to create rendering objects given a rendering context (see below)
- `gx::context`
     The graphics context. This creates the instance and device for a given rendering API.
- `gx::queue`
     A wrapper for graphics submission operations.
- `gx::render_block`
     A wrapper for a particular graphics state.
- `gx::render_pass`
     A wrapper for graphics command execution.
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
     A reference made from a `gx::buffer<>` used in conjunction with `gx::render_pipeline` to define shader resource and barrier transition information.
- `gx::render_pipeline`
     A wrapper for a particular graphics configuration (eg: raster, depth, stencil, shader binding, etc.)
- `gx::shader`
     A wrapper for a shader object.

### Usage

1. Create a `gx::factory` with your intended context. Available contexts are:
    - `gx::mtl::context` A Metal API context (for Apple Platforms)
    - `gx::dx::context` A DirectX 12 API context (for Windows and Xbox Platforms)
    - `gx::vk::context` A Vulkan API context

```[C++]
#include <directx/factory.h>
gx::factory<gx::dx::context> factory{ gx::dx::context() };
```

2. Create a `gx::queue` to issue commands:

```[C++]
#include <queue.hpp>
cx::exp::ptr<gx::queue> queue{ factory.create_queue() };
```

Take note of the pointer `cx::exp::ptr<gx::queue>` usage since `create_queue` returns a polymorphic instance of `gx::queue`.

3. Create a `gx::swap_chain` to present render commands to:

```[C++]
#include <swap_chain.hpp>
cx::exp::ptr<gx::swap_chain> swap_chain{ factory.create_swap_chain(queue, window) };
```
A target window is required. You can create a window by calling any of the constructor of `ui::window` or get the default window of an application by calling `application::get_main_window()`.

4. Begin queue execution block:

```[C++]
queue->begin({ }); // Parameter in begin is a list of queues this queue will wait before beggining

queue->submit({ }); // Parameter in submit is a list of render blocks to execute
```

5. To use the swap chain, use a `gx::swap_chain_boundary`:

```[C++]
queue->begin({ }); {
    gx::swap_chain_boundary cur_frame{ cx::exp::ptr_ref{ swap_chain } }; {

    } 
    queue->submit({ });
}
```

Take note of the execution order of `gx::swap_chain_boundary`. It **must** be after a queue's `begin` call but `gx::swap_chain_boundary`'s scope must be ended before a `submit` call is executed. This is so that the queue can wait for GPU execution completion before obtaining the next frame.

6. Create a `gx::render_pipeline`:

7. Begin command submission to a `gx::queue` by creating a `gx::render_block`:

```[C++]
#include <render_block.hpp>
cx::exp::ptr<gx::render_block> block{ factory.create_render_block(queue) };
```
8. Then create a `gx::block_boundary` to begin block execution:

```[C++]
queue->begin({ }); {
    gx::swap_chain_boundary cur_frame{ cx::exp::ptr_ref{ swap_chain } }; {
        gx::block_boundary exec{ cx::exp::ptr_ref{ block }, render_pipeline }; {

        }
    } 
    queue->submit({ cx::exp::ptr_ref{ block } }); // Add the block here for execution!
}
```

9. And then create a `gx::render_pass` to bind input/output objects:

_TBD..._
