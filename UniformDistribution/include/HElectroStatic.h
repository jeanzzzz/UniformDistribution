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
	

	void LocationCorrection_Node(void);
	void LocationCorrection_Face(void);
	void LocationCorrection_OneSource(int);
	void LocationCorrection_near(void);
	void NearNode(void);
	std::vector<int> FindFace(int);

	//normal function
	TNode3D<double> NormalDirection(int);
	TNode3D<double> GetNormal(int, int, int);

	//projection function
	bool ProjectionInFace(int, int);
	bool SameSide(TNode3D<double>,
		TNode3D<double>,
		TNode3D<double>,
		TNode3D<double>);
	double DistancePoint2Face(int, int);
	TNode3D<double> GetProjection(int, int, double);
};

