

#include <iostream>

#include <cuda_runtime.h>



// CUDA-C includes
#include <cuda.h>
#include <thrust/device_vector.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
void
DeviceQuery()
{
   
 

    std::cout<<"Starting..."<< std::endl;
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


	//test thrust
thrust::host_vector<int> H(4);
	 H[0] = 14;
    H[1] = 20;
    H[2] = 38;
    H[3] = 46;
    
    // H.size() returns the size of vector H
    std::cout << "H has size " << H.size() << std::endl;

    // print contents of H
    for(int i = 0; i < H.size(); i++)
        std::cout << "H[" << i << "] = " << H[i] << std::endl;

    //exit(EXIT_SUCCESS);
  return; 

}

