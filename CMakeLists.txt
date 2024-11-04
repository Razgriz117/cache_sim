cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(sim_cache)

add_subdirectory(enums)
add_subdirectory(mem_cache)

add_executable(sim_cache 
     mem_architecture_sim.cpp
     main.cpp
)

target_include_directories(sim_cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sim_cache
     SIM::enums
     SIM::mem_cache
)

