//
// Created by Michael Ong on 27/4/20.
//
#pragma once

#include <header.hpp>
#include "graphics_render_block.hpp"

#include "metal/mtl_context.h"
#include "metal/mtl_queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_render_block: graphics_render_block {

	mtl_render_block    (const cx::exp::ptr_ref<mtl_queue>& queue);


	void  render_pass   (const std::function<void(graphics_render_pass&)> &configure) override;


	void  begin         () override;

	void  end           () override;


	cx::exp::ptr_ref<mtl_queue> queue;
};

SNOW_OWL_NAMESPACE_END
