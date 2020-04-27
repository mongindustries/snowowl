#pragma once

#include <header.hpp>

#include "directx/context.h"
#include "graphics_render_block.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct render_block final : graphics_render_block {

	render_block          (const cx::exp::ptr_ref<context>& context);

	cx::exp::ptr_ref<graphics_render_pass>
		create_render_pass  () override;


	void
		commit_render_pass  () override;

	void
		compile             () override;


	winrt::com_ptr<ID3D12GraphicsCommandList4> command_list;
};

SNOW_OWL_NAMESPACE_END
