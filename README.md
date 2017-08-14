# VapourSynth-BezierCurve 
A plugin which adjust the clip via bezier curve. 

## Description 
BezierCurve is a plugin which allow you to adjust
a clip by specifying a quadratic or a cubic bezier curve. <br />
The curve is specified by giving the values of begin point, end point and coordinates of anchor points. <br />
It's suited for ajusting the mask clips such as sharp-weight or nr-weight in a easy and customized way without going
deep into mathematics.

## Supported Formats
8bit or 16bit integer YUV or GRAY clip. 

## Usage
```python
core.bezier.Quadratic(clip, accur, range, begin, end, x1, y1)
core.bezier.Cubic(clip, accur, range, begin, end, x1, y1, x2, y2)
```

## Parameters
- clip:  a 8bit or 16bit integer YUV clip.
- accur:  accuracy of the bezier curve. Smaller values result in a more accurate curve. <br />
Note that the accur only affect initializing speed and affect little on processing speed.
- range:  specify the range of input clip.  0 - PC range,  1 - TV range.
- begin:  begin values of the curve.
- end:  end values of the curve.
- x1, y1, x2, y2:  the coordinates of anchors.
