#pragma once

#include <fstream>
#include <complex>
#include <math.h>
#include <vector>
#include "TQua.h"
#include "TTri.h"

// ---------------------------------------------
// global variables for control parameters
// ---------------------------------------------

extern std::string _PrjName;
extern std::string _FileName;

extern double _GMemory, _GlbTime, _PeakMem; //Global memory, time and Peak memory
extern std::ofstream _LogFile; //The log file.

// ---------------------------------------------
// pre-calculate
// ---------------------------------------------
extern int _NumNode;
extern int _NumTri;
extern int _NumQua;
extern int _NUM;

extern std::vector <TNode3D<REAL> > _NodeList;
extern std::vector <std::vector<int> > _FaceList;

// ----------------------------------------------
// solve
// ----------------------------------------------
extern std::vector <TNode3D<REAL> > _SourceList;
extern std::vector <TNode3D<REAL> > _VelocityList;
extern std::vector <std::vector<int> > _NearNodeList;
extern std::vector <double> _ForceTotal;
extern std::vector <std::vector<int> > _SourceLocate;

typedef double Real;
typedef int Integer;
// typedef Real Complex[2];
typedef std::complex<Real> Complex;
typedef std::complex<double> InputComplex;
typedef long LongInt;