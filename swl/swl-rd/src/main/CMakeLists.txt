cmake_minimum_required(VERSION 3.16.0)

set(HEADER_LIST
        public/world.hpp public/layer.hpp public/entity.hpp public/world_renderer.hpp public/graph_node.hpp public/graph_nodes/diffuse.hpp public/graph_node_argument.hpp public/graph_node_effect.hpp public/graph_nodes/vertex.hpp public/layer_camera.hpp public/world_renderer_context.hpp)

set(PRIVATE_HEADER_LIST )

set(SOURCE_LIST
        cpp/world.cpp cpp/entity.cpp cpp/world_renderer.cpp cpp/graph_nodes/vertex.cpp cpp/graph_nodes/diffuse.cpp cpp/layer.cpp cpp/graph_node_argument.cpp)

add_library(rd
        ${SOURCE_LIST}
        ${HEADER_LIST}
        ${PRIVATE_HEADER_LIST})

target_include_directories(rd
        PUBLIC  public
        PRIVATE headers

        PRIVATE ../../../swl-cx/src/main/public
        PRIVATE ../../../swl-gx/src/main/public

        PRIVATE ${SRC_OS}
        PRIVATE ${VULKAN_SDK}/include)

target_link_libraries(ui
        PUBLIC  cx
        PRIVATE gx)
