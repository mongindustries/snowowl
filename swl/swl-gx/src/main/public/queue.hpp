//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#include <vector>

#include <header.hpp>
#include <ownership.hpp>

#include "context.hpp"

#include "transfer_block.hpp"
#include "render_block.hpp"

SNOW_OWL_NAMESPACE(gx)

/**
 * A graphics queue construct.
 *
 * A graphics queue represents an abstraction of command execution to the GPU.
 * Communication to GPU is asynchronous given the fact that GPUs are technically
 * another processor in a machine. A graphics queue traffics graphics commands in
 * a deterministic and in-order way.
 *
 * This has three methods: <code>begin</code> for waiting for previous command
 * execution, <code>submit</code> for submitting <code>render_block</code>s, and
 * <code>transfer</code> for updating/synchronizing resource buffers.
 *
 */
struct queue { SWL_REFERENCEABLE(queue) SWL_POLYMORPHIC(queue)

  explicit
    queue     (context &context);

  /**
   *
   * Begins execution of a queue. This method CPU waits for current execution
   * submitted to the GPU to finish before continuing.
   *
   * @param dependencies A list of queues to wait  before waiting
   * this queue's execution to complete (TODO: add CPU or GPU type
   * wait).
   */
  virtual void
    begin     (std::vector < std::reference_wrapper < queue > > const &dependencies);

  /**
   *
   * Informs the GPU a list of <code>gx::render_block</code> to be executed.
   * This method runs asynchronously, use <code>begin()</code> to wait for the next frame.
   *
   * @param commands The list of <code>gx::render_block</code> to execute.
   */
  virtual void
    submit    (std::vector < std::reference_wrapper < render_block > > const &commands);

  /**
   *
   * Informs the GPU a list of <code>gx::buffer_staging</code> to be executed.
   * This method runs asynchronously, use <code>begin()</code> to wait for the next frame.
   * @param buffers The list of <code>gx::buffer_staging</code> to execute.
   */
  virtual void
    transfer  (std::vector < cx::exp::ptr_ref < transfer_block > > const &buffers);

protected:

  cx::exp::ptr_ref < context > context;
};

SNOW_OWL_NAMESPACE_END
