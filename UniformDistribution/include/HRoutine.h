#pragma once

#include "HMessage.h"
#include "HPreCal.h"
#include "HElectroStatic.h"
#include "HPostCal.h"

class HRoutine
{
public:
	HPreCal preCal;
	HElectroStatic electroStatic;
	HPostCal postCal;
	HMessage message;

public:
	HRoutine(void);
	~HRoutine(void);

	void RunRoutine(void);
};

