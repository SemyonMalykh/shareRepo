#include <iostream>
#include <math.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <chrono>
#include <algorithm>
#include <stdio.h>

#include <Eigen/Dense>

using namespace Eigen;
using namespace std::chrono;

typedef void(*addFunc)(float*, float*, int);

__device__
void add(float *x, float *y, int i)
{
	y[i] = 2 * x[i] + y[i];
}

__global__
void kern(addFunc func, int n, float *x, float *y) {

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	for (int i = index; i < n; i += stride)
		func(x, y, i);
}


__device__ addFunc d_add = add;

int main(void)
{
	addFunc h_add;
	cudaMemcpyFromSymbol(&h_add, d_add, sizeof(addFunc));

	std::cout << "h_add adr:" << h_add << std::endl;
	std::cout << "d_add adr:" << h_add << std::endl;

	int N = 1 << 20;
	float *x, *y;


	// Allocate Unified Memory – accessible from CPU or GPU
	cudaMallocManaged(&x, N * sizeof(float));
	cudaMallocManaged(&y, N * sizeof(float));

	// initialize x and y arrays on the host
	for (int i = 0; i < N; i++) {
		x[i] = 1.0f;
		y[i] = 2.0f;
	}


	// Run kernel on 1M elements on the GPU
	int blockSize = 256;
	int numBlocks = (N + blockSize - 1) / blockSize;



	auto start = high_resolution_clock::now();
	kern <<< numBlocks, blockSize >>> (h_add, N, x, y);
	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

	// Check for errors (all values should be 3.0f)
	float maxError = 0.0f;
	for (int i = 0; i < N; i++)
		maxError = fmax(maxError, fabs(y[i] - 3.0f));
	std::cout << "Max error: " << maxError << std::endl;
	// Free memory
	cudaFree(x);
	cudaFree(y);
  
  return 0;
}