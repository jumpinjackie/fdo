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
class SdfConnection;

/// <summary>The ISpatialContextReader interface provides forward only, readonly
/// functionality for enumerating spatial contexts.  A reference to an
/// ISpatialContextReader is returned from the GetSpatialContexts command.
/// The initial position of the ISpatialContextReader is prior to the
/// first item. Therefore you must call ReadNext to begin accessing any data.</summary>
class SdfSpatialContextReader : public FdoISpatialContextReader
{
 
    //-------------------------------------------------------
    // Constructors / Destructor
    //-------------------------------------------------------

public:

    //    Constructs an instance of a FdoRdbmsSpatialContextReader using the
    //    specified arguments.
    SdfSpatialContextReader (SdfConnection* conn);

protected:

    virtual ~SdfSpatialContextReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------


    void Dispose();

    //-------------------------------------------------------
    // FdoISpatialContextReader implementation
    //-------------------------------------------------------


public:

	/// <summary>Gets the name of the spatial context currently being read.</summary>
	/// <returns>Returns the name of the spatial context.</returns> 
	SDF_API virtual FdoString* GetName();

	/// <summary>Gets the description of the spatial context currently being read.</summary>
	/// <returns>Returns the description of the spatial context.</returns> 
	SDF_API virtual FdoString* GetDescription();

	/// <summary>Gets the name of the coordinate system of the spatial context currently
    /// being read. If null or empty string then the coordinate system is
    /// unknown to the spatial context.</summary>
	/// <returns>Returns the coordinate system name of the spatial context.</returns> 
	SDF_API virtual FdoString* GetCoordinateSystem();

    /// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
    SDF_API virtual FdoString* GetCoordinateSystemWkt();

	/// <summary>Gets the extent type of the spatial context currently being read.</summary>
	/// <returns>Returns the extent type.</returns> 
	SDF_API virtual FdoSpatialContextExtentType GetExtentType();

	/// <summary>Gets the extent of the spatial context currently being read as a byte
    /// array in FGF format.</summary>
	/// <param name="length">Output the length of the returned byte array.</param> 
	/// <returns>Returns the extent as a byte array in FGF format.</returns> 
	SDF_API virtual FdoByteArray* GetExtent();

    /// <summary>Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a lot in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    SDF_API virtual const double GetXYTolerance();

    /// <summary>Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a lot in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    SDF_API virtual const double GetZTolerance();

	/// <summary>Returns true if the spatial context currently being read is the active
	/// spatial context.</summary>
	/// <returns>Returns true if the current spatial context is the active one.</returns> 
	SDF_API virtual const bool IsActive();

	/// <summary>Advances the reader to the next item. The default position of the reader
	/// is prior to the first item. Therefore you must call ReadNext to begin
	/// accessing any data.</summary>
	/// <returns>Returns true if there is a next item.</returns> 
	SDF_API virtual bool ReadNext();


    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------


private:


    SdfConnection*              m_connection;
    bool                        m_activeOnly;
    bool                        m_firstRead;
    bool                        m_isActiveSC;

    bool                        m_noMoreSC;
    wchar_t*                    m_scName;
    wchar_t*                    m_description;
    wchar_t*                    m_srsName;

    FdoSpatialContextExtentType m_extentType;
    FdoByteArray*               m_extent;

    double                      m_xyTolerance;
    double                      m_zTolerance;
    int                         m_dimensionality;
};

