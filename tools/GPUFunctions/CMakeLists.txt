cuda_add_library(GPUFunctions SHARED DeviceQuery.cpp)


#Make sure the compiler can find include files for our GPUfunctions library
# when other libraries or executables link to  GPUFunctions
target_include_directories (GPUFunctions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(gpu-test main.cpp)
target_link_libraries(gpu-test GPUFunctions)
