#include "HOutPut.h"

HOutPut::HOutPut(void)
{
}


HOutPut::~HOutPut(void)
{
}

void HOutPut::OutPut_source(std::string a)
{
	std::string foutname; // .txt file name
	std::ofstream fout;

	foutname = _PrjName + "_" + std::to_string(int(_NUM)) + "_" + a + ".txt";

	fout.open(foutname.c_str());
	if (!fout.is_open()) {
		message.Error("Error: open ffd file" + foutname + " failed!");
	}

	for (int ii = 0; ii < _NUM; ii++)
	{
		fout << sourceList[ii].x << "\t" << sourceList[ii].y << "\t" << sourceList[ii].z << "\n";
	}

	fout.close();
}