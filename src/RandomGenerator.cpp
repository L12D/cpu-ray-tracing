#include "RandomGenerator.hpp"


const char* kernelSource = R"CLC(
uint pcg32(uint *state) {
    uint oldstate = *state;
    *state = oldstate * 747796405u + 2891336453u;
    uint xorshifted = ((oldstate >> ((oldstate >> 28u) + 4u)) ^ oldstate) >> 18u;
    uint rot = oldstate >> 27u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

__kernel void generate_randoms(__global float *output, uint seed, uint N) {
    int id = get_global_id(0);
    if (id * 2 + 1 >= N) return; // Ensure space for 2 outputs per thread

    uint state = seed ^ (id * 747796405u); // Per-thread unique initial state

    // First random number
    uint r1 = pcg32(&state);
    output[id * 2] = (float)r1 / (float)0xFFFFFFFF;

    // Second random number
    uint r2 = pcg32(&state);
    output[id * 2 + 1] = (float)r2 / (float)0xFFFFFFFF;
}
)CLC";


RandomGenerator::RandomGenerator() {
    // 1. Platform/device selection
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cl::Platform platform = platforms[0];
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    device = devices[0];

    // 2. Create context and command queue
    context = cl::Context(device);
    queue = cl::CommandQueue(context, device);

    // 3. Build program and create kernel
    cl::Program program(context, kernelSource);
    program.build({device});
    kernel = cl::Kernel(program, "generate_randoms");

    // 4. Create OpenCL buffer once, sized for max expected N
    buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(float)*N);

    randomNumbers.resize(N);

    generate(); // Initial generation
}


void RandomGenerator::generate() {
    static uint64_t counter = 0;
    unsigned int seed = static_cast<unsigned int>(time(nullptr)) ^ (counter++);

    kernel.setArg(0, buffer);
    kernel.setArg(1, seed);
    kernel.setArg(2, N);

    // Only enqueue N elements even if buffer is larger
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(N), cl::NullRange);
    queue.finish();

    // Read back just the N floats
    queue.enqueueReadBuffer(buffer, CL_TRUE, 0, sizeof(float) * N, randomNumbers.data());

    currentIndex = 0; // Reset index after generation
}


float RandomGenerator::next() {
    if (currentIndex >= N) {
        generate();
    }
    return randomNumbers[currentIndex++];
}

void RandomGenerator::reset() {
    currentIndex = N; // Force generate() on next `next()`
}