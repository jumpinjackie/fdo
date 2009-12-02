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

#ifndef MZDATABASE_H
#define MZDATABASE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class MZDataBase
{
public:
    MZDataBase (void* pMemory);
    virtual ~MZDataBase ();

    virtual double GetRangeMin ();
    virtual double GetRangeMax ();

    virtual double* GetArray ();
    virtual void SetArray (double* pMArray);

protected:
    double* mArray; // pointer to the one data value (more in subclasses)
};

inline double MZDataBase::GetRangeMin ()
{
    return (mArray[0]);
}

inline double MZDataBase::GetRangeMax ()
{
    return (mArray[0]);
}

inline double* MZDataBase::GetArray ()
{
    return (mArray);
}

inline void MZDataBase::SetArray (double* pMArray)
{
    mArray = pMArray;
}

#endif // MZDATABASE_H

