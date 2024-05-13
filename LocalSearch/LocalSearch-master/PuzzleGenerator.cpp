#include "PuzzleGenerator.h"
using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal),
	currBestPuzzle(_nRows, _nColumns, _minVal, _maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
	timer.StartTimer();
	maxtime = 59.9;	// To make sure we don't exceed a minute

	Puzzle bestPuzzleFound = RandomWalk(4);
	
	while (timer.GetElapsedTime() + 4 < maxtime / 6) {
		Puzzle p = RandomWalk(4);
		p = HillClimbOnce(p);
		if (p.GetValue() > bestPuzzleFound.GetValue()) {
			bestPuzzleFound = p;
		}
	}

	currBestPuzzle = bestPuzzleFound;

	double elapsed = timer.GetElapsedTime();
    double remainingTime = maxtime - elapsed;

	Puzzle saPuzzle = SimulatedAnnealing(remainingTime);
	if (saPuzzle.GetValue() > bestPuzzleFound.GetValue()) {
		bestPuzzleFound = saPuzzle;
	}

	return bestPuzzleFound;
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that starts at a random configuration and keeps randomly modifying it
	// until the time limit is reached. Returns the best solution found so far.

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.

	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;
	int bestValue = p.GetValue();

	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();

	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell
		// (since we are doing a random walk, we just replace p with its successor).
		p = p.GetRandomSuccessor();

		// Update the current best solution.
		if (p.GetValue() > bestValue)	// Calling GetValue() for the first time is costly
										// since the puzzle has to be evaluated first.
		{
			bestValue = p.GetValue();	// Calling it a second time simply returns the value that was computed before.
			bestPuzzle = p;
		}
	}

	return bestPuzzle;

	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
	vector<Puzzle> successors;
	bestPuzzle.GetAllSuccessors(successors);
}

Puzzle PuzzleGenerator::SimulatedAnnealing(double timelimit) {
	// Initialize the puzzle with the generator's configuration
	Puzzle p = currBestPuzzle;
	Puzzle bestPuzzle = p;
	int bestVal = p.GetValue();
	// Initial temperature and cooling rate
	double T = 1000.0;
	double coolingRate = 0.99;
	// Define the iterations without improvement to adjust the T and cooling rate
	int iterations = 0;

	// Timer to set counterdown
	Timer t;
	t.StartTimer();
	int currVal = p.GetValue();
 	
	while (t.GetElapsedTime() < timelimit && T > 1) {
		Puzzle nextPuzzle = p.GetRandomSuccessor();
		int nextVal = nextPuzzle.GetValue();
		// Accept the new state based on the calculated probability or if it is better
		if (nextVal > currVal || exp((nextVal - currVal) / T) > ((double) rand() / RAND_MAX)) {
            p = nextPuzzle;
			currVal = nextVal;
            if (nextVal > bestVal) {
                bestPuzzle = nextPuzzle;
                bestVal = nextVal;
				iterations = 0;
            }
        } else {
            iterations++;
        }
		//Temperature cooling to enable precise search 
		if (iterations >= 50) {
            T *= coolingRate;
            iterations= 0;
            coolingRate *= 0.99;
        }
		// Adjust cooling rate dynamically based on the progress
        if (coolingRate < 0.95 && iterations < 10) {
            coolingRate *= 1.01;
        }
	}

	return bestPuzzle;
}


Puzzle PuzzleGenerator::HillClimbOnce(Puzzle startPuzzle) {
    Puzzle currPuzzle = startPuzzle;
    vector<Puzzle> successors;
	// Generate and evaluate all successors of the current puzzle
    currPuzzle.GetAllSuccessors(successors);
    int bestVal = currPuzzle.GetValue();

    for (Puzzle &succ : successors) {
        int succValue = succ.GetValue();
		// Move to a successor if it has a better value
        if (succValue > bestVal) {
            currPuzzle = succ;
            bestVal = succValue;
        }
    }

    return currPuzzle;
}