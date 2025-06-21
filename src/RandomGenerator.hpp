#pragma once


#include <vector>
#include <CL/opencl.hpp>


class RandomGenerator {

    private:

        const unsigned int N = 1024;
        std::vector<float> randomNumbers;
        cl::Kernel kernel;
        cl::Buffer buffer;
        cl::CommandQueue queue;
        cl::Context context;
        cl::Device device;
        size_t currentIndex = 0;

        void generate();
    
    public:

        RandomGenerator();
        float next();
        void reset();
        const std::vector<float>& getRandomNumbers() const { return randomNumbers; }
        ~RandomGenerator() = default;

};