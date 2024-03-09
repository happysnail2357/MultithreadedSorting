/**
*  seqQuickSort.cpp
*
*  Defines the sequential Quick Sort function
*/

#include "seqSorts.hpp"
#include <vector>

int partition(std::vector<int32_t>& arr, int low, int high);
void quickSort(std::vector<int32_t>& arr, int low, int high);
void seqQuickSort(std::vector<int32_t>* arr);

void seqQuickSort(std::vector<int32_t>* arr)
{
	if(arr == nullptr || arr->empty()) 
	{
		return;
	}

	quickSort(*arr,0, arr->size() - 1);
}

int partition(std::vector<int32_t>& arr, int low, int high)
{
	int32_t pivot = arr[high];

	int32_t i = low-1;

	for(int j = low; j <= high - 1; j++) 
	{
		if(arr[j] < pivot) 
		{
			i += 1;
			std::swap(arr[i],arr[j]);
		}
	}
	i += 1; 
	std::swap(arr[i], arr[high]);

	return i;
}

void quickSort(std::vector<int32_t>& arr, int low, int high)
{
	if(low >= high) 
	{
		return;
	}

	int p = partition(arr, low, high);

	quickSort(arr, low, p-1);
	quickSort(arr, p+1,  high);
}
