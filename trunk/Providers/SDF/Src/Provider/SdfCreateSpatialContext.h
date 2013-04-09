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
#include "SdfCommand.h"

/// <summary>The ICreateSpatialContext interface defines the CreateSpatialContext
/// command, which creates a new spatial context. Input to the command includes
/// the name, description, coordinate system, extent type, and extent for the
/// new context. Command execution will fail if the context already exists, or
/// if the coordinate system or extent type is not supported by the provider.
/// Note that the geographic extent of a coordinate system should generally be
/// defined as an envelope (or bounding box). However any geometry type that
/// falls into the surface geometric types can be used (Polygons, etc.). If a
/// non-rectangular area is passed in and the provider only supports a
/// rectangular extent then the provider will compute the minimum box that
/// encloses the given geometry and use that for the extent value.</summary>
class SdfCreateSpatialContext : public SdfCommand<FdoICreateSpatialContext>
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:

    SDF_API SdfCreateSpatialContext(SdfConnection* connection);

protected:

    SDF_API virtual ~SdfCreateSpatialContext();


    //-------------------------------------------------------
    // FdoICreateSpatialContext implementation
    //-------------------------------------------------------

public:
    /// <summary>Gets the name of the context to create as a string.</summary>
    /// <returns>Returns the name of the spatial context</returns> 
    SDF_API virtual FdoString* GetName();

    /// <summary>Sets the name of the context to create as a string.</summary>
    /// <param name="value">Input the name of the spatial context</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetName(FdoString* value);

    /// <summary>Gets the description of the context to create as a string.</summary>
    /// <returns>Returns the description of the spatial context</returns> 
    SDF_API virtual FdoString* GetDescription();

    /// <summary>Sets the description of the context to create as a string.</summary>
    /// <param name="value">Input the description of the spatial context</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetDescription(FdoString* value);

  	/// <summary>Sets/add a geometric property definition to this spatial context.</summary>
	/// <param name="value">Input the geometric property definition to be added. 
    /// Defaults to the main geometry property of the class, if any. </param> 
	/// <returns>Returns nothing</returns> 
    SDF_API virtual void SetGeometricProperty(FdoPropertyDefinition *value);

   	/// <summary>Sets the desired dimensionality of the geometries.</summary>
	/// <param name="value">Input the dimensionality. Valid values are bitmapped 
    /// FdoDimensionality values. Defaults to (FdoDimensionality_XY | FdoDimensionality_Z).</param> 
	/// <returns>Returns nothing</returns> 
    SDF_API virtual void SetDimensionality(int value);

   	/// <summary>Gets the dimensionality of the geometries.</summary>
	/// <returns>Returns the dimensionality as bitmapped FdoDimensionality values.</returns> 
    SDF_API virtual int GetDimensionality();

    /// <summary>Gets the coordinate system of the context to create as a string
    /// in OpenGIS SRS WKT format.</summary>
    /// <returns>Returns the coordinate system</returns> 
    SDF_API virtual FdoString* GetCoordinateSystem();

    /// <summary>Sets the coordinate system of the context to create as a string in
    /// OpenGIS SRS WKT format.</summary>
    /// <param name="value">Input the coordinate system</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetCoordinateSystem(FdoString* value);

    /// <summary>Gets the coordinate system definition in OGC SRS WKT format for the 
    /// spatial context currently being created. If the result is an empty string, 
    /// the coordinate system WKT description has not been set.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
    SDF_API virtual FdoString* GetCoordinateSystemWkt();

    /// <summary>Sets the coordinate system definition in OGC SRS WKT 
    /// format for the spatial context currently being created.</summary>
    /// <param name="value">Input the coordinate system WKT</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetCoordinateSystemWkt(FdoString* value);

    /// <summary>Gets the desired extent type of the context to create, either static or dynamic.</summary>
    /// <returns>Returns the extent type</returns> 
    SDF_API virtual FdoSpatialContextExtentType GetExtentType();

    /// <summary>Sets the desired extent type of the context to create, either static or dynamic.</summary>
    /// <param name="value">Input the extent type</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetExtentType(FdoSpatialContextExtentType value);

    /// <summary>Gets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic, any value specified via this function is ignored.</summary>
    /// <returns>Returns the extent of the spatial context</returns> 
    SDF_API virtual FdoByteArray* GetExtent();

    /// <summary>Sets the extent of the context to create as a byte array in FGF format.
    /// If the extent type is dynamic this is optional and any value specified is ignored.</summary>
    /// <param name="value">Input the extent of the spatial context</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetExtent(FdoByteArray* value);

    /// <summary>Gets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    SDF_API virtual const double GetXYTolerance();

    /// <summary>Sets the tolerance value to use for X/Y ordinates. Tolerances are used
    /// in some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <param name="value">Input the tolerance</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetXYTolerance(const double value);

    /// <summary>Gets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    SDF_API virtual const double GetZTolerance();

    /// <summary>Sets the tolerance value to use for Z ordinates. Tolerances are used in
    /// some geometric tests, mostly for equality between coordinates.  This
    /// occurs frequently in spatial queries, especially with "on boundary" or "just
    /// touching" conditions.  It is also an objective amount that can be used
    /// when formulating step distances for parametric curve interpolation.</summary>
    /// <param name="value">Input the tolerance</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetZTolerance(const double value);

    /// <summary>Gets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.</summary>
    /// <returns>Returns true if command should update existing contexts</returns>
    SDF_API virtual const bool GetUpdateExisting();

    /// <summary>Sets a Boolean flag that indicates how the CreateSpatialContext command
    /// should behave if the spatial context already exists. If update existing
    /// is true and a spatial context with the specified name already exists
    /// then it will be updated. If update existing is false and a spatial
    /// context with the specified name already exists, command execution
    /// will fail.</summary>
    /// <param name="value">Input true if existing contexts should be updated</param>
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetUpdateExisting(const bool value);

    /// <summary>Executes the CreateSpatialContext command. An exception is thrown if
    /// the spatial context already exists, or if the coordinate system or
    /// extent type is not supported by the provider.</summary>
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void Execute();

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

private:
    wchar_t*                    m_scName;
    wchar_t*                    m_description;
    wchar_t*                    m_coordSysName;
    wchar_t*                    m_coordSysWkt;
    
    FdoSpatialContextExtentType m_extentType;
    FdoByteArray*               m_extent;
    FdoInt32                    m_extentLength;

    double                      m_xyTolerance;
    double                      m_zTolerance;

    bool                        m_updateExisting;

    int                         m_dimensionality;

};

