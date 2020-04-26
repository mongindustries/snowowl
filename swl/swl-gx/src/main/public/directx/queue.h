#pragma once

#include <atomic>

#include <header.hpp>

#include "context.h"
#include "graphics_queue.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

struct queue final: graphics_queue {

	queue   (const cx::exp::ptr_ref<context>& context);

	~queue  () override;


	void   begin   () override;
	
	void   submit  (const std::vector<cx::exp::ptr_ref<graphics_pass>>& commands) override;


	std::atomic<uint64_t>              frame;


	winrt::com_ptr<ID3D12CommandQueue> command_queue;

	winrt::com_ptr<ID3D12Fence>        fence;
};

SNOW_OWL_NAMESPACE_END
