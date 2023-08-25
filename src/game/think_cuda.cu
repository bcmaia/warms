#include 'think_cuda.h'


// CUDA kernel
__global__ void thinkKernel(const float* mind_factor, const float* sensorial_input,
                            const float* mind_addends, float* result, size_t mind_size) {
    size_t i = blockIdx.x;
    size_t j = threadIdx.x;

    result[i] += mind_factor[i * mind_size + j] * sensorial_input[j] + mind_addends[i];
}

vectorf32 thinkCUDA(const vectorf32& mind_factor, const vectorf32& sensorial_input, const vectorf32& mind_addends) {
    vectorf32 result(OUTPUT_SIZE);

    float* d_mind_factor;
    float* d_sensorial_input;
    float* d_mind_addends;
    float* d_result;

    cudaMalloc((void**)&d_mind_factor, OUTPUT_SIZE * MIND_SIZE * sizeof(float));
    cudaMalloc((void**)&d_sensorial_input, MIND_SIZE * sizeof(float));
    cudaMalloc((void**)&d_mind_addends, OUTPUT_SIZE * sizeof(float));
    cudaMalloc((void**)&d_result, OUTPUT_SIZE * sizeof(float));

    cudaMemcpy(d_mind_factor, mind_factor.data(), OUTPUT_SIZE * MIND_SIZE * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_sensorial_input, sensorial_input.data(), MIND_SIZE * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_mind_addends, mind_addends.data(), OUTPUT_SIZE * sizeof(float), cudaMemcpyHostToDevice);

    dim3 gridDim(OUTPUT_SIZE);
    dim3 blockDim(MIND_SIZE);

    thinkKernel<<<gridDim, blockDim>>>(d_mind_factor, d_sensorial_input, d_mind_addends, d_result, MIND_SIZE);

    cudaMemcpy(result.data(), d_result, OUTPUT_SIZE * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(d_mind_factor);
    cudaFree(d_sensorial_input);
    cudaFree(d_mind_addends);
    cudaFree(d_result);

    return result;
}
