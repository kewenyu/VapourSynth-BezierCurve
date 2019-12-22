/*
MIT License

Copyright (c) 2017 kewenyu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _BezierCurve
#define _BezierCurve

#include <VapourSynth.h>
#include <VSHelper.h>


typedef struct {
	VSNodeRef* node;
	const VSVideoInfo* vi;

	int begin, end, x1, y1;
	int range;
	double accur;
	bool process[3];

	int* lutTable;
} quadraticBezierCurveData;


typedef struct {
	VSNodeRef* node;
	const VSVideoInfo* vi;

	int begin, end, x1, x2, y1, y2;
	int range;
	double accur;
	bool process[3];

	int* lutTable;
} cubicBezierCurveData;


class BezierCurve {
protected:
	int m_range;
	int m_bits;
	double m_accur;

public:
	BezierCurve(int range, double accur, int bits);
	double normalize(int x);
};


class QuadraticBezierCurve : public BezierCurve {
private:
	int m_begin;
	int m_end;
	double m_x1;
	int m_y1;

public:
	QuadraticBezierCurve(int range, double accur, int bits, int begin, int end, int x1, int y1);
	double bezierX(double t);
	double bezierT(double x);
	double bezierY(double t);
};


class CubicBezierCurve : public BezierCurve {
private:
	int m_begin;
	int m_end;
	double m_x1;
	double m_x2;
	int m_y1;
	int m_y2;

public:
	CubicBezierCurve(int range, double accur, int bits, int begin, int end, int x1, int y1, int x2, int y2);
	double bezierX(double t);
	double bezierT(double x);
	double bezierY(double t);
};

#endif // !_BezierCurve
