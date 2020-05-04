#pragma once

#include <header.hpp>

#include "swap_chain.hpp"

SNOW_OWL_NAMESPACE(gx::mtl)

struct swap_chain: gx::swap_chain {

  swap_chain    (gx::context& context, gx::queue& present_queue, ui::window& window);

  cx::exp::ptr_ref<gx::swap_chain::frame>
    next_frame  () override;

  void
    present     (std::vector<cx::exp::ptr_ref<gx::queue>> const& dependencies) override;


  void
    resize      (cx::size_2d const& new_size) override;

  OBJC_OBJECT(CAMetalLayer)     layer;
  cx::exp::ptr_ref<mtl::queue>  present_queue;

  uint64_t                      current_frame;

private:

  struct mtl_frame: frame {
    OBJC_ID(CAMetalDrawable) drawable;
  };
};

SNOW_OWL_NAMESPACE_END
