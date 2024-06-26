#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <math.h> //for acceptance probability function
#include "Puzzle.h"
#include "Timer.h"

class PuzzleGenerator{
public:
	PuzzleGenerator(int nRows, int nColumns, int minVal, int maxVal);
	Puzzle GeneratePuzzle();
	Puzzle RandomWalk(double timelimit);	// Time limit given in seconds.
	Puzzle SimulatedAnnealing(double timelimit); //added 10/20/16
	Puzzle HillClimbOnce(Puzzle startPuzzle);

	double AcceptanceProbability(int newValue, int bestValue, double T); //added 10/20/16
private:
	int nRows, nColumns, minVal, maxVal;
	Timer timer;
	double maxtime;
	Puzzle currBestPuzzle;
};
