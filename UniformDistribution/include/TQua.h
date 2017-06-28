// -------------------------------------------------------
#ifndef TQUA_H
#define TQUA_H
// -------------------------------------------------------

#include "Constants.h"
#include "TNode2D.h"
#include "TNode3D.h"

class TQua
{
public:
	int node1;
	int node2;
	int node3;
	int node4;
	REAL area;
	TNode3D<REAL> center;
	TNode3D<REAL> unitN; // unit outer normal vector

public:
	TQua(void); // OK

	int& operator[](int ii); //  4 nodes
};
// -------------------------------------------------------

#endif

