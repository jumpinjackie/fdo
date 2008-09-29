#ifndef _FDORDBMSCREATESPATIALINDEX_H_
#define _FDORDBMSCREATESPATIALINDEX_H_
//
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
#pragma once
#endif

#include "FdoRdbms.h"
#include "../FeatureCommands/FdoRdbmsCommand.h"
#include "../Connection/FdoRdbmsConnection.h"

#include "Rdbms/ICreateSpatialIndex.h"

/// <summary>Implementation class for ICreateSpatialIndex interface. </summary>

class FdoRdbmsCreateSpatialIndex : public FdoRdbmsCommand<FdoICreateSpatialIndex>
{
    friend class FdoIConnection;

public:

    FdoRdbmsCreateSpatialIndex(FdoIConnection *connection);

    /// <summary>Gets the name of the Spatial index as a String.</summary>
    /// <returns>Returns the name of the spatial Index </returns>
    FDORDBMS_API virtual const wchar_t* GetName();

    /// <summary>Sets the name of the spatial Index  to create as a String.</summary>
    /// <param name="value">Input the name of the spatial Index </param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetName(const wchar_t* value);

    /// <summary>Gets the name of the spatial context as a String.</summary>
    /// <returns>Returns the description of the spatial context </returns>
    FDORDBMS_API virtual const wchar_t* GetSpatialContextName();

    /// <summary>Sets the name of the spatial context to create as a String.</summary>
    /// <param name="value">Input the description of the spatial spatial Index </param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetSpatialContextName(const wchar_t* value);

    /// <summary>Gets the type of the spatial Index, either Quad-Tree or R-Tree.</summary>
    /// <returns>Returns the spatial index type</returns>
    FDORDBMS_API virtual SpatialIndexType GetSpatialIndexType();

    /// <summary>Sets the desired spatial Index  to create, either Quad-Tree or R-Tree.</summary>
    /// <param name="value">Input the index type</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetSpatialIndexType(SpatialIndexType value);

    /// <summary>Gets the number of dimensions of the spatial Index.</summary>
    /// <returns>Returns the extent type</returns>
    FDORDBMS_API virtual SpatialIndexDimensionType GetNumDimensions();

    /// <summary>Sets the desired value of the spatial Index dimension.</summary>
    /// <param name="value">Input the index dimension </param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetNumDimensions(SpatialIndexDimensionType value);

    /// <summary>Sets the desired value of the geometric property.</summary>
    /// <param name="value">Input the geometric property definition </param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetGeometricProperty(FdoGeometricPropertyDefinition *value);

    /// <summary>Gets the geometric property previously set, if any.</summary>
    /// <returns>Returns NULL if not set or a pointer to a geometry property definition</returns>
    FDORDBMS_API virtual FdoGeometricPropertyDefinition *GetGeometricProperty();

    /// <summary>Executes the CreateSpatialIndex command. An exception is thrown if
    /// ... .</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();

protected:
    DbiConnection*  mConnection;

    virtual ~FdoRdbmsCreateSpatialIndex();

private:
    wchar_t*                    mSIName;
    SpatialIndexType            mSIType;
    SpatialIndexDimensionType   mSIDimensions;
    wchar_t*                    mSpatialContextName;

    FdoGeometricPropertyDefinition* mGeometricProperty;
};
#endif

