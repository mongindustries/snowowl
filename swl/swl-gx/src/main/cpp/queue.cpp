//
// Created by Michael Ong on 25/4/20.
//
#include "queue.hpp"

#include "buffer.hpp"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;


queue::queue  () noexcept : context(nullptr) {
}

queue::~queue () = default;


queue::queue              (queue &&move) noexcept: context(move.context) {
  move.context = cx::exp::ptr_ref<gx::context>{ nullptr };
}

queue& queue::operator=   (queue&& move) noexcept {
  context = move.context;
  return *this;
}


queue::queue        (gx::context& context) : context(&context) {}

void
  queue::begin      (std::vector<std::reference_wrapper<queue>> const &dependencies) {}

void
  queue::submit     (std::vector<std::reference_wrapper<render_block>> const &commands) {}

void
  queue::transfer   (std::vector<cx::exp::ptr_ref<transfer_block>> const &buffers) {}

SNOW_OWL_NAMESPACE_END
