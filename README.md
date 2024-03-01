# Implementing Multi-threaded Sorting Algorithms

*Project Group 22*

Comparison-based sorting algorithms, provably, cannot have an average case runtime better than O(n log n). 
While this is a very good runtime in many cases, extremely large datasets can still take long periods of time to process. 
We believe sorting is a prime candidate for parallelization due to its nature of many repetitive comparison operations. 
We will test the performance of four well-known comparison-based sorting algorithms both before and after applying multi-threading techniques. 
We expect significant speedup of the parallelized versions over their single threaded counterparts, and if this is not the case, 
we will determine the reasons why speedup could not be achieved.

We will be implementing the following sorting algorithms:

- Merge sort
- Quick sort
- Bubble sort
- Insertion sort

## Build Instructions

Run the following make command:

`make sorttest`

Alternatively, compile with g++ directly:

`g++ -o sorttest main.cpp Stopwatch.cpp Sequential/*.cpp Parallel/*.cpp`

## Usage

Usage: `sorttest [Options...]`

| Options        | Explanation                                                |
| -------------- | ---------------------------------------------------------- |
| -s             | Use the sequential version of the sorting algorithm        |
| -p             | Use the parallel version of the sorting algorithm          |
| -d --data      | Specify file name for input data                           |
| -a --algorithm | Specify sort algorithm \<bubble\|insertion\|merge\|quick\> |
| -t --threads   | Specify number of threads to use for parallel sort         |
| -v --verify    | Verify that the results are sorted                         |
|    --help      | Show this message                                          |
