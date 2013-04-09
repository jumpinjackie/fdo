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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpVariant.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPVARIANT_H
#define FDORFPVARIANT_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

enum FdoRfpVariantType
{
	FRVT_UNKNOWN,
	FRVT_STRING,
	FRVT_BOOLEAN,
	FRVT_GEOMETRY
};

class FdoRfpVariant :
	public FdoDisposable
{
private:
	FdoRfpVariantType vt;
	union {
		FdoString* strVal;
		FdoBoolean boolVal;
		FdoGeometryValue* geoVal;
	};
protected:
	virtual ~FdoRfpVariant(void);
	void Dispose() { delete this; }
public:
	FdoRfpVariant(void);
	void SetString(FdoString* str) { vt = FRVT_STRING; strVal = str; }
	FdoString* GetString() { return strVal; }
	void SetBoolean(FdoBoolean bval) { vt = FRVT_BOOLEAN; boolVal = bval; }
	FdoBoolean GetBoolean() { return boolVal; }
	void SetGeometryValue(FdoGeometryValue* geometry) { vt = FRVT_GEOMETRY; geoVal = geometry; }
	FdoGeometryValue* GetGeometryValue() { return geoVal; }
	FdoRfpVariantType GetType() { return vt; }

};

typedef FdoPtr<FdoRfpVariant> FdoRfpVariantP;

class FdoRfpVariantCollection : public FdoCollection<FdoRfpVariant, FdoException>
{
protected:
	void Dispose() { delete this; }
};

typedef FdoPtr<FdoRfpVariantCollection> FdoRfpVariantsP;

#endif
