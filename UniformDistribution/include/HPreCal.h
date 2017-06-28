#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "HMessage.h"
#include "GlbParam.h"
#include "TTri.h"
#include "TQua.h"
#include "Constants.h"


class HPreCal
{
public:
	HPreCal(void);
	~HPreCal(void);
	HMessage message;

public:
	void PreCalculate(void);
	void ReadMesh(void);
	double Area(std::vector<int>);
	double Helen(int, int, int);
	double Length(TNode3D<REAL>, TNode3D<REAL>);
};