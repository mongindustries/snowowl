//
//  render_pipeline.m
//  gx_macOS
//
//  Created by Michael Ong on 11/5/20.
//  Copyright © 2020 mong Industries. All rights reserved.
//

#import <Metal/Metal.h>
#import <array>

#import "metal/context.h"
#import "metal/render_pipeline.h"

SNOW_OWL_NAMESPACE(gx::mtl)

MTLPixelFormat gx_format(pipeline::format format) {

  std::array _deph{
    MTLPixelFormatDepth32Float,
    MTLPixelFormatDepth32Float_Stencil8,
    MTLPixelFormatDepth24Unorm_Stencil8,
    MTLPixelFormatDepth16Unorm
  };

  if (0x0000'D000 & format) {
    const int index = (0x00FF'0000 & format) >> 16;
    return _deph[index];
  }

  std::array _swap{
    MTLPixelFormatInvalid,
    MTLPixelFormatBGRA8Unorm
  };

  if (0x0000'B000 & format) {
    const int index = (0x00FF'0000 & format) >> 16;
    return _swap[index];
  }

  std::array _4_32{
    MTLPixelFormatRGBA32Sint, MTLPixelFormatInvalid,
    MTLPixelFormatRGBA32Uint, MTLPixelFormatInvalid,
    MTLPixelFormatRGBA32Float,
  };

  std::array _x_xx{
    MTLPixelFormatInvalid, MTLPixelFormatInvalid,
    MTLPixelFormatInvalid, MTLPixelFormatInvalid,
    MTLPixelFormatInvalid,
  };

  std::array _2_32{
    MTLPixelFormatRG32Sint, MTLPixelFormatInvalid,
    MTLPixelFormatRG32Uint, MTLPixelFormatInvalid,
    MTLPixelFormatRG32Float
  };

  std::array _1_32{
    MTLPixelFormatR32Sint, MTLPixelFormatInvalid,
    MTLPixelFormatR32Uint, MTLPixelFormatInvalid,
    MTLPixelFormatR32Float
  };

  std::array _4_16{
    MTLPixelFormatRGBA16Sint, MTLPixelFormatRGBA16Snorm,
    MTLPixelFormatRGBA16Uint, MTLPixelFormatRGBA16Unorm,
    MTLPixelFormatRGBA16Float,
  };

  std::array _2_16{
    MTLPixelFormatRG16Sint, MTLPixelFormatRG16Snorm,
    MTLPixelFormatRG16Uint, MTLPixelFormatRG16Unorm,
    MTLPixelFormatRG16Float
  };

  std::array _1_16{
    MTLPixelFormatR16Sint, MTLPixelFormatR16Snorm,
    MTLPixelFormatR16Uint, MTLPixelFormatR16Unorm,
    MTLPixelFormatR16Float
  };

  std::array _4_8{
    MTLPixelFormatRGBA8Sint, MTLPixelFormatRGBA8Snorm,
    MTLPixelFormatRGBA8Uint, MTLPixelFormatRGBA8Unorm,
    MTLPixelFormatInvalid,
  };

  std::array _2_8{
    MTLPixelFormatRG8Sint, MTLPixelFormatRG8Snorm,
    MTLPixelFormatRG8Uint, MTLPixelFormatRG8Unorm,
    MTLPixelFormatInvalid,
  };

  std::array _1_8{
    MTLPixelFormatR8Sint, MTLPixelFormatR8Snorm,
    MTLPixelFormatR8Uint, MTLPixelFormatR8Unorm,
    MTLPixelFormatInvalid,
  };

  std::array _x_32{ _4_32, _x_xx, _2_32, _1_32 };
  std::array _x_24{ _x_xx, _x_xx, _x_xx, _x_xx };
  std::array _x_16{ _4_16, _x_xx, _2_16, _1_16 };
  std::array _x_8 { _4_8 , _x_xx, _2_8 , _1_8  };

  std::array _comp{ _x_32, _x_16, _x_8 };

  const int component  = (0x00FF'0000 & format >> 16) - 1;
  const int byte       = (0x0000'FF00 & format >>  8) - 1;
  const int type       = (0x0000'00FF & format);

  return _comp[component][byte][type];
}

MTLBlendFactor gx_blend(pipeline::blend_type blend) {

  std::array _src_c{ MTLBlendFactorSourceColor, MTLBlendFactorOneMinusSourceColor };
  std::array _src_a{ MTLBlendFactorSourceAlpha, MTLBlendFactorOneMinusSourceAlpha };

  std::array _dst_c{ MTLBlendFactorDestinationColor, MTLBlendFactorOneMinusDestinationColor };
  std::array _dst_a{ MTLBlendFactorDestinationAlpha, MTLBlendFactorOneMinusDestinationAlpha };

  std::array _bnd_f{ MTLBlendFactorBlendColor, MTLBlendFactorOneMinusBlendColor };

  const std::array _s_l{ _src_c, _src_a };
  const std::array _d_l{ _dst_c, _dst_a };
  const std::array _b_l{ _bnd_f, std::array{ MTLBlendFactorZero, MTLBlendFactorZero } };

  std::array list{ _s_l, _d_l, _b_l };

  const int location   = ((0xFFFF'0000 & blend) >> 16) - 1;
  const int component  = ((0x0000'FF00 & blend) >>  8) - 1;
  const int kind       =  (0x0000'00FF & blend)        - 1;

  return list[location][component][kind];
}

render_pipeline::render_pipeline(context &context): gx::render_pipeline(context), device(context.device) {
}

void render_pipeline::construct() {

  MTLRenderPipelineDescriptor *descriptor = [MTLRenderPipelineDescriptor new];

  for (auto i = 0u; i < render_inputs[pipeline::shader_stage::vertex].resource_binding.size(); i += 1) {
    descriptor.vertexBuffers[i].mutability = MTLMutabilityImmutable; // WTF?
  }

  for (auto i = 0u; i < render_inputs[pipeline::shader_stage::fragment].resource_binding.size(); i += 1) {
    descriptor.fragmentBuffers[i].mutability = MTLMutabilityImmutable; // again, WTF?
  }

  for (auto i = 0u; i < render_outputs.size(); i += 1) {

    auto &output = render_outputs[i];
    auto cattach = descriptor.colorAttachments[i];

    cattach.blendingEnabled             = output.blend.enabled;

    cattach.sourceRGBBlendFactor        = gx_blend(output.blend.source_blend);
    cattach.sourceAlphaBlendFactor      = gx_blend(output.blend.source_alpha_blend);

    cattach.destinationRGBBlendFactor   = gx_blend(output.blend.destination_blend);
    cattach.destinationAlphaBlendFactor = gx_blend(output.blend.destination_alpha_blend);

    cattach.pixelFormat      = gx_format(output.format);
  }

  descriptor.depthAttachmentPixelFormat = gx_format(depth.format);

  pipeline = [device newRenderPipelineStateWithDescriptor:descriptor error:nil];
}

SNOW_OWL_NAMESPACE_END
