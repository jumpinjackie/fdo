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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSpatialContextReader.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPSPATIALCONTEXTREADER_H
#define FDORFPSPATIALCONTEXTREADER_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpGetSpatialContexts;
class FdoRfpSpatialContextCollection;

class FdoRfpSpatialContextReader : public FdoISpatialContextReader
{
	friend class FdoRfpGetSpatialContexts;
private:
	FdoPtr<FdoRfpSpatialContextCollection> m_spatialContexts;
	FdoStringP m_activeSC;
	FdoInt32 m_cursor;
//Helper functions
private:
	void _validate();

protected:
	FdoRfpSpatialContextReader(const FdoPtr<FdoRfpSpatialContextCollection>& spatialContexts, FdoString* activeSC);
	~FdoRfpSpatialContextReader();
public:
    /// <summary>Gets the name of the spatial context currently being read.</summary>
    /// <returns>Returns the name of the spatial context.</returns> 
    virtual FdoString* GetName();

    /// <summary>Gets the description of the spatial context currently being read.</summary>
    /// <returns>Returns the description of the spatial context.</returns> 
    virtual FdoString* GetDescription();

    /// <summary>Gets the name of the coordinate system of the spatial context currently
    /// being read. If the result is a null or empty string, the coordinate system is
    /// unknown to the spatial context.</summary>
    /// <returns>Returns the coordinate system name of the spatial context.</returns> 
    virtual FdoString* GetCoordinateSystem();

    /// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.</summary>
    /// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
    virtual FdoString* GetCoordinateSystemWkt();

    /// <summary>Gets the extent type of the spatial context currently being read.</summary>
    /// <returns>Returns the extent type.</returns> 
    virtual FdoSpatialContextExtentType GetExtentType();

    /// <summary>Gets the extent of the spatial context currently being read as a byte
    /// array in AGF format.</summary>
    /// <param name="length">Output the length of the returned byte array.</param> 
    /// <returns>Returns the extent as a byte array in AGF format.</returns> 
    virtual FdoByteArray* GetExtent();

    /// <summary>Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    virtual const double GetXYTolerance();

    /// <summary>Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. Tolerances are used in some geometric tests,
    /// mostly for equality between coordinates.  This occurs a frequently in spatial
    /// queries, especially with "on boundary" or "just touching" conditions.
    /// It is also an objective amount that can be used when formulating step
    /// distances for parametric curve interpolation.</summary>
    /// <returns>Returns the tolerance</returns>
    virtual const double GetZTolerance();

    /// <summary>Returns true if the spatial context currently being read is the active
    /// spatial context.</summary>
    /// <returns>Returns true if the current spatial context is the active one.</returns> 
    virtual const bool IsActive();

    /// <summary>Advances the reader to the next item. The default position of the reader
    /// is prior to the first item. Thus, you must call ReadNext to begin
    /// accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext();

    /// <summary>Closes the FdoISpatialContextReader object, freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Dispose();

};

typedef FdoPtr<FdoRfpSpatialContextReader> FdoRfpSpatialContextReaderP;


#endif
