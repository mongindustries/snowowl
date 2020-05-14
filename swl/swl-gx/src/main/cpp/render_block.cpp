#include "queue.hpp"

#include "render_block.hpp"
#include "render_pass.hpp"

#include "render_pipeline.hpp"

SNOW_OWL_NAMESPACE(gx)

render_block::render_block      () noexcept = default;

render_block::render_block      (render_block &&) noexcept = default;

render_block::render_block      (queue& queue, render_pipeline* pipeline) {

}

render_block::~render_block     () = default;


render_block&
  render_block::operator=       (render_block &&) noexcept = default;


block_boundary::block_boundary  (render_block& block, render_pipeline& pipeline): block(&block) {
  block.reset(pipeline);
}

block_boundary::~block_boundary () {
  block.get().close();
}


void
  render_block::reset  (render_pipeline& pipeline) { }

void
  render_block::close  () { }

SNOW_OWL_NAMESPACE_END
