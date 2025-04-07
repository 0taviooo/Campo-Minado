#ifndef RANDOM_H

#define RANDOM_H

#include <random>

struct Random {
    static int gen(int min, int max) {
        static std::mt19937 generator;
        std::uniform_int_distribution<int> distribution(min, max);
        int result = distribution(generator);
        return result;
    }
};

#endif
