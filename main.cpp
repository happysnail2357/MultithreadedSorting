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



/*** Constants ***/

const char* usageStr = "\n Usage: sorttest [options...]\n\n -s             : Use sequential version of sorting algorithm\n -p             : Use parallel version of sorting algorithm\n -d --data      : Specify file name for input data\n -a --algorithm : Specify algorithm <bubble|insertion|merge|quick>\n -t --threads   : Specify number of threads to use for parallel sort\n -v --verify    : Verify that results are sorted\n    --help      : Show this message\n\n";

const int32_t MIN_NUM_THREADS = 2;
const int32_t MAX_NUM_THREADS = 100;
const int32_t DEFAULT_NUM_THREADS = 4;



/*** Data Structures ***/

enum class SortAlgorithm
{
	None,
	Bubble,
	Insertion,
	Merge,
	Quick
};

struct SortParameters
{
	std::string dataFile = "";
	SortAlgorithm algorithm{};
	int32_t numThreads = DEFAULT_NUM_THREADS;
	bool parallel{};
	bool verify{};
	
	std::vector<int32_t> data;
};

struct OutputInfo
{
	std::string timestamp;
	std::string stampedFilename;
	
	bool sortedCorrectly{};
	
	std::string runTime;
};



/*** Function Definitions ***/

// Parses the command line arguments and sets the values of 'param' appropriatly
//
void parseCommandLineArgs(int argc, char** argv, SortParameters* param)
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
			param->parallel = false;
		}
		else if (arg == "-p")
		{
			param->parallel = true;
		}
		else if (arg == "-d" || arg == "--data")
		{
			argi++;
			
			if (argi < argc)
			{
				param->dataFile = argv[argi];
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
					param->algorithm = SortAlgorithm::Bubble;
				else if (sort == "insertion")
					param->algorithm = SortAlgorithm::Insertion;
				else if (sort == "merge")
					param->algorithm = SortAlgorithm::Merge;
				else if (sort == "quick")
					param->algorithm = SortAlgorithm::Quick;
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
						param->numThreads = std::stoi(num);
						
						if (param->numThreads < MIN_NUM_THREADS)
						{
							std::cout << "\n   ERROR: Value for " << arg << " must be at least " << MIN_NUM_THREADS << "\n\n";
							exit(1);
						}
						else if (param->numThreads > MAX_NUM_THREADS)
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
			param->verify = true;
		}
		else
		{
			std::cout << "\n   ERROR: Unrecognized parameter: \"" << arg << "\"\n\n";
			exit(1);
		}
	}
}


// Opens 'fileName', reads integers, and places them into 'buffer' 
//
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


// Calls the correct sorting function based on the values in 'param'
//
void runSortingAlgorithm(SortParameters* param)
{
	switch (param->algorithm)
	{
	case SortAlgorithm::Bubble:
		
		if (param->parallel)
			parBubbleSort(&(param->data), param->numThreads);
		else
			seqBubbleSort(&(param->data));
		break;
	
	case SortAlgorithm::Insertion:
		
		if (param->parallel)
			parInsertionSort(&(param->data), param->numThreads);
		else
			seqInsertionSort(&(param->data));
		break;
	
	case SortAlgorithm::Merge:
		
		if (param->parallel)
			parMergeSort(&(param->data), param->numThreads);
		else
			seqMergeSort(&(param->data));
		break;
	
	case SortAlgorithm::Quick:
		
		if (param->parallel)
			parQuickSort(&(param->data), param->numThreads);
		else
			seqQuickSort(&(param->data));
		break;
	}
}


// Gets a string with the current time and date (uses filename safe characters)
//
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


// Creates a unique and descriptive filename using a timestamp and the sort parameters
//
std::string getTimestampedFilename(std::string timestamp, SortParameters* param)
{
	std::string file;
	
	switch (param->algorithm)
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
	
	file.append(((param->parallel) ? "par_" : "seq_"));
	
	file.append(timestamp);
	
	return file;
}


