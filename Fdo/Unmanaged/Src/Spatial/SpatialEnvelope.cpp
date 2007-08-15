/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include "SpatialEnvelope.h"
#include <math.h>
#include <Fdo.h>

FdoSpatialEnvelope::FdoSpatialEnvelope(void)
{
    Initialize();
}

FdoSpatialEnvelope::~FdoSpatialEnvelope(void)
{
}

void FdoSpatialEnvelope::Initialize(void)
{
    minx = HUGE_VAL;
    signbit_minx = sign_bit(minx);
    
    maxx = -HUGE_VAL;
    signbit_maxx = sign_bit(maxx);

    miny = HUGE_VAL;
    signbit_miny = sign_bit(miny);

    maxy = -HUGE_VAL;
    signbit_maxy = sign_bit(maxy);

    minz = HUGE_VAL;
    signbit_minz = sign_bit(minz);

    maxz = -HUGE_VAL;
    signbit_maxz = sign_bit(maxz);

    is_z_valid = true;
}

void FdoSpatialEnvelope::AddBound(double x, double y, double z, bool processZ)
{
    minimize(minx, signbit_minx, x);
    maximize(maxx, signbit_maxx, x);
    
    minimize(miny, signbit_miny, y);
    maximize(maxy, signbit_maxy, y);

    if (processZ)
    {
        minimize(minz, signbit_minz, z);
        maximize(maxz, signbit_maxz, z);
    }
}

void FdoSpatialEnvelope::AddMinBound(double x, double y, double z, bool processZ)
{
    minimize(minx, signbit_minx, x);
    minimize(miny, signbit_miny, y);
    if (processZ)
        minimize(minz, signbit_minz, z);
}

void FdoSpatialEnvelope::AddMaxBound(double x, double y, double z, bool processZ)
{
    maximize(maxx, signbit_maxx, x);
    maximize(maxy, signbit_maxy, y);
    if (processZ)
        maximize(maxz, signbit_maxz, z);
}

void FdoSpatialEnvelope::minimize(double& value_min, int &signbit_min, double value)
{
    int signbitvalue = sign_bit(value);

    if (signbitvalue == signbit_min)
    {
        //otherwise signs are the same and we can compare as ints
        switch (signbitvalue)
        {
        case 0:
            if (*(FdoInt64*)&value < *(FdoInt64*)&value_min)
                value_min = value;
            break;
        default:
            if (*(FdoInt64*)&value > *(FdoInt64*)&value_min)
                value_min = value;
            break;
        }
    }
    else
    {
        //signs different

        switch (signbitvalue)
        {
        case 0:
            break;
        default:
            //is x the negative?
            value_min = value;
            signbit_min = signbitvalue;
            break;
        }
    }
}

void FdoSpatialEnvelope::maximize(double& value_max, int &signbit_max, double value)
{
    int signbitvalue = sign_bit(value);

    if (signbitvalue == signbit_max)
    {
        //otherwise signs are the same and we can compare as ints
        switch (signbitvalue)
        {
        case 0:
            if (*(FdoInt64*)&value > *(FdoInt64*)&value_max)
                value_max = value;
            break;
        default:
            if (*(FdoInt64*)&value < *(FdoInt64*)&value_max)
                value_max = value;
            break;
        }
    }
    else
    {
        //signs different

        switch (signbitvalue)
        {
        case 0:
            //is x the positive?
            value_max = value;
            signbit_max = signbitvalue;
            break;
        default:
            break;
        }
    }
}

