#
# A Makefile... to make ours lives a little easier
#
# Run:
#       make sorttest
#


BUILDTARGETS = main.o Stopwatch.o seqMergeSort.o seqQuickSort.o seqBubbleSort.o seqInsertionSort.o parMergeSort.o parQuickSort.o parBubbleSort.o parInsertionSort.o


sorttest: $(BUILDTARGETS)
	g++ -o sorttest $(BUILDTARGETS)


main.o: main.cpp
	g++ -c main.cpp

Stopwatch.o: Stopwatch.cpp
	g++ -c Stopwatch.cpp


# Sequential Algorithms

seqBubbleSort.o: Sequential/seqBubbleSort.cpp
	g++ -c Sequential/seqBubbleSort.cpp

seqInsertionSort.o: Sequential/seqInsertionSort.cpp
	g++ -c Sequential/seqInsertionSort.cpp

seqMergeSort.o: Sequential/seqMergeSort.cpp
	g++ -c Sequential/seqMergeSort.cpp

seqQuickSort.o: Sequential/seqQuickSort.cpp
	g++ -c Sequential/seqQuickSort.cpp


# Parallel Algorithms

parBubbleSort.o: Parallel/parBubbleSort.cpp
	g++ -c Parallel/parBubbleSort.cpp

parInsertionSort.o: Parallel/parInsertionSort.cpp
	g++ -c Parallel/parInsertionSort.cpp

parMergeSort.o: Parallel/parMergeSort.cpp
	g++ -c Parallel/parMergeSort.cpp

parQuickSort.o: Parallel/parQuickSort.cpp
	g++ -c Parallel/parQuickSort.cpp


# Clean Target

clean:
	rm *.o
	rm sorttest

clean-outputs:
	rm *.report
	rm *.dump
