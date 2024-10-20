add_library(enums INTERFACE)

target_include_directories(enums INTERFACE include)

add_library(SIM::enums ALIAS enums)