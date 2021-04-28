#pragma once

#include <complex>
#include <fstream>
#include <mutex>

const int WIDTH = 1600;
const int HEIGHT = 900;

struct ImageCoordinates
{
	//Default image values - will display the whole mandelbrot set
	double left = -2.0;
	double right = 1.0;
	double top = 1.125;
	double bottom = -1.125;

	int maxIterations = 500;
};

struct ImageDimensions
{
	int minX = 0;
	int minY = 0;
	int maxX = WIDTH;
	int maxY = HEIGHT;
};

class Mandelbrot
{
public:
	Mandelbrot(ImageCoordinates* coords, ImageDimensions* dims);
	~Mandelbrot();

	void compute();
	void writeToTGA(const char* fileName);

private:
	std::mutex imageMutex;

	ImageCoordinates coordinates;
	ImageDimensions dimensions;
};