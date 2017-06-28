#include "../include/HRoutine.h"

HRoutine::HRoutine(void)
{
}


HRoutine::~HRoutine(void)
{
}

// --------------------------- RunRoutine -------------------------------
void HRoutine::RunRoutine(void)
{
	message.BegProj();

	// Pre-processing
	preCal.PreCalculate();

	// Main-processing
	electroStatic.Solve(); //This is using near nodes
	/// electroStatic.Solve(); 


	// Post-processing
	postCal.PostCalculate();

	message.EndProj();
	return;
}