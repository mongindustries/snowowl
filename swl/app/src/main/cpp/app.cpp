#include <iostream>
#include <sstream>
#include <chrono>
#include <cmath>

#include <game_loop.hpp>
#include <file_manager.hpp>
#include <application.hpp>

#if defined(SWL_WIN32)
#define _USE_MATH_DEFINES
#include <directx/context.h>
#elif defined(SWL_DARWIN)
#include <metal/mtl_context.h>
#endif

#include <graphics_swap_chain.hpp>
#include <graphics_render_block.hpp>

using namespace swl;
using namespace cx;
using namespace ui;

using namespace std::chrono;

template<typename c>
using ptr = cx::exp::ptr<c>;

struct AppGameLoop: game_loop {

  cx::exp::ptr_ref<window>        window;

  ptr<gx::graphics_context>       context;

  ptr<gx::graphics_queue>         main_queue;
  ptr<gx::graphics_swap_chain>    swap_chain;
  ptr<gx::graphics_render_block>  clear_block;

  float                           peg{0};

  AppGameLoop    (const cx::exp::ptr_ref<ui::window>& window): game_loop(60, 4),
    window      (window),
#if defined(SWL_WIN32)
    context     (new gx::dx::context()),
#elif defined(SWL_DARWIN)
    context     (new gx::mtl::mtl_context()),
#endif
    main_queue  (context->create_queue()),
    swap_chain  (context->create_swap_chain(window, cx::exp::ptr_ref{ main_queue })),
    clear_block (main_queue->create_render_block(cx::exp::ptr_ref<gx::graphics_render_pipeline>{ nullptr })) {

    window->bind_loop(cx::exp::ptr_ref<game_loop>{ this });
  }

  void create    () override {
  }

  void update    (std::chrono::milliseconds delta) override {
    peg += 1;
  }

  void render    (float offset) override {
    main_queue->begin({ }); {
      gx::swap_chain_boundary swap_chain{ cx::exp::ptr_ref{ this->swap_chain } }; {
        gx::block_boundary block{ cx::exp::ptr_ref{ clear_block }, cx::exp::ptr_ref<gx::graphics_render_pipeline>{ nullptr } }; {

          cx::exp::ptr_ref<gx::graphics_swap_chain::frame>  frame = swap_chain;
          gx::graphics_render_pass_context                  frame_context;

          frame_context.action_load       = gx::loadClear;
          frame_context.action_store      = gx::store;

          float rad = peg * 0.01745329f;

          float r = sin(rad);
          float g = sin(rad - (2.0f / 3.0f) * M_PI);
          float b = sin(rad - (4.0f / 3.0f) * M_PI);

          frame_context.load_clear        = std::array<float, 4>{ r, g, b, 0.75f };

          frame_context.transition_before = gx::transitionInherit;
          frame_context.transition_during = gx::transitionRenderTargetView;
          frame_context.transition_after  = gx::transitionInherit;

          frame_context.reference         = cx::exp::ptr_ref{ frame->reference };

          clear_block->render_pass(std::vector{ frame_context }, [](gx::graphics_render_pass& pass) {
          });
        }
      }
      main_queue->submit(std::vector{ cx::exp::ptr_ref{ clear_block } });
    }
  }
};

struct App: application {

  exp::ptr<window>       window;
  exp::ptr<AppGameLoop>  gameLoop;

  App(void* instance): application(instance), window(nullptr), gameLoop(nullptr) {
  }

  void on_create  () override {

    window = create_window("[SnowOwl:] App", rect{ {100, 100}, {800, 480} });

    gameLoop = exp::ptr<AppGameLoop>{ new AppGameLoop(cx::exp::ptr_ref{ window }) };
    gameLoop->frame_callback = [&](auto fps) {
      window->set_title((std::stringstream() << "[SnowOwl: | " << fps << "FPS] App").str());
    };

    gameLoop->open();
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

  return application::run(App(instance));
}
