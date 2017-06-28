#include "../include/HMessage.h"



HMessage::HMessage(void)
{
}


HMessage::~HMessage(void)
{
}

// --------------------- Begin project --------------------------
void HMessage::BegProj(void)
{
	std::cout << "==============================================" << std::endl;
	std::cout << "     " << "Begin of Project: " << _PrjName << std::endl;
	std::cout << "==============================================" << std::endl;

	// Create Log file
	std::string fname = _PrjName + "_log.log";
	_LogFile.open(fname.c_str());
	if (!_LogFile.is_open()) // check for file open error
	{
		Error("Error: create log file" + fname + " failed!");
	}
	return;
}

// -------------------- End project -----------------------------
void HMessage::EndProj(void) {
	std::cout << "\t\t\t==============================================" << std::endl;
	std::cout << "\t\t\t     " << "End of Project: " << _PrjName << std::endl;
	std::cout << "\t\t\t==============================================" << std::endl;
	_LogFile.close();
	this->Reset();
	return;
}

// --------------------- Reset ----------------------------
void HMessage::Reset(void) {
	// disp = true;
	_GMemory = 0;
	_GlbTime = 0;
	_PeakMem = 0;
}

// -------------------- Display message --------------------
void HMessage::Display(const std::string &str) {
	if (!disp) return;
	_LogFile << "\t" << str << std::endl;
	std::cout << "\t" << str << std::endl;

	return;
}

void HMessage::Display(const std::string &str, int data) {
	if (!disp) return;
	_LogFile << "\t" << str << data << std::endl;
	std::cout << "\t" << str << data << std::endl;

	return;
}

void HMessage::Display(const std::string &str, double data) {
	if (!disp) return;
	_LogFile << "\t" << str << data << std::endl;
	std::cout << "\t" << str << data << std::endl;

	return;
}

//-------------- show the start of a process -----------------
void HMessage::Start(const std::string &str) {
	if (!disp) return;
	sTime = time(NULL);
	_LogFile << "\tTime:" << ctime(&sTime) << "\t" << str << "..." << std::endl;
	std::cout << "\tTime:" << ctime(&sTime) << "\t" << str << "..." << std::endl;

	return;
}

// ------------ show the end of a process (m can be negative) -------------
void HMessage::End(const std::string &str, double data) {
	if (!disp) return;
	eTime = time(NULL);
	double dTime = difftime(eTime, sTime);
	_GlbTime += dTime;
	_GMemory += abs(data);
	_PeakMem += abs(data);

	_LogFile << "\t" << str << "!" << "\tTime used:  " << dTime << "s.";
	std::cout << "\t" << str << "!" << "\tTime used:  " << dTime << "s.";
	double per;
	std::string unit;
	if (data > 0) {
		if (data < KBytes_) { per = 1;		unit = " B"; }
		else if (data < MBytes_) { per = KBytes_;	unit = "KB"; }
		else if (data < GBytes_) { per = MBytes_;	unit = "MB"; }
		else { per = GBytes_;	unit = "GB"; }
		_LogFile << "\tmemery used: " << std::right << std::setw(4) << data / per << unit << std::endl;
		std::cout << "\tmemery used: " << std::right << std::setw(4) << data / per << unit << std::endl;
	}
	else if (data < 0) {
		_LogFile << "\tmemery used: Un-estimated !" << std::endl;
		std::cout << "\tmemery used: Un-estimated !" << std::endl;
	}
	else if (data == 0) {
		_LogFile << "\tmemery used: No more memory added !" << std::endl;
		std::cout << "\tmemery used: No more memory added !" << std::endl;
	}
	_LogFile << "+------+-----+----+---+--+-+++++GTime:" << std::right << std::setw(4)
		<< _GlbTime << " SEC.  GMemo:" << std::right << std::setw(4)
		// << _GMemory/MBytes_ << " MBYTES" << std::endl;
		<< "***" << " MBYTES" << std::endl;
	std::cout << "+------+-----+----+---+--+-+++++GTime:" << std::right << std::setw(4)
		<< _GlbTime << " SEC.  GMemo:" << std::right << std::setw(4)
		//<< _GMemory/MBytes_ << " MBYTES" << std::endl;
		<< "***" << " MBYTES" << std::endl;

	return;
}

// ----------------------- show a error --------------------------
void HMessage::Error(const std::string &str) {
	_LogFile << "Error---" << str << std::endl;
	std::cerr << "Error---" << str << std::endl;
	exit(EXIT_FAILURE);

	return;
}

// --------------------------------------------------------------------
void HMessage::DisMemry(double mem, const std::string &str) {
	if (!disp) return;
	double per;
	std::string unit;
	if (mem<KBytes_) {
		per = 1;		unit = " B";
	}
	else if (mem<MBytes_) {
		per = KBytes_;	unit = "KB";
	}
	else if (mem<GBytes_) {
		per = MBytes_;	unit = "MB";
	}
	else if (mem<TBytes_) {
		per = GBytes_;	unit = "GB";
	}
	else {
		per = TBytes_;	unit = "TB";
	}
	_LogFile << "\tmemery: " << std::right << std::setw(4) << mem / per << unit << str << std::endl;
	std::cout << "\tmemery: " << std::right << std::setw(4) << mem / per << unit << str << std::endl;
}
// -------------------------------------------------------------
void HMessage::Debug(void) {
	std::cout << "==============================================" << std::endl;
	std::cout << "     " << "Debug Here 1 sec sleep" << std::endl;
	std::cout << "==============================================" << std::endl;

	//_sleep(1000);

	// Create Log file
	std::string fname = _PrjName + ".log";
	_LogFile.open(fname.c_str());
	if (!_LogFile.is_open()) // check for file open error
	{
		Error("Error: create log file" + fname + " failed!");
	}

	return;
}
