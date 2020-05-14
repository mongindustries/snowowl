//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#define SNOW_OWL_NAMESPACE(group) namespace swl::group {

#define SNOW_OWL_NAMESPACE_END }

#if defined(SWL_WIN32)
#define SWL_EXPORT
#else
#define SWL_EXPORT
#endif

#if defined(__OBJC__)
#define OBJC_OBJECT(type) type*
#else
#define OBJC_OBJECT(type) void*
#endif

#if defined(__OBJC__)
#define OBJC_ID(type) id<type>
#else
#define OBJC_ID(type) void*
#endif

#define SWL_NO_CPY_CTOR(type) \
  type              (const type&) = delete; \
  type& operator=   (const type&) = delete;

#define SWL_NO_MOVE_CTOR(type) \
  type              (type&&) = delete; \
  type& operator=   (type&&) = delete;

#define SWL_MOVE_CTOR(type) \
  type              (type&&) noexcept; \
  type& operator=   (type&&) noexcept;

#define SWL_BLOCK_CONTEXT(type) \
  type(const type&) = delete; \
  type(type&&) = delete; \
  type& operator= (const type&) = delete; \
  type& operator= (type&&) = delete; 

/**
 * Designates an object to only have a move constructor. Instances
 * of this object is usually wrapped inside a <code>swl::cx::ptr</code>
 * instance.
 */
#define SWL_REFERENCEABLE(type) \
  SWL_NO_CPY_CTOR(type) \
  SWL_MOVE_CTOR(type)

/**
 * Designates an object intending to be inherited from: it creates a default constructor and a virtual destructor.
 * This is helpful for standardised polymorphic type requirements. (Inspiration from <code>NSObject</code> inheritance)
 */
#define SWL_POLYMORPHIC(type) \
  explicit type () noexcept; \
  virtual ~type ();
