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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSpatialContextReader.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpSpatialContextReader.h"
#include "FdoRfpSpatialContext.h"

FdoRfpSpatialContextReader::FdoRfpSpatialContextReader(const FdoPtr<FdoRfpSpatialContextCollection>& spatialContexts, FdoString* activeSC) :
			m_spatialContexts(spatialContexts), m_activeSC(activeSC), m_cursor(-1)
{

}

FdoRfpSpatialContextReader::~FdoRfpSpatialContextReader()
{

}

void FdoRfpSpatialContextReader::_validate()
{
	if (m_cursor == -1)
		throw FdoCommandException::Create(NlsMsgGet(GRFP_57_READNEXT_MUST_BE_CALLED, "ReadNext must be called before fetching data."));
}

/// <summary>Gets the name of the spatial context currently being read.</summary>
/// <returns>Returns the name of the spatial context.</returns> 
FdoString* FdoRfpSpatialContextReader::GetName()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetName();
}

/// <summary>Gets the description of the spatial context currently being read.</summary>
/// <returns>Returns the description of the spatial context.</returns> 
FdoString* FdoRfpSpatialContextReader::GetDescription()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetDescription();
}

/// <summary>Gets the name of the coordinate system of the spatial context currently
/// being read. If the result is a null or empty string, the coordinate system is
/// unknown to the spatial context.</summary>
/// <returns>Returns the coordinate system name of the spatial context.</returns> 
FdoString* FdoRfpSpatialContextReader::GetCoordinateSystem()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetCoordinateSystem();
}

/// <summary>Gets the name of the coordinate system in OpenGIS SRS WKT format of the spatial context currently
/// being read. If the result is an empty string, the coordinate system WKT description is
/// not available.</summary>
/// <returns>Returns the coordinate system description in WKT of of the spatial context.</returns> 
FdoString* FdoRfpSpatialContextReader::GetCoordinateSystemWkt()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetCoordinateSystemWkt();
}

/// <summary>Gets the extent type of the spatial context currently being read.</summary>
/// <returns>Returns the extent type.</returns> 
FdoSpatialContextExtentType FdoRfpSpatialContextReader::GetExtentType()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetExtentType();
}


/// <summary>Gets the extent of the spatial context currently being read as a byte
/// array in AGF format.</summary>
/// <param name="length">Output the length of the returned byte array.</param> 
/// <returns>Returns the extent as a byte array in AGF format.</returns> 
FdoByteArray* FdoRfpSpatialContextReader::GetExtent()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetExtent();
}


/// <summary>Gets the tolerance value for XY ordinates of the spatial context
/// currently being read. Tolerances are used in some geometric tests,
/// mostly for equality between coordinates.  This occurs frequently in spatial
/// queries, especially with "on boundary" or "just touching" conditions.
/// It is also an objective amount that can be used when formulating step
/// distances for parametric curve interpolation.</summary>
/// <returns>Returns the tolerance</returns>
const double FdoRfpSpatialContextReader::GetXYTolerance()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetXYTolerance();
}


/// <summary>Gets the tolerance value for Z ordinates of the spatial context
/// currently being read. Tolerances are used in some geometric tests,
/// mostly for equality between coordinates.  This occurs a frequently in spatial
/// queries, especially with "on boundary" or "just touching" conditions.
/// It is also an objective amount that can be used when formulating step
/// distances for parametric curve interpolation.</summary>
/// <returns>Returns the tolerance</returns>
const double FdoRfpSpatialContextReader::GetZTolerance()
{
	_validate();
	return FdoRfpSpatialContextP(m_spatialContexts->GetItem(m_cursor))->GetZTolerance();
}

/// <summary>Returns true if the spatial context currently being read is the active
/// spatial context.</summary>
/// <returns>Returns true if the current spatial context is the active one.</returns> 
const bool FdoRfpSpatialContextReader::IsActive()
{
	_validate();
	return STREQUAL(m_activeSC, GetName());
}

/// <summary>Advances the reader to the next item. The default position of the reader
/// is prior to the first item. Thus, you must call ReadNext to begin
/// accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool FdoRfpSpatialContextReader::ReadNext()
{
	if (m_cursor + 1 < m_spatialContexts->GetCount())
	{
		m_cursor++;
		return true;
	}
	return false;
}

/// <summary>Closes the FdoISpatialContextReader object, freeing any resources it may
/// be holding.</summary>
/// <returns>Returns nothing</returns> 
void FdoRfpSpatialContextReader::Dispose()
{
	delete this;
}


