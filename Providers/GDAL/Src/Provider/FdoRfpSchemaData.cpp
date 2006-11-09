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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSchemaData.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#include "FDORFP.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpClassData.h"
#include "FdoRfpSchemaData.h"
#include <GdalFile/Override/FdoGrfpPhysicalSchemaMapping.h>
#include <GdalFile/Override/FdoGrfpClassDefinition.h>

FdoRfpSchemaData::FdoRfpSchemaData(void)
{
}

FdoRfpSchemaData::~FdoRfpSchemaData(void)
{
}

void FdoRfpSchemaData::_buildUp(FdoRfpConnection *conn, const FdoPtr<FdoFeatureSchema>& featureSchema, const FdoPtr<FdoGrfpPhysicalSchemaMapping>& schemaMapping)
{
	m_classDatas = FdoRfpClassDataCollection::Create();
	m_featureSchema = featureSchema;
	FdoClassesP classes = featureSchema->GetClasses();
	FdoGrfpClassesP classesMapping;
	if (schemaMapping != NULL)
		classesMapping = schemaMapping->GetClasses();
	FdoInt32 count = classes->GetCount();
	for (FdoInt32 i = 0; i < count; i++)
	{
		FdoClassDefinitionP classDefinition = classes->GetItem(i);
		FdoGrfpClassDefinitionP classMapping;
		if (classesMapping != NULL)
			classMapping = classesMapping->GetItem(classDefinition->GetName());
		FdoRfpClassDataP classData = FdoRfpClassData::Create(conn, classDefinition, classMapping);
		m_classDatas->Add(classData);
	}

}

FdoRfpSchemaData* FdoRfpSchemaData::Create(FdoRfpConnection *conn, const FdoPtr<FdoFeatureSchema>& featureSchema, const FdoPtr<FdoGrfpPhysicalSchemaMapping>& schemaMapping)
{
	FdoRfpSchemaData* schemaData = new FdoRfpSchemaData();
	schemaData->_buildUp(conn, featureSchema, schemaMapping);
	return schemaData;
}

FdoRfpSchemaDataCollection* FdoRfpSchemaDataCollection::Create()
{
	return new FdoRfpSchemaDataCollection();
}
