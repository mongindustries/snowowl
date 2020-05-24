#include <chrono>
#include <cmath>
#include <sstream>

#include <application.hpp>
#include <game_loop.hpp>

#include <factory.hpp>

#include <swap_chain.hpp>
#include <tell.hpp>

using namespace swl;
using namespace cx;
using namespace ui;

using namespace std::chrono;

template < typename c >
using ptr = exp::ptr < c >;

#if defined(SWL_WIN32)
#define _USE_MATH_DEFINES
#include <directx/factory.h>

namespace shaders::simple {

#define CPP_SHADER

#include <simple-frag.shader>
#include <simple-vert.shader>

#include "hlsl/common.h"
}

gx::factory < gx::dx::context >         factory{gx::dx::context()};
typedef gx::factory < gx::dx::context > Factory;

#elif defined(SWL_DARWIN)
#include <metal/mtl_context.h>
#endif

struct app_game_loop final : game_loop {

  window& window;

  Factory::t_queue           main_queue;
  Factory::t_swap_chain      swap_chain;
  Factory::t_render_block    clear_block;
  Factory::t_render_pipeline render_pipeline;

  ptr < gx::buffer_allocator > allocator;

  ptr < gx::buffer < gx::typeData > > buffer_vertex;
  ptr < gx::buffer < gx::typeData > > buffer_index;

  float peg{0};

  app_game_loop(ui::window &window)
    : game_loop(60, 4)
    , window(window)
    , main_queue(factory.queue())
    , swap_chain(factory.swap_chain(main_queue, window))
    , clear_block(factory.render_block(main_queue, nullptr))
    , allocator(factory.buffer_allocator((sizeof(float) * 12 + sizeof(uint16_t) * 6) * 2))
    , buffer_vertex(nullptr)
    , buffer_index(nullptr) {

    buffer_vertex = allocator->create_data < shaders::simple::vertex_input, 4 >(gx::dataUsagePrivate, gx::viewTypeShader, 0);
    buffer_index  = allocator->create_data < uint32_t, 6 >(gx::dataUsagePrivate, gx::viewTypeShader, sizeof(shaders::simple::vertex_input) * 4);

    render_pipeline = cx::tell < Factory::t_render_pipeline >(factory.render_pipeline(), [&](Factory::t_render_pipeline &pipeline) {
      pipeline.topology_type = gx::pipeline::topologyTypeTriangle;

      pipeline.raster.cull_mode                = gx::pipeline::modeNone;
      pipeline.raster.fill_mode                = gx::pipeline::modeFill;
      pipeline.raster.render_counter_clockwise = false;

      const unsigned char *vert_prog = shaders::simple::vert_prog;
      const unsigned char *frag_prog = shaders::simple::frag_prog;

      pipeline.shader_stages[gx::pipeline::shader_stage::vertex] = gx::shader{
        .byte_code = (char*) vert_prog, .byte_size = sizeof(shaders::simple::vert_prog) };

      pipeline.shader_stages[gx::pipeline::shader_stage::fragment] = gx::shader{
        .byte_code = (char*) frag_prog, .byte_size = sizeof(shaders::simple::frag_prog) };

      pipeline.render_outputs[0].format        = gx::pipeline::format_4_8_int_u_norm_flipped;
      pipeline.render_outputs[0].blend.enabled = false;

      pipeline.render_inputs[gx::pipeline::shader_stage::vertex] = cx::tell < gx::pipeline::render_input >({}, [](auto &object) {
        // vertex
        object.bindings[0] = cx::tell < gx::pipeline::render_input_item >({}, [](auto &object) {
          object.format   = gx::pipeline::format_4_32_float;
          object.indirect = false;
          object.type     = gx::pipeline::typeBuffer;
        });

        // index
        object.bindings[1] = cx::tell < gx::pipeline::render_input_item >({}, [](auto &object) {
          object.format   = gx::pipeline::format_1_16_int_u;
          object.indirect = false;
          object.type     = gx::pipeline::typeBuffer;
        });
      });

      pipeline.construct();
    });
  }

