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
