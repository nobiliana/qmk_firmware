/* Copyright 2023 nobiliana, aka. Surnia
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "report.h"

//friction, needs grav constant. a = gu
const float grav = 9.82;
const float PI = 3.14159;
const float rad2deg = 180/PI;
int32_t frictionMultiplier = 100;
const int16_t decimalPrecision = 1000;

#ifndef friction
#      define friction 350
#endif

//Take Final delta values pre-liftoff, apply kinetic. May need to pull LAST deltas, as liftoff detection may occur at same time of x/y wipe. 
int16_t xVal = 0; //will be int in the final op, currently a test value. Will be equal to x delta.
int16_t yVal = 0; //will be int in the final op, currently a test value. Will be equal to y delta. 


int8_t LIFTOFF = 1; //False being contact, True being finger off. 
int8_t kineticInit = 1; //variable to initialize the kinetic values before start. "inverted" to make logic more visually correct


typedef struct {
    int16_t xPoint;
    int16_t yPoint;
    float magValue;
    float angValue;
    int16_t xTemp;
    int16_t yTemp;
    int16_t xDel;
    int16_t yDel;
} mouseThings;

//Function Declarations
float kineticDrag (float vecAngle, float vecMagn);
void kineticVector (int32_t xMouse, int32_t yMouse);
void kineticCirque (report_mouse_t *mouse_report);
