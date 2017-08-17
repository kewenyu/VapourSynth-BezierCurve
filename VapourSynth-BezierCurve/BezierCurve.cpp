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
