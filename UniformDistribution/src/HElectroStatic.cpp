#include "../include/HElectroStatic.h"

HElectroStatic::HElectroStatic(void)
{
}

HElectroStatic::~HElectroStatic(void)
{
}

void HElectroStatic::Solve(void)
{
	InitializeSource();
	
	// -----------------------------------------
	// calculate next status until electrostatic
	// -----------------------------------------
	message.Start("calculating next status");
	_ForceTotal.resize(REPEATMAX);
	for (int ii = 0; ii < REPEATMAX; ii++)
	{
		message.Display("Start: Loop NO. ", ii);
		_ForceTotal[ii] = NextStatusNear();
		message.Display("The Force is ", _ForceTotal[ii]);
		LocationCorrection();
		message.Display("Finish: Loop NO. ", ii);
	}
	message.End("Success calculating next status", -1);
}

// --------------------------------------------------------------------------
// initialize sources and velocity
// --------------------------------------------------------------------------
void HElectroStatic::InitializeSource(void)
{
	message.Start("initializing sources");
	message.Display("NUM is ", _NUM);
	_SourceList.resize(_NUM);
	_SourceLocate.resize(_NUM);
	_VelocityList.resize(_NUM);

	// initialize random numbers
	std::vector<int> temp;
	for (int i = 0; i < _NumNode; ++i)
	{
		temp.push_back(i + 1);
	}
	random_shuffle(temp.begin(), temp.end());

	for (int ii = 0; ii < _NUM; ii++)
	{
		_SourceList[ii] = _NodeList[temp[ii]];
		_SourceLocate[ii] = FindFace(temp[ii]); ///find the connected faces to each sources
		_VelocityList[ii].x = 0;
		_VelocityList[ii].y = 0;
		_VelocityList[ii].z = 0;
	}

	_OutPut.OutPut_source("initial");
	message.End("Success initializing sources", -1);
}

// ------------------------------------------------------------------------
// solve near
// ------------------------------------------------------------------------
void HElectroStatic::Solve_near(void)
{
	// initialize sources and velocity
	message.Start("initializing sources");
	message.Display("NUM is ", _NUM);
	_SourceList.resize(_NUM);
	_VelocityList.resize(_NUM);
	for (int ii = 0; ii < _NUM; ii++)
	{
		_SourceList[ii] = _NodeList[ii];
		_VelocityList[ii].x = 0;
		_VelocityList[ii].y = 0;
		_VelocityList[ii].z = 0;
	}

	_OutPut.OutPut_source("initial_near");
	message.End("Success initializing sources", -1);

	// initialize near nodes
	message.Display("Start initializing near nodes");
	NearNode();
	message.Display("Success initializing near nodes");

	// calculate next status until electrostatic
	message.Start("calculating next status");
	_ForceTotal.resize(REPEATMAX);
	for (int ii = 0; ii < REPEATMAX; ii++)
	{
		message.Display("Start: Loop NO. ", ii);
		_ForceTotal[ii] = NextStatusNear();
		message.Display("The Force is ", _ForceTotal[ii]);
		LocationCorrection_near();
		message.Display("Finish: Loop NO. ", ii);
	}
	message.End("Success calculating next status", -1);
}

