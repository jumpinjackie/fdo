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
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Error.h>
#include <Sm/Ph/ClassWriter.h>
#include <Sm/Ph/DependencyWriter.h>

FdoSmLpFeatureClass::FdoSmLpFeatureClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
	FdoSmLpClassDefinition(classReader, parent),
	FdoSmLpClassBase(classReader, parent),
    mGeometryPropertyName(classReader->GetGeometryProperty())
{
}

FdoSmLpFeatureClass::FdoSmLpFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
	FdoSmLpClassDefinition(pFdoClass, bIgnoreStates, parent),
	FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
    FdoGeometricPropertyP geomProp = pFdoClass->GetGeometryProperty();
    if (geomProp != NULL)
        mGeometryPropertyName = geomProp->GetName();
}

FdoClassType FdoSmLpFeatureClass::GetClassType() const
{
    return FdoClassType_FeatureClass;
}

const FdoSmLpGeometricPropertyDefinition* FdoSmLpFeatureClass::RefGeometryProperty() const
{
	return (FdoSmLpGeometricPropertyDefinition*)((FdoSmLpFeatureClass*) this)->GetGeometryProperty();
}

FdoSmLpGeometricPropertyP FdoSmLpFeatureClass::GetGeometryProperty()
{
	Finalize();

	return mGeometryProperty;
}

void FdoSmLpFeatureClass::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
	FdoSmLpClassDefinition::Update(pFdoClass, elementState, pClassOverrides, bIgnoreStates);

    if ( (GetElementState() == FdoSchemaElementState_Added) || 
         (GetElementState() == FdoSchemaElementState_Modified) ||
         GetIsFromFdo()
    ) {
		if ( pFdoClass->GetClassType() == FdoClassType_FeatureClass ) {
			FdoFeatureClass* pFeatClass = (FdoFeatureClass*) pFdoClass;
			FdoPtr<FdoGeometricPropertyDefinition> pFdoGeomProp = pFeatClass->GetGeometryProperty();

			// Set the geometry property from the FDO class.

			mGeometryProperty = NULL;

			if ( pFdoGeomProp )
				mGeometryPropertyName = pFdoGeomProp->GetName();
            else
                mGeometryPropertyName = L"";
		}
	}
}

void FdoSmLpFeatureClass::Commit( bool fromParent )
{
	FdoSmPhMgrP                     pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhDependencyWriterP        pDepWriter = pPhysical->GetDependencyWriter();
    FdoSmPhColumnListP              classIdColNames = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
    FdoStringP                      classDefTable(pPhysical->GetDcDbObjectName("f_classdefinition"));

    classIdColNames->Add( FdoStringP(pPhysical->GetDcColumnName(L"classid")) );

    FdoSmLpClassDefinition::Commit();

	switch ( GetElementState() ) {
	case FdoSchemaElementState_Added:
        if ( GetDbObject() && IsDbObjectCreator() &&
             (wcslen(GetOwner()) == 0) 
        ) {

            /* Add a dependency between the class table and F_ClassDefinition.
               This dependency shows how to join to the class definition table
               to get class name and schema name for a feature.
            */

            pDepWriter->SetPkTableName( GetDbObjectName() );
			pDepWriter->SetPkColumnNames( classIdColNames );
            pDepWriter->SetFkTableName( classDefTable );
			pDepWriter->SetFkColumnNames( classIdColNames );
			pDepWriter->SetCardinality(	1 );

            pDepWriter->Add();

        }
		break;

	case FdoSchemaElementState_Deleted:
		if ( (wcslen(GetDbObjectName()) > 0) && GetIsDbObjectCreator() ) {
            pDepWriter->Delete( GetDbObjectName(), (FdoString*) classDefTable );
		}

		break;
    }
}

FdoSmPhClassWriterP FdoSmLpFeatureClass::GetPhysicalAddWriter()
{
    FdoSmPhClassWriterP pWriter  = FdoSmLpClassDefinition::GetPhysicalAddWriter();

    if ( IsGeomInMetaschema() )
        pWriter->SetGeometryProperty( mGeometryPropertyName );

    return( pWriter );
}

