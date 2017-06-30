#include "../include/HPreCal.h"


HPreCal::HPreCal(void)
{
}


HPreCal::~HPreCal(void)
{
}

// -----------------MAIN pre-calculate ------------------
void HPreCal::PreCalculate(void)
{
	ReadMesh();
	
	// calculate max and min coordinate
	_Max = GetMax(_NodeList);
	_Min = GetMin(_NodeList);
	message.Display("The max coordinate is ", _Max);
	message.Display("The min coordinate is ", _Min);

	// calculate area 
	double area_tri = 0;
	double area_qua = 0;
	double area_total = 0;
	for (int ii = 0; ii < _NumTri; ii++) {
		area_tri = area_tri + Area(_FaceList[ii]);
	}
	for (int ii = _NumTri; ii < (_NumTri + _NumQua); ii++) {
		area_qua = area_qua + Area(_FaceList[ii]);
	}
	area_total = area_tri + area_qua;
	message.Display("The total area is ", area_total);

	// sample numbers
	_NUM = round(DENSITY * area_total / (LAMBDA*LAMBDA));
	message.Display("The number of sample sources is ", _NUM);
	message.Display("Finish pre-calculation");
	
	// initialize a structure to store nodes
	// Here I divide the domain into square parts and store the NO. of nodes inside the square
	InitializeDomain();


	return;
}

// ----------------- Read .msh file ----------------------
void HPreCal::ReadMesh(void)
{
	std::string fname;
	fname = _FileName + ".msh";

	std::fstream foo;
	foo.open(fname, std::ios::in);
	if (!foo.is_open()) {
		std::cout << "Error reading " << fname << ". Exiting in shame." << std::endl;
		exit(-1);
	}

	REAL _Unit;
	foo >> _Unit;
	foo >> _NumNode;
	message.Display("The _Unit is ", _Unit);
	message.Display("The number of nodes is ", _NumNode);
	_NodeList.resize(_NumNode);

	for (int ii = 0; ii < _NumNode; ii++)
	{
		foo >> _NodeList[ii].x;
		foo >> _NodeList[ii].y;
		foo >> _NodeList[ii].z;

		// multiply with the _Unit
		_NodeList[ii] = _NodeList[ii] * _Unit;
	}

	foo >> _NumTri;
	message.Display("The number of triangles is ", _NumTri);
	_FaceList.resize(_NumTri);
	int a;
	for (int ii = 0; ii < _NumTri; ii++) {
		foo >> a;
		_FaceList[ii].push_back(a);
		foo >> a;
		_FaceList[ii].push_back(a);
		foo >> a;
		_FaceList[ii].push_back(a);
	}

	foo >> _NumQua;

	_FaceList.resize(_NumTri + _NumQua);
	message.Display("The number of quadrangle is ", _NumQua);
	for (int ii = _NumTri; ii < (_NumTri+_NumQua); ii++) {
		foo >> a;
		_FaceList[ii].push_back(a);
		foo >> a;
		_FaceList[ii].push_back(a);
		foo >> a;
		_FaceList[ii].push_back(a);
		foo >> a;
		_FaceList[ii].push_back(a);
	}

	foo.close();
	message.Display("Finish reading file");
}

// ------------------ calculate area of the quadrangle and triangle ----------------
double HPreCal::Area(std::vector<int> a)
{
	if (a.size() == 3) {
		double area = Helen(a[0], a[1], a[2]);
		return area;
	} 
	else if (a.size() == 4) {
		double area = Helen(a[0], a[1], a[2]) + Helen(a[0], a[1], a[3]);
		return area;
	}
	else {
		message.Error("Wrong face structure.");
	}


}
double HPreCal::Helen(int a, int b, int c)
{
	double x = Length(_NodeList[a], _NodeList[b]);
	double y = Length(_NodeList[a], _NodeList[c]);
	double z = Length(_NodeList[b], _NodeList[c]);
	double p = (x + y + z) / 2;
	double s = sqrt(p*(p - x)*(p - y)*(p - z));
	return s;
}
double HPreCal::Length(TNode3D<REAL> T1, TNode3D<REAL> T2)
{
	double len = sqrt((T1.x - T2.x)*(T1.x - T2.x)
		+ (T1.y - T2.y)*(T1.y - T2.y)
		+ (T1.z - T2.z)*(T1.z - T2.z));
	return len;
}

// ----------------- Get max and min from _NodeList -----------------
TNode3D<double> HPreCal::GetMax(std::vector<TNode3D<double> > &List) {
	TNode3D<double> max_temp = List[0];
	for (int ii = 0; ii < _NumNode; ii++) {
		max_temp.x = (max_temp.x < List[ii].x) ? List[ii].x : max_temp.x;
		max_temp.y = (max_temp.y < List[ii].y) ? List[ii].y : max_temp.y;
		max_temp.z = (max_temp.z < List[ii].z) ? List[ii].z : max_temp.z;
	}
	return max_temp;
}
TNode3D<double> HPreCal::GetMin(std::vector<TNode3D<double> > &List) {
	TNode3D<double> min_temp = List[0];
	for (int ii = 0; ii < _NumNode; ii++) {
		min_temp.x = (min_temp.x > List[ii].x) ? List[ii].x : min_temp.x;
		min_temp.y = (min_temp.y > List[ii].y) ? List[ii].y : min_temp.y;
		min_temp.z = (min_temp.z > List[ii].z) ? List[ii].z : min_temp.z;
	}
	return min_temp;
}

// ------------------ Get domain ------------------------
void HPreCal::InitializeDomain(void) {

}

