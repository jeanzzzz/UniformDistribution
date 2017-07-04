#pragma once


#include "GlbParam.h"
#include "HMessage.h"
#include "HOutPut.h"

class HPostCal
{
public:
	HPostCal(void);
	~HPostCal(void);
	HMessage message;
	HOutPut _OutPut;

	void PostCalculate(void);
	void ReadTxt(void);
	void CombineElements(void);
	TNode3D<double> Barycenter(std::vector<int> v);

	bool SameSide(int, int);
	TNode3D<double> GetNormal(int, int, int);
	void SourceLocate(void);
};

