#pragma once

#include <cuda_runtime.h>
#include <vector>

#include "types.hpp"
#include "utils.hpp"
#include "Genome.hpp"

__global__ void thinkKernel(const float* mind_factor, const float* sensorial_input,
                            const float* mind_addends, float* result, size_t mind_size);

vectorf32 thinkCUDA(const vectorf32& mind_factor, const vectorf32& sensorial_input, const vectorf32& mind_addends);