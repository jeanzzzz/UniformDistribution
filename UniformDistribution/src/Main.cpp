// ----------------------------------------------------------------------
// % HomoSource_fewPoints
// Purpose:
//      read <.msh> data and plot few homogeneous sources on the surface
//      locations of sources are chosen on the nodes to simplify the
//      problem
// Character :
//		1) Update all the points at the same time
//		2) Only take forces from close sources into consideration
// Input:
//      filename(xxx.msh)
// Output :
//		1) data_source      <-data of sources location
//		2)
// Name :
//		global variable name format:	VARNAME, _VarName
//		variable name format:			varName
//		function name format:			FuncName
//		class name format:				HClassName
//		pointer name format:			pPointerName
//		function pointer name format:	PPointerName
// 
// Author:   JY Zhang	06/16/2017	@ESL
// ----------------------------------------------------------------------

#include "HRoutine.h"
#include <iostream>
#include "GlbParam.h"

int main(int argc, char *argv[])
{
	// -------------------------------------------------------
	// check input arguments, only one allowed (filename.msh)
	// -------------------------------------------------------
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << "<filename>" << std::endl;
		exit(EXIT_FAILURE);
	}
	// define input argument as filename
	const char *fileName	= argv[1];
	_FileName				= std::string(fileName);
	_PrjName				= std::string(fileName);

	// --------------------
	// start routine
	// --------------------
	HRoutine routine;
	routine.RunRoutine();

	// -----------------------------
	// program finish successfully!
	// -----------------------------
	std::cout << "--------------------------------" << std::endl;
	std::cout << " Program finished successfully!  " << std::endl;
	std::cout << "--------------------------------" << std::endl;
}
