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
	
	// calculate area 
	double area_tri = 0;
	double area_qua = 0;
	double area_total = 0;
	for (int ii = 0; ii < _NumTri; ii++) {
		area_tri = area_tri + Area(triList[ii]);
	}
	for (int ii = 0; ii < _NumQua; ii++) {
		area_qua = area_qua + Area(quaList[ii]);
	}
	area_total = area_tri + area_qua;
	message.Display("The total area is ", area_total);

	// sample numbers
	_NUM = round(DENSITY * area_total / (LAMBDA*LAMBDA));
	message.Display("The number of sample sources is ", _NUM);
	message.Display("Finish pre-calculation");
	return;
}

// ----------------- Read file ----------------------
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

	REAL unit;
	foo >> unit;
	foo >> _NumNode;
	message.Display("The unit is ", unit);
	message.Display("The number of nodes is ", _NumNode);
	nodeList.resize(_NumNode);

	for (int ii = 0; ii < _NumNode; ii++)
	{
		foo >> nodeList[ii].x;
		foo >> nodeList[ii].y;
		foo >> nodeList[ii].z;

		// multiply with the unit
		nodeList[ii] = nodeList[ii] * unit;
	}

	foo >> _NumTri;
	message.Display("The number of triangles is ", _NumTri);
	triList.resize(_NumTri);
	for (int ii = 0; ii < _NumTri; ii++) {
		foo >> triList[ii].node1;
		foo >> triList[ii].node2;
		foo >> triList[ii].node3;
	}

	foo >> _NumQua;

	quaList.resize(_NumQua);
	message.Display("The number of quadrangle is ", _NumQua);
	for (int ii = 0; ii < _NumQua; ii++) {
		foo >> quaList[ii].node1;
		foo >> quaList[ii].node2;
		foo >> quaList[ii].node3;
		foo >> quaList[ii].node4;
	}

	foo.close();
	message.Display("Finish reading file");
}

// ------------------ calculate area of the quadrangle and triangle ----------------
double HPreCal::Area(TTri a)
{
	double area = Helen(a.node1, a.node2, a.node3);
	return area;
}

double HPreCal::Area(TQua a)
{
	double area = Helen(a.node1, a.node2, a.node3) + Helen(a.node1, a.node2, a.node4);
	return area;
}

double HPreCal::Helen(int a, int b, int c)
{
	double x = Length(nodeList[a], nodeList[b]);
	double y = Length(nodeList[a], nodeList[c]);
	double z = Length(nodeList[b], nodeList[c]);
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