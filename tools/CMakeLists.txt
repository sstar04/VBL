add_subdirectory(tests)
add_subdirectory(bico)

if(USE_GPU)
  find_package(CUDA QUIET REQUIRED)
  set(CUDA_NVCC_FLAGS ${OPTFLAGS}
    ${CUDA_NVCC_FLAGS};
    -gencode arch=compute_35,code=sm_35
    -gencode arch=compute_30,code=sm_30)
#       include_directories ("${PROJECT_SOURCE_DIR}/gDel3D")
#add_subdirectory(gDel3D-master)
     add_subdirectory(GPUFunctions)
endif(USE_GPU)

