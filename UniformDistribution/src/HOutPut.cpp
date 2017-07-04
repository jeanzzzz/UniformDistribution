#include "../include/HOutPut.h"

HOutPut::HOutPut(void)
{
}


HOutPut::~HOutPut(void)
{
}

// ------------------------ output source list -----------------------------
void HOutPut::OutPut_source(std::string a)
{
	std::string foutname; // .txt file name
	std::ofstream fout;

	foutname = _PrjName + "_" + std::to_string(int(_NUM)) + "_" + a + ".txt";

	fout.open(foutname.c_str());
	if (!fout.is_open()) {
		message.Error("Error: open txt file" + foutname + " failed!");
	}

	for (int ii = 0; ii < _NUM; ii++)
	{
		fout << _SourceList[ii].x << "\t" << _SourceList[ii].y << "\t" << _SourceList[ii].z << "\n";
	}

	fout.close();
}

// ------------------------Output combined element list ----------------------
void HOutPut::OutPut_CombinedElement(std::string a)
{
	std::string foutname; // .txt file name
	std::ofstream fout;

	foutname = _PrjName + "_" + std::to_string(int(_NUM)) + "_CombinedElement_" + a + ".txt";

	fout.open(foutname.c_str());
	if (!fout.is_open()) {
		message.Error("Error: open txt file" + foutname + " failed!");
	}

	for (int ii = 0; ii < _NUM; ii++)
	{
		std::vector<int>::iterator t;
		for (t = _CombineList[ii].begin(); t != _CombineList[ii].end(); t++)
			fout << *t << " ";
		fout << "\n";
	}

	fout.close();
}