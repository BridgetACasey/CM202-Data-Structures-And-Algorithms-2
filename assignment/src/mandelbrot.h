#pragma once

#include <complex>
#include <fstream>

class Mandelbrot
{
public:
	Mandelbrot();
	~Mandelbrot();

	void compute(double left, double right, double top, double bottom);

	void writeToTGA(const char* fileName);
};