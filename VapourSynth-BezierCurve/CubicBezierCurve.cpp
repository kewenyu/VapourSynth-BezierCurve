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

#include <cmath>

#include "BezierCurve.h"


CubicBezierCurve::CubicBezierCurve(int range, double accur, int bits, int begin, int end, int x1, int y1, int x2, int y2) :
	BezierCurve(range, accur, bits)
{
	m_begin = begin;
	m_end = end;
	m_x1 = normalize(x1);
	m_x2 = normalize(x2);
	m_y1 = y1;
	m_y2 = y2;
}

inline double CubicBezierCurve::bezierX(double t)
{
	return 3 * t * m_x1 * pow(1 - t, 2) + \
		3 * (1 - t) * m_x2 * pow(t, 2) + \
		pow(t, 3);
}

double CubicBezierCurve::bezierY(double t)
{
	return m_begin * pow(1 - t, 3) + \
		3 * t * m_y1 * pow(1 - t, 2) + \
		3 * (1 - t) * m_y2 * pow(t, 2) + \
		m_end * pow(t, 3);
}

double CubicBezierCurve::bezierT(double x)
{
	double t = 0;
	double absDiffLast = 2;  // Anything larger than 1

	while (t <= 1 + m_accur)
	{
		double xCalculated = bezierX(t);
		double absDiff = abs(xCalculated - x);

		if (absDiff - absDiffLast >= 0)
		{
			return t;
		}

		absDiffLast = absDiff;
		t += m_accur;
	}

	return 1;
}
