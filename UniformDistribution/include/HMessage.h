#pragma once

#include <ctime>
#include <string>
#include <complex>
#include <iostream>
#include <cstdlib>
#include <ios>
#include <iomanip>

#include "Constants.h"
#include "GlbParam.h"

class HMessage
{
private:
	time_t sTime, eTime;

public:
	HMessage(void);
	~HMessage(void);

	void BegProj(void);
	void EndProj(void);

	void Start(const std::string &str);
	void End(const std::string &str, double data);

	void Reset(void);
	void TurnOn(void);
	void TurnOff(void);

	void Error(const std::string &str);

	void Display(const std::string &str);
	void Display(const std::string &str, int data);
	void Display(const std::string &str, double data);
	void Display(const std::string &str, TNode3D<double> data);

	void DisMemry(double mem, const std::string &str);
	void Debug(void);
};

