//***************************************************************************
//
//  File........: LDR.h
//
//  Author(s)...: Gerd Bartelt
//
//  Target(s)...: ATmega169
//
//  Compiler....: AVR-GCC 4.3.4; avr-libc 1.0
//
//  Description.: Header file for LCD.c
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

void MeasureLight(void);
void NewDay(void);
void LDR_init(void);

extern volatile double sunrise_meas;
extern volatile double sunset_meas;
extern volatile int sunRiseLDRVal;
extern volatile int sunSetLDRVal;
extern volatile double daytime_meas;
extern volatile double daymedian_meas;
