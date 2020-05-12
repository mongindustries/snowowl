//
// Created by Michael Ong on 25/4/20.
//
#include "queue.hpp"

#include "render_pipeline.hpp"
#include "render_block.hpp"

#include "buffer.hpp"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename c>
using ptr_ref = cx::exp::ptr_ref<c>;

queue::queue        () noexcept: context(nullptr) {
}

queue::queue        (queue &&move) noexcept: context(move.context) {
  move.context = cx::exp::ptr_ref<gx::context>{ nullptr };
}

queue::queue        (gx::context& context) : context(&context) {}

queue::~queue       () = default;

queue&
  queue::operator=  (queue &&move) noexcept {
  context = move.context;
  return *this;
}


void
  queue::begin      (const std::vector<ptr_ref<queue>> &dependencies) {}

void
  queue::submit     (const std::vector<ptr_ref<render_block>> &commands) {}

void
  queue::transfer   (const std::vector<cx::exp::ptr_ref<buffer_staging>> &buffers) {}

SNOW_OWL_NAMESPACE_END
