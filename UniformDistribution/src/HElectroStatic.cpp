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
		LocationCorrection_Face();
		//LocationCorrection_Node();
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
		for (int jj = 0; (jj < _NUM); jj++){
			if (jj != ii) {
				distance_temp = (_SourceList[ii] - _SourceList[jj]).Norm();
				rr_temp = (_SourceList[ii] - _SourceList[jj]) / distance_temp;
				distance_temp = (distance_temp < DISTANCE_MIN) ? DISTANCE_MIN : distance_temp;
				distance_temp = (distance_temp > DISTANCE_MAX) ? INFINITY : distance_temp;
				FORCE[ii] = FORCE[ii] + rr_temp * COULOMB_CONSTANT * CHARGE * CHARGE / (distance_temp * distance_temp);
			}
		}
		_ForceTotal_temp = _ForceTotal_temp + FORCE[ii].Norm();
	}

	// Correct force along tangential direction
	for (int ii = 0; ii < _NUM; ii++) {
		TNode3D<double> radial_direction = NormalDirection(ii);
		///_LogFile << "the point is  " << _SourceList[ii].x << " " << _SourceList[ii].y << " " << _SourceList[ii].z << std::endl;
		///_LogFile << "radial direction is  " << radial_direction.x << " " << radial_direction.y << " " << radial_direction.z << std::endl; ///to test normal
		TNode3D<double> FORCE_temp = FORCE[ii] - radial_direction * (radial_direction * FORCE[ii]);
		FORCE[ii] = (FORCE_temp.Norm() > MAXMOVE) ? FORCE_temp * MAXMOVE / FORCE_temp.Norm() : FORCE_temp;
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
	return (_ForceTotal_temp / _NUM);
}

// ------------------------------------------------------------------------
// Location correction parallel: Node
// ------------------------------------------------------------------------
void HElectroStatic::LocationCorrection_Node(void)
{
	std::vector<double> distance_temp(_NumNode, 0);
	for (int ii = 0; ii < _NUM; ii++) {
#pragma omp parallel for
		for (int jj = 0; jj < _NumNode; jj++) {
			distance_temp[jj] = sqrt((_SourceList[ii] - _NodeList[jj]).AbsSquare());
		}
		std::vector<double>::iterator smallest = std::min_element(std::begin(distance_temp), std::end(distance_temp));
		_SourceList[ii] = _NodeList[std::distance(std::begin(distance_temp), smallest)];
		_SourceLocate[ii].clear();
		_SourceLocate[ii] = FindFace(ii);
	}
}

// ------------------------------------------------------------------------
// Location correction: Face
// ------------------------------------------------------------------------
void HElectroStatic::LocationCorrection_Face(void)
{
	for (int ii = 0; ii < _NUM; ii++) {
		double distance_record = 10;// a hard code!!!!!!!!!!
		double face_record;
#pragma omp parallel for
		for (int jj = 0; jj < _NumTri + _NumQua; jj++) {
			if (ProjectionInFace(ii, jj)) {
				double distance_temp = DistancePoint2Face(ii, jj);
				if (distance_temp < distance_record) {
					distance_record = distance_temp;
					face_record = jj;
				}
			}
		}
		if (distance_record < 0.1) {
			_SourceList[ii] = GetProjection(ii, face_record, distance_record);
			_SourceLocate[ii].clear();
			_SourceLocate[ii].push_back(face_record);
		}
		else {
			LocationCorrection_OneSource(ii);
			_SourceLocate[ii].clear();
			_SourceLocate[ii] = FindFace(ii);
		}
	}
}
void HElectroStatic::LocationCorrection_OneSource(int a)
{
	// a is the number in _SourceList
	std::vector<double> distance_temp(_NumNode, 0);
#pragma omp parallel for
	for (int jj = 0; jj < _NumNode; jj++) {
		distance_temp[jj] = (_SourceList[a] - _NodeList[jj]).Norm();
	}
	std::vector<double>::iterator smallest = std::min_element(std::begin(distance_temp), std::end(distance_temp));
	_SourceList[a] = _NodeList[std::distance(std::begin(distance_temp), smallest)];
}


