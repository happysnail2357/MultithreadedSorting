/**
 *  parMergeSort.cpp
 *
 *  Defines the parallel Merge Sort function
 */

#include "parSorts.hpp"
#include <math.h>
#include <stdexcept>
#include <thread>

/**
 * @brief  Merges two subarrays of arr[]
 * @param  arr: The array to be sorted
 * @param  l: The left index of the first subarray
 * @param  m: The right index of the first subarray
 * @param  r: The right index of the second subarray
 */
void merge(std::vector<int32_t> *arr, int32_t l, int32_t m, int32_t r)
{
    int32_t i, j, k;
    int32_t n1 = m - l + 1;
    int32_t n2 = r - m;

    // create temp arrays
    std::vector<int32_t> L(n1), R(n2);

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr->at(l + i);
    for (j = 0; j < n2; j++)
        R[j] = arr->at(m + 1 + j);

    // Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr->at(k) = L[i];
            i++;
        }
        else
        {
            arr->at(k) = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1)
    {
        arr->at(k) = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2)
    {
        arr->at(k) = R[j];
        j++;
        k++;
    }
}

/**
 * @brief  Sorts an array using merge sort algorithm
 * @param  arr: The array to be sorted
 * @param  begin: The left index of the array
 * @param  end: The right index of the array
 */
void mergeSort(std::vector<int32_t> *arr, int32_t begin, int32_t end)
{
    // Base case
    if (begin >= end)
        return;

    // Sort the left and right halves of the array
    int32_t middle = begin + (end - begin) / 2;
    mergeSort(arr, begin, middle);
    mergeSort(arr, middle + 1, end);
    
    // Merge the sorted halves
    merge(arr, begin, middle, end);
}

/**
 * @author John Boyd
 * @brief  Sorts an array using a parallelize version of the merge sort algorithm
 * @param  arr: The array to be sorted
 * @param  numThreads: The number of threads to use
 */
void parMergeSort(std::vector<int32_t> *arr, int32_t numThreads)
{
    // Ensure that the number of threads is valid
    if (numThreads < 1)
    {
        throw std::invalid_argument("Number of threads must be at least 1");
    }

    // Sort blocks of the array in parallel
    int32_t blockSize = ceil(arr->size() / numThreads);
    std::vector<std::thread> threads(numThreads);
    for (int32_t i = 0; i < numThreads; i++)
    {
        int32_t end = std::min((i + 1) * blockSize - 1, (int32_t)arr->size() - 1); // Bounds check
        threads[i] = std::thread(mergeSort, arr, i * blockSize, end);
    }

    // Wait for all threads to finish
    for (std::thread &t : threads)
    {
        t.join();
    }

    // Merge sorted blocks back together
    for (int32_t size = blockSize; size < arr->size(); size = 2 * size)
    {
        // Pick starting point of different subarrays of current size
        for (int32_t left_start = 0; left_start < arr->size(); left_start += 2 * size)
        {
            int32_t mid = left_start + size - 1;
            int32_t right_end = std::min(left_start + 2 * size - 1, (int32_t)(arr->size() - 1));

            merge(arr, left_start, mid, right_end);
        }
    }
}
