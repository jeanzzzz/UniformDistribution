#include "../include/HPostCal.h"


HPostCal::HPostCal(void)
{
}


HPostCal::~HPostCal(void)
{
}

void HPostCal::PostCalculate(void)
{
	message.Start("Post-Calculating");
	
	_OutPut.OutPut_source("result");

	// combine elements
	CombineElements();
	_OutPut.OutPut_CombinedElement("test");

	message.End("OK!", -1); //////////

	return;
}

// --------------- Combine elements --------------------
void HPostCal::CombineElements(void) {
	_CombineList.resize(_NUM);
	for (int ii = 0; ii < (_NumTri + _NumQua); ii++) {
		TNode3D<double> barycenter = Barycenter(_FaceList[ii]);
		int closest_source = 0;
		double closest_distance = (barycenter - _SourceList[0]).Norm();
		for (int jj = 1; jj < _NUM; jj++) {
			double distance_temp = (barycenter - _SourceList[jj]).Norm();
			//if ((distance_temp < closest_distance) && SameSide(ii, jj)){
			if (distance_temp < closest_distance){
				closest_distance = distance_temp;
				closest_source = jj;
			}
		}
		_CombineList[closest_source].push_back(ii);
	}
}
TNode3D<double> HPostCal::Barycenter(std::vector<int> vv) {
	TNode3D<double> barycenter = (0, 0, 0);
	for (int kk = 0; kk < vv.size(); kk++) {
		barycenter = barycenter + _NodeList[vv[kk]];
	}
	return (barycenter / vv.size());
}

// ------------------ Read .txt ------------------------
void HPostCal::ReadTxt(void) {
	std::string fname;
	_TxtName = std::string(_FileName) + "_" + std::to_string(int(_NUM)) + "_" + "result";
	fname = _TxtName + ".txt";

	std::fstream foo;
	foo.open(fname, std::ios::in);
	if (!foo.is_open()) {
		std::cout << "Error reading " << fname << ". Exiting in shame." << std::endl;
		exit(-1);
	}

	_SourceList.resize(_NUM);

	for (int ii = 0; ii < _NUM; ii++)
	{
		foo >> _SourceList[ii].x;
		foo >> _SourceList[ii].y;
		foo >> _SourceList[ii].z;
	}

	foo.close();
	message.Display("Finish reading .txt file");
}

bool HPostCal::SameSide(int a, int b) {
	//a is face number, b is source number
	TNode3D<double> normal_1 = GetNormal(_FaceList[a][0], _FaceList[a][1], _FaceList[a][2]);
	int c = _SourceLocate[b][0];
	TNode3D<double> normal_2 = GetNormal(_FaceList[c][0], _FaceList[c][1], _FaceList[c][2]);
	return (normal_1.Dot(normal_2) > -0);
}

TNode3D<double> HPostCal::GetNormal(int a, int b, int c) {
	TNode3D<double> normal;
	TNode3D<double> p1 = _NodeList[a]; ///point
	TNode3D<double> p2 = _NodeList[b];
	TNode3D<double> p3 = _NodeList[c];
	normal.x = (p2.y - p1.y)*(p3.z - p2.z) - (p2.z - p1.z)*(p3.y - p1.y);
	normal.y = (p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z);
	normal.z = (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
	return normal/normal.Norm();
}

void HPostCal::SourceLocate(void) {
	_SourceLocate.resize(_NUM);
	for (int ii = 0; ii < _NUM; ii++) {
		TNode3D<double> barycenter = Barycenter(_FaceList[0]);
		int closest_face = 0;
		double closest_distance = (barycenter - _SourceList[ii]).Norm();
		for (int jj = 1; jj < _NUM; jj++) {
			TNode3D<double> barycenter = Barycenter(_FaceList[jj]);
			double distance_temp = (barycenter - _SourceList[ii]).Norm();
			if (distance_temp < closest_distance) {
				closest_distance = distance_temp;
				closest_face = jj;
			}
		}
		_SourceLocate[ii].push_back(closest_face);
	}
}