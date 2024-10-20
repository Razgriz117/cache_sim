cmake_minimum_required(VERSION 3.22)

project(sim_cache)

add_subdirectory(enums)
add_subdirectory(mem_cache)

add_executable(sim_cache 
     sim_cache.cpp
     simulator.cpp
)

target_include_directories(sim_cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sim_cache
     SIM::enums
     SIM::mem_cache
)

