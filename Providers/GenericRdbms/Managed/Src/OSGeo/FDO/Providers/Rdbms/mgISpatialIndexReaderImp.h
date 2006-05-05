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
*
*/

#pragma once

#include "mgISpatialIndexReader.h"

class FdoISpatialIndexReader;

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

/// <summary>The ISpatialIndexReader interface provides forward only, readonly
/// functionality for enumerating spatial indexes.  A reference to an
/// ISpatialIndexReader is returned from the GetSpatialIndexes command.
/// The initial position of the ISpatialIndexReader is prior to the
/// first item. Therefore you must call ReadNext to begin accessing any data.</summary>
private __gc class ISpatialIndexReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable,  public NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::ISpatialIndexReader
{
public private:
	FdoISpatialIndexReader* GetImpObj();
	
	ISpatialIndexReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete);   

public:
    /// <summary>Gets the name of the spatial index currently being read.</summary>
    /// <returns>Returns the name of the spatial context.</returns>
	__property System::String* get_Name();

    /// <summary>Gets the name of the current spatial context as a String.</summary>
    /// <returns>Returns the description of the spatial context </returns>
	__property System::String* get_SpatialContextName();

    /// <summary>Gets the type of the current spatial Index, either Quad-Tree or R-Tree.</summary>
    /// <returns>Returns the spatial index type</returns>
	__property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType get_RdbmsSpatialIndexType();

    /// <summary>Gets the number of dimensions of the current spatial Index.</summary>
    /// <returns>Returns the extent type</returns>
    __property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType get_NumDimensions();

    /// <summary>Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Therefore you must call ReadNext to begin
    /// accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns>
	System::Boolean ReadNext();

    /// <summary>Closes the ISpatialIndexReader object freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing</returns>
	System::Void Dispose();

protected:
	System::Void Dispose(System::Boolean disposing);
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS


