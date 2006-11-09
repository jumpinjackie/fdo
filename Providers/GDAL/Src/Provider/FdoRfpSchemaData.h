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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSchemaData.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPSCHEMADATA_H
#define FDORFPSCHEMADATA_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class FdoRfpClassDataCollection;
class FdoRfpRasterFileCollection;
class FdoGrfpPhysicalSchemaMapping;
class FdoRfpSpatialContextCollection;

class FdoRfpSchemaData :
	public FdoDisposable
{
//
// Data members
//
private:
	FdoPtr<FdoRfpClassDataCollection> m_classDatas;
	FdoFeatureSchemaP	m_featureSchema;

//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpSchemaData(void);
	virtual ~FdoRfpSchemaData(void);
public:
	static FdoRfpSchemaData* Create(FdoRfpConnection *conn, const FdoPtr<FdoFeatureSchema>& featureSchema, const FdoPtr<FdoGrfpPhysicalSchemaMapping>& schemaMapping);

//
// Exposed functions
//
public:
	FdoString* GetName() { return m_featureSchema->GetName(); }
	FdoBoolean CanSetName() { return false; }
	FdoFeatureSchemaP GetSchema() { return m_featureSchema; }
	FdoPtr<FdoRfpClassDataCollection> GetClassDatas() { return m_classDatas; }

//
// Internally used helper functions
//
private:
	// called by the static create
	void _buildUp(FdoRfpConnection *conn, const FdoPtr<FdoFeatureSchema>& featureSchema, const FdoPtr<FdoGrfpPhysicalSchemaMapping>& schemaMapping);

};

typedef FdoPtr<FdoRfpSchemaData> FdoRfpSchemaDataP;

class FdoRfpSchemaDataCollection : public FdoNamedCollection<FdoRfpSchemaData, FdoException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpSchemaDataCollection() {}
	virtual ~FdoRfpSchemaDataCollection() {}
	void Dispose() { delete this; }
public:
	static FdoRfpSchemaDataCollection* Create();
};
typedef FdoPtr<FdoRfpSchemaDataCollection> FdoRfpSchemaDatasP;

#endif
