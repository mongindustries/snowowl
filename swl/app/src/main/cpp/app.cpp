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

struct console_game_loop final : game_loop {

  exp::ptr_ref<window>          window;

  ptr<gx::queue>                main_queue;
  ptr<gx::swap_chain>           swap_chain;
  ptr<gx::render_block>         clear_block;

  console_game_loop(ui::window& window) : game_loop(60, 4),
    window(&window),

    main_queue(factory.queue()),
    swap_chain(factory.swap_chain(main_queue, window)),
    clear_block(factory.render_block(main_queue, cx::exp::ptr_ref<gx::render_pipeline>{nullptr})) {

    window.bind_loop(cx::exp::ptr_ref<game_loop>{this});
  }

  void create() override {
  }

  void update(std::chrono::milliseconds delta) override {
  }

  void render(float offset) override {
    main_queue->begin({ }); {
      const gx::swap_chain_boundary frame_block{ cx::exp::ptr_ref{ swap_chain } }; {
        gx::block_boundary block{ clear_block, cx::exp::ptr_ref<gx::render_pipeline>{nullptr } }; {

          auto& frame = static_cast<gx::swap_chain::frame&>(frame_block);
          gx::render_pass_context frame_context;

          frame_context.action_load = gx::loadOpClear;
          frame_context.action_store = gx::storeOpStore;

          frame_context.load_clear = std::array<float, 4>{ 0.94f, 0.94f, 0.94f, 1.0f };

          frame_context.transition_before = gx::transitionInherit;
          frame_context.transition_during = gx::transitionRenderTargetView;
          frame_context.transition_after = gx::transitionInherit;

          frame_context.reference = exp::ptr_ref{ frame.reference };

          auto pass = factory.render_pass(clear_block, std::vector{ frame_context });
        }
      }
      main_queue->submit(std::vector{ exp::ptr_ref{ clear_block } });
    }
  }
};

struct app_game_loop final : game_loop {

  exp::ptr_ref<window>          window;

  ptr<gx::queue>                main_queue;
  ptr<gx::swap_chain>           swap_chain;
  ptr<gx::render_block>         clear_block;

  float                   peg{0};

  app_game_loop    (ui::window& window): game_loop(60, 4),
    window      (&window),

    main_queue  (factory.queue        ()),
    swap_chain  (factory.swap_chain   (main_queue, window)),
    clear_block (factory.render_block (main_queue, cx::exp::ptr_ref<gx::render_pipeline>{nullptr})) {

    window.bind_loop(cx::exp::ptr_ref<game_loop>{this});
  }

  void create    () override {
  }

  void update    (std::chrono::milliseconds delta) override {
    peg += 1;
  }

  void render    (float offset) override {
    main_queue->begin({ }); {
      const gx::swap_chain_boundary frame_block{ cx::exp::ptr_ref{ swap_chain } }; {
        gx::block_boundary block{ clear_block, cx::exp::ptr_ref<gx::render_pipeline>{nullptr } }; {

          auto&                   frame = static_cast<gx::swap_chain::frame&>(frame_block);
          gx::render_pass_context frame_context;

          float rad = peg * 0.01745329f;

          float r = sin(rad);
          float g = sin(rad - (2.0f / 3.0f) * M_PI);
          float b = sin(rad - (4.0f / 3.0f) * M_PI);

          frame_context.action_load       = gx::loadOpClear;
          frame_context.action_store      = gx::storeOpStore;

          frame_context.load_clear        = std::array<float, 4>{ r, g, b, 1.0f };

          frame_context.transition_before = gx::transitionInherit;
          frame_context.transition_during = gx::transitionRenderTargetView;
          frame_context.transition_after  = gx::transitionInherit;

          frame_context.reference         = exp::ptr_ref{ frame.reference };

          auto pass = factory.render_pass(clear_block, std::vector{ frame_context });
        }
      }
      main_queue->submit(std::vector{ exp::ptr_ref{ clear_block } });
    }
  }
};

struct app final : application {

  exp::ptr_ref<ui::window>    window;
  exp::ptr<ui::window>        window_aux;

  exp::ptr<app_game_loop>     game_loop;
  exp::ptr<console_game_loop> console_loop;

  explicit app    (void* instance): application(instance), window(nullptr), game_loop(nullptr), console_loop(nullptr) { }

  void on_create  () override {

    window     = get_main_window(); {
      game_loop = exp::ptr<app_game_loop>{ new app_game_loop(window) };
      game_loop->frame_callback = [&](auto fps) {
        window->set_title((std::stringstream() << "[SnowOwl: | " << fps << "FPS] App").str());
      };

      game_loop->open();
    }

    /* window_aux = cx::exp::ptr<ui::window>{ new ui::window("SnowOwl: | Console", rect{ {100, 100}, {640, 480}}) }; {
      console_loop = exp::ptr<console_game_loop>{ new console_game_loop(window_aux) };
      console_loop->open();
    } */
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
