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

	return 0;
}
