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

#ifndef MZDATA_H
#define MZDATA_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "MZDataBase.h"

class MZData : public MZDataBase
{
public:
    MZData (void* pMemory);
    virtual ~MZData ();

    virtual double* GetRange ();
    virtual double GetRangeMin ();
    virtual double GetRangeMax ();

    virtual void SetRangeMin (double dValue);
    virtual void SetRangeMax (double dValue);
    virtual void SetRange (double* array);

protected:
    double* mRange; //[2]
};

inline double* MZData::GetRange ()
{
    return (mRange);
}

inline double MZData::GetRangeMin ()
{
    return (mRange[0]);
}

inline double MZData::GetRangeMax ()
{
    return (mRange[1]);
}

inline void MZData::SetRangeMin (double dValue)
{
    mRange[0] = dValue;
}

inline void MZData::SetRangeMax (double dValue)
{
    mRange[1] = dValue;
}

inline void MZData::SetRange (double* array)
{
    mRange = mRange;
}

#endif // MZDATA_H

