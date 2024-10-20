
add_library(mem_cache)

target_include_directories(mem_cache PUBLIC include)

add_subdirectory(src)

target_link_libraries(mem_cache
     SIM::enums
)

add_library(SIM::mem_cache ALIAS mem_cache)