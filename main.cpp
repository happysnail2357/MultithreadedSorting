/**
*  main.cpp
*
*  Defines the entry point of the program
*/

#include "Sequential/seqSorts.hpp"
#include "Parallel/parSorts.hpp"
#include "Stopwatch.hpp"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>



enum class SortAlgorithm
{
	None,
	Bubble,
	Insertion,
	Merge,
	Quick
};

const char* usageStr = "\n Usage: sorttest [options...]\n\n -s             : Use sequential version of sorting algorithm\n -p             : Use parallel version of sorting algorithm\n -d --data      : Specify file name for input data\n -a --algorithm : Specify algorithm <bubble|insertion|merge|quick>\n -t --threads   : Specify number of threads to use for parallel sort\n -v --verify    : Verify that results are sorted\n    --help      : Show this message\n\n";


const int32_t MIN_NUM_THREADS = 2;
const int32_t MAX_NUM_THREADS = 100;
const int32_t DEFAULT_NUM_THREADS = 4;


void parseCommandLineArgs(int argc, char** argv, std::string* dataFile, SortAlgorithm* algo, int32_t* numThreads, bool* parallel, bool* verify)
{
	if (argc == 1)
	{
		std::cout << usageStr;
		exit(0);
	}
	
	for (int argi = 1; argi < argc; argi++)
	{
		std::string arg = argv[argi];
		
		if (arg == "--help")
		{
			std::cout << usageStr;
			exit(0);
		}
		else if (arg == "-s")
		{
			*parallel = false;
		}
		else if (arg == "-p")
		{
			*parallel = true;
		}
		else if (arg == "-d" || arg == "--data")
		{
			argi++;
			
			if (argi < argc)
			{
				*dataFile = argv[argi];
			}
			else
			{
				std::cout << "\n   ERROR: Missing value for " << arg << "\n\n";
				exit(1);
			}
		}
		else if (arg == "-a" || arg == "--algorithm")
		{
			argi++;
			
			if (argi < argc)
			{
				std::string sort = argv[argi];
				
				if (sort == "bubble")
					*algo = SortAlgorithm::Bubble;
				else if (sort == "insertion")
					*algo = SortAlgorithm::Insertion;
				else if (sort == "merge")
					*algo = SortAlgorithm::Merge;
				else if (sort == "quick")
					*algo = SortAlgorithm::Quick;
				else
				{
					std::cout << "\n   ERROR: Unrecognized value for " << arg <<"\n\n";
					exit(1);
				}
			}
			else
			{
				std::cout << "\n   ERROR: Missing value for " << arg << "\n\n";
				exit(1);
			}
		}
		else if (arg == "-t" || arg == "--threads")
		{
			argi++;
			
			if (argi < argc)
			{
				std::string num = argv[argi];
				
				if (num.find_first_not_of("0123456789") == std::string::npos)
				{
					try
					{
						*numThreads = std::stoi(num);
						
						if (*numThreads < MIN_NUM_THREADS)
						{
							std::cout << "\n   ERROR: Value for " << arg << " must be at least " << MIN_NUM_THREADS << "\n\n";
							exit(1);
						}
						else if (*numThreads > MAX_NUM_THREADS)
						{
							std::cout << "\n   ERROR: Value for " << arg << " must be no larger than " << MAX_NUM_THREADS << "\n\n";
							exit(1);
						}
					}
					catch (std::invalid_argument const& e)
					{
						std::cout << "\n   ERROR: Invalid value for " << arg << "\n\n";
						exit(1);
					}
					catch (std::out_of_range const& e)
					{
						std::cout << "\n   ERROR: Value for " << arg << " too large for 32-bit integer\n\n";
						exit(1);
					}
					catch (std::exception const& e)
					{
						std::cout << "\n   ERROR: " << e.what() << "\n\n";
						exit(1);
					}
				}
				else
				{
					std::cout << "\n   ERROR: Value for " << arg << " contains non-digit characters\n\n";
					exit(1);
				}
			}
			else
			{
				std::cout << "\n   ERROR: Missing value for " << arg << "\n\n";
				exit(1);
			}
		}
		else if (arg == "-v" || arg == "--verify")
		{
			*verify = true;
		}
		else
		{
			std::cout << "\n   ERROR: Unrecognized parameter: \"" << arg << "\"\n\n";
			exit(1);
		}
	}
}


