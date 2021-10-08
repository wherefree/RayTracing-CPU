//rtweekend.h
#ifndef TRACING_H
#define TRACING_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <functional>
#include <random>
#include<iostream>


// Usings

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
inline double random_double() {
    std::random_device rd;  // 将用于获得随机数引擎的种子
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator =
        std::bind(distribution, generator);
    return rand_generator();
}
/*inline double random_double(double a,double b) {
    if (a == 0 && b == 1) return random_double();
    std::random_device rd;  // 将用于获得随机数引擎的种子
    static std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution1(a, b);
    std::function<double()> rand_generator =
        std::bind(distribution1, generator);
    double ans=rand_generator();
    //std::cout << distribution.a() << " " << distribution.b()<<"-----\n";
    return ans;
    //return distribution(generator);
}
inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}*/

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}
// Common Headers

#include "ray.h"
#include "vec3.h"

#endif