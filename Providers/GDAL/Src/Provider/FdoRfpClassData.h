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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpClassData.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPCLASSDATA_H
#define FDORFPCLASSDATA_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpGeoRasterCollection;
class FdoGrfpClassDefinition;
class FdoGrfpRasterFeatureCollection;
class FdoRfpSpatialContext;
class FdoRfpSpatialContextCollection;

class FdoRfpClassData :
	public FdoDisposable
{
//
// Data members
//
private:
	FdoPtr<FdoRfpGeoRasterCollection> m_geoRasters;
	FdoClassDefinitionP m_classDefinition;

	// these two are used to form spatial context for this feature class
	FdoRfpRect			m_extent; // extent
	FdoStringP			m_coord;  // coordinate system name
	bool				m_bFirstRaster; // to mark whether it's the first raster contained in the feature class

	//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpClassData(void);
	virtual ~FdoRfpClassData(void);
public:
	static FdoRfpClassData* Create(const FdoPtr<FdoRfpSpatialContextCollection>& contexts, const FdoClassDefinitionP& classDefinition, const FdoPtr<FdoGrfpClassDefinition>& classMapping);

//
// Exposed functions
//
public:
	FdoString* GetName() { return m_classDefinition->GetName(); }
	FdoBoolean CanSetName() { return false; }
	FdoClassDefinitionP GetClass() { return m_classDefinition; }
	FdoPtr<FdoRfpGeoRasterCollection> GetGeoRasters() { return m_geoRasters; }

//
// Internally used helper functions
//
private:
	// called by the static create
	void _buildUp(const FdoPtr<FdoRfpSpatialContextCollection>& contexts, const FdoClassDefinitionP& classDefinition, const FdoPtr<FdoGrfpClassDefinition>& classMapping);

	// Extract all rasters from specified location
	void _buildUpGeoRastersFromLocation(FdoString* location, FdoStringCollection* coordSystems);

	// Extract all rasters from image catalogue
	void _buildUpGeoRastersFromCatalogue(const FdoPtr<FdoGrfpRasterFeatureCollection>& featureCatalogue, FdoStringCollection* coordSystems);

};

typedef FdoPtr<FdoRfpClassData> FdoRfpClassDataP;

class FdoRfpClassDataCollection : public FdoNamedCollection<FdoRfpClassData, FdoException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpClassDataCollection() {}
	virtual ~FdoRfpClassDataCollection() {}
	void Dispose() { delete this; }
public:
	static FdoRfpClassDataCollection* Create();
};

typedef FdoPtr<FdoRfpClassDataCollection> FdoRfpClassDatasP;

#endif
