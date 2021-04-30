#include "mandelbrot.h"

#include <vector>
#include <thread>

uint32_t image[HEIGHT][WIDTH];	//Mandelbrot image array

Mandelbrot::Mandelbrot(ImageCoordinates* coords, ImageDimensions* dims)
{
	coordinates = *coords;
	dimensions = *dims;
}

Mandelbrot::~Mandelbrot()
{

}

void Mandelbrot::compute()
{
	for (int y = dimensions.minY; y < dimensions.maxY; ++y)
	{
		for (int x = dimensions.minX; x < dimensions.maxX; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			std::complex<double> c(coordinates.left + (x * (coordinates.right - coordinates.left) / WIDTH),
				coordinates.top + (y * (coordinates.bottom - coordinates.top) / HEIGHT));

			// Start off z at (0, 0).
			std::complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < coordinates.maxIterations)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == coordinates.maxIterations)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				std::unique_lock<std::mutex> lock(imageMutex);

				//image[y][x] = 0x000000; // black
				image[y][x] = (16 << iterations) + (8 << iterations) + (iterations);
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				std::unique_lock<std::mutex> lock(imageMutex);

				//image[y][x] = 0xFFFFFF; // white
				image[y][x] = (16 << iterations) + (8 << iterations) + (iterations);
			}
		}
	}
}