#pragma once

#include <header.hpp>
#include <rect.hpp>
#include <ownership.hpp>

#include "shader.hpp"
#include "resource_reference.hpp"

#define NBS 16
#define NRS  8

SNOW_OWL_NAMESPACE(gx)

struct render_pipeline;

SNOW_OWL_NAMESPACE_END

SNOW_OWL_NAMESPACE(gx::pipeline)

enum buffer_type {
  typeData                        ,
  typeTexture2d                   ,
  typeTexture3d
};

enum buffer_usage_type {

  usageTypeShader                 ,
  usageTypeConstant               ,
  usageTypeTexture                ,
  usageTypeRenderTarget
};

enum buffer_visibility_type {
  dataUsagePrivate                ,
  dataUsageReadBack
};

enum resource_transition_type {

  transitionTypeInherit           ,

  transitionTypeShaderView        ,
  transitionTypeConstantView      ,
  transitionTypeRenderTargetView  ,

  transitionTypeSwapChainPresent  ,

  transitionTypeCopySource        ,
  transitionTypeCopyDestination
};

enum pass_load_op {
  loadOpLoad                      ,
  loadOpClear                     ,
  loadOpDoNotCare                 ,
  loadOpNoAccess
};

enum pass_store_op {
  storeOpStore                    ,
  storeOpDoNotCare                ,
  storeOpResolve                  ,
  storeOpNoAccess
};

enum format {

  format_unknown                  = 0,

  // standard formats

  format_4_32_int                 = 0x04'04'01,
  format_4_32_int_u               = 0x04'04'03,
  format_4_32_float               = 0x04'04'05,

  format_3_32_int                 = 0x03'04'01,
  format_3_32_int_u               = 0x03'04'03,
  format_3_32_float               = 0x03'04'05,

  format_2_32_int                 = 0x02'04'01,
  format_2_32_int_u               = 0x02'04'03,
  format_2_32_float               = 0x02'04'05,

  format_1_32_int                 = 0x01'04'01,
  format_1_32_int_u               = 0x01'04'03,
  format_1_32_float               = 0x01'04'05,


  format_4_16_int                 = 0x04'02'01,
  format_4_16_int_norm            = 0x04'02'02,
  format_4_16_int_u               = 0x04'02'03,
  format_4_16_int_u_norm          = 0x04'02'04,
  format_4_16_float               = 0x04'02'05,

  format_2_16_int                 = 0x02'02'01,
  format_2_16_int_norm            = 0x02'02'02,
  format_2_16_int_u               = 0x02'02'03,
  format_2_16_int_u_norm          = 0x02'02'04,
  format_2_16_float               = 0x02'02'05,

  format_1_16_int                 = 0x01'02'01,
  format_1_16_int_norm            = 0x01'02'02,
  format_1_16_int_u               = 0x01'02'03,
  format_1_16_int_u_norm          = 0x01'02'04,
  format_1_16_float               = 0x01'02'05,

  format_4_8_int                  = 0x04'01'01,
  format_4_8_int_norm             = 0x04'01'02,
  format_4_8_int_u                = 0x04'01'03,
  format_4_8_int_u_norm           = 0x04'01'04,

  format_2_8_int                  = 0x02'01'01,
  format_2_8_int_norm             = 0x02'01'02,
  format_2_8_int_u                = 0x02'01'03,
  format_2_8_int_u_norm           = 0x02'01'04,

  format_1_8_int                  = 0x01'01'01,
  format_1_8_int_norm             = 0x01'01'02,
  format_1_8_int_u                = 0x01'01'03,
  format_1_8_int_u_norm           = 0x01'01'04,

  // specialized formats

  // depth and stencil

  format_1_32_float_depth         = 0x01'D4'00,
  format_2_32_float_8_uint        = 0x02'D4'00,
  format_3_24_int_u_8_uint        = 0x03'D4'00,
  format_1_16_int_u_depth         = 0x04'D4'00,

  // swapchain

  format_4_8_int_flipped          = 0x0001'B100,
  format_4_8_int_u_norm_flipped   = 0x0002'B100
};

enum raster_fill_mode {
  modeWireFrame                   ,
  modeFill
};

enum raster_cull_mode {
  modeNone                        ,
  modeFront                       ,
  modeBack
};

enum address_mode {
  modeWrap                        ,
  modeClamp                       ,
  modeMirror
};

enum filter_mode {
  modePoint                       ,
  modeLiner                       ,
  modeAnisotropic
};

enum blend_type {
  typeZero                        = 0x0000'0000,
  typeOne                         = 0x0000'0001,
  typeSourceColor                 = 0x0001'0100,
  typeSourceColorInvert           = 0x0001'0101,
  typeSourceAlpha                 = 0x0001'0200,
  typeSourceAlphaInvert           = 0x0001'0201,
  typeDestinationColor            = 0x0002'0100,
  typeDestinationColorInvert      = 0x0002'0101,
  typeDestinationAlpha            = 0x0002'0200,
  typeDestinationAlphaInvert      = 0x0002'0201,
  typeBlendFactor                 = 0x0003'0100,
  typeBlendFactorInvert           = 0x0003'0101
};

enum blend_op {
  opAdd                           ,
  opSubtract                      ,
  opMin                           ,
  opMax
};

enum shader_argument_type {
  typeNotUsed                     ,

