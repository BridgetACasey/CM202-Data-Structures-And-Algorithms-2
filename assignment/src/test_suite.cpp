//@BridgetACasey

#include "test_suite.h"

#include <thread>
#include <chrono>

typedef std::chrono::steady_clock chrono_clock;

TestSuite::TestSuite()
{
	averageSetupTime = 0;
	averageRunTime = 0;
}

TestSuite::~TestSuite()
{

}

void TestSuite::setupMandelbrotQueue(ImageCoordinates& imageCoordinates, int maxItrs, int threadsUsed)
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

void TestSuite::testMandelbrot(ImageCoordinates& imageCoordinates, int testIterations, int threadsUsed, const char* fileName)
{
	std::ofstream writeFile(fileName, std::ofstream::trunc);	//Open new file and overwrite any pre-existing contents

	std::vector<float> totalSetupTimes;
	std::vector<float> totalRunTimes;

	chrono_clock::time_point startTime;
	chrono_clock::time_point endTime;

	float timeTaken = 0;
	averageSetupTime = 0;
	averageRunTime = 0;

	for (int i = 0; i < testIterations; ++i)
	{
		std::vector<std::thread*> threads;

		setupMandelbrotQueue(imageCoordinates, 500, threadsUsed);	//Load strips into mandelbrot queue

		//Calculate the time taken when generating the mandelbrot image
		startTime = chrono_clock::now();

			for (int j = 0; j < threadsUsed; ++j)
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

				if (j == (threadsUsed - 1))	//Check if thread setup is complete, then measure the time taken before Mandelbrot calculations begin
				{
					endTime = chrono_clock::now();
				
					timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
				
					totalSetupTimes.push_back(timeTaken);
				
					writeFile << "Test No:	," << j + 1 << ", Max Itrs: ," << imageCoordinates.maxIterations << ",Setup Time: ," << timeTaken << ",ms";
				
					startTime = chrono_clock::now();
				}
			}

			for (int k = 0; k < threadsUsed; ++k)
			{
				threads[k]->join();
			}

		//Measure the time to process the parallelised Mandelbrot image and write to .csv file
		endTime = chrono_clock::now();

		timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

		totalRunTimes.push_back(timeTaken);

		writeFile << ",Run Time: ," << timeTaken << ",ms" << std::endl;
	}

	//Work out the average run time across all test iterations
	for (float setupTime : totalSetupTimes)
	{
		averageSetupTime += setupTime;
	}
	
	for (float runTime : totalRunTimes)
	{
		averageRunTime += runTime;
	}

	averageSetupTime /= testIterations;
	averageRunTime /= testIterations;

	writeFile << "Avg Setup Time: ," << averageSetupTime << ",ms" << ",Avg Run Time: ," << averageRunTime << ",ms" << std::endl;
}