void loadTestData(std::string fileName, std::vector<int32_t>* buffer)
{
	std::ifstream dataFile;
	
	dataFile.open(fileName);
	
	if (dataFile.is_open())
	{
		int32_t next;
		
		while (dataFile >> next)
		{
			buffer->push_back(next);
		}
		
		if (!dataFile.eof())
		{
			dataFile.close();
			
			std::cout << "\n   ERROR: Failure occured while reading from \"" << fileName << "\"\n\n";
			
			exit(2);
		}
		
		dataFile.close();
	}
	else
	{
		std::cout << "\n   ERROR: Cannot open file \"" << fileName << "\"\n\n";
		
		exit(2);
	}
}


void runSortingAlgorithm(SortAlgorithm algorithm, std::vector<int32_t>* buffer, bool parallelVersion, int32_t numThreads)
{
	switch (algorithm)
	{
	case SortAlgorithm::Bubble:
		
		if (parallelVersion)
			parBubbleSort(buffer, numThreads);
		else
			seqBubbleSort(buffer);
		break;
	
	case SortAlgorithm::Insertion:
		
		if (parallelVersion)
			parInsertionSort(buffer, numThreads);
		else
			seqInsertionSort(buffer);
		break;
	
	case SortAlgorithm::Merge:
		
		if (parallelVersion)
			parMergeSort(buffer, numThreads);
		else
			seqMergeSort(buffer);
		break;
	
	case SortAlgorithm::Quick:
		
		if (parallelVersion)
			parQuickSort(buffer, numThreads);
		else
			seqQuickSort(buffer);
		break;
	}
}


bool isSorted(std::vector<int32_t>* buffer)
{
	for (int i = 0; i < buffer->size() - 1; i++)
	{
		if (buffer->at(i) > buffer->at(i + 1))
		{
			return false;
		}
	}

	return true;
}


void dumpToFile(std::string outputFileName, std::vector<int32_t>* buffer)
{
	outputFileName.append(".dump");
	
	std::ofstream dump;
	
	dump.open(outputFileName);
	
	if (dump.is_open())
	{
		for (int i = 0; i < buffer->size(); i++)
		{
			dump << buffer->at(i) << " ";
		}
		
		dump.close();
	}
	else
	{
		std::cout << "   ERROR: Cannot create file \"" << outputFileName << "\"\n\n";
	}
}


void generateReport(std::string outputFileName, std::string inputFileName, SortAlgorithm algorithm, bool parallel, int32_t numThreads, int32_t dataSize, std::string executionTime, std::string dateTime, bool wasVerified, bool verifyStatus)
{
	std::cout << " Saving report... ";
	
	/* Create report inside of string stream */
	
	std::stringstream reportStr;
	
	reportStr << "Timestamp         : " << dateTime << "\n";
	reportStr << "Test Data         : " << inputFileName << "\n";
	reportStr << "Data Length       : " << dataSize << "\n";
	reportStr << "Sorting Algorithm : ";
	
	switch (algorithm)
	{
	case SortAlgorithm::Bubble:
		
		reportStr << "Bubble Sort";
		break;
	
	case SortAlgorithm::Insertion:
		
		reportStr << "Insertion Sort";
		break;
	
	case SortAlgorithm::Merge:
		
		reportStr << "Merge Sort";
		break;
	
	case SortAlgorithm::Quick:
		
		reportStr << "Quick Sort";
		break;
	}
	
	reportStr << "\n";
	reportStr << "Parallel Version  : " << ((parallel) ? "yes" : "no") << "\n";
	
	if (parallel)
	{
		reportStr << "Number of Threads : " << numThreads << "\n";
	}
	
	reportStr << "Execution Time    : " << executionTime << " seconds\n";
	reportStr << "Verification      : ";
	
	if (wasVerified)
	{
		reportStr << ((verifyStatus) ? "Data was properly sorted" : "(ERROR) Data was not properly sorted");
	}
	else
	{
		reportStr << "Disabled";
	}
	
	reportStr << "\n\n";
	
	
	/* Write report to file or stdout */
	
	std::ofstream report;
	
	outputFileName.append(".report");
	
	report.open(outputFileName);
	
	if (report.is_open())
	{
		report << reportStr.str();
		
		report.close();
		
		std::cout << "Done\n\n";
	}
	else
	{
		std::cout << "   ERROR: Failed to create report. Printing results to stdout...\n\n";
		
		std::cout << reportStr.str();
	}
}


