#include "MidpointCircle.h"

void AddCirclePixel(int x, int y)
{
	/**
	 *  Adds a pixel to the vector who's points are to be drawn.
	 */
	vertexPositions.push_back(x);
	vertexPositions.push_back(y);
	vertexPositions.push_back(0.0);
}

void CircleDraw(int xc, int yc, int r)
{
	/**
	 *  The Midpoint Circle Drawing algorithm.
	 */
	int x = 0, y = r;
	// initial decision parameter d
	int d = 1 - r;
	// to update d
	int deltaE = 3, deltaSE = -2 * r + 5;
	// initial point
	AddCirclePixel(x + xc, y + yc);

	while (y > x)
	{
		if (d < 0)
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else
		{
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;

		// add all 8 symmetries
		AddCirclePixel(x + xc, y + yc);
		AddCirclePixel(-x + xc, y + yc);
		AddCirclePixel(x + xc, -y + yc);
		AddCirclePixel(-x + xc, -y + yc);
		AddCirclePixel(y + xc, x + yc);
		AddCirclePixel(-y + xc, x + yc);
		AddCirclePixel(y + xc, -x + yc);
		AddCirclePixel(-y + xc, -x + yc);
	}
}