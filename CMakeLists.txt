cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
set(PROJECT_NAME VapourSynth-BezierCurve)

project(${PROJECT_NAME})

include_directories("C:/Program Files (x86)/VapourSynth/sdk/include/vapoursynth")
include_directories(VapourSynth-BezierCurve)

link_directories("C:/Program Files (x86)/VapourSynth/sdk/lib32")
link_directories("C:/Program Files (x86)/VapourSynth/sdk/lib64")

add_library(${PROJECT_NAME} SHARED
        VapourSynth-BezierCurve/BezierCurve.cpp
        VapourSynth-BezierCurve/BezierCurve.h
        VapourSynth-BezierCurve/CubicBezierCurve.cpp
        VapourSynth-BezierCurve/QuadraticBezierCurve.cpp
        VapourSynth-BezierCurve/VapourSynth-BezierCurve.cpp)
