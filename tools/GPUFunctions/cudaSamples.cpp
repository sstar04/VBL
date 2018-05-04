

#include <iostream>

#include <cuda_runtime.h>



int *pArgc = NULL;
char **pArgv = NULL;


// CUDA-C includes
#include <cuda.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main(int argc, char **argv)
{
    pArgc = &argc;
    pArgv = argv;

    std::cout<<argv[0] <<"Starting..."<< std::endl;
    std::cout<<" CUDA Device Query (Runtime API) version (CUDART static linking)" << std::endl;

    int deviceCount = 0;
    cudaError_t error_id = cudaGetDeviceCount(&deviceCount);


    // This function call returns 0 if there are no CUDA capable devices.
    if (deviceCount == 0)
    {
        cout<<"There are no available device(s) that support CUDA"<<endl;
    }
    else
    {
        cout<<"Detected" << deviceCount << "CUDA Capable device(s)" << endl;
	}

   int dev, driverVersion = 0, runtimeVersion = 0;

	for (dev = 0; dev < deviceCount; ++dev)
    {
        cudaSetDevice(dev);
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, dev);

	cout<<"Device" << dev << ": " << deviceProp.name << endl;
     }

    //exit(EXIT_SUCCESS);
  return 0; 

}