// Checks if the data in 'buffer' is sorted from smallest to largest
//
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


// Saves the integer data to a ".dump" file
//
void dumpToFile(std::vector<int32_t>* buffer, std::string outputFileName)
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


// Checks if the data was sorted correctly. If it is not, it calls dumpToFile()
//
bool verifyResults(std::vector<int32_t>* buffer, std::string outputFileName)
{
	std::cout << " Verifying... ";
		
	if (!isSorted(buffer))
	{
		std::cout << "\n\n   WARNING: Failed to sort test data. Dumping results to \"" << outputFileName << ".dump\"\n\n";
		
		dumpToFile(buffer, outputFileName);
		
		return false;
	}
	else
	{
		std::cout << "Done\n\n";
		
		return true;
	}
}


// Saves a ".report" file with the results of the sorting
//
void generateReport(SortParameters* param, OutputInfo* info)
{
	std::cout << " Saving report... ";
	
	/* Create report inside of string stream */
	
	std::stringstream reportStr;
	
	reportStr << "Timestamp         : " << info->timestamp << "\n";
	reportStr << "Test Data         : " << param->dataFile << "\n";
	reportStr << "Data Length       : " << param->data.size() << "\n";
	reportStr << "Sorting Algorithm : ";
	
	switch (param->algorithm)
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
	reportStr << "Parallel Version  : " << ((param->parallel) ? "yes" : "no") << "\n";
	
	if (param->parallel)
	{
		reportStr << "Number of Threads : " << param->numThreads << "\n";
	}
	
	reportStr << "Execution Time    : " << info->runTime << " seconds\n";
	reportStr << "Verification      : ";
	
	if (param->verify)
	{
		reportStr << ((info->sortedCorrectly) ? "Data was properly sorted" : "(ERROR) Data was not properly sorted");
	}
	else
	{
		reportStr << "Disabled";
	}
	
	reportStr << "\n\n";
	
	
	/* Write report to file or stdout */
	
	std::ofstream report;
	
	info->stampedFilename.append(".report");
	
	report.open(info->stampedFilename);
	
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


// Adds an entry to "log.csv"
//
void logInfo(SortParameters* param, OutputInfo* info)
{
	std::cout << " Saving to log... ";
	
	std::fstream log("log.csv", std::ios::in | std::ios::out | std::ios::app);
	
	if (log.is_open())
	{
		switch (param->algorithm)
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
		
		log << ((param->parallel) ? param->numThreads : 1) << "," << param->data.size() << "," << info->runTime << "\n";
		
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
	/* Obtain parameters from user */
	
	SortParameters param{};
	
	parseCommandLineArgs(argc, argv, &param);
	
	if (param.dataFile == "")
	{
		std::cout << "\n   ERROR: Input file not specified\n\n";
		exit(1);
	}
	else if (param.algorithm == SortAlgorithm::None)
	{
		std::cout << "\n   ERROR: Sorting algorithm not specified\n\n";
		exit(1);
	}
	
	
	/* Prepare Test Data */
	
	loadTestData(param.dataFile, &(param.data));
	
	
	/* Sort Test Data */
	
	std::cout << "\n *** Starting Sort ***\n";
	
	Stopwatch timer;
	timer.start();
	
	runSortingAlgorithm(&param);
	
	timer.stop();
	
	std::cout << "\n *** Sort complete ***\n\n";
	
	
	/* Get Execution Time */
	
	OutputInfo info{};
	
	info.runTime = timer.getFormattedTime();
	
	
	/* Generate Timestamp Info */
	
	info.timestamp = getTimestamp();
	info.stampedFilename = getTimestampedFilename(info.timestamp, &param);
	
	
	/* Verify Results */
	
	if (param.verify)
	{
		info.sortedCorrectly = verifyResults(&(param.data), info.stampedFilename);
	}
	
	generateReport(&param, &info);
	
	logInfo(&param, &info);
	
	return 0;
}

