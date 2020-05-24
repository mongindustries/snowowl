//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership.hpp>
#include <point.hpp>

#include "context.hpp"
#include "queue.hpp"
#include "resource_reference.hpp"

SNOW_OWL_NAMESPACE(ui)

struct window;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(gx)

struct swap_chain {
  SWL_REFERENCEABLE(swap_chain)
  SWL_POLYMORPHIC(swap_chain)

  struct frame {
    SWL_REFERENCEABLE(frame)
    SWL_POLYMORPHIC(frame)

    unsigned int index;

    cx::exp::ptr < resource_reference > reference;
    cx::exp::ptr_ref < swap_chain >     swap_chain{nullptr};
  };

  swap_chain(context &context, queue &present_queue, ui::window &window);


  virtual cx::exp::ptr_ref < frame >
    next_frame();

  virtual void
    present(std::vector < cx::exp::ptr_ref < queue > > const &dependencies);


  virtual void
    resize(cx::size_2d const &new_size);

  virtual cx::size_2d
    size() const;

  bool swaps_immediately{false};

  cx::exp::ptr_ref < ui::window > window;

  std::vector < cx::exp::ptr < frame > > frames;
};

struct swap_chain_boundary {
  SWL_BLOCK_CONTEXT(swap_chain_boundary)

  explicit
    swap_chain_boundary(swap_chain &swap_chain);

  ~swap_chain_boundary();


  explicit
    operator
    swap_chain::frame &() const;

private:

  std::vector < cx::exp::ptr_ref < queue > > dependencies;


  cx::exp::ptr_ref < swap_chain > swap_chain;

  cx::exp::ptr_ref < swap_chain::frame > flighting_frame;
};

SNOW_OWL_NAMESPACE_END
