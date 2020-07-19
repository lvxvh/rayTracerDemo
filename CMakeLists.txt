cmake_minimum_required(VERSION 3.15)
project(raytrace)

set(CMAKE_CXX_STANDARD 14)

add_executable(raytrace main.cpp vec3.h color.h ray.h hittable.h sphere.h hittable_list.h rtweekend.h camera.h material.h)