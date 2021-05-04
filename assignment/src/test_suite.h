//@BridgetACasey

#pragma once

#include <queue>

#include "mandelbrot.h"

class TestSuite
{
public:
	TestSuite();
	~TestSuite();

	void testMandelbrot(ImageCoordinates& imageCoordinates, int testIterations, int threadsUsed, const char* fileName);

	inline float getAverageSetupTime() { return averageSetupTime; }
	inline float getAverageRunTime() { return averageRunTime; }

private:
	void setupMandelbrotQueue(ImageCoordinates& imageCoordinates, int maxItrs, int threadsUsed);

	std::mutex mandelbrotMutex;
	std::queue<Mandelbrot*> mandelbrotQueue;
	std::condition_variable queueCondition;

	float averageSetupTime;
	float averageRunTime;
};