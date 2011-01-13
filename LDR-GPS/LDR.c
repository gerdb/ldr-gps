//***************************************************************************
//
//  File........: LDR.c
//
//  Author(s)...: Gerd Bartelt
//
//  Target(s)...: ATmega169
//
//  Compiler....: AVR-GCC 4.3.4; avr-libc 1.0
//
//  Description.: AVR Butterfly LDR routines
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Revisions...: 1.0
//
//  YYYYMMDD - VER. - COMMENT                                       - SIGN.
//
//  20110107 - 1.0  - Created                                       - Gerd Bartelt
//
//***************************************************************************

#include <avr/io.h>

#include "button.h"
#include "main.h"
#include "LDR.h"
#include "RTC.h"
#include "GPS.h"
#include "ADC.h"

// Flags for sun rise and sun set sampling
unsigned char bSunRiseSampled;
unsigned char bSunSetSampled;
unsigned char bSunRiseDetected;
unsigned char bSunSetDetected;

// Threshold to detect the sun rise and sun set with the LDR
unsigned int threshold_SunRise;
unsigned int threshold_SunSet;

// Sun rise and sun set time at the home position
double sunrise_home;
double sunset_home;

// Measured time of sun rise and sun set
volatile double sunrise_meas;
volatile double sunset_meas;
volatile double daytime_meas = 9.724;
volatile double daymedian_meas = 11.571;

// LDR value at calculates sun rise and sun set
volatile int sunRiseLDRVal = 0;
volatile int sunSetLDRVal = 0;

/**
 * Initializes the ADC for light measurement
 */
void LDR_init(void) {
	ADC_init(LIGHT_SENSOR);     // Init the ADC;
}

/**
 * Do (every 1sec) a measurement of the light (LDR) value.
 *
 * If a sun rise or sun set is detected, the time and LDR value are
 * sampled.
 *
 */
void MeasureLight(void) {

	// Read the ADC value
	int LDRValue = ADC_read();

	// Detect a sun rise (only before 12:00)
	if (!bSunRiseDetected  && (gHOUR < 12) ) {
		if (LDRValue < threshold_SunRise) {

			// Store the time of the sun rise
			bSunRiseDetected = 1;
			sunrise_meas = gTIME;
		}
	}
	// Detect a sun set (only before 12:00)
	else if (!bSunSetDetected  && (gHOUR > 12) ) {
		if (LDRValue > threshold_SunSet) {
			bSunSetDetected = 1;

			// Store the time of the sun set
			// and calculate the mean time an the time between both.
			sunset_meas = gTIME;
			daytime_meas = sunset_meas - sunrise_meas;
			daymedian_meas = (sunset_meas + sunrise_meas) * 0.5;
		}
	}

	// Sample the LDR value at the calculated time for sun rise
	if (!bSunRiseSampled) {
		if (gTIME > sunrise_home) {
			bSunRiseSampled = 1;
			sunRiseLDRVal = LDRValue;
		}
	}

	// Sample the LDR value at the calculated time for sun set
	else if (!bSunSetSampled) {
		if (gTIME > sunset_home) {
			bSunSetSampled = 1;
			sunSetLDRVal = LDRValue;
		}
	}

}


/**
 * A new day (23:59:59 -> 00:00:00) has started.
 *
 * So reinitialize the measurement for a new pair of sun rise and sun set
 *
 */
void NewDay(void) {

	// Reset all flags
	bSunRiseDetected = 0;
	bSunSetDetected = 0;
	bSunRiseSampled = 0;
	bSunSetSampled = 0;

	// Default threshold at
	threshold_SunRise = 840; // R_LDR = 5Meg
	threshold_SunSet = 942;  // R_LDR = 12Meg

	// Calculate sunrise and sunset at home position
	CalcDayParameter();
	pos_lat = 0.84910268;
	pos_lon = 0.14195345;
	CalcSunSetAndRise();
	sunrise_home = sunrise_calc;
	sunset_home = sunset_calc;

}