  typeTexture                     ,
  typeConstant                    ,
  typeBuffer
};

enum topology_type {
  topologyTypePoint               = 0,
  topologyTypeLine                = 1,
  topologyTypeTriangle            = 2
};

enum comparison_type {
  comparisonNever                 ,
  comparisonAlways                ,
  comparisonLess                  ,
  comparisonLessEqual             ,
  comparisonMore                  ,
  comparisonMoreEqual             ,
  comparisonEqual                 ,
  comparisonNotEqual
};

enum stencil_op {
  opKeep                          ,
  opZero                          ,
  opReplace                       ,
  opSatIncrease                   ,
  opSatDecrease                   ,
  opInvert                        ,
  opIncrease                      ,
  opDecrease
};

struct write_mask {

  operator uint8_t() const;

  bool
    operator&(const write_mask& rhs) const;

  uint8_t value;

  static write_mask                     r;
  static write_mask                     g;
  static write_mask                     b;
  static write_mask                     a;
};

struct shader_stage {

  operator uint8_t() const;

  uint8_t value;

  static shader_stage                   vertex;
  static shader_stage                   fragment;

};

struct raster {

  raster_fill_mode                      fill_mode                   {raster_fill_mode::modeFill};
  raster_cull_mode                      cull_mode                   {raster_cull_mode::modeBack};

  bool                                  render_counter_clockwise    {false};
};

struct blend {

  bool                                  enabled                     {false};
  bool                                  op_enabled                  {false};

  blend_type                            source_blend                {blend_type::typeOne};
  blend_type                            source_alpha_blend          {blend_type::typeZero};

  blend_type                            destination_blend           {blend_type::typeOne};
  blend_type                            destination_alpha_blend     {blend_type::typeZero};

  blend_op                              blend_operation             {blend_op::opAdd};
  blend_op                              blend_alpha_operation       {blend_op::opAdd};

  write_mask                            write_mask                  {0b1111};
};

struct depth {

  bool                                  enabled                     {true};
  format                                format                      {format::format_2_32_float_8_uint};

  comparison_type                       comparison                  {comparison_type::comparisonLess};
};

struct stencil {

  bool                                  enabled                     {false};

  stencil_op                            front_pass                  {stencil_op::opKeep};
  stencil_op                            front_fail                  {stencil_op::opKeep};
  stencil_op                            front_depthFail             {stencil_op::opKeep};
  comparison_type                       front_comparison            {comparison_type::comparisonAlways};

  stencil_op                            back_pass                   {stencil_op::opKeep};
  stencil_op                            back_fail                   {stencil_op::opKeep};
  stencil_op                            back_depthFail              {stencil_op::opKeep};
  comparison_type                       back_comparison             {comparison_type::comparisonAlways};
};

struct sample {

  unsigned int                          count                       {1};
  unsigned int                          quality                     {0};
};

struct sampler {

  address_mode                          address_x                   {address_mode::modeClamp};
  address_mode                          address_y                   {address_mode::modeClamp};
  address_mode                          address_z                   {address_mode::modeClamp};

  filter_mode                           min_filter                  {filter_mode::modePoint};
  filter_mode                           mag_filter                  {filter_mode::modePoint};
  filter_mode                           mip_filter                  {filter_mode::modePoint};

  int                                   max_anisotropy              {1};

  comparison_type                       comparison_func             {comparison_type::comparisonAlways};

  float                                 min_lod                     {0};
  float                                 max_lod                     {FLT_MAX};
};

struct shader_argument {

  shader_argument_type                  type                        {shader_argument_type::typeNotUsed};
  format                                format                      {format::format_unknown};

  bool                                  indirect                    {true};
};

struct shader_input {

  std::array < shader_argument, NBS >   arguments;
  std::array < sampler, NBS >           samplers;
};

struct shader_output {

  format                                format                      {format::format_unknown};
  pipeline::blend                       blend                       {};
};

struct pipeline_stage {

  shader                                function{nullptr, 0};

  shader_input                          input;
  std::array < shader_output, NRS >     output;
};

struct pass_input {

  cx::exp::ptr_ref<resource_reference>  input;

  shader_stage                          stage;

  resource_transition_type              during                      {resource_transition_type::transitionTypeShaderView};

  resource_transition_type              before                      {resource_transition_type::transitionTypeInherit};
  resource_transition_type              after                       {resource_transition_type::transitionTypeInherit};
};

struct pass_output {

  cx::exp::ptr_ref<resource_reference>  output;

  cx::size_2d                           viewport;
  cx::rect                              scissor;

  pass_load_op                          action_load                 {pass_load_op ::loadOpClear};
  pass_store_op                         action_store                {pass_store_op::storeOpStore};

  std::array < float, 4 >               clear_color                 { 1, 1, 1, 1 };

  resource_transition_type              during                      {resource_transition_type::transitionTypeRenderTargetView};
  resource_transition_type              before                      {resource_transition_type::transitionTypeInherit};
  resource_transition_type              after                       {resource_transition_type::transitionTypeInherit};
};

struct draw_range {
  unsigned int                          begin;
  unsigned int                          size;
};

struct upload_desc {

  size_t                                start                       { 0};
  size_t                                size                        { 0};

  char*                                 data                        {nullptr};
};

struct resource_state_desc {

  shader_stage                          stage;
  buffer_usage_type                     usage;
};

SNOW_OWL_NAMESPACE_END
 