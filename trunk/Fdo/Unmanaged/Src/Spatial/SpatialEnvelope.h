/*
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
 */

#ifdef _WIN32
#pragma once
#endif

#ifndef FdoSpatialEnvelope___H
#define FdoSpatialEnvelope___H	1

struct FdoSpatialEnvelope
{
public:
    FdoSpatialEnvelope(void);
    ~FdoSpatialEnvelope(void);
    void Initialize(void);
    void AddBound(double x, double y, double z, bool processZ);
    void AddMinBound(double x, double y, double z, bool processZ);
    void AddMaxBound(double x, double y, double z, bool processZ);
    
    double minx;
    double miny;
    double minz;
    double maxx;
    double maxy;
    double maxz;
    bool is_z_valid;
private:
    inline int sign_bit(double d)
    {
        return *((int*)&d+1) & 0x80000000;
    }
    
    void minimize(double& value_min, int &signbit_min, double value);
    void maximize(double& value_max, int &signbit_max, double value);
private:
    int signbit_minx; 
    int signbit_miny; 
    int signbit_minz; 
    int signbit_maxx; 
    int signbit_maxy; 
    int signbit_maxz; 
};

#endif // FdoSpatialEnvelope___H

