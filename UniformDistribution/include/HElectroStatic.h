#pragma once
#include "Constants.h"
#include "GlbParam.h"
#include "HMessage.h"
#include <algorithm>
#include "HOutPut.h"
#include <omp.h>

class HElectroStatic
{
public:
	HElectroStatic(void);
	~HElectroStatic(void);
	HMessage message;
	HOutPut _OutPut;

public:
	void Solve(void);
	void Solve_near(void);

	void InitializeSource(void);
	double NextStatusNear(void);
	void LocationCorrection(void);
	void LocationCorrection_near(void);
	void NearNode(void);
};

