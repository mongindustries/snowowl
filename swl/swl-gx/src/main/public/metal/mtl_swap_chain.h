//
// Created by Michael Ong on 25/4/20.
//
#pragma once

#import <atomic>

#import <header.hpp>

#import "graphics_context.hpp"
#import "graphics_swap_chain.hpp"

#import "mtl_context.h"
#import "mtl_queue.h"

SNOW_OWL_NAMESPACE(gx::mtl)

struct mtl_swap_chain: graphics_swap_chain {

    struct mtl_frame: frame {
#ifdef __OBJC__
        id<MTLTexture>          texture;
        id<MTLDrawable>         drawable;
#else
        void*                   texture;
        void*                   drawable;
#endif

        unsigned int            id;
    };


    mtl_swap_chain      (const cx::exp::ptr_ref<mtl_context> &ctx, const cx::exp::ptr_ref<mtl_queue>& queue, const cx::exp::ptr_ref<ui::window> &window);

    cx::exp::ptr_ref <frame>
            next_frame  () override;

    void    present     () override;


    std::atomic<uint64_t>       cur_frame;

    cx::exp::ptr_ref<mtl_queue> present_queue;

#ifdef __OBJC__
    __weak CAMetalLayer*        layer;
#else
    void*                       layer;
#endif
};

SNOW_OWL_NAMESPACE_END