FdoSmPhClassWriterP FdoSmLpFeatureClass::GetPhysicalModifyWriter()
{
    FdoSmPhClassWriterP pWriter  = FdoSmLpClassDefinition::GetPhysicalModifyWriter();

    if ( IsGeomInMetaschema() )
        pWriter->SetGeometryProperty( mGeometryPropertyName );

    return( pWriter );
}

bool FdoSmLpFeatureClass::IsGeomInMetaschema()
{
    bool                bGeomInMetaschema   = true;
 	FdoSmPhMgrP         pPhysical           = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhDbObjectP    pClassTable;

    if (FdoSmPhOwnerP(pPhysical->GetOwner())->GetHasMetaSchema()) {
        pClassTable = pPhysical->FindDbObject(pPhysical->GetDcDbObjectName(L"f_classdefinition"));

		if ( pClassTable ) {
			FdoSmPhColumnsP columns = pClassTable->GetColumns();
            bGeomInMetaschema = (columns->RefItem(pPhysical->GetDcColumnName(L"geometryproperty")) != NULL);
		}
    }

    return( bGeomInMetaschema );
}

void FdoSmLpFeatureClass::XMLSerializeSubElements(  FILE* xmlFp, int ref ) const
{
	if ( RefGeometryProperty() ) 
		fprintf( xmlFp, "<geometryProperty>%s</geometryProperty>\n",(const char*) FdoStringP(RefGeometryProperty()->GetName()) );
}

void FdoSmLpFeatureClass::PostFinalize()
{
	int i;

    FdoSmLpGeometricPropertyP geomProp;
	bool bGeomInherited = false;
    bool bGeomInMetaschema = IsGeomInMetaschema();

	// Find the Geometry property, if any
	mGeometryProperty = NULL;

    if ( GetElementState() != FdoSchemaElementState_Deleted ) {

	    if ( bGeomInMetaschema || (GetElementState() != FdoSchemaElementState_Unchanged) )  {
		    // Set the geometry property from the FDO geometry name

		    if ( mGeometryPropertyName.GetLength() > 0 ) {
                geomProp = GetProperties()->FindItem( mGeometryPropertyName )->SmartCast<FdoSmLpGeometricPropertyDefinition>(true);
    				
			    if ( geomProp ) {
				    mGeometryProperty = geomProp;

                    if ( mGeometryProperty->GetElementState() == FdoSchemaElementState_Deleted ) {
                        // Can't set to deleted geometric property
                        AddGeomDeletedError();
                    }
                    else {
    	    		    if ( (GetElementState() != FdoSchemaElementState_Unchanged) &&
                             (mGeometryProperty->GetElementState() == FdoSchemaElementState_Unchanged) )
	    	 		    mGeometryProperty->SetElementState( FdoSchemaElementState_Modified );
                    }
			    }
			    else {
				    AddGeomPropNotFoundError( mGeometryPropertyName );
			    }
			}
            if ( GetElementState() != FdoSchemaElementState_Unchanged ) {

                // Update the is-primary (main) geometry status for all geometric properties
                // in case the main geometry for this feature class has changed.
                for ( i = 0; i < GetProperties()->GetCount(); i++ ) {
                    FdoSmLpGeometricPropertyP currGeomProp = GetProperties()->GetItem( i )->SmartCast<FdoSmLpGeometricPropertyDefinition>(true);

                    if ( currGeomProp )
                        currGeomProp->SetPrimary( wcscmp(currGeomProp->GetName(), mGeometryPropertyName) == 0 );
                }
            }
        }
        else {
            for ( i = 0; i < GetProperties()->GetCount(); i++ ) {
                FdoSmLpGeometricPropertyP currGeomProp = GetProperties()->GetItem( i )->SmartCast<FdoSmLpGeometricPropertyDefinition>(true);

                if ( currGeomProp ) {
				    bool bCurrGeomInherited = false;

    			    if ( !currGeomProp->GetIsSystem() ) {

					    bCurrGeomInherited = (currGeomProp->GetBaseProperty() != NULL);
					    if ( (!geomProp) || (bGeomInherited && !bCurrGeomInherited) ) {
    							
						    // Use this property as the geometry property if it
						    // is the first one found. Also, the above gives priority to 
						    // a non-inherited property over an inherited one, 
						    // allowing sub-classes to override the geometry 
						    // of the superclass. FDO currently doesn't allow
						    // such overriding, but we'll allow it when reading
						    // schemas and disallow it on updating schemas to 
						    // make it easier to allow overrides in the future.
    						
						    geomProp = currGeomProp;
						    bGeomInherited = bCurrGeomInherited;
					    }
					    else {
						    // 2 Geometry properties found. Use the first one but
						    // log an error. Error logging is skipped if both properties
						    // are inherited since this error would already have been
						    // logged on the superclass.
						    if ( geomProp && !bGeomInherited && ! bCurrGeomInherited )
							    AddMultiGeomPropError( geomProp, currGeomProp );
					    }
				    }
			    }
		    }

		    mGeometryProperty = geomProp;
	    }

        if ( !bGeomInMetaschema ) {
	        for ( int i = 0; i < GetProperties()->GetCount(); i++ ) {
                geomProp = GetProperties()->GetItem( i )->SmartCast<FdoSmLpGeometricPropertyDefinition>(true);

                // Do not log an error for system properties, since
	            // these are handled by the RDBMS provider. Also skip other inherited properties
		        // since these will be caught when checking the superclass.
		        if ( geomProp && 
                    (!geomProp->GetIsSystem()) && 
                    (!geomProp->GetBaseProperty()) && 
                    (wcscmp(geomProp->GetName(), mGeometryProperty->GetName()) != 0) && 
                    (geomProp->GetElementState() != FdoSchemaElementState_Deleted) 
                ) {
		            AddSecondaryGeomError( geomProp->GetName() );
			    }
		    }
        }

	    // Check if this class overrides an inherited geometry property.
	    // Schema Manager does not support overriding.

	    if ( mGeometryProperty ) {
		    FdoSmLpFeatureClassP currClass = FDO_SAFE_ADDREF(this);
		    FdoSmLpFeatureClassP baseClass;

		    // Find the lowest base class with a GeometryProperty
		    while ( currClass && currClass->GetBaseClass() ) {
			    currClass = currClass->GetBaseClass()->SmartCast<FdoSmLpFeatureClass>();

			    if ( currClass->GetGeometryProperty() )
				    baseClass = currClass;
		    }

		    if ( baseClass  ) {
			    FdoSmLpGeometricPropertyP baseGeom = baseClass->GetGeometryProperty();

			    if ( baseGeom ) {
				    if ( wcscmp( mGeometryProperty->GetName(), baseGeom->GetName() ) != 0 )
					    AddGeomOverrideError( baseClass->GetName() );
			    }
		    }
	    }

	    // A class without identity properties has no table. Since feature classes cannot 
	    // be used in object properties, a feature class without identity properties 
	    // is not very useful.
	    if ( GetIdentityProperties()->GetCount() == 0 ) 
		    AddNoIdError();
	}

}

