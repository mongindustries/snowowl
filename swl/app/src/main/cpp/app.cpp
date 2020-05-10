#include <chrono>
#include <cmath>
#include <sstream>

#include <application.hpp>
#include <game_loop.hpp>

#include <factory.hpp>

#if defined(SWL_WIN32)
#define _USE_MATH_DEFINES
#include <directx/factory.h>
#elif defined(SWL_DARWIN)
#include <metal/mtl_context.h>
#endif

#include <swap_chain.hpp>
#include <tell.hpp>

using namespace swl;
using namespace cx;
using namespace ui;

using namespace std::chrono;

template<typename c>
using ptr = cx::exp::ptr<c>;

gx::factory<gx::dx::context> factory{ gx::dx::context() };

struct app_game_loop final : game_loop {

  exp::ptr_ref<window>          window;

  ptr<gx::queue>                main_queue;
  ptr<gx::swap_chain>           swap_chain;
  ptr<gx::render_block>         clear_block;
  ptr<gx::render_pipeline>      render_pipeline;

  ptr<gx::buffer_allocator>     allocator;

  ptr<gx::buffer<gx::typeData>> buffer_vertex;
  ptr<gx::buffer<gx::typeData>> buffer_index;

  float                   peg{0};

  app_game_loop    (ui::window& window): game_loop(60, 4),
    window      (&window),

    allocator   (factory.buffer_allocator(gx::usageShared)),

    main_queue  (factory.queue        ()),
    swap_chain  (factory.swap_chain   (main_queue, window)),
    clear_block (factory.render_block (main_queue, cx::exp::ptr_ref<gx::render_pipeline>{ nullptr })) {

    buffer_vertex   = allocator->create_data(sizeof(float) * 12);
    buffer_index    = allocator->create_data(sizeof(uint16_t) * 6);

    render_pipeline = cx::tell<ptr<gx::render_pipeline>>(factory.render_pipeline(), [&](ptr<gx::render_pipeline>& pipeline) {

      pipeline->topology_type                     = gx::pipeline::topologyTypeTriangle;

      pipeline->raster.cull_mode                  = gx::pipeline::modeBack;
      pipeline->raster.fill_mode                  = gx::pipeline::modeFill;
      pipeline->raster.render_counter_clockwise   = false;

      pipeline->render_outputs[0].format          = gx::pipeline::format_4_8_int_u_norm_flipped;
      pipeline->render_outputs[0].blend.enabled   = false;

      pipeline->shader_stages[gx::pipeline::shader_stage::vertex]   = gx::shader{ };
      pipeline->shader_stages[gx::pipeline::shader_stage::fragment] = gx::shader{ };

      pipeline->render_inputs[gx::pipeline::shader_stage::vertex]   = TELL_O(gx::pipeline::render_input, {

        // vertex
        object.resource_binding.emplace_back(TELL_O(gx::pipeline::render_input_item, {
          object.format   = gx::pipeline::format_4_32_float;
          object.location = 0;
          object.region   = 0;
          object.count    = 0;
          object.type     = gx::pipeline::typeBuffer;
        }));

        // index
        object.resource_binding.emplace_back(TELL_O(gx::pipeline::render_input_item, {
          object.format   = gx::pipeline::format_1_16_int_u;
          object.location = 1;
          object.region   = 0;
          object.count    = 0;
          object.type     = gx::pipeline::typeBuffer;
        }));

        // matrices
        object.resource_binding.emplace_back(TELL_O(gx::pipeline::render_input_item, {
          object.format   = gx::pipeline::format_unknown;
          object.location = 2;
          object.region   = 0;
          object.count    = 0;
          object.type     = gx::pipeline::typeConstant;
        }));
      });

      pipeline->construct();
    });

    main_queue->begin({});

    const std::vector<float>    data = {
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, };

    const std::vector<uint16_t> idxs = {
      0, 1, 2, 0, 2, 3 };

    const auto v_staging = buffer_vertex->update_data(0, data);
    const auto i_staging = buffer_index ->update_data(0, idxs);

    main_queue->transfer({exp::ptr_ref{v_staging}, exp::ptr_ref{i_staging}});

    window.bind_loop(cx::exp::ptr_ref<game_loop>{this});
  }

  void create    () override {
  }

  void update    (std::chrono::milliseconds delta) override {
    peg += 1;
  }

  void render    (float offset) override {
    main_queue->begin({ }); {
      const gx::swap_chain_boundary frame_block{swap_chain}; {
        gx::block_boundary block{clear_block, render_pipeline}; {

          auto&                   frame = static_cast<gx::swap_chain::frame&>(frame_block);
          gx::render_pass_context frame_context;

          float rad = peg * 0.01745329f;

          float r   = sin(rad);
          float g   = sin(rad - (2.0f / 3.0f) * M_PI);
          float b   = sin(rad - (4.0f / 3.0f) * M_PI);

          frame_context.action_load       = gx::loadOpClear;
          frame_context.action_store      = gx::storeOpStore;

          frame_context.load_clear        = std::array<float, 4>{r, g, b, 1.0f};

          frame_context.transition_before = gx::transitionInherit;
          frame_context.transition_during = gx::transitionRenderTargetView;
          frame_context.transition_after  = gx::transitionInherit;

          frame_context.reference         = frame.reference;

          auto pass = factory.render_pass(clear_block, std::vector{frame_context}); {
            pass->bind_buffers(gx::bindingGraphicsVertex,   {
              buffer_vertex-> reference(0),
              buffer_index->  reference(0) }); // three items: vertex, index, matrix

            const auto size = swap_chain->window->get_size();
            pass->set_viewport  (size);
            pass->set_scissor   (cx::rect{{}, size});

            pass->set_topology  ();
            pass->draw          (gx::render_pass_draw_range{ 0, 3 });

            buffer_vertex-> dereference();
            buffer_index->  dereference();
          }
        }
      }
      main_queue->submit(std::vector{exp::ptr_ref{clear_block}});
    }
  }
};

struct app final : application {

  exp::ptr_ref<ui::window>    window;
  exp::ptr<app_game_loop>     game_loop;

  explicit app    (void* instance): application(instance), window(nullptr), game_loop(nullptr) { }

  void on_create  () override {

    window     = get_main_window(); {

      game_loop                 = exp::make_ptr<app_game_loop>(window);
      game_loop->frame_callback = [&](auto fps) {
        window->set_title((std::stringstream() << "[SnowOwl: | " << fps << "FPS] App").str());
      };

      game_loop->open();
    }
  }

  void on_destroy () override {
  }
};

#if defined(SWL_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR cmdArgs, int cmdShow) {
#elif defined(SWL_DARWIN)
int main () {
  std::nullptr_t instance = nullptr; // implied [NSApplication sharedApplication];
#endif

  return application::run(app(instance));
}
