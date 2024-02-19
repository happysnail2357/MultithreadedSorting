/**
*  parSorts.hpp
*
*  Declares the parallel versions of the sorting functions
*/

#ifndef PAR_SORTS_HPP_MULTITHREADED_SORTING
#define PAR_SORTS_HPP_MULTITHREADED_SORTING


#include <vector>
#include <cstdint>

void parBubbleSort(std::vector<int32_t>*);
void parInsertionSort(std::vector<int32_t>*);
void parMergeSort(std::vector<int32_t>*);
void parQuickSort(std::vector<int32_t>*);


#endif
