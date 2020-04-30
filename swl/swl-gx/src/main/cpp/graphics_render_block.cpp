#include "graphics_render_block.hpp"
#include "graphics_render_pass.hpp"

#include "graphics_render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

graphics_render_block::graphics_render_block  (cx::exp::ptr_ref<graphics_context> const& context, cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline) {

}


void  graphics_render_block::reset            (cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline) {
}

void  graphics_render_block::close            () {

}


void  graphics_render_block::render_pass      (const std::function<void(graphics_render_pass&)>& configure) {
  graphics_render_pass pass;
  configure(pass);
}

SNOW_OWL_NAMESPACE_END
