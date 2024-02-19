/**
*  seqSorts.hpp
*
*  Declares the sequential versions of the sorting functions
*/

#ifndef SEQ_SORTS_HPP_MULTITHREADED_SORTING
#define SEQ_SORTS_HPP_MULTITHREADED_SORTING


#include <vector>
#include <cstdint>

void seqBubbleSort(std::vector<int32_t>*);
void seqInsertionSort(std::vector<int32_t>*);
void seqMergeSort(std::vector<int32_t>*);
void seqQuickSort(std::vector<int32_t>*);


#endif
