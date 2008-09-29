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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoreference.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPGEOREFERENCE_H
#define FDORFPGEOREFERENCE_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpGeoreference : public FdoIDisposable
{
public:
	FdoRfpGeoreference(void);

public:
	bool HasCoordSystem(void) { return m_hasCoord; } 
	FdoString* GetCoorSystem(void) { return m_coordSystem; } 
	void SetCoordSystem(FdoStringP coordSystem) { m_coordSystem = coordSystem; m_hasCoord = true; }

	FdoDouble GetXInsertion(void) { return m_insX; }
	void SetXInsertion(FdoDouble value) { m_insX = value; }

	FdoDouble GetYInsertion(void) { return m_insY; }
	void SetYInsertion(FdoDouble value) { m_insY = value; }

	FdoDouble GetXRotation(void) { return m_rotX; }
	void SetXRotation(FdoDouble value) { m_rotX = value; } 

	FdoDouble GetYRotation(void) { return m_rotY; } 
	void SetYRotation(FdoDouble value) { m_rotY = value; } 

	FdoDouble GetXResolution(void) { return m_resX; }
	void SetXResolution(FdoDouble value) { m_resX = value; }

	FdoDouble GetYResolution(void) { return m_resY; }
	void SetYResolution(FdoDouble value) { m_resY = value; }

	FdoDouble GetXYTolerance(void) { return m_toleranceXY; }
	void SetXYTolerance(FdoDouble value) { m_toleranceXY = value; }

	FdoDouble GetZTolerance(void) { return m_toleranceZ; }
	void SetZTolerance(FdoDouble value) { m_toleranceZ = value; }

	//FdoInt32 GetImageWidth(void) { return m_imageWidth; }
	//void SetImageWidth(FdoInt32 value) { m_imageWidth = value; }

	//FdoInt32 GetImageHeight(void) { return m_imageHeight; }
	//void SetImageHeight(FdoInt32 value) { m_imageHeight = value; }

protected:
	virtual ~FdoRfpGeoreference(void);
	void Dispose() { delete this; }

private:
	FdoDouble m_insX; // inserting point
	FdoDouble m_insY; //
	FdoDouble m_resX; // resolution
	FdoDouble m_resY;
	FdoDouble m_rotX; // rotation
	FdoDouble m_rotY;
	FdoDouble m_toleranceXY; // tolerance
	FdoDouble m_toleranceZ;

	bool m_hasCoord;
	FdoStringP m_coordSystem;

	//FdoInt32 m_imageWidth;
	//FdoInt32 m_imageHeight;
};

typedef FdoPtr<FdoRfpGeoreference> FdoRfpGeoreferenceP;

#endif
