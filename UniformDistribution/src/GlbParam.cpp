#include "../include/GlbParam.h"

// ---------------------------------------------
// global variables for control parameters
// ---------------------------------------------

std::string _PrjName;
std::string _FileName;

double _GMemory(0), _GlbTime(0), _PeakMem(0); //Global memory, time and Peak memory. 
std::ofstream _LogFile; //The log file

// ---------------------------------------------
// pre-calculate
// ---------------------------------------------
int _NumNode;
int _NumTri;
int _NumQua;
int _NUM;

std::vector <TNode3D<REAL> > nodeList;
std::vector <TTri> triList;
std::vector <TQua> quaList;

// ----------------------------------------------
// solve
// ----------------------------------------------
std::vector <TNode3D<REAL> > sourceList;
std::vector <TNode3D<REAL> > velocityList;
std::vector <double> FORCE_TOTAL;
std::vector <std::vector<int> > nearnodeList;	