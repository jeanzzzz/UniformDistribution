#pragma once

#include <limits>
#include <cmath>
#include "Macro.h"

// Set parameters
const bool disp = true;
const double C = 1;
const double FREQUENCY			= 3.0e9;					//Hz
const double CHARGE				= (8e-7) / pow(C, 1.0 / 2);					//C
//const double CHARGE = (1.0e-6) / C;
//const double CHARGE = (1.0e-6);


// Physics parameters
const double SPEED_LIGHT = 3.0e8;					//ms-1
const double COULOMB_CONSTANT = 8.987551787e9;			//Nm2C-2
const double LAMBDA = SPEED_LIGHT / FREQUENCY;	//m


// Method parameters
const double STOP_FORCE			= 0.00005;					//N
const double DENSITY			= 0.1 * C; //lambda-2
const int REPEATMAX				= 1000; 
const int NEARNODE				= 20;
const int MAXMOVE = 1;
const double DISTANCE_MAX		= 1;	
//const double DISTANCE_MAX = sqrt(10 / (DENSITY/LAMBDA/LAMBDA)); //m
const double DISTANCE_MIN = 1e-3;
//const double DISTANCE_MIN = 1e-2;

// ------------------------------------
// file sizes
// ------------------------------------
const double KBytes_ = 1024;					const double MBytes_ = KBytes_ * KBytes_;
const double GBytes_ = MBytes_ * KBytes_;		const double TBytes_ = GBytes_ * KBytes_;