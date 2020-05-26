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

  Factory::Queue            main_queue;
  Factory::SwapChain        swap_chain;
  Factory::RenderBlock      clear_block;
  Factory::RenderPipeline   render_pipeline;

  Factory::BufferAllocator  allocator;

  ptr < gx::buffer < gx::pipeline::typeData > > buffer_vertex;
  ptr < gx::buffer < gx::pipeline::typeData > > buffer_index;

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

    buffer_vertex = allocator.create_data< shaders::simple::vertex_input, 4 >
      (gx::pipeline::dataUsagePrivate, 0 );
    buffer_index  = allocator.create_data< uint32_t, 6 >
      (gx::pipeline::dataUsagePrivate, sizeof(shaders::simple::vertex_input) * 4);

    render_pipeline = cx::tell < Factory::RenderPipeline >(factory.render_pipeline(), [&](Factory::RenderPipeline &pipeline) {
      pipeline.topology_type = gx::pipeline::topologyTypeTriangle;

      pipeline.raster.cull_mode                = gx::pipeline::modeNone;
      pipeline.raster.fill_mode                = gx::pipeline::modeFill;
      pipeline.raster.render_counter_clockwise = false;

      pipeline.depth.format   = gx::pipeline::format_unknown;
      pipeline.depth.enabled  = false;

      pipeline.stages[gx::pipeline::shader_stage::vertex] = cx::tell<gx::pipeline::pipeline_stage>({}, [&](gx::pipeline::pipeline_stage &stage) {

        stage.function = { (char *)shaders::simple::vert_prog, sizeof shaders::simple::vert_prog };

        stage.input.arguments[0] = cx::tell < gx::pipeline::shader_argument >({}, [](auto &object) {
          object.format   = gx::pipeline::format_4_32_float;
          object.type     = gx::pipeline::typeBuffer;
        });

        stage.input.arguments[1] = cx::tell < gx::pipeline::shader_argument >({}, [](auto &object) {
          object.format   = gx::pipeline::format_1_16_int_u;
          object.type     = gx::pipeline::typeBuffer;
        });
      });

      pipeline.stages[gx::pipeline::shader_stage::fragment] = cx::tell<gx::pipeline::pipeline_stage>({}, [&](gx::pipeline::pipeline_stage &stage) {

        stage.function = { (char *)shaders::simple::frag_prog, sizeof shaders::simple::frag_prog };

        stage.output[0].format        = gx::pipeline::format_4_8_int_u_norm_flipped;
        stage.output[0].blend.enabled = false;
      });

      pipeline.construct();
    });
  }

  void
    create() override {
  }

  void
    update(milliseconds delta) override { peg += 1; }

  void
    render(float offset) override {

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

    const auto v_staging = buffer_vertex->set_data(
    0, data, { gx::pipeline::shader_stage::vertex, gx::pipeline::usageTypeShader });
    const auto i_staging = buffer_index ->set_data(
    0, indices, { gx::pipeline::shader_stage::vertex, gx::pipeline::usageTypeShader });

    main_queue.transfer({ exp::ptr_ref{v_staging}, exp::ptr_ref{i_staging} });

    main_queue.begin({});
    {
      const gx::swap_chain_scope frame_block{swap_chain};
      {
        gx::render_block_scope block{clear_block, render_pipeline};
        {
          auto &frame       = static_cast < gx::swap_chain::frame & >(frame_block);
          auto  swap_output = cx::tell < gx::pipeline::pass_output >({}, [&](auto &object) {

            const auto size = swap_chain.size();

            float rad = peg * 0.01745329f;

            float r   = sin(rad);
            float g   = sin(rad - (2.0f / 3.0f) * M_PI);
            float b   = sin(rad - (4.0f / 3.0f) * M_PI);

            object.action_load  = gx::pipeline::loadOpClear;
            object.action_store = gx::pipeline::storeOpStore;
            object.clear_color  = std::array < float, 4 >{r, g, b, 1.0f};

            object.viewport     = size;
            object.scissor      = cx::rect{ {}, size }; 

            object.output       = frame.reference;
          });

          auto pass = factory.render_pass(clear_block, {swap_output});
          {
            const auto v_ref = buffer_vertex->reference(
              cx::exp::ptr_ref<gx::render_pipeline>{ &render_pipeline }, gx::pipeline::shader_stage::vertex, 0);

            const auto i_ref = buffer_index->reference(
              cx::exp::ptr_ref<gx::render_pipeline>{ &render_pipeline }, gx::pipeline::shader_stage::vertex, 1);

            const auto v_pass = gx::pipeline::pass_input{ v_ref, gx::pipeline::shader_stage::vertex, gx::pipeline::transitionTypeShaderView };
            const auto i_pass = gx::pipeline::pass_input{ i_ref, gx::pipeline::shader_stage::vertex, gx::pipeline::transitionTypeShaderView };

            auto resource_block = pass.buffer_prepare({ v_pass, i_pass }); {

              pass.buffer_bind(gx::pipeline::shader_stage::vertex, 0, v_ref);
              pass.buffer_bind(gx::pipeline::shader_stage::vertex, 1, i_ref);

              pass.topology   (gx::render_pass::typeTriangleList);
              pass.draw       (gx::pipeline::draw_range{0, 6});
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
