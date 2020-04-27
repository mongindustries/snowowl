#include "graphics_render_block.hpp"
#include "graphics_render_pass.hpp"

SNOW_OWL_NAMESPACE(gx)

void  graphics_render_block::render_pass  (const std::function<void(graphics_render_pass&)>& configure) {
	graphics_render_pass pass;
	configure(pass);
}

void  graphics_render_block::begin        () { }

void  graphics_render_block::end          () { }

SNOW_OWL_NAMESPACE_END
