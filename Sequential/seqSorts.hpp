/**
*  seqSorts.hpp
*
*  Declares the sequential versions of the sorting functions
*/

#ifndef SEQ_SORTS_HPP_MULTITHREADED_SORTING
#define SEQ_SORTS_HPP_MULTITHREADED_SORTING


#include <vector>
#include <cstdint>

void seqBubbleSort(std::vector<int64_t>*);
void seqInsertionSort(std::vector<int64_t>*);
void seqMergeSort(std::vector<int64_t>*);
void seqQuickSort(std::vector<int64_t>*);


#endif
