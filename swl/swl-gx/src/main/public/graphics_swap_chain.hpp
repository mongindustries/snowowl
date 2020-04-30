//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership_exp.hpp>

#include <window.hpp>

#include "graphics_context.hpp"
#include "graphics_queue.hpp"

SNOW_OWL_NAMESPACE(gx)

struct graphics_swap_chain {

	struct frame {
		unsigned int                          index;
		cx::exp::ptr_ref<graphics_swap_chain> swap_chain{ nullptr };
	};

	graphics_swap_chain
										(const cx::exp::ptr_ref<graphics_context>&  ctx,
										 const cx::exp::ptr_ref<graphics_queue>&    queue,
										 const cx::exp::ptr_ref<ui::window>&        window);

	virtual ~graphics_swap_chain
										() = default;


	virtual cx::exp::ptr_ref<frame>
				next_frame  ();

	virtual void
				present     ();


	virtual void
				resize      (const cx::size_2d& new_size);


	bool                             should_present     { true };

	bool                             swaps_immediately  { false };


	cx::exp::ptr_ref<ui::window>     window;

	std::vector<cx::exp::ptr<frame>> frames;
};

SNOW_OWL_NAMESPACE_END
