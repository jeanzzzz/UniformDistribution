#pragma once

#include <string>
#include "GlbParam.h"
#include "HMessage.h"

class HOutPut
{
public:
	HOutPut(void);
	~HOutPut(void);
	HMessage message;

	void OutPut_source(std::string);
};

