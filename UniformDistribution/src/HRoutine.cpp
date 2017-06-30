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

	// test if the result file exist
	std::string fname;
	_TxtName = std::string(_FileName) + "_" + std::to_string(int(_NUM)) + "_" + "result";
	fname = _TxtName + ".txt";
	
	std::fstream foo;
	foo.open(fname, std::ios::in);
	if (!foo.is_open()) {
		// Main - processing
		electroStatic.Solve(); //This is using near nodes
	}
	else {
		message.Display("Find result file: ");
		message.Display(fname);
		postCal.ReadTxt();
	}
	
	// Post-processing
	postCal.PostCalculate();

	message.EndProj();
	return;
}