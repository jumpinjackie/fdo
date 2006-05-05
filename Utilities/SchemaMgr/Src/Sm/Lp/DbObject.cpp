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
 */


#include "stdafx.h"
#include <Sm/Lp/DbObject.h>
//#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ClassDefinition.h>

FdoSmLpDbObject::FdoSmLpDbObject(
	FdoString* objectName, 
	FdoSmPhDbObjectP physicalObject, 
	FdoSmLpPropertiesP properties, 
	bool bClassTable, 
	FdoSmLpSchemaElement* parent
) : 
	FdoSmLpSchemaElement(objectName, L"" ),
	mPhDbObject(physicalObject),
	miPathDist(-1),
	mbClassTable(bClassTable)
{
    mProperties     = new FdoSmLpPropertyDefinitionCollection();
	mSourceColumns  = new FdoSmPhColumnCollection();
	mTargetColumns  = new FdoSmPhColumnCollection();
	mEmptyPkey      = new FdoSmPhColumnCollection();

	if ( mPhDbObject ) {
		// This table exist, so build the list of properties
		// stored in this table.
		for ( int i = 0; i < properties->GetCount(); i++ ) {
			FdoSmLpPropertyP pProp = properties->GetItem(i);

			switch ( pProp->GetPropertyType() ) {
			case FdoPropertyType_DataProperty:
			case FdoPropertyType_GeometricProperty:

                if ( (pProp->RefContainingDbObject()) && (pProp->RefContainingDbObject()->GetQName() == mPhDbObject->GetQName()) ) {
                    const FdoSmLpSimplePropertyDefinition* pSimpleProp = 
                        FdoSmLpSimplePropertyDefinition::Cast( pProp );

                    if ( pSimpleProp && pSimpleProp->RefColumn() )
    					mProperties->Add( pProp );
                }

				break;

			case FdoPropertyType_ObjectProperty:
				if ( FdoStringP(pProp->RefParentClass()->GetDbObjectName()).ICompare(mPhDbObject->GetName()) == 0 ) 
                    mProperties->Add( pProp );

				break;
			}
		}
	}
}

FdoSmLpDbObject::~FdoSmLpDbObject(void)
{
}

FdoSmPhColumnsP FdoSmLpDbObject::GetPkeyColumns()
{
    FdoSmPhTableP table = mPhDbObject.p->SmartCast<FdoSmPhTable>();

	return table ? table->GetPkeyColumns() : mEmptyPkey;
}

const FdoSmLpPropertyDefinitionCollection* FdoSmLpDbObject::RefProperties() const
{
	return (FdoSmLpPropertyDefinitionCollection*) mProperties;
}

const FdoSmPhDbObject* FdoSmLpDbObject::RefDbObject() const
{
	return (FdoSmPhDbObject*)((FdoSmLpDbObject*)this)->GetDbObject();
}

FdoSmPhDbObjectP FdoSmLpDbObject::GetDbObject()
{
	return mPhDbObject;
}

FdoString* FdoSmLpDbObject::GetPkeyName() const
{
    FdoSmPhTableP table = mPhDbObject.p->SmartCast<FdoSmPhTable>();

	if ( table )
		return table->GetPkeyName();
	else
		return( L"" );
}

const FdoSmPhColumnCollection* FdoSmLpDbObject::RefSourceColumns() const
{
	return (FdoSmPhColumnCollection*) mSourceColumns;
}

const FdoSmPhColumnCollection* FdoSmLpDbObject::RefTargetColumns() const
{
	return (FdoSmPhColumnCollection*) mTargetColumns;
}

const FdoSmLpDbObject* FdoSmLpDbObject::RefTargetDbObject() const
{
	return (FdoSmLpDbObject*) mTargetDbObject;
}

int FdoSmLpDbObject::GetPathDist() const
{
	return miPathDist;
}

bool FdoSmLpDbObject::GetIsClasstable() const
{
	return( mbClassTable );
}

void FdoSmLpDbObject::AddSourceColumn( FdoSmPhColumnP pColumn )
{
	mSourceColumns->Add( pColumn );
}

void FdoSmLpDbObject::AddTargetColumn( FdoSmPhColumnP pColumn )
{
	mTargetColumns->Add( pColumn );
}

void FdoSmLpDbObject::SetTargetDbObject( FdoSmLpDbObjectP dbObject )
{
	mTargetDbObject = dbObject;
}

void FdoSmLpDbObject::SetPathDist( int iDist )
{
	miPathDist = iDist;
}

void FdoSmLpDbObject::SetIsClassTable( bool bClassTable )
{
	mbClassTable = bClassTable;
}

FdoSchemaExceptionP FdoSmLpDbObject::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	if ( mPhDbObject ) 
		pException = mPhDbObject->Errors2Exception(pException);

	return pException;
}

void FdoSmLpDbObject::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<table name=\"%ls\" description=\"%ls\" pkeyName=\"%ls\" %ls>\n",
			RefDbObject()->GetName(), RefDbObject()->GetDescription(), GetPkeyName(),
            (FdoString*) RefDbObject()->XMLSerializeProviderAtts()
	);

	if ( RefTargetDbObject() ) {
		fprintf( xmlFp, "<target>\n" );
		RefTargetDbObject()->XMLSerialize( xmlFp, 1 );

		fprintf( xmlFp, "<sourceColumns>\n" );
		for ( int i = 0; i < RefSourceColumns()->GetCount(); i++ ) 
			RefSourceColumns()->RefItem(i)->XMLSerialize(xmlFp, 1);
		fprintf( xmlFp, "</sourceColumns>\n" );

		fprintf( xmlFp, "<targetColumns>\n" );
		for ( int i = 0; i < RefTargetColumns()->GetCount(); i++ ) 
			RefTargetColumns()->RefItem(i)->XMLSerialize(xmlFp, 1);
		fprintf( xmlFp, "</targetColumns>\n" );

		fprintf( xmlFp, "</target>\n" );
	}

	if ( ref == 0 ) {
		for ( int i = 0; i < RefProperties()->GetCount(); i++ ) 
			RefProperties()->RefItem(i)->XMLSerialize(xmlFp, 1);

		for ( int i = 0; i < RefDbObject()->RefColumns()->GetCount(); i++ ) 
			RefDbObject()->RefColumns()->RefItem(i)->XMLSerialize(xmlFp, ref);

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );
	}

	fprintf( xmlFp, "</table>\n" );

}
