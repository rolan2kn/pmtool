//
// Created by rkindela on 27-05-18.
//

#ifndef PMTOOL_UTILS_H
#define PMTOOL_UTILS_H

#include <limits>
#include <iostream>
#include <typeinfo>
#include <cmath>
#include <ctime>

#include <cstdio>
#include <unistd.h>
#include <cerrno>


# define M_PI           3.14159265358979323846
# define RAD            0.01745329251   // M_PI/180
#define INV_RAD         57.2957795131   // 180/M_PI

using namespace std;

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y, int ulp)
{

    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return abs(x-y) <= std::numeric_limits<T>::epsilon() * abs(x+y) * ulp
           // unless the result is subnormal
           || abs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
double toRadians(T angle)
{
    return angle*RAD;
}

template<class T>
double toDegree(T angle)
{
    return angle*INV_RAD;
}

inline float clock2milisec(clock_t old)
{
    return (clock() - old)/(CLOCKS_PER_SEC/1000);
};


inline std::string currentDirectory()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == nullptr)
        return "";
    string currentDir = cwd;
    int idx = currentDir.rfind("/");
    if (idx == -1)
        return "";

    return currentDir.substr(0, idx);
}

#endif //PMTOOL_UTILS_H
