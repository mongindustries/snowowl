#pragma once

#include <header.hpp>

#include "factory.hpp"

#include <directx/context.h>

SNOW_OWL_NAMESPACE(gx)

template<>
struct factory<dx::context> {

  explicit
    factory       (dx::context &&instance) : instance(std::move(instance)) {}

  [[nodiscard]] cx::exp::ptr<gx::swap_chain>
    swap_chain    (gx::queue& queue, ui::window& window);

  [[nodiscard]] cx::exp::ptr<gx::queue>
    queue         ();

  [[nodiscard]] cx::exp::ptr<gx::render_block>
    render_block  (gx::queue& queue, gx::render_pipeline& pipeline);

  [[nodiscard]] cx::exp::ptr<gx::render_pass>
    render_pass   (gx::render_block& block, std::vector<gx::render_pass_context> const& pass_context);

private:

  dx::context instance;
};

SNOW_OWL_NAMESPACE_END
