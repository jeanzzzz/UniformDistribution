#pragma once

#include <limits>
#include <cmath>
#include "Macro.h"

// Set parameters
const bool disp = true;
const int C = 8;
const double FREQUENCY			= 3.0e9;					//Hz
const double CHARGE				= (1.0e-6) / pow(C, 1.0 / 2);					//C
//const double CHARGE = (1.0e-6) / C;
//const double CHARGE = (1.0e-6);

// Method parameters
const double DISTANCE_MAX		= 1 / pow(C, 1.0/2);						//m
const double DISTANCE_MIN = 1e-2 / pow(C, 1.0 / 2);
//const double DISTANCE_MIN = 1e-2;
const double STOP_FORCE			= 0.00005;					//N
const double DENSITY			= 0.02 * C;
const int REPEATMAX				= 10; 
const int NEARNODE				= 20;


// Physics parameters
const double SPEED_LIGHT		= 3.0e8;					//ms-1
const double COULOMB_CONSTANT	= 8.987551787e9;			//Nm2C-2
const double LAMBDA				= SPEED_LIGHT / FREQUENCY;	//m

// ------------------------------------
// file sizes
// ------------------------------------
const double KBytes_ = 1024;					const double MBytes_ = KBytes_ * KBytes_;
const double GBytes_ = MBytes_ * KBytes_;		const double TBytes_ = GBytes_ * KBytes_;