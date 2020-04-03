//
// Created by Michael Ong on 1/4/20.
//
#pragma once

#include <headerconv.hpp>
#include "context_backend.hpp"

SNOW_OWL_NAMESPACE(gx::backend)

struct SWL_EXPORT ContextBackendVulkan: public ContextBackend {

	struct Context;

	cx::Himplem<Context> context;


	ContextBackendVulkan();

	ContextBackendVulkan(ContextBackendVulkan &&mov) noexcept;


	[[nodiscard]] std::string name() const override;
};

SNOW_OWL_NAMESPACE_END