bool verifyResults(std::vector<int32_t>* buffer, std::string outputFile)
{
	std::cout << " Verifying... ";
		
	if (!isSorted(buffer))
	{
		std::cout << "\n\n   WARNING: Failed to sort test data. Dumping results to \"" << outputFile << ".dump\"\n\n";
		
		dumpToFile(outputFile, buffer);
		
		return false;
	}
	else
	{
		std::cout << "Done\n\n";
		
		return true;
	}
}


std::string getTimestamp()
{
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	
	std::stringstream timestamp;
	
	timestamp	<< (now->tm_mon + 1) << '-'
				<<  now->tm_mday << '-'
				<< (now->tm_year + 1900) << '_'
				<<  now->tm_hour << '\''
				<<  now->tm_min  << '\''
				<<  now->tm_sec;
	
	return timestamp.str();
}

std::string getTimestampedFilename(std::string timestamp, SortAlgorithm algorithm, bool parallel)
{
	std::string file;
	
	switch (algorithm)
	{
	case SortAlgorithm::Bubble:
		
		file = "bubble_";
		break;
	
	case SortAlgorithm::Insertion:
		
		file = "insertion_";
		break;
	
	case SortAlgorithm::Merge:
		
		file = "merge_";
		break;
	
	case SortAlgorithm::Quick:
		
		file = "quick_";
		break;
	}
	
	file.append(((parallel) ? "par_" : "seq_"));
	
	file.append(timestamp);
	
	return file;
}


void logInfo(SortAlgorithm algorithm, int32_t numThreads, int32_t dataSize, std::string executionTime)
{
	std::cout << " Saving to log... ";
	
	std::fstream log("log.csv", std::ios::in | std::ios::out | std::ios::app);
	
	if (log.is_open())
	{
		switch (algorithm)
		{
		case SortAlgorithm::Bubble:
			
			log << "Bubble Sort,";
			break;
		
		case SortAlgorithm::Insertion:
			
			log << "Insertion Sort,";
			break;
		
		case SortAlgorithm::Merge:
			
			log << "Merge Sort,";
			break;
		
		case SortAlgorithm::Quick:
			
			log << "Quick Sort,";
			break;
		}
		
		log << numThreads << "," << dataSize << "," << executionTime << "\n";
		
		std::cout << "Done\n\n";
	}
	else
	{
		std::cout << "   ERROR: Failed to open log.\n\n";
	}
}


/*** *** *** ENTRY POINT *** *** ***/

int main(int argc, char** argv)
{
	/* Obtain options from user */
	
	std::string dataFile = "";
	SortAlgorithm algorithm{};
	int32_t numThreads = DEFAULT_NUM_THREADS;
	bool parallel = false;
	bool verify = false;
	
	parseCommandLineArgs(argc, argv, &dataFile, &algorithm, &numThreads, &parallel, &verify);
	
	if (dataFile == "")
	{
		std::cout << "\n   ERROR: Input file not specified\n\n";
		exit(1);
	}
	else if (algorithm == SortAlgorithm::None)
	{
		std::cout << "\n   ERROR: Sorting algorithm not specified\n\n";
		exit(1);
	}
	
	
	/* Prepare Test Data */
	
	std::vector<int32_t> data;
	
	loadTestData(dataFile, &data);
	
	
	/* Sort Test Data */
	
	std::cout << "\n *** Starting Sort ***\n";
	
	Stopwatch timer;
	timer.start();
	
	runSortingAlgorithm(algorithm, &data, parallel, numThreads);
	
	timer.stop();
	
	std::cout << "\n *** Sort complete ***\n\n";
	
	
	/* Generate Timestamp Info */
	
	std::string timestamp = getTimestamp();
	std::string stampedFilename = getTimestampedFilename(timestamp, algorithm, parallel);
	
	
	/* Verify and Report */
	
	bool success = false;
	
	if (verify)
	{
		success = verifyResults(&data, stampedFilename);
	}
	
	std::string runTime = timer.getFormattedTime();
	
	generateReport(stampedFilename, dataFile, algorithm, parallel, numThreads, data.size(), runTime, timestamp, verify, success);
	
	logInfo(algorithm, ((parallel) ? numThreads : 1), data.size(), runTime);
	
	return 0;
}

