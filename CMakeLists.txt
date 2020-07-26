cmake_minimum_required(VERSION 3.15)
project(raytrace)

set(CMAKE_CXX_STANDARD 14)

add_executable(raytrace main.cpp src/utils/vec3.h src/utils/color.h ray.h src/geometries/hittable.h src/geometries/sphere.h src/geometries/hittable_list.h src/utils/rtweekend.h src/view/camera.h src/textures/material.h src/geometries/moving_sphere.h src/bvh/aabb.h src/bvh/bvh.h src/textures/texture.h src/textures/perlin.h src/utils/rtw_stb_image.h src/external/stb_image.h src/geometries/aarect.h src/geometries/box.h src/geometries/constant_medium.h)