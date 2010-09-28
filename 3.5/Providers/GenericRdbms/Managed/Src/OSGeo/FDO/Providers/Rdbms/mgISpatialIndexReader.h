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

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class GeometricPropertyDefinition;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS

/// <summary>The ISpatialIndexReader interface provides forward only, readonly
/// functionality for enumerating spatial indexes.  A reference to an
/// ISpatialIndexReader is returned from the GetSpatialIndexes command.
/// The initial position of the ISpatialIndexReader is prior to the
/// first item. Therefore you must call ReadNext to begin accessing any data.</summary>
public interface class ISpatialIndexReader : public System::IDisposable
{
public:
    /// <summary>Gets the name of the spatial index currently being read.</summary>
    /// <returns>Returns the name of the spatial context.</returns>
    property System::String^ Name
    {
        System::String^ get();
    }

    /// <summary>Gets the name of the current spatial context as a String.</summary>
    /// <returns>Returns the description of the spatial context </returns>
    property System::String^ SpatialContextName
    {
        System::String^ get();
    }

    /// <summary>Gets the type of the current spatial Index, either Quad-Tree or R-Tree.</summary>
    /// <returns>Returns the spatial index type</returns>	
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType RdbmsSpatialIndexType
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType get();
    }

    /// <summary>Gets the number of dimensions of the current spatial Index.</summary>
    /// <returns>Returns the extent type</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType NumDimensions
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType get();
    }

    /// <summary>Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Therefore you must call ReadNext to begin
    /// accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns>
	System::Boolean ReadNext();

    /// <summary>Closes the ISpatialIndexReader object freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing</returns>
	System::Void DisposeReader();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS


