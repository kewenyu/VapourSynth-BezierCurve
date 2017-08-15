#include <cmath>

#include "BezierCurve.h"


QuadraticBezierCurve::QuadraticBezierCurve(int range, double accur, int bits, int begin, int end, int x1, int y1) :
	BezierCurve(range, accur, bits)
{
	m_begin = begin;
	m_end = end;
	m_x1 = normalize(x1);
	m_y1 = y1;
}

inline double QuadraticBezierCurve::bezierX(double t)
{
	return 2 * (1 - t) * t * m_x1 + pow(t, 2);
}

double QuadraticBezierCurve::bezierY(double t)
{
	return m_begin * pow(1 - t, 2) + \
		2 * (1 - t) * t * m_y1 + \
		m_end * pow(t, 2);
}

double QuadraticBezierCurve::bezierT(double x)
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