// -----------------------------------------------------------------------
// Next status
// -----------------------------------------------------------------------
double HElectroStatic::NextStatusNear(void)
{
	// This method is to update all the locatioins at the same time

	// distance and force between two points
	std::vector <TNode3D<REAL> > FORCE;
	FORCE.resize(_NUM);
	double _ForceTotal_temp = 0;
	double distance_temp = 0;
	TNode3D<REAL> rr_temp;
	for (int ii = 0; ii < _NUM; ii++) {
		FORCE[ii] = 0;
		for (int jj = 0; jj < _NUM; jj++) {
			distance_temp = sqrt((_SourceList[ii] - _SourceList[jj]).AbsSquare());
			distance_temp = (distance_temp < DISTANCE_MIN) ? DISTANCE_MIN : distance_temp;
			distance_temp = (distance_temp > DISTANCE_MAX) ? INFINITY : distance_temp;
			rr_temp = (_SourceList[ii] - _SourceList[jj]) / distance_temp;
			FORCE[ii] = FORCE[ii] + rr_temp * COULOMB_CONSTANT * CHARGE * CHARGE / (distance_temp * distance_temp);
		}
		_ForceTotal_temp = _ForceTotal_temp + sqrt( FORCE[ii].AbsSquare());
	}

	// update source location and velocity
	for (int ii = 0; ii < _NUM; ii++) {
		//this will determine how _SourceList will be updated: before or after
		//_SourceList[ii] = _SourceList[ii] + _VelocityList[ii]; 

		//this will determine how _VelocityList will be updated
		_VelocityList[ii] = FORCE[ii];
		//_VelocityList[ii] = _VelocityList[ii] + FORCE[ii]; 

		//this will determine how _SourceList will be updated: before or after
		_SourceList[ii] = _SourceList[ii] + _VelocityList[ii];
	}
	return (_ForceTotal_temp/_NUM);
}



// ------------------------------------------------------------------------
// Location correction parallel: Point
// ------------------------------------------------------------------------
void HElectroStatic::LocationCorrection(void)
{
	std::vector<double> distance_temp (_NumNode, 0);
	for (int ii = 0; ii < _NUM; ii++) {
#pragma omp parallel for
		for (int jj = 0; jj < _NumNode; jj++) {
			distance_temp[jj] = sqrt((_SourceList[ii] - _NodeList[jj]).AbsSquare());
		}
		std::vector<double>::iterator smallest = std::min_element(std::begin(distance_temp), std::end(distance_temp));
		_SourceList[ii] = _NodeList[std::distance(std::begin(distance_temp), smallest)];
	}
}

// ------------------------------------------------------------------------ 
// location correction near node: Point
// ------------------------------------------------------------------------
void HElectroStatic::LocationCorrection_near(void)
{
	std::vector<double> distance_temp(NEARNODE, 0);
	for (int ii = 0; ii < _NUM; ii++) {
		for (int kk = 0; kk < NEARNODE; kk++) {
			int jj = _NearNodeList[ii][kk];
			distance_temp[jj] = sqrt((_SourceList[ii] - _NodeList[jj]).AbsSquare());
		}
		std::vector<double>::iterator smallest = std::max_element(std::begin(distance_temp), std::end(distance_temp));
		_SourceList[ii] = _NodeList[std::distance(std::begin(distance_temp), smallest)];
	}
}

void HElectroStatic::NearNode(void)
{
	_NearNodeList.resize(_NumNode, std::vector<int>(NEARNODE));
	for (int ii = 0; ii < _NumNode; ii++) {
		int kk = 0;
		for (int jj = 0; jj < _NumNode; jj++) {
			double distance_temp = sqrt((_NodeList[ii] - _NodeList[jj]).AbsSquare());
			if (distance_temp < DISTANCE_MAX) {
				_NearNodeList[ii][kk] = jj;
				kk = kk + 1;
			}
		}
	}
}

// ------------------------------------------------------------------------
// Initialize storing source on which face
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// find the connected faces to each sources
// ------------------------------------------------------------------------
std::vector<int> HElectroStatic::FindFace(int a) {
	std::vector<int> FFace;
#pragma omp parallel for
	for (int ii = 0; ii < _NumTri; ii++) {
		if ((_FaceList[ii][0] == a) || (_FaceList[ii][1] == a) || (_FaceList[ii][0] == a)) {
			FFace.push_back(ii);
		}
	}
#pragma omp parallel for
	for (int ii = _NumTri; ii < _NumTri + _NumQua; ii++) {
		if ((_FaceList[ii][0] == a) || (_FaceList[ii][1] == a) || (_FaceList[ii][0] == a) || (_FaceList[ii][0] == a)) {
			FFace.push_back(ii);
		}
	}
	return FFace;
}