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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSpatialContext.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPSPATIALCONTEXT_H
#define FDORFPSPATIALCONTEXT_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpSpatialContext :
	public FdoDisposable
{
public:
	FdoRfpSpatialContext(void);
	virtual ~FdoRfpSpatialContext(void);
	void Dispose() { delete this; }

	FdoString* GetName() const { return m_name; }
	void SetName(FdoString* name) { m_name = name; }

	FdoString* GetDescription() const { return m_description; }
	void SetDescription(FdoString* description) { m_description = description; }

	FdoString* GetCoordinateSystem() const { return m_coordSystem; }
	void SetCoordinateSystem(FdoString* coordSystem) { m_coordSystem = coordSystem; }

	FdoString* GetCoordinateSystemWkt() const { return m_coordSystemWkt; }
	void SetCoordinateSystemWkt(FdoString* coordSystemWkt) { m_coordSystemWkt = coordSystemWkt; }

	FdoSpatialContextExtentType GetExtentType() const { return m_extentType; }
	void SetExtentType(FdoSpatialContextExtentType extentType) { m_extentType = extentType; }

	FdoByteArray* GetExtent() const { return FDO_SAFE_ADDREF(m_extent.p); }
	void SetExtent(const FdoPtr<FdoByteArray>& extent) { m_extent = extent; }

	double GetXYTolerance() const { return m_XYTolerance; }
	void SetXYTolerance(double XYTolerance) { m_XYTolerance = XYTolerance; }

	double GetZTolerance() const { return m_ZTolerance; }
	void SetZTolerance(double ZTolerance) { m_ZTolerance = ZTolerance; }

	FdoBoolean CanSetName() { return false; }

private:
	FdoStringP					m_name;
	FdoStringP					m_description;
	FdoStringP					m_coordSystem;
	FdoStringP					m_coordSystemWkt;
	FdoSpatialContextExtentType	m_extentType;
	FdoPtr<FdoByteArray>		m_extent;
	double						m_XYTolerance;
	double						m_ZTolerance;
};
typedef FdoPtr<FdoRfpSpatialContext> FdoRfpSpatialContextP;

class FdoRfpSpatialContextCollection : public FdoNamedCollection<FdoRfpSpatialContext, FdoException>
{
public:
	FdoRfpSpatialContextCollection() {}
	virtual ~FdoRfpSpatialContextCollection();
protected:
	void Dispose() { delete this; }
public:
	void ReadXml(FdoXmlReader* reader);
};
typedef FdoPtr<FdoRfpSpatialContextCollection> FdoRfpSpatialContextsP;

#endif
