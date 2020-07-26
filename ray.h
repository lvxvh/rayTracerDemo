//
// Created by 吕旭晖 on 2020/7/18.
//

#ifndef RAYTRACE_RAY_H
#define RAYTRACE_RAY_H

#include "src/utils/vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time)
    {}

    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }
    double time() const    { return tm; }

    point3 at(double t) const {
        return orig + t*dir;
    }

public:
    point3 orig;
    vec3 dir;
    double tm; // time 在计算与物体是否相交时用上
};

#endif //RAYTRACE_RAY_H
