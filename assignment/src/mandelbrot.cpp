#include "mandelbrot.h"

#include <vector>

const int WIDTH = 1600;
const int HEIGHT = 900;

const int MAX_ITERATIONS = 500;

uint32_t image[HEIGHT][WIDTH];

Mandelbrot::Mandelbrot()
{

}

Mandelbrot::~Mandelbrot()
{

}

// Render the Mandelbrot set into the image array.
// The parameters specify the region on the complex plane to plot.
void Mandelbrot::compute(double left, double right, double top, double bottom)
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			std::complex<double> c(left + (x * (right - left) / WIDTH),
				top + (y * (bottom - top) / HEIGHT));

			// Start off z at (0, 0).
			std::complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == MAX_ITERATIONS)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				//image[y][x] = 0x000000; // black
				image[y][x] = (16 << iterations) + (8 << iterations) + (iterations);
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				//image[y][x] = 0xFFFFFF; // white
				image[y][x] = (16 << iterations) + (8 << iterations) + (iterations);
			}
		}
	}
}

void Mandelbrot::assignPixels()
{
	std::vector<sf::Uint8> pixels;

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			sf::Uint8 pixel[4] =
			{
				image[y][x] & 0xFF,			// blue channel
				(image[y][x] >> 8) & 0xFF,	// green channel
				(image[y][x] >> 16) & 0xFF, // red channel
				255							// alpha channel
			};

			pixels.push_back(pixel[2]);
			pixels.push_back(pixel[1]);
			pixels.push_back(pixel[0]);
			pixels.push_back(pixel[3]);
		}
	}

	displayImage.create(WIDTH, HEIGHT, pixels.data());
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