//
// Created by micha on 5/3/2020.
//
#pragma once

#include <type_traits>

#include <header.hpp>
#include <ownership.hpp>

#include "context.hpp"
#include "queue.hpp"
#include "swap_chain.hpp"
#include "render_block.hpp"
#include "render_pass.hpp"
#include "render_pipeline.hpp"

#include "buffer.hpp"
#include "buffer_allocator.hpp"

SNOW_OWL_NAMESPACE(gx)

template<typename context, std::enable_if_t<std::is_base_of_v<gx::context, context>, int> = 0>
struct factory {

  explicit
    factory           (context &&instance): instance(instance) {}

  [[nodiscard]] cx::exp::ptr<swap_chain>
    swap_chain        (queue& queue, ui::window& window) { return cx::exp::ptr{nullptr}; }

  [[nodiscard]] cx::exp::ptr<queue>
    queue             () { return cx::exp::ptr{nullptr}; }

  [[nodiscard]] cx::exp::ptr<render_block>
    render_block      () { return cx::exp::ptr{nullptr}; }

  [[nodiscard]] cx::exp::ptr<render_pass>
    render_pass       (gx::render_block& block, std::vector<render_pass_context> const& pass_context) { return cx::exp::ptr{nullptr}; }

  [[nodiscard]] cx::exp::ptr<render_pipeline>
    render_pipeline   () { return cx::exp::ptr{ nullptr }; }

  [[nodiscard]] cx::exp::ptr<buffer_allocator>
    buffer_allocator  (buffer_allocator_usage usage) { return cx::exp::ptr{ nullptr }; }

private:

  context instance;
};

SNOW_OWL_NAMESPACE_END
