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
};

