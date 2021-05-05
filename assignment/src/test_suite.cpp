//@BridgetACasey

#include "test_suite.h"

#include <windows.h>
#include <psapi.h>
#include <thread>
#include <chrono>

typedef std::chrono::steady_clock chrono_clock;

int totalCPUs = 0;	//Total available processors

ULARGE_INTEGER lastCPU;
ULARGE_INTEGER lastSystemCPU;
ULARGE_INTEGER lastUserCPU;

HANDLE selfHandle;

TestSuite::TestSuite()
{
	averageSetupTime = 0.0f;
	averageCalcTime = 0.0f;

	averageCPUsUsed = 0.0;
	averageVirtualMemory = 0.0;

	setSystemInfo();
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
	std::vector<float> totalCalcTimes;
	std::vector<double> totalUsageCPU;
	std::vector<double> totalUsageVirtualMemory;

	chrono_clock::time_point startTime;
	chrono_clock::time_point endTime;

	//Resetting average values for new test run
	averageSetupTime = 0.0f;
	averageCalcTime = 0.0f;

	averageCPUsUsed = 0.0;
	averageVirtualMemory = 0.0;

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

				if (j == (threadsUsed - 1))	//Check if thread setup is complete
				{
					endTime = chrono_clock::now();
				
					//Take key measurements before parallelised Mandelbrot calculations begin
					float setupTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
					setupTime /= 1000000.0f;	//Converting to milliseconds
					totalSetupTimes.push_back(setupTime);

					double cpuUsed = getCurrentProcessCPU();
					totalUsageCPU.push_back(cpuUsed);

					double memoryUsed = getCurrentProcessVirtualMemory();
					memoryUsed /= 1024.0;	//Converting to kilobytes
					memoryUsed /= 1024.0;	//Converting to megabytes
					totalUsageVirtualMemory.push_back(memoryUsed);
				
					//Write current results to .csv file
					writeFile << "Test Itr:," << i << ",CPU Usage:," << cpuUsed << ",%" << ",VM Usage:," << memoryUsed << ",MB"
						<< ",Setup Time:," << setupTime << ",ms";
				
					//Restart the timer and let the threads run
					startTime = chrono_clock::now();
				}
			}

			for (int k = 0; k < threadsUsed; ++k)
			{
				threads[k]->join();
			}

		//Measure the time to process the parallelised Mandelbrot image and write to .csv file
		endTime = chrono_clock::now();

		float calcTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		calcTime /= 1000000.0f; //Converting to milliseconds
		totalCalcTimes.push_back(calcTime);

		writeFile << ",Calc Time:," << calcTime << ",ms" << std::endl;
	}

	//Work out the averages across all test iterations
	for (float setupTime : totalSetupTimes)
	{
		averageSetupTime += setupTime;
	}
	
	for (float calcTime : totalCalcTimes)
	{
		averageCalcTime += calcTime;
	}

	for (double usedCPU : totalUsageCPU)
	{
		averageCPUsUsed += usedCPU;
	}

	for (double usedVirtualMemory : totalUsageVirtualMemory)
	{
		averageVirtualMemory += usedVirtualMemory;
	}

	averageSetupTime /= testIterations;
	averageCalcTime /= testIterations;
	averageCPUsUsed /= testIterations;
	averageVirtualMemory /= testIterations;

	writeFile << std::endl;

	//Write the final averages to the .csv file
	writeFile << "Total Tests:," << testIterations << ",Avg CPU Usage:," << averageCPUsUsed << ",%" << ",Avg VM Usage:," << averageVirtualMemory
		<< ",MB" << ",Avg Setup Time:," << averageSetupTime << ",ms" << ",Avg Calc Time:,"<< averageCalcTime << ",ms" << std::endl;

	writeFile.close();
}

void TestSuite::setSystemInfo()
{
	//Retrieving and setting initial time data for CPU usage calculations
	SYSTEM_INFO systemInfo;

	FILETIME timeFile;
	FILETIME systemFile;
	FILETIME userFile;

	GetSystemInfo(&systemInfo);
	totalCPUs = systemInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&timeFile);
	memcpy(&lastCPU, &timeFile, sizeof(FILETIME));

	selfHandle = GetCurrentProcess();

	GetProcessTimes(selfHandle, &timeFile, &timeFile, &systemFile, &userFile);

	memcpy(&lastSystemCPU, &systemFile, sizeof(FILETIME));
	memcpy(&lastUserCPU, &userFile, sizeof(FILETIME));
}

double TestSuite::getCurrentProcessCPU()
{
	//Percentage of CPUs being actively used by the current process
	double cpuPercentage = 0.0;

	FILETIME currentFile;
	FILETIME systemFile;
	FILETIME userFile;

	ULARGE_INTEGER current;
	ULARGE_INTEGER system;
	ULARGE_INTEGER user;

	GetSystemTimeAsFileTime(&currentFile);

	memcpy(&current, &currentFile, sizeof(FILETIME));

	//Retrieving and storing timing information for the current process
	GetProcessTimes(selfHandle, &currentFile, &currentFile, &systemFile, &userFile);

	memcpy(&system, &systemFile, sizeof(FILETIME));
	memcpy(&user, &userFile, sizeof(FILETIME));

	//Calculating percentage of CPUs used relative to the max available
	cpuPercentage = (system.QuadPart - lastSystemCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart);
	cpuPercentage /= (current.QuadPart - lastCPU.QuadPart);
	cpuPercentage /= totalCPUs;

	lastCPU = current;
	lastUserCPU = user;
	lastSystemCPU = system;

	cpuPercentage *= 100.0;

	return cpuPercentage;
}

unsigned long TestSuite::getCurrentProcessVirtualMemory()
{
	//Amount of virtual memory in bytes used by the current process
	PROCESS_MEMORY_COUNTERS_EX processMemoryCounters{};
	SIZE_T virtualMemoryUsed = 0;

	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&processMemoryCounters, sizeof(processMemoryCounters));

	virtualMemoryUsed = processMemoryCounters.PrivateUsage;

	return virtualMemoryUsed;
}