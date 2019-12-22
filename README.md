# VapourSynth-BezierCurve 
VapourSynth-BezierCurve is a plugin that adjusts the clip via a bezier curve. 

## Description 
Vapoursynth-BezierCurve is a plugin that allows you to adjust a clip by specifying a quadratic or a cubic bezier curve.  <br />
The curve is specified by giving the values of begin point, end point and coordinates of control points. <br />
It's suited for adjusting the mask clips such as sharp-weight or nr-weight in a easy and customized way without going
deep into mathematics.

## Supported Formats
8~16 bit integer YUV or GRAY clip. 

## Usage
```python
bezier.Quadratic(clip, accur, input_range, begin, end, x1, y1, planes)
bezier.Cubic(clip, accur, input_range, begin, end, x1, y1, x2, y2, planes)
```

## Parameters
* clip:  a 8~16 bit integer YUV/GRAY clip.

* accur:  accuracy of the bezier curve. Smaller values result in a more accurate curve. 

	* This parameter affects mostly on initializing speed and affects little on processing speed.
	
	* The default value (0.01) is accurate enough to process most of the mask clips.
	
	* It's recommended to use a value smaller than 0.001 to process ordinary clips to avoid banding.

* input_range:  specify the range of input clip.
	
	* 0 - Full range (PC range)
	
	* 1 - Limited range (TV range)

* begin:  begin values of the curve.

* end:  end values of the curve.

* x1, y1, x2, y2:  the coordinates of control points.

* planes: specify which plane to be processed.

Note: All values related to curve are in scale of clip's bit-depth.
