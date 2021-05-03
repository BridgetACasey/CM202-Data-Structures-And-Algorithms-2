//@BridgetACasey

#pragma once

#include <queue>

#include "mandelbrot.h"

class TestSuite
{
public:
	TestSuite();
	~TestSuite();

	void testMandelbrot(ImageCoordinates& imageCoordinates, int iterations, int threadsUsed, const char* fileName);

	inline int getAverageTimeTaken() { return averageTimeTaken; }

private:
	void setupMandelbrot(ImageCoordinates& imageCoordinates, int maxItrs, int threadsUsed);
	void runMandelbrot(int threadsUsed);

	std::mutex mandelbrotMutex;
	std::queue<Mandelbrot*> mandelbrotQueue;
	std::condition_variable queueCondition;

	int averageTimeTaken;
};