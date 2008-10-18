#ifndef _FDORDBMSDESTROYSPATIALINDEX_H_
#define _FDORDBMSDESTORYSPATIALINDEX_H_
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
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsConnection.h"
#include "Rdbms/IDestroySpatialIndex.h"

/// <summary>Implementation class for IDestroySpatialIndex interface. </summary>
class FdoRdbmsDestroySpatialIndex : public FdoRdbmsCommand<FdoIDestroySpatialIndex>
{
    friend class FdoIConnection;

public:

    FdoRdbmsDestroySpatialIndex(FdoIConnection *connection);

    /// <summary>Gets the name of the spatial index to destroy as a String.</summary>
    /// <returns>Returns the name of the spatial index to be drop</returns>
    FDORDBMS_API virtual const wchar_t* GetName();

    /// <summary>Sets the name of the spatial index to drop as a String.</summary>
    /// <param name="value">Input the name of the spatial index to be drop</param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetName(const wchar_t* value);

    /// <summary>Sets the desired value of the geometric property.</summary>
    /// <param name="value">Input the geometric property definition </param>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void SetGeometricProperty(FdoGeometricPropertyDefinition *value);

    /// <summary>Gets the geometric property previously set, if any.</summary>
    /// <returns>Returns NULL if not set or a pointer to a geometry property definition</returns>
    FDORDBMS_API virtual FdoGeometricPropertyDefinition *GetGeometricProperty();

    /// <summary>Executes the DestroySpatialIndex command dropping the specified
    /// spatial index. An exception is thrown if the spatial index does not exist.</summary>
    /// <returns>Returns nothing</returns>
    FDORDBMS_API virtual void Execute();

protected:
    DbiConnection*  mConnection;

    virtual ~FdoRdbmsDestroySpatialIndex();

private:
    wchar_t*        mSIName;

    FdoGeometricPropertyDefinition*     mGeometricProperty;

    void            getPhysicalTableColumn( char *tbl_name, char *col_name);
    void            dropSpatialIndex( char *table_name, char *si_name );
    bool            findSpatialIndexName( char *db_name, char *tbl_name, char *col_name, char *si_name );
    bool            findSpatialIndexTableName( char *db_name, char *si_name, char *table_name );

};
#endif