  void
    create() override {

    main_queue.begin({});

    std::vector < shaders::simple::vertex_input > data = {
        shaders::simple::vertex_input{ { -0.5,  0.5, 0.5, 1 }, { 1, 1, 1, 1 } },
        shaders::simple::vertex_input{ {  0.5,  0.5, 0.5, 1 }, { 1, 1, 1, 1 } },
        shaders::simple::vertex_input{ {  0.5, -0.5, 0.5, 1 }, { 0, 1, 1, 1 } },
        shaders::simple::vertex_input{ { -0.5, -0.5, 0.5, 1 }, { 1, 0, 1, 1 } },
    };

    std::vector < uint32_t > indices = {
        0, 1, 2, 0, 2, 3
    };

    const auto v_staging = buffer_vertex->set_data(0, data);
    const auto i_staging = buffer_index->set_data(0, indices);

    main_queue.transfer({ exp::ptr_ref{v_staging}, exp::ptr_ref{i_staging} });
  }

  void
    update(milliseconds delta) override { peg += 1; }

  void
    render(float offset) override {

    main_queue.begin({});
    {
      const gx::swap_chain_boundary frame_block{swap_chain};
      {
        gx::block_boundary block{clear_block, render_pipeline};
        {
          auto &frame         = static_cast < gx::swap_chain::frame & >(frame_block);
          auto  frame_context = cx::tell < gx::render_pass_context >({}, [&](auto &object) {
            float rad = peg * 0.01745329f;

            float r = sin(rad);
            float g = sin(rad - (2.0f / 3.0f) * M_PI);
            float b = sin(rad - (4.0f / 3.0f) * M_PI);

            object.action_load  = gx::loadOpClear;
            object.action_store = gx::storeOpStore;
            object.load_clear   = std::array < float, 4 >{r, g, b, 1.0f};

            object.transition_before = gx::transitionInherit;
            object.transition_during = gx::transitionRenderTargetView;
            object.transition_after  = gx::transitionInherit;

            object.reference = frame.reference;
          });

          auto pass = factory.render_pass(clear_block, {frame_context});
          {
            const auto v_ref = std::make_pair(buffer_vertex->reference(),
                                              gx::buffer_transition{gx::pipeline::shader_stage::vertex, gx::buffer_transition::transitionShaderView});
            const auto i_ref = std::make_pair(buffer_index->reference(),
                                              gx::buffer_transition{gx::pipeline::shader_stage::vertex, gx::buffer_transition::transitionShaderView});

            const auto size = swap_chain.size();

            gx::buffer_usage_block resource_block{pass, {v_ref, i_ref}};
            {
              pass.bind_buffers(gx::bindingGraphicsVertex, {get < 0 >(v_ref), get < 0 >(i_ref)});

              pass.set_viewport(size);
              pass.set_scissor(cx::rect{{}, size});

              pass.set_topology(gx::render_pass::typeTriangleList);
              pass.draw(gx::render_pass_draw_range{0, 6});
            }
          }
        }
      }

      main_queue.submit({clear_block});
    }
  }
};

struct app final : application {

  explicit
    app(void *instance)
    : application(instance) { }

  void
    on_create() override {

    auto &window = main_window().get();
    {
      window.game_loop(exp::ptr<game_loop, app_game_loop>{new app_game_loop(window)}.abstract());

      auto &loop = window.game_loop();

      loop.frame_callback = [&window](auto fps) {
        window.title((std::stringstream() << "[SnowOwl: | " << fps << "FPS] App").str());
      };

      loop.open();
    }
  }

  void
    on_destroy() override { 
    }
};

#if defined(SWL_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int
  __stdcall wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR cmdArgs, int cmdShow) {
#elif defined(SWL_DARWIN)
int
  main() {

  std::nullptr_t instance = nullptr; // implied [NSApplication sharedApplication];
#endif

  return application::run(app(instance));
}
