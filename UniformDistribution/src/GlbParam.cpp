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

std::vector <TNode3D<REAL> > _NodeList;
std::vector <std::vector<int> > _FaceList;

TNode3D<double> _Max;
TNode3D<double> _Min;

// ----------------------------------------------
// solve
// ----------------------------------------------
std::vector <TNode3D<REAL> > _SourceList;
std::vector <TNode3D<REAL> > _VelocityList;
std::vector <double> _ForceTotal;
std::vector <std::vector<int> > _NearNodeList;
std::vector <std::vector<int> > _SourceLocate;