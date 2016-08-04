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

/// <summary>The ICreateSpatialIndex interface defines the CreateSpatialIndex
/// command, which is spatial indexing the speciified spatial context.
/// Input to the command includes the name, spatial context, special index type,
/// the number of dimensions for the new spatial index.</summary>
public interface class ICreateSpatialIndex  : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// <summary>Gets the name of the Spatial index as a String.</summary>
    /// <returns>Returns the name of the spatial Index </returns>
    /// <summary>Sets the name of the spatial Index  to create as a String.</summary>
    /// <param name="value">Input the name of the spatial Index </param>
    /// <returns>Returns nothing</returns>
    property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// <summary>Gets the name of the spatial context as a String.</summary>
    /// <returns>Returns the description of the spatial context </returns>
    /// <summary>Sets the name of the spatial context to create as a String.</summary>
    /// <param name="value">Input the description of the spatial spatial Index </param>
    /// <returns>Returns nothing</returns>
    property System::String^ SpatialContextName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// <summary>Gets the type of the spatial Index, either Quad-Tree or R-Tree.</summary>
    /// <returns>Returns the spatial index type</returns>	
    /// <summary>Sets the desired spatial Index  to create, either Quad-Tree or R-Tree.</summary>
    /// <param name="value">Input the index type</param>
    /// <returns>Returns nothing</returns>	
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType RdbmsSpatialIndexType
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexType value);
    }

    /// <summary>Gets the number of dimensions of the spatial Index.</summary>
    /// <returns>Returns the extent type</returns>
    /// <summary>Sets the desired value of the spatial Index dimension.</summary>
    /// <param name="value">Input the index dimension </param>
    /// <returns>Returns nothing</returns>
    property NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType NumDimensions
    {
        NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType get();
        System::Void set(NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS::RdbmsSpatialIndexDimensionType value);
    }

    /// <summary>Sets the geometric property.</summary>
    /// <param name="value">Input the geometric property definition.</param>
    /// <returns>Returns nothing</returns>
    /// <summary>Gets the geometric property previously set, if any.</summary>
    /// <returns>Returns NULL if not set or a pointer to a geometry property definition</returns>
    property NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ GeometricProperty
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ value);
    }

    /// <summary>Executes the CreateSpatialIndex command. An exception is thrown if
    /// the spatial index or the spatial context names are not indicated.</summary>
    /// <returns>Returns nothing</returns>
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_PROVIDERS_RDBMS


