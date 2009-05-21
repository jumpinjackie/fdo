/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifndef _c_KgKmlSpatialContextReader_h
#define _c_KgKmlSpatialContextReader_h

#include "c_KgKmlSpatialContext.h"

class c_KgKmlSpatialContextReader :
    public FdoISpatialContextReader
{



public:
    c_KgKmlSpatialContextReader (c_KgKmlSpatialContextCollection*  spatialContextColl );
    virtual ~c_KgKmlSpatialContextReader (void);

    virtual void Dispose ();
    
protected:
	FdoPtr<c_KgKmlSpatialContextCollection>	m_SpatialContextColl;
  FdoPtr<c_KgKmlSpatialContext>				m_Context; // the current read context
	int								m_CurrIndex;  

    

public:
    //
    // FdoISpatialContextReader interface
    //

    /// <summary>Gets the name of the spatial context currently being read.</summary>
    /// <returns>Returns the name of the spatial context.</returns> 
    virtual FdoString* GetName ();

    /// <summary>Gets the description of the spatial context currently being read.</summary>
    /// <returns>Returns the description of the spatial context.</returns> 
    virtual FdoString* GetDescription ();

    /// <summary>Gets the name of the coordinate system of the spatial context currently
    /// being read. If the result is a null or empty string, the coordinate system is
    /// unknown to the spatial context.</summary>
    /// <returns>Returns the coordinate system name of the spatial context.</returns> 
    virtual FdoString* GetCoordinateSystem ();

    /// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
    virtual FdoString* GetCoordinateSystemWkt ();

    /// <summary>Gets the extent type of the spatial context currently being read.</summary>
    /// <returns>Returns the extent type.</returns> 
    virtual FdoSpatialContextExtentType GetExtentType ();

    /// <summary>Gets the extent of the spatial context currently being read as a byte
    /// array in AGF format.</summary>
    /// <param name="length">Output the length of the returned byte array.</param> 
    /// <returns>Returns the extent as a byte array in AGF format.</returns> 
    virtual FdoByteArray* GetExtent ();

    /// <summary>Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    virtual const double GetXYTolerance ();

    /// <summary>Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    virtual const double GetZTolerance ();

    /// <summary>Returns true if the spatial context currently being read is the active
    /// spatial context.</summary>
    /// <returns>Returns true if the current spatial context is the active one.</returns> 
    virtual const bool IsActive ();

    /// <summary>Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Thus, you must call ReadNext to begin
    /// accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext ();


};

#endif 
