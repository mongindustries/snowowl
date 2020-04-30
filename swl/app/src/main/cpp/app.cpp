#include <iostream>
#include <sstream>
#include <chrono>

#include <game_loop.hpp>
#include <file_manager.hpp>
#include <application.hpp>

#if defined(SWL_WIN32)
#include <directx/context.h>
#include <directx/swap_chain.h>
#elif defined(SWL_DARWIN)
#include <metal/mtl_context.h>
#include <metal/mtl_swap_chain.h>
#endif

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

  AppGameLoop    (const cx::exp::ptr_ref<ui::window>& window): game_loop(60, 4),
    window      (window),
#if defined(SWL_WIN32)
    context     (new gx::dx::context()),
#elif defined(SWL_DARWIN)
    context     (new gx::mtl::mtl_context()),
#endif
    main_queue  (context->create_queue()),
    swap_chain  (context->create_swap_chain(window, cx::exp::ptr_ref{ main_queue })),
    clear_block (main_queue->create_render_block()) {

    window->bind_loop(cx::exp::ptr_ref<game_loop>{ this });


  }

  void create    () override {
  }

  void update    (std::chrono::milliseconds delta) override {
  }

  void render    (float offset) override {

    main_queue->begin       ({ });

    auto frame = swap_chain->next_frame();

    swap_chain->present     ();

    main_queue->submit      ({ cx::exp::ptr_ref{ clear_block } });
  }
};

struct App: application {

  exp::ptr<window>       window;
  exp::ptr<AppGameLoop>  gameLoop;

  App(void* instance): application(instance), window(nullptr), gameLoop(nullptr) {
  }

  void on_create  () override {

    const auto pp = cx::file_manager::resourcePath;

    window = create_window("[SnowOwl:] App", rect{ {100, 100}, {800, 480} });

    gameLoop = exp::ptr<AppGameLoop>{ new AppGameLoop(cx::exp::ptr_ref{ window }) };
    gameLoop->open();

    window->event_on_close.emplace_back([&](const ui::window&) {
      gameLoop->close();
    });
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
