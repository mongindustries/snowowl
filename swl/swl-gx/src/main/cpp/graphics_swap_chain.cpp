//
// Created by Michael Ong on 25/4/20.
//
#include "graphics_swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

graphics_swap_chain::graphics_swap_chain
  ( ptr_ref<graphics_context> const&  context,
    ptr_ref<graphics_queue> const&    present_queue,
    ptr_ref<ui::window> const&        window) : window(window) { }

cx::exp::ptr_ref<graphics_swap_chain::frame>
      graphics_swap_chain::next_frame () { return cx::exp::ptr_ref<frame> { nullptr }; }

void  graphics_swap_chain::present    (std::vector<ptr_ref<graphics_queue>> const&) { }


void  graphics_swap_chain::resize     (cx::size_2d const&) { }


swap_chain_boundary::swap_chain_boundary            (ptr_ref<graphics_swap_chain> const& swap_chain) :
  swap_chain(swap_chain), flighting_frame(swap_chain->next_frame()) { }

swap_chain_boundary::~swap_chain_boundary           () {
  swap_chain->present(dependencies);
}

swap_chain_boundary::operator
      cx::exp::ptr_ref<graphics_swap_chain::frame>  () {

  return flighting_frame;
}

SNOW_OWL_NAMESPACE_END