// location correction near node: Point
void HElectroStatic::LocationCorrection_near(void)
{
	std::vector<double> distance_temp(NEARNODE, 0);
	for (int ii = 0; ii < _NUM; ii++) {
		for (int kk = 0; kk < NEARNODE; kk++) {
			int jj = _NearNodeList[ii][kk];
			distance_temp[jj] = (_SourceList[ii] - _NodeList[jj]).Norm();
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

// -----------------------------------
// find the connected faces to each sources
// -----------------------------------
std::vector<int> HElectroStatic::FindFace(int a) {
	// a is the number in _SourceList
	std::vector<int> FFace;
	//#pragma omp parallel for
	for (int ii = 0; ii < _NumTri; ii++) {
		if ((_FaceList[ii][0] == a) || (_FaceList[ii][1] == a) || (_FaceList[ii][2] == a)) {
			FFace.push_back(ii);
		}
	}
	//#pragma omp parallel for
	for (int ii = _NumTri; ii < _NumTri + _NumQua; ii++) {
		if ((_FaceList[ii][0] == a) || (_FaceList[ii][1] == a) || (_FaceList[ii][2] == a) || (_FaceList[ii][3] == a)) {
			FFace.push_back(ii);
		}
	}
	return FFace;
}

// -----------------------------------
// find the radial direction of a source
// -----------------------------------
TNode3D<double> HElectroStatic::NormalDirection(int a) {

	/// on the face
	/*if (_SourceLocate[a].size() == 1) {
		int b = _SourceLocate[a][0];
		TNode3D<double> radial_direction = GetNormal(_FaceList[b][0], _FaceList[b][1], _FaceList[b][2]);
		return radial_direction / radial_direction.Norm();
	}

	/// on the edge
	else if (_SourceLocate[a].size() == 2) {
		int b = _SourceLocate[a][0];
		int c = _SourceLocate[a][1];
		TNode3D<double> radial_direction1 = GetNormal(_FaceList[b][0], _FaceList[b][1], _FaceList[b][2]);
		TNode3D<double> radial_direction2 = GetNormal(_FaceList[c][0], _FaceList[c][1], _FaceList[c][2]);
		TNode3D<double> radial_direction = radial_direction1 + radial_direction2;
		return (radial_direction / radial_direction.Norm());
	}

	/// at the node
	else if (_SourceLocate[a].size() >= 3) {*/
		int ll = _SourceLocate[a].size();
		TNode3D<double> radial_direction(0, 0, 0);
		for (int ii = 0; ii < ll; ii++) {
			TNode3D<double> normal_temp = GetNormal(_FaceList[_SourceLocate[a][ii]][0],
				_FaceList[_SourceLocate[a][ii]][1],
				_FaceList[_SourceLocate[a][ii]][2]);
			radial_direction = radial_direction + normal_temp/normal_temp.Norm();
		}
		return (radial_direction / radial_direction.Norm());
	//}
}
TNode3D<double> HElectroStatic::GetNormal(int a, int b, int c) {
	TNode3D<double> normal;
	TNode3D<double> p1 = _NodeList[a]; ///point
	TNode3D<double> p2 = _NodeList[b];
	TNode3D<double> p3 = _NodeList[c];
	normal.x = (p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y);
	normal.y = (p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z);
	normal.z = (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
	return normal;
}

// -----------------------------------
// is the projection in the face?
// -----------------------------------
bool HElectroStatic::ProjectionInFace(int a, int b) {
	// a is the number in the _SourceList
	// b is the number in the _FaceList
	TNode3D<double> P = _SourceList[a];
	TNode3D<double> A = _NodeList[_FaceList[b][0]];
	TNode3D<double> B = _NodeList[_FaceList[b][1]];
	TNode3D<double> C = _NodeList[_FaceList[b][2]];

	if (_FaceList[b].size() == 3) {
		return SameSide(A, B, C, P) &&
			SameSide(B, C, A, P) &&
			SameSide(C, A, B, P);
	}
	else if (_FaceList[b].size() == 4) {
		TNode3D<double> D = _NodeList[_FaceList[b][3]];
		return SameSide(A, B, C, P) &&
			SameSide(B, C, D, P) &&
			SameSide(C, D, A, P) &&
			SameSide(D, A, B, P);
	}
	else {
		message.Error("Wrong _FaceList size");
	}
}
bool HElectroStatic::SameSide(TNode3D<double> A, TNode3D<double> B, TNode3D<double> C, TNode3D<double> P) {

	TNode3D<double> AB = B - A;
	TNode3D<double> AC = C - A;
	TNode3D<double> AP = P - A;

	TNode3D<double> vector1 = AB.Cross(AC);
	TNode3D<double> vector2 = AB.Cross(AP);

	return (vector1.Dot(vector2) >= 0);
}

// -----------------------------------
// calculate the distance
// -----------------------------------
double HElectroStatic::DistancePoint2Face(int a, int b) {
	// a is the number in the _SourceList
	// b is the number in the _FaceList
	TNode3D<double> normal = GetNormal(_FaceList[b][0], _FaceList[b][1], _FaceList[b][2]);
	double A = normal.x;
	double B = normal.y;
	double C = normal.z;
	double D = 0 - normal.Dot(_NodeList[_FaceList[b][0]]);
	double distance = abs(normal.Dot(_SourceList[a]) + D) / normal.Norm();
	return distance;
}

// -----------------------------------
// calculate the projection
// -----------------------------------
TNode3D<double> HElectroStatic::GetProjection(int a, int b, double t) {
	// a is the number in the _SourceList
	// b is the number in the _FaceList
	// t is the distance
	TNode3D<double> normal = GetNormal(_FaceList[b][0], _FaceList[b][1], _FaceList[b][2]);
	double D = 0 - normal.Dot(_NodeList[_FaceList[b][0]]);
	double tt = (normal.Dot(_SourceList[a]) + D) / normal.AbsSquare();
	TNode3D<double> projection = _SourceList[a] - normal * tt;
	return projection;
}