project('BezierCurve',
        'cpp',
        default_options : 'buildtype=release',
        license : 'MIT',
        version : '2')


# Dependencies
vapoursynth = dependency('vapoursynth')


# Sources
src = [
    'VapourSynth-BezierCurve/BezierCurve.cpp',
    'VapourSynth-BezierCurve/CubicBezierCurve.cpp',
    'VapourSynth-BezierCurve/QuadraticBezierCurve.cpp',
    'VapourSynth-BezierCurve/VapourSynth-BezierCurve.cpp']


# Libs
library(
    'beziercurve',
    src,
    dependencies : [vapoursynth],
    install_dir : join_paths(get_option('prefix'), get_option('libdir'), 'vapoursynth'),
    install : true
)
