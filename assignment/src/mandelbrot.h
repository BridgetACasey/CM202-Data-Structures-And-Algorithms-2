#pragma once

#include <complex>
#include <fstream>

#include <SFML/Graphics/Image.hpp>

class Mandelbrot
{
public:
	Mandelbrot();
	~Mandelbrot();

	void compute(double left, double right, double top, double bottom);

	void assignPixels();

	void writeToTGA(const char* fileName);

	inline sf::Image& getImage() { return displayImage; }

private:
	sf::Image displayImage;
};