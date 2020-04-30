#include "directx/context.h"
#include "directx/render_block.h"

#include "graphics_render_pass.hpp"

SNOW_OWL_NAMESPACE(gx::dx)

render_block::render_block      (const cx::exp::ptr_ref<dx::context>& ctx, const cx::exp::ptr_ref<graphics_render_pipeline>& pipeline):
  graphics_render_block(ctx.cast<graphics_context>(), pipeline), allocator(ctx->command_allocator) {

  ctx->device->CreateCommandList(0,
    D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.get(), nullptr,
    __uuidof(ID3D12GraphicsCommandList4), command_list.put_void());

  command_list->Close();
}

void render_block::reset        (const cx::exp::ptr_ref<graphics_render_pipeline>& pipeline) {

  command_list->Reset(allocator.get(), nullptr);
}

void render_block::render_pass  (const std::function<void(graphics_render_pass&)>& configure) {

  command_list->BeginRenderPass(0, nullptr, nullptr, D3D12_RENDER_PASS_FLAG_NONE);

  command_list->EndRenderPass();
}

SNOW_OWL_NAMESPACE_END
