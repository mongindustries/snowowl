//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#define SNOW_OWL_NAMESPACE(group) namespace swl::group {

#define SNOW_OWL_NAMESPACE_END }

#if defined(SWL_WIN32)
#define SWL_EXPORT __declspec( dllexport )
#else
#define SWL_EXPORT
#endif

#define SWL_NO_CPY_CTOR(type) \
  type              (const type&) = delete; \
  type& operator=   (const type&) = delete;

#define SWL_MOVE_CTOR(type) \
  type              (type&&) noexcept; \
  type& operator=   (type&&) noexcept;

/**
 * Designates an object to only have a move constructor. Instances
 * of this object is usually wrapped inside a <code>swl::cx::ptr</code>
 * instance.
 */
#define SWL_REFERENCEABLE(type) \
  SWL_NO_CPY_CTOR(type) \
  SWL_MOVE_CTOR(type)

#define SWL_POLYMORPHIC(type) \
  explicit type () noexcept; \
  virtual ~type ();
