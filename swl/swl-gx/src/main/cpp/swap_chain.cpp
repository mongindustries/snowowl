//
// Created by Michael Ong on 25/4/20.
//
#include "swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

swap_chain::swap_chain            (swap_chain&& move) noexcept : window(move.window), frames(std::move(move.frames)) { }

swap_chain &swap_chain::operator= (swap_chain&& move) noexcept {
  window = move.window;
  frames = std::move(move.frames);
}


swap_chain::frame::frame                        (frame&& move) noexcept : index(move.index), reference(std::move(move.reference)), swap_chain(move.swap_chain) {
  move.swap_chain = ptr_ref<gx::swap_chain>{ nullptr };
  index = 0;
}

swap_chain::frame &swap_chain::frame::operator= (frame&& move) noexcept {
  index = move.index;
  swap_chain = move.swap_chain;
  reference = std::move(move.reference);

  move.swap_chain = ptr_ref<gx::swap_chain>{ nullptr };
  index = 0;

  return *this;
}


swap_chain::swap_chain    () noexcept: window(nullptr), frames() { }

swap_chain::~swap_chain   () = default;


swap_chain::swap_chain    (context& context, queue& present_queue, ui::window& window) : window(&window) { }


swap_chain::frame::frame  () noexcept: index(0), reference(nullptr) {
}

swap_chain::frame::~frame () = default;


swap_chain_scope::swap_chain_scope  (gx::swap_chain& swap_chain) : swap_chain(&swap_chain), flighting_frame(swap_chain.next_frame()) { }

swap_chain_scope::~swap_chain_scope () { swap_chain->present(); }



void
  swap_chain::resize      (const cx::size_2d &new_size) { }

cx::size_2d
  swap_chain::size        () const {
  return cx::size_2d();
}

ptr_ref<swap_chain::frame>
  swap_chain::next_frame  () { return ptr_ref<frame>{frames[0]}; }

void
  swap_chain::present     () { }


swap_chain_scope::operator
  swap_chain::frame&      () const { return flighting_frame.get(); }

SNOW_OWL_NAMESPACE_END
