#include "TTri.h"

// ---------------------------------------------------
TTri::TTri(void) {
	node1 = -1;
	node2 = -1;
	node3 = -1;
	area = 0;
}
// ---------------------------------------------------
// node ii
int& TTri::operator[](int ii) {
	switch (ii) {
	case 0:	return node1; break;
	case 1:	return node2; break;
	case 2:	return node3; break;
	default:
		assert(0); return node1;
	}
}