/**
 * This represents a random thought on how to consume snow owl reflection metadata.
 * 
 * This is a super early 5 minute thought process...
 */
'use strict'

const RM = swl.cx.ReflectionManager

// swl.cx.ReflectionManager.instaniate has to types:
// 1. instantiate(ConstructorObject, configuration)
// 2. instantiate(ConstructorObject, constructor)

const instance = RM.instantiate(swl.rd.Node, function(base) {

    Name = "Some Entity"

    const reference_vrt = AddEffect(RM.instantiate(swl.rd.effects.Vertex, function() {
        Vertices    = [
            RM.instantiate(swl.rd.effects.vertex.Buffer, [ 0, 1, 0 ]),
            RM.instantiate(swl.rd.effects.vertex.Buffer, [ 1, 0, 0 ]),
            RM.instantiate(swl.rd.effects.vertex.Buffer, [ 0, 0, 1 ]),
        ],

        Indices     = [ 0, 1, 2 ]
    }))

    const reference_dif = AddEffect(RM.instantiate(swl.rd.effects.Diffuse, function() {
        Texture = RM.instantiate(swl.cx.Path, "characters:girl:generic")
    }))

    methods.OnUpdate(function(time_delta) {
        base.OnUpdate(time_delta)
    })
});

// creating new types

const NodeType = RM.inherit(swl.rd.Node, function(base) {

    methods.OnUpdate(function(time_delta) {

    })
})

const node_type_instance = RM.instantiate(NodeType, function(base) {

})

// creating a window and a game loop

const factory = RM.instantiate(swl.gx.Factory(swl.gx.dx.Context))

const GameGameLoop = RM.inherit(swl.cx.GameLoop, function(base, window) {

    const queue         = factory.CreateQueue()
    const swap_chain    = factory.CreateSwapChain(queue, window)

    const render_block  = factory.CreateRenderBlock(queue, null)

    methods.Create(function() {
    })

    methods.Update(function(time_delta) {
    })

    methods.Render(function(frame_offset) {

        queue.Begin()
        var frame = swap_chain.NextFrame()

        var target1 = RM.transient(swl.gx.RenderPassContext, function() {
            ActionLoad  = swl.gx.e.LoadAction.Clear,
            ActionStore = swl.gx.e.StoreAction.Store

            ClearColor  = [ 1, 1, 1, 1 ]

            TransitionBefore    = swl.gx.e.ResourceTransition.Inherit
            TransitionDuring    = swl.gx.e.ResourceTransition.TargetView
            TransitionAfter     = swl.gx.e.ResourceTransition.Inherit

            Reference           = RM.transient(swl.cx.Reference, frame.Reference)
        })

        render_block.RenderPass([ target1 ])

        swap_chain.Present()
        queue.Submit([ render_block ])
    })
})

swl.ui.App = RM.instantiate(swl.ui.Application, function() {

    const gl

    methods.Create(function() {

        const window        = GetMainWindow()
        const game_loop     = RM.instantiate(GameGameLoop, window)
        gl                  = gl
    
        game_loop.Open()
    })

    methods.Destroy(function() {
    })
})