void FdoSmLpFeatureClass::AddMultiGeomPropError( 
	const FdoSmLpGeometricPropertyDefinition* pProp1,
	const FdoSmLpGeometricPropertyDefinition* pProp2
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_161),
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpFeatureClass::AddGeomPropNotFoundError( FdoString* propName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_162),
				(FdoString*) GetQName(), 
				propName
			)
		)
	);
}

void FdoSmLpFeatureClass::AddSecondaryGeomError( FdoString* propName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_232),
				propName,
				(FdoString*) GetQName() 
			)
		)
	);
}

void FdoSmLpFeatureClass::AddGeomOverrideError( FdoString* baseClassName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_235),
				(FdoString*) GetQName(), 
				baseClassName
			)
		)
	);
}

void FdoSmLpFeatureClass::AddGeomRemoveError( FdoString* basePropName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_237),
				(FdoString*) GetQName(),
				basePropName
			)
		)
	);
}

void FdoSmLpFeatureClass::AddGeomDeletedError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_321),
				(mGeometryProperty->GetBaseProperty()) ?
                    (FdoString*) mGeometryProperty->GetBaseProperty()->GetQName() :
                    (FdoString*) mGeometryProperty->GetQName(),
                (FdoString*) GetQName()
			)
		)
	);

    if ( GetElementState() == FdoSchemaElementState_Unchanged )
        SetElementState( FdoSchemaElementState_Modified );
}

void FdoSmLpFeatureClass::AddNoIdError( )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_268),
				(FdoString*) GetQName()
			)
		)
	);
}

