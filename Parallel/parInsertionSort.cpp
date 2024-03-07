/**
*  parInsertionSort.cpp
*
*  Defines the parallel Insertion Sort function
*/

#include "parSorts.hpp"

#include <thread>


// Reuse the merge function from merge sort
//
void merge(std::vector<int32_t> *arr, int32_t l, int32_t m, int32_t r);


// Sorts the values in 'arr' between the indices 'left' and 'right'
//
void insertionSort(std::vector<int32_t>* arr, int32_t left, int32_t right)
{
	for (int32_t i = left + 1; i <= right; i++)
	{
		int32_t curr = arr->at(i);
		
		int32_t j;
		
		for (j = i; j > left && arr->at(j - 1) > curr; j--)
		{
			arr->at(j) = arr->at(j - 1);
		}
		
		arr->at(j) = curr;
	}
}


// Splits 'arr' recursively to be sorted by multiple threads using insertionSort(). After each
// sub-array is sorted, they are merged together.
//
void splitWork(std::vector<int32_t>* arr, int32_t left, int32_t right, int32_t threadsRemaining)
{
	if (threadsRemaining)
	{
		threadsRemaining--;
		
		int32_t center = left + (right - left) / 2;
		
		std::thread twin = std::thread(splitWork, arr, center, right, threadsRemaining / 2);
		
		splitWork(arr, left, center - 1, threadsRemaining / 2);
		
		twin.join();
		
		merge(arr, left, center - 1, right);
	}
	else
	{
		insertionSort(arr, left, right);
	}
}


// Sorts an array of numbers using a parallel insertion sort
//
void parInsertionSort(std::vector<int32_t>* arr, int32_t numThreads)
{
	std::thread firstThread = std::thread(splitWork, arr, 0, arr->size() - 1, numThreads - 1);
	
	firstThread.join();
}

