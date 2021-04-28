#include "mandelbrot.h"

#include <vector>
#include <thread>

extern uint32_t image[HEIGHT][WIDTH];

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

void Mandelbrot::writeToTGA(const char* fileName)
{
	std::ofstream outfile(fileName, std::ofstream::binary);

	uint8_t header[18] =
	{
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};

	outfile.write((const char*)header, 18);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			uint8_t pixel[3] =
			{
				image[y][x] & 0xFF, // blue channel
				(image[y][x] >> 8) & 0xFF, // green channel
				(image[y][x] >> 16) & 0xFF, // red channel
			};

			outfile.write((const char*)pixel, 3);
		}
	}

	outfile.close();

	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		//cout << "Error writing to " << filename << endl;
		exit(1);
	}
}