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
	FORCE_TOTAL.resize(REPEATMAX);
	for (int ii = 0; ii < REPEATMAX; ii++)
	{
		message.Display("Start: Loop NO. ", ii);
		FORCE_TOTAL[ii] = NextStatusNear();
		message.Display("The Force is ", FORCE_TOTAL[ii]);
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
	sourceList.resize(_NUM);
	velocityList.resize(_NUM);

	// initialize random numbers
	std::vector<int> temp;
	for (int i = 0; i < _NumNode; ++i)
	{
		temp.push_back(i + 1);
	}
	random_shuffle(temp.begin(), temp.end());

	for (int ii = 0; ii < _NUM; ii++)
	{
		sourceList[ii] = nodeList[temp[ii]];
		velocityList[ii].x = 0;
		velocityList[ii].y = 0;
		velocityList[ii].z = 0;
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
	sourceList.resize(_NUM);
	velocityList.resize(_NUM);
	for (int ii = 0; ii < _NUM; ii++)
	{
		sourceList[ii] = nodeList[ii];
		velocityList[ii].x = 0;
		velocityList[ii].y = 0;
		velocityList[ii].z = 0;
	}

	_OutPut.OutPut_source("initial_near");
	message.End("Success initializing sources", -1);

	// initialize near nodes
	message.Display("Start initializing near nodes");
	NearNode();
	message.Display("Success initializing near nodes");

	// calculate next status until electrostatic
	message.Start("calculating next status");
	FORCE_TOTAL.resize(REPEATMAX);
	for (int ii = 0; ii < REPEATMAX; ii++)
	{
		message.Display("Start: Loop NO. ", ii);
		FORCE_TOTAL[ii] = NextStatusNear();
		message.Display("The Force is ", FORCE_TOTAL[ii]);
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
	double FORCE_TOTAL_temp = 0;
	double distance_temp = 0;
	TNode3D<REAL> rr_temp;
	for (int ii = 0; ii < _NUM; ii++) {
		FORCE[ii] = 0;
		for (int jj = 0; jj < _NUM; jj++) {
			distance_temp = sqrt((sourceList[ii] - sourceList[jj]).AbsSquare());
			distance_temp = (distance_temp < DISTANCE_MIN) ? DISTANCE_MIN : distance_temp;
			distance_temp = (distance_temp > DISTANCE_MAX) ? INFINITY : distance_temp;
			rr_temp = (sourceList[ii] - sourceList[jj]) / distance_temp;
			FORCE[ii] = FORCE[ii] + rr_temp * COULOMB_CONSTANT * CHARGE * CHARGE / (distance_temp * distance_temp);
		}
		FORCE_TOTAL_temp = FORCE_TOTAL_temp + sqrt( FORCE[ii].AbsSquare());
	}

	// update source location and velocity
	for (int ii = 0; ii < _NUM; ii++) {
		//this will determine how sourceList will be updated: before or after
		//sourceList[ii] = sourceList[ii] + velocityList[ii]; 

		//this will determine how velocityList will be updated
		velocityList[ii] = FORCE[ii];
		//velocityList[ii] = velocityList[ii] + FORCE[ii]; 

		//this will determine how sourceList will be updated: before or after
		sourceList[ii] = sourceList[ii] + velocityList[ii];
	}
	return (FORCE_TOTAL_temp/_NUM);
}



// ------------------------------------------------------------------------
// Location correction parallel
// ------------------------------------------------------------------------
void HElectroStatic::LocationCorrection(void)
{
	std::vector<double> distance_temp (_NumNode, 0);
	for (int ii = 0; ii < _NUM; ii++) {
#pragma omp parallel for
		for (int jj = 0; jj < _NumNode; jj++) {
			distance_temp[jj] = sqrt((sourceList[ii] - nodeList[jj]).AbsSquare());
		}
		std::vector<double>::iterator smallest = std::min_element(std::begin(distance_temp), std::end(distance_temp));
		sourceList[ii] = nodeList[std::distance(std::begin(distance_temp), smallest)];
	}
}

// ------------------------------------------------------------------------ 
// location correction near node
// ------------------------------------------------------------------------
void HElectroStatic::LocationCorrection_near(void)
{
	std::vector<double> distance_temp(NEARNODE, 0);
	for (int ii = 0; ii < _NUM; ii++) {
		for (int kk = 0; kk < NEARNODE; kk++) {
			int jj = nearnodeList[ii][kk];
			distance_temp[jj] = sqrt((sourceList[ii] - nodeList[jj]).AbsSquare());
		}
		std::vector<double>::iterator smallest = std::max_element(std::begin(distance_temp), std::end(distance_temp));
		sourceList[ii] = nodeList[std::distance(std::begin(distance_temp), smallest)];
	}
}

void HElectroStatic::NearNode(void)
{
	nearnodeList.resize(_NumNode, std::vector<int>(NEARNODE));
	for (int ii = 0; ii < _NumNode; ii++) {
		int kk = 0;
		for (int jj = 0; jj < _NumNode; jj++) {
			double distance_temp = sqrt((nodeList[ii] - nodeList[jj]).AbsSquare());
			if (distance_temp < DISTANCE_MAX) {
				nearnodeList[ii][kk] = jj;
				kk = kk + 1;
			}
		}
	}
}

// ------------------------------------------------------------------------
// Initialize storing source on which face
// ------------------------------------------------------------------------
