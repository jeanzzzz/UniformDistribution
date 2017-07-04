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
			if (distance_temp < closest_distance) {
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
		barycenter = barycenter + _NodeelemList[vv[kk]];
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
