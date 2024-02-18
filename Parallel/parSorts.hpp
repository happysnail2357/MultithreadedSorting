/**
*  parSorts.hpp
*
*  Declares the parallel versions of the sorting functions
*/

#ifndef PAR_SORTS_HPP_MULTITHREADED_SORTING
#define PAR_SORTS_HPP_MULTITHREADED_SORTING


#include <vector>
#include <cstdint>

void parBubbleSort(std::vector<int64_t>*);
void parInsertionSort(std::vector<int64_t>*);
void parMergeSort(std::vector<int64_t>*);
void parQuickSort(std::vector<int64_t>*);


#endif
