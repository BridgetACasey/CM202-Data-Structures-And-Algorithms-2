//@BridgetACasey

#include "test_suite.h"

#include <thread>
#include <chrono>

typedef std::chrono::steady_clock chrono_clock;

TestSuite::TestSuite()
{
	averageTimeTaken = 0;
}

TestSuite::~TestSuite()
{

}

void TestSuite::setupMandelbrot(ImageCoordinates& imageCoordinates, int maxItrs, int threadsUsed)
{
	ImageDimensions dims;
	ImageCoordinates coords;

	coords = imageCoordinates;

	coords.maxIterations = maxItrs;

	//Setting the parameters for each image strip relative to the number of assigned threads
	for (int i = 0; i < threadsUsed; ++i)
	{
		dims.minX = 0;
		dims.maxX = WIDTH;

		dims.minY = i * (HEIGHT / threadsUsed);
		dims.maxY = dims.minY + (HEIGHT / threadsUsed);

		mandelbrotQueue.push(new Mandelbrot(&coords, &dims));
	}
}

void TestSuite::runMandelbrot(int threadsUsed)
{
	std::vector<std::thread*> threads;
	
	for (int i = 0; i < threadsUsed; ++i)
	{
		threads.push_back(new std::thread([&]
		{
			std::unique_lock<std::mutex> lock(mandelbrotMutex);	//Check if any image strips remain in queue, otherwise move to calculation
			queueCondition.wait(lock, [this]() { return !mandelbrotQueue.empty(); });

			Mandelbrot* current = mandelbrotQueue.front();	//Assign the topmost strip to this thread
			mandelbrotQueue.pop();

			lock.unlock();
			queueCondition.notify_all();

			current->compute();	//Calculate the given strip of the mandelbrot image

			delete current;
		}));
	}

	for (int i = 0; i < threadsUsed; ++i)
	{
		threads[i]->join();
	}
}

void TestSuite::testMandelbrot(ImageCoordinates& imageCoordinates, int iterations, int threadsUsed, const char* fileName)
{
	std::ofstream writeFile(fileName);

	std::vector<int> totalTimes;

	averageTimeTaken = 0;

	for (int i = 0; i < iterations; ++i)
	{
		setupMandelbrot(imageCoordinates, 500, threadsUsed);	//Load strips into mandelbrot queue

		chrono_clock::time_point startTime = chrono_clock::now();

		runMandelbrot(threadsUsed);

		//Calculate the time taken when generating the mandelbrot image
		chrono_clock::time_point endTime = chrono_clock::now();

		auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

		totalTimes.push_back(timeTaken);

		writeFile << "Test No:	," << i + 1 << ", Max Itrs: ," << imageCoordinates.maxIterations << ",Time: ," << timeTaken << ",ms" << std::endl;
	}

	for (int t : totalTimes)
	{
		averageTimeTaken += t;
	}

	averageTimeTaken /= iterations;

	writeFile << "Avg Time: ," << averageTimeTaken << ",ms" << std::endl;
}