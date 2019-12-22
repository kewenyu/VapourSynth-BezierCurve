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

#include "BezierCurve.h"


BezierCurve::BezierCurve(int range, double accur, int bits)
{
	m_range = range;
	m_accur = accur;
	m_bits = bits;
}

double BezierCurve::normalize(int x)
{
	double result;
	int scale = ((1 << m_bits) - 1) / 255;

	if (m_range == 0)  // PC range
	{
		result = x / (255. * scale);
	}
	else  // TV range
	{
		if (x < 16 * scale)
			result = 0;
		else if (x > 235 * scale)
			result = 1;
		else
			result = (x - 16 * scale) / (219. * scale);
	}

	return result;
}
