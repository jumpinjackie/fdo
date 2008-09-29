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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpRasterGeoreferenceLocation.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */

#include "Overrides.h"
#include "FdoGrfpXmlGlobals.h"
#include <GdalFile/Override/FdoGrfpRasterGeoreferenceLocation.h>

FdoGrfpRasterGeoreferenceLocation::FdoGrfpRasterGeoreferenceLocation(void) : 
	m_resX(0.0), m_resY(0.0), m_insX(0.0), m_insY(0.0), m_rotX(0.0), m_rotY(0.0)
{
}

FdoGrfpRasterGeoreferenceLocation::~FdoGrfpRasterGeoreferenceLocation(void)
{
}

void FdoGrfpRasterGeoreferenceLocation::Dispose()
{
	delete this;
}

FdoGrfpRasterGeoreferenceLocation* FdoGrfpRasterGeoreferenceLocation::Create()
{
	return new FdoGrfpRasterGeoreferenceLocation();
}

FdoDouble FdoGrfpRasterGeoreferenceLocation::GetXInsertionPoint (void) const
{
	return m_insX;
}
void FdoGrfpRasterGeoreferenceLocation::SetXInsertionPoint (FdoDouble point)
{
	m_insX = point;
}

FdoDouble FdoGrfpRasterGeoreferenceLocation::GetYInsertionPoint (void) const
{
	return m_insY;
}

void FdoGrfpRasterGeoreferenceLocation::SetYInsertionPoint (FdoDouble point)
{
	m_insY = point;
}

FdoDouble FdoGrfpRasterGeoreferenceLocation::GetXResolution (void) const
{
	return m_resX;
}

void FdoGrfpRasterGeoreferenceLocation::SetXResolution (FdoDouble resolution)
{
	m_resX = resolution;
}

FdoDouble FdoGrfpRasterGeoreferenceLocation::GetYResolution (void) const
{
	return m_resY;
}

void FdoGrfpRasterGeoreferenceLocation::SetYResolution (FdoDouble resolution)
{
	m_resY = resolution;
}

FdoDouble FdoGrfpRasterGeoreferenceLocation::GetXRotation (void) const
{
	return m_rotX;
}

void FdoGrfpRasterGeoreferenceLocation::SetXRotation (FdoDouble rotation)
{
	m_rotX = rotation;
}

FdoDouble FdoGrfpRasterGeoreferenceLocation::GetYRotation (void) const
{
	return m_rotY;
}

void FdoGrfpRasterGeoreferenceLocation::SetYRotation (FdoDouble rotation)
{
	m_rotY = rotation;
}

