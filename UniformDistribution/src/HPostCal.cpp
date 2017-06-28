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

	message.End("OK!", -1); //////////

	return;
}


