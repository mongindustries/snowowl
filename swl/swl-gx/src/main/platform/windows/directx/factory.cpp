#include "directx/factory.h"
#include "directx/swap_chain.h"
#include "directx/queue.h"
#include "directx/render_block.h"


SNOW_OWL_NAMESPACE(gx)

cx::exp::ptr<queue>
  factory<dx::context, 0>::queue            () {
  return cx::exp::ptr<gx::queue>{ new dx::queue(instance) };
}

cx::exp::ptr<swap_chain>
  factory<dx::context, 0>::swap_chain       (gx::queue& queue, ui::window& window) {
  return cx::exp::ptr<gx::swap_chain>{ new dx::swap_chain(instance, dynamic_cast<dx::queue&>(queue), window) };
}

cx::exp::ptr<render_block>
  factory<dx::context, 0>::render_block     (gx::queue& queue, gx::render_pipeline& pipeline) {
  return cx::exp::ptr<gx::render_block>{ new dx::render_block(dynamic_cast<dx::queue&>(queue), dynamic_cast<dx::render_pipeline&>(pipeline)) };
}

cx::exp::ptr<gx::render_pass>
  factory<dx::context, 0>::render_pass      (gx::render_block& block, std::vector<gx::render_pass_context> const& pass_context) {
  return cx::exp::ptr<gx::render_pass>{ new dx::render_pass(dynamic_cast<dx::render_block&>(block), pass_context) };
}

cx::exp::ptr<render_pipeline>
  factory<dx::context, 0>::render_pipeline  () {
  return cx::exp::ptr<gx::render_pipeline>{ new dx::render_pipeline(dynamic_cast<dx::context&>(instance)) };
}

[[nodiscard]] cx::exp::ptr<buffer_allocator>
  factory<dx::context, 0>::buffer_allocator (buffer_allocator_usage usage) {
  return cx::exp::ptr{ nullptr };
}

SNOW_OWL_NAMESPACE_END
