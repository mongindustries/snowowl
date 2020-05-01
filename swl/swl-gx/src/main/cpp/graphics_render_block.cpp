#include "graphics_render_block.hpp"
#include "graphics_render_pass.hpp"

#include "graphics_render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

graphics_render_block::graphics_render_block  (cx::exp::ptr_ref<graphics_queue> const& context, cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline) {

}


void  graphics_render_block::reset            (cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline) {
}

void  graphics_render_block::close            () {

}

void graphics_render_block::render_pass       (const std::vector<graphics_render_pass_context>& context, const std::function<void(graphics_render_pass&)>& configure) {

  graphics_render_pass pass;
  configure(pass);
}

block_boundary::block_boundary(cx::exp::ptr_ref<graphics_render_block> const& block, cx::exp::ptr_ref<graphics_render_pipeline> const& pipeline): block(block) {

  block->reset(pipeline);
}

block_boundary::~block_boundary() {

  block->close();
}

SNOW_OWL_NAMESPACE_END
