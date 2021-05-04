//@BridgetACasey

#pragma once

#include <queue>

#include "mandelbrot.h"

//Manages how the Mandelbrot processes are executed and records key information for each run
class TestSuite
{
public:
	TestSuite();
	~TestSuite();

	void testMandelbrot(ImageCoordinates& imageCoordinates, int testIterations, int threadsUsed, const char* fileName);

	inline float getAverageSetupTime() { return averageSetupTime; }
	inline float getAverageCalcTime() { return averageCalcTime; }
	inline double getAverageCPU() { return averageCPUsUsed; }
	inline double getAverageVirtualMemory() { return averageVirtualMemory; }

private:
	void setupMandelbrotQueue(ImageCoordinates& imageCoordinates, int maxItrs, int threadsUsed);

	void setSystemInfo();

	double getCurrentProcessCPU();
	unsigned long getCurrentProcessVirtualMemory();

	std::mutex mandelbrotMutex;
	std::queue<Mandelbrot*> mandelbrotQueue;
	std::condition_variable queueCondition;

	float averageSetupTime;	//Time to create all the thread functions
	float averageCalcTime;	//Time to actually execute the thread functions and re-join the main thread
	double averageCPUsUsed;
	double averageVirtualMemory;
};