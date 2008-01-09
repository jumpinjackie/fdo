/******************************************************************************
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
 ******************************************************************************/


#ifndef FdoRdbmsSpatialSecondaryFilter_h
#define FdoRdbmsSpatialSecondaryFilter_h

#ifdef _WIN32
#pragma once
#endif


/// <summary>This abstract class may be used for precise secondary filtering of 
/// geometry values.  It is used to compare each incoming geometry
/// property value to a filter condition.</summary>
class FdoRdbmsSpatialSecondaryFilter : public FdoDisposable
{
public:

    virtual bool MeetsCondition(
        FdoIGeometry * geometry)= 0;

    virtual FdoString *  GetPropertyName() = 0;
   	virtual FdoSpatialOperations GetOperation() = 0;

protected:

    FdoRdbmsSpatialSecondaryFilter          () {};
    virtual ~FdoRdbmsSpatialSecondaryFilter () {};

};  //  class FdoRdbmsSpatialSecondaryFilter

#endif


