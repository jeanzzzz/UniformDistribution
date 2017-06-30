// -------------------------------------------------------
#ifndef TTri_H
#define TTri_H
// -------------------------------------------------------

#include "Constants.h"
#include "TNode2D.h"
#include "TNode3D.h"

class TTri
{
public:
	int node1;
	int node2;
	int node3;
	REAL area;
	TNode3D<REAL> center;
	TNode3D<REAL> _UnitN; // _Unit outer normal vector

public:
	TTri(void); // OK

	int& operator[](int ii); //  3 nodes
};
// -------------------------------------------------------

#endif

