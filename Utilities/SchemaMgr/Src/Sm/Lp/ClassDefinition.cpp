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
#include <math.h>
#include <Sm/Ph/DependencyCollection.h>
#include <Sm/Lp/ClassDefinition.h>
#include <Sm/Lp/DataPropertyDefinition.h>
#include <Sm/Lp/GeometricPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/ClassTypeMapper.h>
#include <Sm/Lp/DataTypeMapper.h>
#include <Sm/Lp/UniqueConstraint.h>

#include <Sm/Error.h>

const FdoStringP FdoSmLpClassDefinition::ClassDefinitionName(L"ClassDefinition");
const FdoStringP FdoSmLpClassDefinition::ClassClassName(L"Class");
const FdoStringP FdoSmLpClassDefinition::FeatureClassName(L"Feature");
const FdoStringP FdoSmLpClassDefinition::DefaultGeometricPropertyName(L"Geometry");

FdoSmLpClassBase::FdoSmLpClassBase(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
	FdoSmLpSchemaElement(classReader->GetName(), classReader->GetDescription(), parent),
	mBaseClassName(classReader->GetBaseName()),
	mIsAbstract(classReader->GetIsAbstract()),
	mDbObjectName(classReader->GetTableName()),
	mRootDbObjectName(classReader->GetRootTableName()),
	mId(classReader->GetId()),
    mDatabase(classReader->GetDatabase()),
    mOwner(classReader->GetOwner()),
	mbCheckIds(false),
    mbHasFdoIdentity(false),
    mbIsFixedDbObject(classReader->GetIsFixedTable()),
    mTableMapping(FdoSmOvTableMappingType_Default),
    mbIsDbObjectCreator(classReader->GetIsTableCreator()),
    mCapabilities(NULL)
{
    mFdoIds = FdoStringCollection::Create(); 
}

FdoSmLpClassBase::FdoSmLpClassBase(
    FdoClassDefinition* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
	FdoSmLpSchemaElement(pFdoClass->GetName(), pFdoClass->GetDescription(), parent, true),
	mTableMapping(FdoSmOvTableMappingType_Default),
	mIsAbstract(pFdoClass->GetIsAbstract()),
    mbCheckIds(false),
    mbHasFdoIdentity(false),
    mbIsFixedDbObject(false),
    mbIsDbObjectCreator(false),
    mCapabilities(NULL)
{
    mFdoIds = FdoStringCollection::Create();

    FdoClassDefinitionP pBaseClass = pFdoClass->GetBaseClass();

	if ( (FdoClassDefinition*) pBaseClass ) {
		FdoSchemaElementP pBaseParent = pBaseClass->GetParent();
		if ( pBaseParent ) {
			mBaseClassName =  FdoStringP( pBaseParent->GetName()) +
							L":" + pBaseClass->GetName();
		}
		else {
			AddBaseNoSchemaError( pBaseClass->GetName() );
		}
	}
}

FdoSmLpClassBase::FdoSmLpClassBase(
    FdoString* name, 
    FdoString* description,
    FdoSmLpSchemaElement* parent,
    FdoSmLpClassDefinitionP pSrcClass,
    FdoSmOvTableMappingType tableMapping,
    FdoString* tableName,
    FdoSmPhDbObjectP table,
    FdoSchemaElementState elementState,
    bool fromFdo
) : 
	FdoSmLpSchemaElement(name, description, parent, fromFdo),
	mSrcClass(pSrcClass),
	mTableMapping(tableMapping),
	mIsAbstract(false),
	mDbObjectName(tableName),
	mPhDbObject(table),
	mId(0),
	mbCheckIds(false),
    mbHasFdoIdentity(false),
    mbIsFixedDbObject(false),
    mbIsDbObjectCreator(false),
    mCapabilities(NULL)
{
    mFdoIds = FdoStringCollection::Create();
}


FdoSmLpClassBase::~FdoSmLpClassBase(void)
{
    // Break circular references between Object Properties and this class's table.
    // The class table owns the object property but the Object Property Class table
    // references the class table as its target
    if ( mProperties )
        BreakObjPropTargets( mProperties );

    // Circular references for nested properties must also be broken.
    if ( mNestedProperties )
        BreakObjPropTargets( mNestedProperties );

	if( mCapabilities )
		delete mCapabilities;
}

const FdoSmLpClassDefinition* FdoSmLpClassBase::RefBaseClass() const
{
	if ( !mBaseClass ) 
		((FdoSmLpClassBase*) this)->Finalize();

    return (FdoSmLpClassDefinition*) mBaseClass;
}

FdoSmLpClassDefinitionP FdoSmLpClassBase::GetBaseClass()
{
    return mBaseClass;
}

const FdoSmLpClassDefinition* FdoSmLpClassBase::RefMetaClass() const
{
	// MetaClasses currently do not have a MetaClass 
	if ( wcscmp( RefLogicalPhysicalSchema()->GetName(), FdoSmLpSchema::mMetaClassSchemaName ) == 0 )
		return(NULL);

	// The MetaClass is the class in the MetaClass schema with the same name as the 
	// class type.
	FdoStringP metaClassName = FdoStringP(FdoSmLpSchema::mMetaClassSchemaName) + 
        L":" + FdoSmLpClassTypeMapper::Type2String( GetClassType() );

    return RefLogicalPhysicalSchema()->FindClass( metaClassName );
}

FdoSmLpClassDefinitionP FdoSmLpClassBase::GetMetaClass()
{
    return FDO_SAFE_ADDREF((FdoSmLpClassDefinition*) RefMetaClass());
}

bool FdoSmLpClassBase::GetIsAbstract() const
{
   return mIsAbstract;
}

const FdoSmLpDataPropertyDefinitionCollection* FdoSmLpClassBase::RefIdentityProperties() const
{
	((FdoSmLpClassBase*) this)->Finalize();

    return (FdoSmLpDataPropertyDefinitionCollection*) ((FdoSmLpClassBase*) this)->GetIdentityProperties();
}

const FdoSmLpPropertyDefinitionCollection* FdoSmLpClassBase::RefProperties() const
{
	((FdoSmLpClassBase*) this)->Finalize();

    return (FdoSmLpPropertyDefinitionCollection*) ((FdoSmLpClassBase*) this)->GetProperties();
}

const FdoSmLpPropertyDefinitionCollection* FdoSmLpClassBase::RefNestedProperties() const
{
	((FdoSmLpClassBase*) this)->Finalize();

    return (FdoSmLpPropertyDefinitionCollection*) ((FdoSmLpClassBase*) this)->GetNestedProperties();
}

const FdoSmLpUniqueConstraintCollection* FdoSmLpClassBase::RefUniqueConstraints() const
{
	((FdoSmLpClassBase*) this)->Finalize();
	
	return (FdoSmLpUniqueConstraintCollection*) ((FdoSmLpClassBase*) this)->GetUniqueConstraints();
}

const FdoSmLpCheckConstraintCollection*  FdoSmLpClassBase::RefCheckConstraints() const
{
	((FdoSmLpClassBase*) this)->Finalize();

	return (FdoSmLpCheckConstraintCollection*) ((FdoSmLpClassBase*) this)->GetCheckConstraints();
}

/*
const FdoSmLpIndexCollection* FdoSmLpClassBase::GetIndexes() const
{
	((FdoSmLpClassBase*) this)->Finalize();

    return &mIndexes;
}
*/
const FdoSmLpDbObject* FdoSmLpClassBase::RefDbObject() const
{
	((FdoSmLpClassBase*) this)->Finalize();

	return mDbObject;
}

FdoSmPhDbObjectP FdoSmLpClassBase::FindPhDbObject()
{
    FdoSmPhMgrP         pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhDbObjectP    dbObject;

    FdoStringP ownerName;

    if (!pPhysical->GetOwner()->GetHasMetaSchema())
        ownerName = mOwner;

    if ( mDbObjectName.GetLength() > 0 ) {
        dbObject = pPhysical->FindDbObject( mDbObjectName, ownerName, L"", false );
/* TODO
        if ( dbObject && 
            (dbObject->GetElementState() != FdoSchemaElementState_Added) &&
            (dbObject->GetColumns()->GetCount() == 0) 
        ) {
            if ( GetElementState() == FdoSchemaElementState_Unchanged )
                dbObject = NULL;
            else
                log error, can't update class if its view is invalid
        }
*/
    }            
/* TODO     
    if ( (!dbObject) && (mRootDbObjectName.GetLength() > 0 ) ) {
        dbObject = pPhysical->FindDbObject( mDbObjectName, mOwner, mDatabase, false );
        if ( dbObject && 
            (dbObject->GetElementState() != FdoSchemaElementState_Added) &&
            (dbObject->GetColumns()->GetCount() == 0) 
        )
            dbObject = NULL;
    }
*/
    return dbObject;
}

bool FdoSmLpClassBase::CanInherit( const FdoSmLpPropertyDefinition* pProp )
{
    return true;
}

FdoString* FdoSmLpClassBase::GetDbObjectName() const
{
	return mDbObjectName;
}

FdoStringP FdoSmLpClassBase::GetSubstDbObjectName() const
{
	return GetDbObjectName();
}

FdoString* FdoSmLpClassBase::GetRootDbObjectName() const
{
	return mRootDbObjectName;
}

FdoStringP FdoSmLpClassBase::GetSubstRootDbObjectName() const
{
	return GetRootDbObjectName();
}

FdoStringP FdoSmLpClassBase::GetDbObjectQName( bool includeDefaultOwner ) const
{
    FdoStringP ownerName = mOwner;
    if ( includeDefaultOwner && (ownerName == L"") )
        ownerName = ((FdoSmLpClassBase*)this)->GetLogicalPhysicalSchema()->GetPhysicalSchema()->GetOwner()->GetName();

    return FdoStringP::Format( 
        L"%ls%ls%ls%ls%ls",
        (FdoString*) mDatabase,
        mDatabase.GetLength() > 0 ? L"." : L"",
        (FdoString*) ownerName,
        ownerName.GetLength() > 0 ? L"." : L"",
        (FdoString*) mDbObjectName
    );
}


const FdoSmLpDbObjectCollection* FdoSmLpClassBase::RefDbObjects() const
{
	((FdoSmLpClassBase*) this)->Finalize();

	return (FdoSmLpDbObjectCollection*) mDbObjects;
}

FdoSmOvTableMappingType FdoSmLpClassBase::GetTableMapping() const
{
    if ( mTableMapping == FdoSmOvTableMappingType_Default ) 
    	((FdoSmLpClassBase*) this)->Finalize();

    FdoSmOvTableMappingType tableMapping = mTableMapping;

    if ( mBaseClassName == L"" ) {
        // class with no base class always has concrete table mapping. 
        tableMapping = FdoSmOvTableMappingType_ConcreteTable;
   }
    else {
        if ( tableMapping == FdoSmOvTableMappingType_Default ) {
            // Default the table mapping type to that of the feature schema
            tableMapping = RefLogicalPhysicalSchema()->GetTableMapping();

            if ( tableMapping == FdoSmOvTableMappingType_Default ) 
                // feature schema mapping defaults to concrete. 
                tableMapping = FdoSmOvTableMappingType_ConcreteTable;
        }
    }

	return tableMapping;
}

FdoStringP FdoSmLpClassBase::GetQName() const
{
	return( GetParent()->GetQName() + L":" + GetName() );
}
/*
FdoSmLpJoinTreeNode* FdoSmLpClassBase::GetJoinTree( const FdoStringsP& properties ) const
{
	if ( !mDbObject ) 
		return(NULL);

	FdoSmLpJoinTreeNodeCollection joinNodes;

	// The Class table is always at the root of the join tree since a select on 
	// this class will always include the Class table.
	FdoSmLpJoinTreeNode* pRootNode = new FdoSmLpJoinTreeNode( mDbObject );
	joinNodes.Add(pRootNode);

	// For each given property, find the tables that need to be included to 
	// select the property.
	for ( int i = 0; i < properties->GetCount(); i++ ) {

		// Join tree currently only includes tables for Data properties.
		// Todo: handle geometric properties not in F_Geometry_<n>.
		const FdoSmLpSimplePropertyDefinition* pProp = 
			FdoSmLpSimplePropertyDefinition::Cast( GetProperties()->RefItem( properties->GetString(i) ) );

		bool bFound = false;

		// Make sure property has a column.
		if ( pProp && pProp->GetColumn() ) {
		
			// Find the property's table.

			const FdoSmPhDbObject* pPhDbObject = pProp->GetContainingTable();
            FdoSmUtil  util;

            // Don't add the geometry table to the join tree. 
            if ( pPhDbObject && (strncmp(util.UnicodeToUtf8(pPhDbObject->GetName()), ADB_GEOMETRY_0_TABLE, strlen(ADB_GEOMETRY_0_TABLE)-1) != 0 ) ) {

				// Find the property table in the tables list.

				const FdoSmLpDbObject* pLpDbObject = GetTables()->RefItem( pPhDbObject->GetName() );

				if ( pLpDbObject ) {
					if ( pLpDbObject == GetTable() ) {
						// This is the class table, property has been found.
						// Nothing else to do since class table already in join tree.
						bFound = true;
					}
					else {

						// Not the class table, add this table to the join tree.
						// However, skip it if it has no target table. The target
						// table provides the join path back to the class table.

						if ( pLpDbObject->GetTargetTable() ) {
							bFound = true;

							// Add the table to the join tree.
							FdoSmLpJoinTreeNode* pNode = AddJoinTreeNode( joinNodes, pLpDbObject );
							pNode->Release();
						}
					}				
				}
			}
		}

		if ( !bFound )
			// This can happen when:
			//	- this is not a property of this class
			//	- the property has no column in its table.
			//  - the property's table cannot be joined back to the class table.
			// If so, report as a property that can't be selected.
			pRootNode->GetMissingProperties()->Add( properties->GetString(i) );
	}

	return( pRootNode );
}
*/

FdoInt64 FdoSmLpClassBase::GetId() const
{
	return mId;
}

const FdoSmLpDataPropertyDefinition* FdoSmLpClassBase::RefFeatIdProperty() const
{
    return (FdoSmLpDataPropertyDefinition*)((FdoSmLpClassBase*) this)->GetFeatIdProperty();
}

FdoSmLpDataPropertyP FdoSmLpClassBase::GetFeatIdProperty()
{
	FdoSmLpDataPropertyP pProp;

	try {
		pProp = FdoSmLpDataPropertyDefinitionCollection::FindFeatIDProperty( mProperties );
	}
	catch ( FdoSchemaException* se ) {
		throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_186),
				(FdoString*)(GetQName()),
				se->GetExceptionMessage()
			)
		);
	}

	return( pProp );
}

FdoPhysicalClassMappingP FdoSmLpClassBase::GetSchemaMappings( bool bIncludeDefaults ) const
{
    return (FdoPhysicalClassMapping*) NULL;
}

const FdoSmLpClassBase::Capabilities* FdoSmLpClassBase::GetCapabilities() const
{
    FdoSmLpClassBase* pClassDef = (FdoSmLpClassBase*) this;

    pClassDef->Finalize();

    if ( !pClassDef->mCapabilities ) 
        pClassDef->mCapabilities = new FdoSmLpClassBase::Capabilities( 
            this
        );

    return mCapabilities;
}

FdoStringP FdoSmLpClassBase::UniqueColumnName( 
    FdoSmPhDbObjectP dbObject, 
    const FdoSmLpPropertyDefinition* pProp, 
    FdoString* columnName, 
    bool bFixed
)
{
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSize colNameMaxLen = pPhysical->ColNameMaxLen();

	// Censor out characters not acceptable to RDBMS
	FdoStringP workName = pPhysical->CensorDbObjectName(columnName);
	FdoStringP outName;

    if ( bFixed ) {
        bool bDerived = pProp ? ( pProp->RefPrevProperty() != NULL ) : false;

        // skip the following tests for derived properties since these error have already 
        // been trapped when the base property was applied.
        if ( !bDerived ) {
            // When workName is different, column name has an invalid character.
            if ( workName.ICompare(columnName) != 0 ) 
                ((FdoSmLpClassBase*) this)->AddColCharError(pProp, columnName);

            if ( workName.GetLength() > colNameMaxLen ) 
                ((FdoSmLpClassBase*) this)->AddColLengthError(pProp, columnName);
            else if ( pPhysical->IsDbObjectNameReserved(workName) )
                ((FdoSmLpClassBase*) this)->AddColReservedError(pProp, workName);
        }
        
        if ( ColumnNameUsed((FdoSmPhDbObject*) NULL, pProp, workName) )
            ((FdoSmLpClassBase*) this)->AddColUsedError(pProp, workName);

        // Set column name to the adjusted valid name, to prevent duplicate error
        // messages for inherited classes.
        outName = (FdoString*) workName.Mid( 0, colNameMaxLen, true );
    }
    else {
        outName = pPhysical->GetDcColumnName(workName);
	    // Name was not fixed by schema override, so adjust the name if it is too long, 
        // is a reserved name, or already exists.
	    // Note that length of char* version is checked since this is what goes into the database.
	    if ( (strlen(outName) >  colNameMaxLen) ||
		    pPhysical->IsDbObjectNameReserved(outName) ||
            ColumnNameUsed(dbObject, pProp, outName)
	    ) {

		    for ( int i = 1; ; i++ ) {
			    // Truncate the name and tack on a unique number.
			    outName = FdoStringP::Format( L"%ls%d",
							    (FdoString*) pPhysical->GetDcColumnName( workName.Mid(0, colNameMaxLen - ((int) log10((double)i)) - 1, true)),
							    i
					    );

  			    // If name not in use then we're done. Otherwise bump up the unique number
  			    // and try again.
			    if ( (!ColumnNameUsed(dbObject, pProp, outName)) && 
                     (!pPhysical->IsDbObjectNameReserved( outName)) )
				    break;
		    }
	    }
    }

	return(outName);
}

void FdoSmLpClassBase::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoStringP	fdoBaseClassName;
	FdoSmLpPropertyP pLpProp;
    FdoInt32 idx;

	FdoSmLpSchemaElement::Update(pFdoClass, elementState, bIgnoreStates );

	int i;

	// Find the lowest base class. The lowest base class defines the identity 
	// properties, if any.

	mFdoClass = FDO_SAFE_ADDREF(pFdoClass);

	FdoClassDefinitionP pBaseClass = pFdoClass->GetBaseClass();
	FdoClassDefinitionP pRootClass = FDO_SAFE_ADDREF(pFdoClass);

    // Check for class conflict type conflict. Not safe to apply class changes if
	// FDO and datastore versions have different class type.
	if ( GetClassType() != pFdoClass->GetClassType() ) {
		AddClassTypeChangeError( pFdoClass->GetClassType() );
		return;
	}

    while ( FdoClassDefinitionP(pRootClass->GetBaseClass()) ) {
		pRootClass = pRootClass->GetBaseClass();

		if ( ((FdoClassDefinition*) pRootClass) == ((FdoClassDefinition*) pFdoClass) )
			//This is an error since a class can't be its own ancestor.
			//This error is trapped in the Finalize() function so just keep on
			//going for now.
			break;
	}

	switch ( GetElementState() ) {
	case FdoSchemaElementState_Modified:
		
		if ( pBaseClass )
			fdoBaseClassName = FdoStringP( FdoSchemaElementP(pBaseClass->GetParent())->GetName()) +
							L":" + pBaseClass->GetName();

		// Adding, removing or changing base class not yet supported.
		// Todo: look into supporting this.
		if ( wcscmp ( mBaseClassName, fdoBaseClassName ) != 0 )
			AddBaseClassChangeError( fdoBaseClassName );

		// Abstract status change not yet supported.
		// Todo: look into supporting this. We'd have to disallow changing
		// a class to abstract when it has objects ( its table has rows ).
		if ( pFdoClass->GetIsAbstract() != GetIsAbstract() ) {
			AddAbstractChangeError();
		}

        break;
	}

	// Handle property updates. This step is skipped when deleting the class since
	// properties are already marked for delete at this point.
	if ( GetElementState() != FdoSchemaElementState_Deleted ) {

		// Handle the properties themselves.
		FdoPropertiesP pFdoProps = pFdoClass->GetProperties();

		for ( i = 0; i < pFdoProps->GetCount(); i++ ) {
			FdoPropertyP pFdoProp = pFdoProps->GetItem(i);

            FdoSchemaElementState fdoPropState = FdoSchemaElementState_Unchanged;

            if ( bIgnoreStates ) {
                // When ignoring element states, operation depends on property's existence.
                idx = GetProperties()->IndexOf(pFdoProp->GetName());
                if ( idx >= 0 ) {
                    // Property already exists
                    if ( elementState == FdoSchemaElementState_Unchanged ) 
                        // FDO Property came from a config document, it takes precedence
                        // over property read from datastore.
                        GetProperties()->RemoveAt( idx );
                    else
                        // Doing an ApplySchema so must be modifying the property
                        fdoPropState = FdoSchemaElementState_Modified;
                }
                else {
                    if ( elementState != FdoSchemaElementState_Unchanged )
                        // Property not in datastore and not coming from config doc,
                        // Therefore, it needs to be added.
                        fdoPropState = FdoSchemaElementState_Added;
                }
            }
            else {
                fdoPropState = pFdoProp->GetElementState();
            }

            if ( (fdoPropState == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
                // Property being added by ApplySchema or coming from config doc.
                // In either case, add it to LogicalPhysical schema.
				if ( (pFdoProp->GetPropertyType() == FdoPropertyType_GeometricProperty) &&
					 !SupportsGeometry() )		
					AddGeomPropError( pFdoProp->GetName() );

				// Can't add property if it already exists
				if ( GetProperties()->RefItem(pFdoProp->GetName()) ) {
					AddPropExistsError( pFdoProp->GetName() );
				}
				else {
					// Create the new LogicalPhysical property from the FDO property.
					pLpProp = CreateProperty( 
                        pFdoProp, 
                        pClassOverrides, 
                        bIgnoreStates,
                        fdoPropState
                    );
					GetProperties()->Add( pLpProp );
				}
            }
            else {
				pLpProp = GetProperties()->FindItem( pFdoProp->GetName() );

				if ( pLpProp ) {
					pLpProp->Update( pFdoProp, fdoPropState, pClassOverrides, bIgnoreStates  );
				}
				else {
					// Error if modifying a non-existing property.
					if ( pFdoProp->GetElementState() == FdoSchemaElementState_Modified ) 
						AddPropNotExistsError( pFdoProp->GetName() );
				}
			}
		}
		
		// Handle the identity properties. Currently, only id properties on new classes
		// can be set.
		// Todo: look at supporting updating id properties

		FdoDataPropertiesP pFdoIdProps = pRootClass->GetIdentityProperties();
        mbHasFdoIdentity = (pFdoIdProps->GetCount() > 0);

        if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
			if ( pRootClass == pFdoClass ) {
				// This class has no base class so load the identity properties 
				// from FDO. For other classes, id properties are inherited in the 
				// FinalizeIdProps() function.
				for ( int i = 0; i < pFdoIdProps->GetCount(); i++ ) {
					FdoDataPropertyP pFdoIdProp = pFdoIdProps->GetItem(i);

					FdoSmLpDataPropertyP pLpIdProp = mProperties->FindItem( pFdoIdProp->GetName() )->SmartCast<FdoSmLpDataPropertyDefinition>(true);

                    if ( pLpIdProp ) {
						GetIdentityProperties()->Add( pLpIdProp );
                    }
				}
			}
			else {
				// This class has a base class. There is a good chance that we're not updating
				// the root base class. If the root base class has different id properties in 
				// FDO and the datastore then this class won't get the expected id properties.
				//
				// Remember the FDO properties and verify that they match the datastore properties.
				// The verification is done in FinalizeIdProps() after the inherited id properties
				// are loaded.
				for ( i = 0; i < pFdoIdProps->GetCount(); i++ ) {
					mFdoIds->Add( FdoDataPropertyP(pFdoIdProps->GetItem(i))->GetName() );
				}
				mbCheckIds = true;
			}
        }
        else if ( GetElementState() == FdoSchemaElementState_Modified ) {
			if ( ((FdoClassDefinition*) pRootClass) == ((FdoClassDefinition*) pFdoClass) ) {
				// Modifying a class with no base class. Make sure the id property list
				// has not been changed. Remember the list for verification in FinalizeIdProps().
				for ( i = 0; i < pFdoIdProps->GetCount(); i++ ) {
					mFdoIds->Add( FdoDataPropertyP(pFdoIdProps->GetItem(i))->GetName() );
				}
				mbCheckIds = true;
			}
		}
    }
}

void FdoSmLpClassBase::SynchPhysical(bool bRollbackOnly)
{	
	int i;
	bool bColRollbackOnly				= bRollbackOnly; 
	FdoSmPhMgrP pPhysical	            = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	bool bStopError						= false;
	bool bTableCreated					= false;
    bool bSynchTable                    = (!bRollbackOnly || pPhysical->RefRollbackTable(GetDbObjectQName(true)));

    Finalize();

    // Check if there are errors
    for ( i = 0; i < GetErrors()->GetCount(); i++ ) {
        FdoSmErrorType errType = GetErrors()->RefItem(i)->GetType();
        // Ignore missing columns since that is what this function will fix.
        // Also ignore join path to metaclasses since these currently always 
        // happen for non-feature classes.

		if ( (errType != FdoSmErrorType_ColumnMissing) && (errType != FdoSmErrorType_NoJoinPath) ) {
	    	bStopError = true;
			break;
		}
	}

    // Do not proceed if there are errors.
	if ( !bStopError ) {

        // Synchronize table only if synchronizing everything or something is in the rollback cache.
	    if ( bSynchTable ) {
			if ( RefIdentityProperties()->GetCount() == 0 ) {

/* For future. Need to think about whether auto-deleting tables is too risky.
				if ( mpPhDbObject ) 
                    // Get rid of table for id-less class.
					mpPhDbObject->SetElementState(FdoSchemaElementState_Deleted);
*/
			}
			else {
				if ( !mPhDbObject ) {

                    // Class has id but no table. If table exists, attach to it.
                    if (pPhysical->GetOwner()->GetHasMetaSchema())
    					mPhDbObject = pPhysical->FindDbObject(mDbObjectName);
                    else
    					mPhDbObject = pPhysical->FindDbObject(mDbObjectName, mOwner);

                    if ( !mPhDbObject ) {
                        // This class owns the Table but it doesn't exist, create it.
                        if ( mRootDbObjectName.GetLength() > 0 ) 
                            // Foreign table so create view around it
                            mPhDbObject = NewView( mDbObjectName, mDatabase, mOwner, mRootDbObjectName );
                        else
                            // Non-foreign so just create the table.
    						mPhDbObject = NewTable( mDbObjectName, mNewPkeyName );
                        mbIsDbObjectCreator = true;
                    }

                    // When table added to class, synchronize all properties, not 
                    // just the ones rolled back.
					bColRollbackOnly = false;
					bTableCreated = true;
				}
            }
        }

		if ( mPhDbObject && (GetIdentityProperties()->GetCount() != 0) ) {
            // Synchronize the class properties.
            // Some object property tables might be in rollback cache even if 
            // this class table isn't.
		    for ( i = 0; i < GetProperties()->GetCount(); i++ ) {
			    FdoSmLpPropertyP pProp = GetProperties()->GetItem(i);

			    pProp->SynchPhysical(
				    pProp->GetPropertyType() == FdoPropertyType_ObjectProperty ?
					    bRollbackOnly : bColRollbackOnly
			    );
		    }

            // Now that properties have been synchronized, we can synchronize primary
            // key and unused columns.
	        if ( bSynchTable ) {
				if ( bTableCreated )
                   // If new table, add primary key to it.
					CreatePkey();
					
					// Add contraints
					CreateCkeys();

					// Add unique keys
					CreateUkeys();
			}

            // Todo: change notnull columns that don't match properties to nullable.
		}
	}
}

void FdoSmLpClassBase::SetElementState(FdoSchemaElementState elementState)
{
	// Set state for this element.
	FdoSmSchemaElement::SetElementState( elementState );

	if ( elementState == FdoSchemaElementState_Deleted ) {
		// Delete all properties as well
        if ( mProperties ) {
		    for ( int i = 0; i < mProperties->GetCount(); i++ ) {
			    FdoSmLpPropertyP pProp = mProperties->GetItem(i);

			    pProp->SetElementState( elementState );
		    }
        }

        if ( mNestedProperties ) {
		    // Delete all nested properties
		    for ( int i = 0; i < mNestedProperties->GetCount(); i++ ) {
			    FdoSmLpPropertyP pProp = mNestedProperties->GetItem(i);

			    // Don't delete inherited properties, just the ones for this class.
			    if ( pProp->RefDefiningClass()->GetQName() == GetQName() ) {
				    pProp->SetElementState( elementState );
			    }
		    }
        }

/* TODO
        // Delete all indexes
		for ( int i = 0; i < mIndexes.GetCount(); i++ ) {
			FdoSmLpIndex* pIndex = mIndexes.GetItem(i);

			// Don't delete inherited indexes, just the ones for this class.
			if ( pIndex->GetDefiningClass() == this ) {
				pIndex->SetElementState( elementState );
			}
		}
*/
	}
}

void FdoSmLpClassBase::Commit( bool fromParent )
{
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhClassWriterP pWriter;

	switch ( GetElementState() ) {
  	case FdoSchemaElementState_Added:
        pWriter = GetPhysicalAddWriter();
        pWriter->Add();
		mId = pWriter->GetId();

		break;

	case FdoSchemaElementState_Deleted:
        pWriter = pPhysical->GetClassWriter();
        pWriter->Delete( GetParent()->GetName(), GetName(), GetId() );

		break;

	case FdoSchemaElementState_Modified:
        pWriter = GetPhysicalModifyWriter();
        pWriter->Modify( GetParent()->GetName(), GetName(), GetId() );

        break;
	}

	// Commit all the properties.
	if ( mProperties ) {
		for ( int i = 0; i < mProperties->GetCount(); i++ ) 
			FdoSmLpPropertyP(mProperties->GetItem(i))->Commit( fromParent );
	}
/* TODO
    // Commit all indexes.
	for ( int i = 0; i < mIndexes.GetCount(); i++ ) 
		mIndexes.GetItem(i)->Commit( fromParent );
*/
	// Commit Schema Attribute Dictionary
	CommitSAD(FdoSmPhMgr::ClassType);
}

FdoSchemaExceptionP FdoSmLpClassBase::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    int i;

	// Need to completely load the class to discover all errors.
	((FdoSmLpClassBase*) this)->Finalize();

	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	// Add errors for class table 
	if ( mDbObject ) 
		pException = mDbObject->Errors2Exception(pException);

	// Add errors for properties
	for ( i = 0; i < mProperties->GetCount(); i++ ) {
		const FdoSmLpPropertyDefinition* pProp = mProperties->RefItem(i);
		pException = pProp->Errors2Exception(pException);
	}
/* TODO
	// Add errors for indexes
	for ( i = 0; i < mIndexes.GetCount(); i++ ) {
		const FdoSmLpIndex* pIndex = mIndexes.RefItem(i);
    	pException = pIndex->Errors2Exception(pException);
	}
*/
	return pException;
}

FdoSmPhClassWriterP FdoSmLpClassBase::GetPhysicalAddWriter()
{
 	FdoSmPhMgrP pPhysical           = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhClassWriterP pWriter     = pPhysical->GetClassWriter();

    pWriter->SetName( GetName() );
    pWriter->SetSchemaName( GetLogicalPhysicalSchema()->GetName() );
    pWriter->SetClassType( FdoSmLpClassTypeMapper::Type2String(GetClassType()) );
    pWriter->SetTableName( mDbObjectName );
    pWriter->SetRootTableName( mRootDbObjectName );
    pWriter->SetBaseName( GetBaseClass() ? GetBaseClass()->GetQName() : L"" );
    pWriter->SetIsAbstract( mIsAbstract );
    pWriter->SetDescription( GetDescription() );
    pWriter->SetIsFixedTable( mbIsFixedDbObject );
    pWriter->SetIsTableCreator( mbIsDbObjectCreator );

    SetPhysicalOptionsWriter( pWriter );

    return( pWriter );
}

FdoSmPhClassWriterP FdoSmLpClassBase::GetPhysicalModifyWriter()
{
 	FdoSmPhMgrP pPhysical           = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhClassWriterP pWriter     = pPhysical->GetClassWriter();

    // Description and abstract flag are the only modifiable things.
    pWriter->SetIsAbstract( GetIsAbstract() );
	pWriter->SetDescription( GetDescription() );

    SetPhysicalOptionsWriter( pWriter );

    return( pWriter );
}

void FdoSmLpClassBase::SetPhysicalOptionsWriter( FdoSmPhClassWriterP classWriter )
{
    // Base function does nothing.
}

FdoSmLpDataPropertiesP FdoSmLpClassBase::GetIdentityProperties()
{
    if ( !mIdentityProperties ) 
        mIdentityProperties = new FdoSmLpDataPropertyDefinitionCollection();

	// Finalize not called since caller might add to the id property collection.
    return mIdentityProperties;
}

FdoSmLpUniqueConstraintsP FdoSmLpClassBase::GetUniqueConstraints()
{
    if ( !mUniqueConstraints ) 
        mUniqueConstraints = new FdoSmLpUniqueConstraintCollection();

    return FDO_SAFE_ADDREF(mUniqueConstraints.p);
}

FdoSmLpCheckConstraintsP FdoSmLpClassBase::GetCheckConstraints()
{
    if ( !mCheckConstraints ) 
		mCheckConstraints = new FdoSmLpCheckConstraintCollection();

    return FDO_SAFE_ADDREF(mCheckConstraints.p);
}

FdoSmLpPropertiesP FdoSmLpClassBase::GetProperties()
{
    if ( !mProperties ) 
        mProperties = new FdoSmLpPropertyDefinitionCollection();

	// Finalize not called since caller might add to the property collection.
    return mProperties;
}

FdoSmLpPropertiesP FdoSmLpClassBase::GetNestedProperties()
{
    if ( !mNestedProperties ) 
        mNestedProperties = new FdoSmLpPropertyDefinitionCollection();

	// Finalize not called since caller might add to the nested property collection.
    return mNestedProperties;
}

FdoSmLpDbObjectP FdoSmLpClassBase::GetDbObject()
{
	return mDbObject;
}

FdoSmPhDbObjectP FdoSmLpClassBase::NewTable( 
    FdoString* tableName, 
    FdoString* pkeyName
)
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhOwnerP owner = pPhysical->FindOwner();

	FdoSmPhTableP table = owner->CreateTable(tableName, pkeyName );

    // Table inherits long transaction and locking modes from current datastore.
    table->SetLtMode( owner->GetLtMode() );    
    table->SetLockingMode( owner->GetLckMode() );

    return table.p->SmartCast<FdoSmPhDbObject>();
}

FdoSmPhDbObjectP FdoSmLpClassBase::NewView( 
    FdoString* viewName, 
    FdoString* database,
    FdoString* owner,
    FdoString* tableName
)
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhOwnerP defaultOwner = pPhysical->FindOwner();

	return( defaultOwner->CreateView(viewName, database, owner, tableName).p->SmartCast<FdoSmPhDbObject>() );
}

void  FdoSmLpClassBase::ValidateForeignObjectName( FdoStringP foreignObjectName )
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    if ( strlen((const char*) foreignObjectName) > pPhysical->DbObjectNameMaxLen() ) {

        GetErrors()->Add( FdoSmErrorType_Other, 
		    //Name of default foreign object exceeds %1$d characters, please specify a schema override for %2$ls",
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_172),
				    pPhysical->DbObjectNameMaxLen(),
                    (FdoString*)GetQName()
			    )
		    )
	    );
    }
}

bool FdoSmLpClassBase::IsDbObjectCreator() const
{
    ((FdoSmLpClassBase*) this)->Finalize();

    return( Is_DbObjectCreator() );

}

bool FdoSmLpClassBase::Is_DbObjectCreator() const
{
    bool isDbObjectCreator = false;

    // mbIsDbObjectCreator tells us whether Schema Manager created the class table.
    // However, when mbIsDbObjectCreator is true, the class table is dropped when
    // the class is deleted. The following does an extra check to ensure that 
    // the class table and base class tables are different. If they are the 
    // same, false is always returned to ensure that the table does not get
    // dropped.
	if ( mPhDbObject && mbIsDbObjectCreator ) {
		// Find the base class table name. 
        FdoStringP baseDbObjectName = mBaseClass ? mBaseClass->GetDbObjectName() : L"";

        // If class table is not the base class table then this class owns it.
        // TODO: generate an error when class table is the base class table. A class
        // can never be the creator of a base class table.
		if ( baseDbObjectName.ICompare(mDbObjectName) != 0 )
            isDbObjectCreator = true;
    }

    return isDbObjectCreator;
}

bool FdoSmLpClassBase::VldDbObjectName( FdoStringP objectName, bool bFromConfigFile )
{
    bool bValid = true;
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSize iNameMaxLen = pPhysical->DbObjectNameMaxLen();


    // Censor out characters not acceptable to RDBMS
	FdoStringP workName = pPhysical->CensorDbObjectName(objectName);

    // When workName is different, column name has an invalid character.
    if ( workName != objectName && !bFromConfigFile) {
       AddTableCharError( objectName );
       bValid = false;
    }

    if ( (int)strlen(workName) > iNameMaxLen ) {
        AddTableLengthError( objectName, iNameMaxLen );
        bValid = false;
    }

    if ( pPhysical->IsDbObjectNameReserved( workName) && !bFromConfigFile ) {
        AddTableReservedError( objectName );
        bValid = false;
    }

    return( bValid );
}

void FdoSmLpClassBase::CreatePkey()
{
	int i;
    FdoSmPhTableP phTable = mPhDbObject.p->SmartCast<FdoSmPhTable>();

    // Skip if no table or table already has a primary key.
    if ( phTable && (phTable->GetPkeyColumns()->GetCount() == 0) ) {
        
        // Set the primary key columns to the identity property columns.
        for ( i = 0; i < mIdentityProperties->GetCount(); i++ ) {
			FdoSmLpDataPropertyP pIdProp = mIdentityProperties->GetItem(i);
			FdoSmPhColumnP pColumn = pIdProp->GetColumn();

			if ( pColumn ) {
  				phTable->AddPkeyCol(pColumn->GetName());
			}
		}

        NewPkey( phTable );
	}
}

void FdoSmLpClassBase::CreateUkeys( bool merge )
{
    FdoSmPhTableP			phTable = mPhDbObject.p->SmartCast<FdoSmPhTable>();
   	FdoSmLpPropertiesP		pLpProps = GetProperties();
   	FdoSmPhBatchColumnsP    ukeysPhColumnsColl;
   	int						count = -1;
   
   	if ( phTable ) {
        ukeysPhColumnsColl = phTable->GetUkeyColumns(); // Reads constrains from db
   
   		FdoSmLpUniqueConstraintsP   pLpUniqueConstraints = GetUniqueConstraints();
   
   		for ( int i = 0; i < ukeysPhColumnsColl->GetCount(); i++ ) {
   			FdoSmPhColumnsP	pUniqueCols = ukeysPhColumnsColl->GetItem(i);
   
   			FdoSmLpUniqueConstraintP	pUniqueC = new FdoSmLpUniqueConstraint();
   			FdoSmLpDataPropertiesP		pProps = pUniqueC->GetProperties();
            bool                        allFound = true;
   
   			for ( int j = 0; j < pUniqueCols->GetCount(); j++ ) {
   				FdoSmPhColumnP	pUniqueCol = pUniqueCols->GetItem(j);
   
   				// Match the data property by column name and add the property to the collection
   				const FdoSmLpSimplePropertyDefinition* pMatchedProp = 		 
   						FdoSmLpSimplePropertyDefinition::ColName2Property( mProperties, pUniqueCol->GetName() );
   				
                if ( pMatchedProp ) {
                    if ( !pMatchedProp->GetIsSystem() )
    					pProps->Add( (FdoSmLpDataPropertyDefinition *)pMatchedProp );
                }
                else {
                    allFound = false;
                }
 			}
            
            // Skip the constraint if any of its columns do not correspond
            // to a property.
            if ( allFound && (pProps->GetCount() > 0) ) 
			{
                pLpUniqueConstraints->Add(pUniqueC);
			}
   		}
   
   		count = ukeysPhColumnsColl->GetCount();
   	}
   
   	// Skip if no table or table already has unique keys.
    if ( phTable && ( count == 0 || merge ) ) {
   
   		for ( int i = 0; i < mUniqueConstraints->GetCount(); i++ ) {
   
   			FdoSmLpUniqueConstraintP	pUniqueC = (FdoSmLpUniqueConstraint *) mUniqueConstraints->GetItem(i);
   			FdoSmLpDataPropertiesP		pProps = pUniqueC->GetProperties();
            // Instantiate new unique key
            NewUkey( phTable, pProps );
		}

        // Some providers, e.g. MySQL, need a unique constraint on each
        // autogenerated column. The following generates these constraints
        // if they don't yet exist. 
        for ( int i = 0; i < mProperties->GetCount(); i++ ) {
            // Check each data property
			FdoSmLpDataPropertyP pProp = mProperties->GetItem(i)->SmartCast<FdoSmLpDataPropertyDefinition>(true);
            if ( pProp && ((mIdentityProperties->GetCount() != 1) || (pProp->GetIdPosition() != 1)) ) {
                FdoSmPhColumnP pColumn = pProp->GetColumn();
                if ( pColumn &&
                     (pColumn->GetElementState() == FdoSchemaElementState_Added) &&
                     pColumn->GetAutoincrement() &&
                     (wcscmp( pColumn->GetParent()->GetName(), phTable->GetName() ) == 0)
                ) {
                    // Property column is new, autoincremented and in the class table.
                    // Check if need a unique constraint for it.

                    // First, check if it already has unique constraint and that it is the 
                    // only property in the constraint.
                    bool found = false;
		            for ( int j = 0; j < mUniqueConstraints->GetCount(); j++ ) {

			            FdoSmLpUniqueConstraintP	pUniqueC = (FdoSmLpUniqueConstraint *) mUniqueConstraints->GetItem(j);
			            FdoSmLpDataPropertiesP		pProps = pUniqueC->GetProperties();
                        if ( pProps->GetCount() == 1 ) {
                            FdoSmLpDataPropertyP pConsProp = pProps->GetItem(0);

                            if ( wcscmp(pProp->GetName(), pConsProp->GetName()) == 0 ) {
                                found = true;
                                break;
                            }
                        }
		            }
   
                    if ( !found ) {
                        // Property does not yet have a constraint so create one.
                        FdoSmLpDataPropertiesP pProps = new FdoSmLpDataPropertyDefinitionCollection();
                        pProps->Add( pProp );
                        NewUkey( phTable, pProps );
                    }
                }
            }
        }
	}
}

void FdoSmLpClassBase::NewUkey( FdoSmPhTableP table, FdoSmLpDataPropertiesP pProps )
{
    FdoSmPhBatchColumnsP    ukeysPhColumnsColl = table->GetUkeyColumns(); 
    
    // Create a new unique key and add it to the collection of collections. 
    FdoSmPhColumnsP			ukeyColumns = new FdoSmPhColumnCollection();
   
	// Test if this PH constraint already exists. First build a PH unique constraint.

	for ( int i = 0; i < pProps->GetCount(); i++ ) {
		FdoSmLpDataPropertyP pProp = pProps->GetItem(i)->SmartCast<FdoSmLpDataPropertyDefinition>(true);
   
		if ( pProp ) {
			FdoSmPhColumnP pColumn = pProp->GetColumn();
			if ( pColumn )
				ukeyColumns->Add(pColumn);
   		}
   	}

	bool found = false;
	for ( int i = 0; i < ukeysPhColumnsColl->GetCount() && !found; i++ ) {
		FdoSmPhColumnsP		ukeyColumnsDb = ((FdoSmPhBatchColumnCollection *)ukeysPhColumnsColl)->GetItem(i);	

		// Skip if size different
		if ( ukeyColumnsDb->GetCount() != ukeyColumns->GetCount() )
			continue;

		// Match each component of the unique key
		bool matchedAll = true;
		for ( int j = 0; j < ukeyColumns->GetCount() && matchedAll; j++ ){
			FdoSmPhColumnP			pPhCol = ukeyColumns->GetItem(j);	

			// Unfortunatelly the order may be different so don't match by position.
			bool matched = false;
			for ( int k = 0; k < ukeyColumnsDb->GetCount() && !matched; k++ ) {
				FdoSmPhColumnP		pPhColDb = ukeyColumnsDb->GetItem(k);
				matched = ( wcscmp(pPhCol->GetName(), pPhColDb->GetName() ) == 0 );
			}
			if ( !matched )
				matchedAll = false; 
		}
		if ( matchedAll )
			found = true;
	}

	// Add to the list if new
	if ( !found ) {
		ukeyColumns->SetElementState(FdoSchemaElementState_Added);
		ukeysPhColumnsColl->Add( ukeyColumns );
	}   
}
   
FdoStringP FdoSmLpClassBase::GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp )
{
    FdoSmPhTableP phTable = mPhDbObject.p->SmartCast<FdoSmPhTable>();
	FdoStringP	clause;

	if ( phTable ) {
		clause = phTable->GetCkeyClause( columnName, fdoProp );
	}

	return clause;
}

void FdoSmLpClassBase::CreateCkeys( bool bMerge )
{
    FdoSmPhTableP				phTable = mPhDbObject.p->SmartCast<FdoSmPhTable>();
	FdoSmLpCheckConstraintsP	pLpCheckConstraints = GetCheckConstraints();
	FdoSmPhCheckConstraintsP	phCkeys;
	int							count = -1;

	if ( phTable ) {

      	phCkeys = phTable->GetCkeyColl(); // Reads from db

       	FdoSmLpPropertiesP		  pLpProps = GetProperties();

		// Just copy from PH table. Maybe each data property should hold its constraint?
		for ( int i = 0; i < phCkeys->GetCount(); i++ ) {
			FdoSmPhCheckConstraintP		phCkey = phCkeys->GetItem(i);
	
			// Ignore the constraints marked for deletion
			if ( phCkey->GetElementState() == FdoSchemaElementState_Deleted )
				continue;

			// Match the data property by column name and add the property to the collection
			const FdoSmLpSimplePropertyDefinition* pMatchedProp = 		 
					FdoSmLpSimplePropertyDefinition::ColName2Property( mProperties, phCkey->GetColumnName() );

			// Check if already exists
			if ( pMatchedProp ) { 
				bool found = false;
				for ( int j = 0; j < pLpCheckConstraints->GetCount() && !found; j++ ) {
					FdoSmLpCheckConstraintP  lpCkey = pLpCheckConstraints->GetItem(j);
					found = ( wcscmp(phCkey->GetColumnName(), lpCkey->GetColumnName()) == 0 );
				}
				if ( !found ) {
					FdoSmLpCheckConstraintP  lpCkey = new FdoSmLpCheckConstraint( pMatchedProp->GetName(), phCkey->GetColumnName(), phCkey->GetName(), phCkey->GetClause());
					pLpCheckConstraints->Add( lpCkey );
				}
			}
		}
		count = phCkeys->GetCount();
	}

    // Skip if no table. Merge new LP constraints into PH.
	if ( phTable && ( count == 0 || bMerge ) ) {
		for ( int i = 0; i < pLpCheckConstraints->GetCount(); i++ ) {
			FdoSmLpCheckConstraintP		lpCkey = pLpCheckConstraints->GetItem(i);
			
			bool	found = false;
			for ( int j = 0; j < phCkeys->GetCount() && !found; j++ ) {
				FdoSmPhCheckConstraintP		phCkey = phCkeys->GetItem(j);
				found = ( wcscmp(phCkey->GetColumnName(), lpCkey->GetColumnName()) == 0 );
			}
			if ( !found ) {
				FdoSmPhCheckConstraintP  phCkey = new FdoSmPhCheckConstraint( L"", lpCkey->GetColumnName(), lpCkey->GetClause());
				phCkey->SetElementState(FdoSchemaElementState_Added);
				phTable->AddCkeyCol( phCkey );
			}
		}
	}
}

void FdoSmLpClassBase::NewPkey( FdoSmPhTableP table )
{
}

FdoStringsP FdoSmLpClassBase::GetDbIds()
{
	int         i;
    FdoStringsP ids = FdoStringCollection::Create();

    for ( i = 0; i < mIdentityProperties->GetCount(); i++ ) {
    	const FdoSmLpDataPropertyDefinition* pIdProp = mIdentityProperties->RefItem(i);
		const FdoSmPhColumn* pColumn = pIdProp->RefColumn();

		if ( pColumn ) {
			ids->Add( pIdProp->GetName() );
		}
	}

    return ids;
}

void FdoSmLpClassBase::SetTableMapping( FdoSmOvTableMappingType mappingType )
{
    mTableMapping = mappingType;
}

void FdoSmLpClassBase::SetIsDbObjectCreator( bool bCreator )
{
    mbIsDbObjectCreator = bCreator;
}

void FdoSmLpClassBase::SetDatabase( FdoString* database )
{
    mDatabase = database;

    if ( mDatabase.GetLength() == 0 ) 
        mDatabase = GetLogicalPhysicalSchema()->GetDatabase();
}

void FdoSmLpClassBase::SetOwner( FdoString* owner )
{
    mOwner = owner;

    if ( mOwner.GetLength() == 0 ) 
        mOwner = GetLogicalPhysicalSchema()->GetOwner();
}

void FdoSmLpClassBase::SetDbObjectName( FdoStringP objectName )
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    if ( mOwner.GetLength() > 0 ) {
        // Owner specified, table is foreign.
        if ( objectName.GetLength() > 0 ) {
            // Foreign table name is the one specified by overrides.
            mRootDbObjectName = objectName;
        }
        else {
            // Foreign table name is the same as class name.
            mRootDbObjectName = pPhysical->GetDcDbObjectName( GetName() );
            ValidateForeignObjectName( mRootDbObjectName );
        }

        if ( pPhysical->GetOwner()->GetHasMetaSchema() ) {
            // Generate name for view that will be put around the foreign table.
            mDbObjectName = pPhysical->GetOwner()->UniqueDbObjectName( mRootDbObjectName );
        }
        else {
            mDbObjectName = mRootDbObjectName;
        }
    }
    else {
        if ( objectName.GetLength() > 0 ) {
            // Table not foreign but name is specified by overrides.
            mDbObjectName =  objectName;

            // Add to fetch candidates list, so these tables are fetched efficiently.
            pPhysical->GetOwner()->AddCandDbObject( objectName );
        }
        else {
            // No overrides so generate a table name for the new class. The table name is the class name
            // adjusted to be RDBMS-friendly.

           mDbObjectName = pPhysical->GetOwner()->UniqueDbObjectName( GetName() );
        }

        // TODO: set classification for foreign table.
        pPhysical->SetDbObjectClassification( mDbObjectName, GetQName() );
    }
}

void FdoSmLpClassBase::Finalize()
{
	// Finalize is not re-entrant.
	if ( GetState() == FdoSmObjectState_Finalizing) {
		// Don't worry about errors for elements that are going away.
		if ( GetElementState() != FdoSchemaElementState_Deleted ) 
			AddFinalizeLoopError();
		return;
	}

	if ( GetState() == FdoSmObjectState_Initial ) {
		SetState( FdoSmObjectState_Finalizing);

		/* Get the base class object */

		if ( mBaseClassName.GetLength() > 0 ) {
			const FdoSmLpClassDefinition* pBaseClass = this->GetLogicalPhysicalSchema()->FindClass(mBaseClassName);
		
			if ( pBaseClass ) {
				if ( pBaseClass->GetState() == FdoSmObjectState_Finalizing ) 
				{
					// If the base class is finalizing then there is a loop in this
					// class's ancestry ( it inherits from itself ).
					AddBaseClassLoopError(pBaseClass);
				}
				else {
					// Force base class to finalize
					pBaseClass->RefBaseClass();

					// Also, if the base class has an ancestry loop so does this class.
					const FdoSmError* pError = pBaseClass->RefErrors()->RefItem(FdoSmErrorType_BaseClassLoop);
					if ( pError ) {
						AddBaseClassLoopError(pBaseClass);
					}
					else {
						// Ancestry is fine, set the base class.
						mBaseClass = FDO_SAFE_ADDREF((FdoSmLpClassDefinition*) pBaseClass);

                        if ( Get_TableMapping() != FdoSmOvTableMappingType_BaseTable ) {
                			// If the class and base class tables are the same then we're doing
			                // basetable mapping. 
			                if ( GetDbObjectQName().ICompare(mBaseClass->GetDbObjectQName()) == 0 ) 
				                SetTableMapping( FdoSmOvTableMappingType_BaseTable );
                            else
                                SetTableMapping( FdoSmOvTableMappingType_ConcreteTable );
                        }

                        // Check for additional errors. We don't care about these
						// errors if this class is going away.
						if ( GetElementState() != FdoSchemaElementState_Deleted ) {

                            // Error if base class is flagged for delete. Adding this error
							// prevents the base class from being deleted.
							if ( mBaseClass->GetElementState() == FdoSchemaElementState_Deleted )
								AddBaseClassDeleteError(mBaseClass);

							// Error if base class has different class type.
							if ( mBaseClass->GetClassType() != GetClassType() ) 
								AddBaseClassTypeError(mBaseClass);
						}
					}
				}
			}
			else {
                SetTableMapping( FdoSmOvTableMappingType_ConcreteTable );
				// Error if the base class is not in the current database.
				if ( GetElementState() != FdoSchemaElementState_Deleted ) 
					AddBaseClassMissingError();
			}
		}
        else {
            SetTableMapping( FdoSmOvTableMappingType_ConcreteTable );
        }

        FinalizePhDbObject();

		const FdoSmLpClassDefinition* pBaseClass = (FdoSmLpClassDefinition*) mBaseClass;

		// If this class has no base or source class then get the base properties for it's
		// class type from the properties of its MetaClass.
		if ( (!pBaseClass) && !(mSrcClass) ) 
			pBaseClass = RefMetaClass();

		if ( pBaseClass ) {

			/* Resolve property inheritances */
			
			FinalizeProps( pBaseClass->RefNestedProperties(), GetNestedProperties() );
			FinalizeProps( pBaseClass->RefProperties(), GetProperties() );
		}

		/* Resolve the class table and match each primary key column in the table
		 * to a class property
		 */

		// Generate the Identity property collection from the class 
		// table pkey columns. Need to be done only if identity
		// properties not yet set.

		FinalizeIdProps();

		// Make sure ID properties identical to those of the base class.
		// Skip this check for MetaClasses since MetaClasses don't have
		// to inherit id properties.
		
		if ( mBaseClass && (wcscmp( GetLogicalPhysicalSchema()->GetName(), FdoSmLpSchema::mMetaClassSchemaName ) != 0) ) {
			const FdoSmLpDataPropertyDefinitionCollection* pBaseIdProps
				= mBaseClass->RefIdentityProperties();

			if ( (mIdentityProperties->GetCount() != pBaseIdProps->GetCount()) &&
				 (GetElementState() != FdoSchemaElementState_Deleted) ) {
//				AddIdDifferentFromBaseError();
			}
			else {
				for ( int i = 0; i < mIdentityProperties->GetCount(); i++ ) {
					if ( (wcscmp( mIdentityProperties->RefItem(i)->GetName(), pBaseIdProps->RefItem(i)->GetName() ) != 0) &&
						 (GetElementState() != FdoSchemaElementState_Deleted) ) {
//						AddIdDifferentFromBaseError();
						break;
					}
				}
			}
		}


		if ( mPhDbObject ) {
			// The class table exists so create a LogicalPhysical wrapper for it.
			// The wrapper also shows which class properties reside in the table.

			mDbObject = new FdoSmLpDbObject(mDbObjectName, mPhDbObject, mProperties, true, this);

			// Find all tables that contain this class's data properties.
			FinalizeDbObjects();

			// Generate the Unique keys collection from the class 
			// table ukeys columns. Need to be done only if unique keys not yet set.

			FinalizeUkeys();

			// Generate the Check keys collection from the class 
			// table ckeys columns. Need to be done only if check keys not yet set.

			FinalizeCkeys();

			if ( GetElementState() != FdoSchemaElementState_Deleted ) {
				// The following checks if the class has multiple FeatId properties
				// and logs an error if it does.
				try {
					const FdoSmLpDataPropertyDefinition* pFeatProp = FdoSmLpDataPropertyDefinitionCollection::FindFeatIDProperty( mProperties );
				}
				catch ( FdoSchemaException* se ) {
					AddGenericError( se->GetExceptionMessage() );
				}
			}
            else {
		        // Drop the class table only if this class created it.
		        if ( Is_DbObjectCreator() )
			        mPhDbObject->SetElementState(GetElementState());
            }
		}

        // Invoke the callback for handling extra finalization for derived
        // classes
        PostFinalize();

		SetState( FdoSmObjectState_Final);
	}

}

void FdoSmLpClassBase::PostFinalize()
{
}

void FdoSmLpClassBase::FinalizeProps( 
	const FdoSmLpPropertyDefinitionCollection* pBaseProps,
	FdoSmLpPropertiesP pProps
)
{
	for ( int i = 0; i < pBaseProps->GetCount(); i++ ) 
	{
		const FdoSmLpPropertyDefinition* pBaseProp = pBaseProps->RefItem(i);
				
        if ( CanInherit(pBaseProp) ) {
            // For each base property, find the property on this class. 
            FdoSmLpPropertyP pProp = MatchInheritedProp( pBaseProp, pProps ); 

            if ( pProp ) {
                // Found, so make the defining class for the property the same
                // as that of the base property.
                pProp->SetInherited( pBaseProp );
            }
            else {
                // Not found, auto-generated an inherited property. 
                FdoStringP propName = pBaseProp->GetName();

                // Create the missing class property or nested property from 
                // the base property.

                FdoSmLpPropertyP pInhProp = pBaseProp->CreateInherited( dynamic_cast<FdoSmLpClassDefinition*>(this) );
                pProps->Add( pInhProp );
            }
        }
	}
}

void FdoSmLpClassBase::FinalizeIdProps()
{
    FdoStringsP     dbIds;
    int				i,j;
    FdoSmLpPropertiesP props = GetProperties();
    FdoSmLpDataPropertiesP idProps = GetIdentityProperties();

    if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {

		if ( mBaseClass ) {
			// For new classes, inherit the id properties from the base class.
			// FDO does not allow sub-classes to redefine the id properties.

			const FdoSmLpDataPropertyDefinitionCollection* pBaseIdProps =
				mBaseClass->RefIdentityProperties();

			for ( i = 0; i < pBaseIdProps->GetCount(); i++ ) {
				FdoSmLpDataPropertyP pIdProp =
					props->FindItem(pBaseIdProps->RefItem(i)->GetName())->SmartCast<FdoSmLpDataPropertyDefinition>(true);

				if ( pIdProp ) 
					mIdentityProperties->Add( pIdProp );
				// Todo: Trap an error when Id property not found.
			}
		}

		// Set the position for each id property

		for ( i = 0; i < idProps->GetCount(); i++ ) {
			FdoSmLpDataPropertyP pIdProp = idProps->GetItem(i);
			pIdProp->SetIdPosition( i + 1 );  // position is 1-based.
		}

		// Add each id property to the class table's primary key.

		if ( mPhDbObject && (GetElementState() == FdoSchemaElementState_Added) ) {
			CreatePkey();
		}
    }
    else {
		// For existing classes, find the id properties by finding all properties
		// with positive id position. Properties are added to the identityProperties
		// collection sorted by position.

		if ( idProps->GetCount() == 0 ) {
			// Find the highest position.

			int maxPosn = 0;

			for ( i = 0; i < props->GetCount(); i++ ) {
				const FdoSmLpDataPropertyDefinition* pProp = 
					FdoSmLpDataPropertyDefinition::Cast( props->RefItem(i) );

				if ( pProp && (pProp->GetIdPosition() > maxPosn) )
					maxPosn = pProp->GetIdPosition();
			}
		
			// Find the identity property at each position.
			// start with 1 and end with the highest position.
			for ( i = 1; i <= maxPosn; i++ ) {
				bool bFound = false;
				for ( j = 0; j < mProperties->GetCount(); j++ ) {
					FdoSmLpDataPropertyP pProp = 
						props->GetItem(j)->SmartCast<FdoSmLpDataPropertyDefinition>(true);

					if ( pProp && pProp->GetIdPosition() == i ) {
						if ( !bFound ) {
							// Property for this position was found, make it an
							// identityproperty.
							idProps->Add(pProp);
							bFound = true;
						}
						else {
/*
							if ( GetElementState() != FdoSchemaElementState_Deleted ) 
								// Can't determine proper id property order if 
								// two have the same position.
								AddDuplicateIdPosnError(i);
							break;
*/
						}
					}

					// It's ok not to find a property for each position.
					// Someone might have deleted an identityProperty in SQLPLUS and 
					// forgot to resequence the rest of the identityProperties. This 
					// is ok since we'll still get the remaining identityProperties 
					// in the right order.
				}
			}
		}
	}

    // Check for conflicts between identity properties in FDO and the MetaSchema.
    // This check is not done if this element is flagged as unchanged.

    if ( (GetElementState() == FdoSchemaElementState_Added) ||
         (GetElementState() == FdoSchemaElementState_Modified) 
    ) {
        // Get the datastore id properties
        dbIds = GetDbIds();

		// Make sure that FDO and Datastore id properties match.

		if ( mbCheckIds && (mFdoIds->ToString() != dbIds->ToString()) ) {
			if ( mBaseClass ) 
                // The mismatch is for the identity properties for the base class.
				AddFdoDbIdMismatchError( mFdoIds, dbIds );
			else
                // The mismatch is for identity properties defined for this class,
                // so log an identity property modification error.
                // Modifying identity property list is not supported. 
				AddModIdPropsError( mFdoIds, dbIds );
		}
    }

	if ( mIdentityProperties->GetCount() > 0 ) {
		// Make all id properties are not nullable and that all non-featid id 
		// properties are not readonly. Featid properties can be readonly since 
		// FDO autogenerates their values.

		for ( i = 0; i < mIdentityProperties->GetCount(); i++ ) {
			const FdoSmLpDataPropertyDefinition* pIdProp = mIdentityProperties->RefItem(i);
			if ( !pIdProp->GetNullable() ) {
				// Just trap readonly id properties in Add. If the administrator REALLY
				// want it to be readonly, it can be updated.
				if ( (GetElementState() == FdoSchemaElementState_Added) && pIdProp->GetReadOnly() && (!pIdProp->GetIsAutoGenerated()) ) 
					AddReadOnlyIdPropError( pIdProp );
			}
			else {
				AddNullIdPropError( pIdProp );
			}
		}
	}
}

void FdoSmLpClassBase::CreateUkeysFromFdo()
{
	FdoClassDefinitionP pBaseClass = mFdoClass->GetBaseClass();
	FdoClassDefinitionP pRootClass = FDO_SAFE_ADDREF(mFdoClass.p);

	FdoPtr<FdoUniqueConstraintCollection> pFdoUniqueConstraints = pRootClass->GetUniqueConstraints();
	FdoSmLpUniqueConstraintsP   pLpUniqueConstraints = GetUniqueConstraints();

	// Add the class constraints to LP
	for ( int i = 0; i < pFdoUniqueConstraints->GetCount(); i++ ) {

		FdoPtr<FdoUniqueConstraint>	pFdoUniqueCsObj = pFdoUniqueConstraints->GetItem(i);
		FdoDataPropertiesP			pFdoUniqueCs = pFdoUniqueCsObj->GetProperties();

		// Each collection of data properties needs to be checked and added.				
		FdoSmLpUniqueConstraint		*pLpUniqueC = new FdoSmLpUniqueConstraint();

		for ( int j = 0; j < pFdoUniqueCs->GetCount(); j++ ) {
			FdoDataPropertyP	 pFdoDataProp = pFdoUniqueCs->GetItem(j);
			FdoSmLpDataPropertyP pLpDataProp = mProperties->FindItem( pFdoDataProp->GetName() )->SmartCast<FdoSmLpDataPropertyDefinition>(true);
			pLpUniqueC->GetProperties()->Add( pLpDataProp );
		}	

		if ( pFdoUniqueCs->GetCount() != 0 )
			pLpUniqueConstraints->Add( pLpUniqueC );
		pLpUniqueC->Release();
	}

	// Add base class collection of unique constraints
	if ( mBaseClass ) {
		FdoSmLpUniqueConstraintsP   pLpUniqueConstraintsB = mBaseClass->GetUniqueConstraints();
		for ( int j = 0; j < pLpUniqueConstraintsB->GetCount(); j++ ) {
			FdoSmLpUniqueConstraintP		pLpUniqueC = pLpUniqueConstraintsB->GetItem(j);
			pLpUniqueConstraints->Add( pLpUniqueC );
		}	
	}
}

bool FdoSmLpClassBase::MatchUkey( FdoClassDefinitionP pClass, FdoSmPhColumnsP pPhColls )
{
	FdoUniqueConstraintCollectionP pFdoUniqueConstraints = pClass->GetUniqueConstraints();

	// Find a match between FDO and PH. Some FDO constraints may be deleted.
	bool found = false;
	for ( int i = 0; i < pFdoUniqueConstraints->GetCount() && !found; i++ ) {

		FdoPtr<FdoUniqueConstraint>		pFdoUniqueC = pFdoUniqueConstraints->GetItem(i);
		FdoDataPropertiesP				pFdoDataProps = pFdoUniqueC->GetProperties();

		// No match if size is different.
		if ( pPhColls->GetCount() != pFdoDataProps->GetCount())
			continue;

		// Match each component of the unique key
		bool matchedAll = true;
		for ( int j = 0; j < pPhColls->GetCount() && matchedAll; j++ ){
			FdoSmPhColumnP			pPhCol = pPhColls->GetItem(j);	

			// Unfortunatelly the order may be different so don't match by position.
			bool matched = false;
			for ( int k = 0; k < pFdoDataProps->GetCount() && !matched; k++ ) {
				FdoDataPropertyP		pFdoDataProp = pFdoDataProps->GetItem(k);
				FdoSmLpDataPropertyP	pLpDataProp = mProperties->FindItem( pFdoDataProp->GetName() )->SmartCast<FdoSmLpDataPropertyDefinition>(true);

				matched = ( wcscmp(pPhCol->GetName(), pLpDataProp->GetColumnName() ) == 0 );
			}
			if ( !matched )
				matchedAll = false; 
		}
		if ( matchedAll )
			found = true;
	}

	// For MySql an unique key has been added for autogenerated identity property. Ignore it, otherwise it will be dropped.
	if ( !found && pPhColls->GetCount() == 1 ) {
		FdoSmPhColumnP			pPhCol = pPhColls->GetItem(0);	
		found = pPhCol->GetAutoincrement();
	}

	return found;
}	

void FdoSmLpClassBase::DropUkeys()
{
	// Check the contraints still match Physical vs. FDO

	FdoClassDefinitionP pRootClass = FDO_SAFE_ADDREF(mFdoClass.p);
    FdoSmPhTableP		phTable = mPhDbObject.p->SmartCast<FdoSmPhTable>();

	if ( phTable ) {

   		const FdoSmPhBatchColumnCollection*	ukeysPhColumnsColl = phTable->RefUkeyColumns(); 

		for ( int i = 0; i < ukeysPhColumnsColl->GetCount(); i++ ) {
			FdoSmPhColumnsP		pPhColls = ((FdoSmPhBatchColumnCollection *)ukeysPhColumnsColl)->GetItem(i);	

			bool	found = MatchUkey( pRootClass, pPhColls );

			// Try also the base classes. Walk up the hierarchy.
			FdoClassDefinitionP		pCurrClass = pRootClass;
			FdoClassDefinitionP		pBaseClass;

			while ( !found && ( (pBaseClass = pCurrClass->GetBaseClass()) != NULL ) ) {
				found = MatchUkey( pBaseClass, pPhColls );
				pCurrClass = pBaseClass;
			}

			// Still not found, add it to the list of constraints to be deleted.
			if ( !found ) {
				FdoStringsP		pDeletedConstraints = phTable->GetDeletedConstraints();
				pDeletedConstraints->Add( pPhColls->GetName() );
			}
		}
	}
}


bool FdoSmLpClassBase::MatchCkey( FdoClassDefinitionP pClass, FdoSmPhCheckConstraintP pCkeyPh )
{
	FdoPropertiesP	pFdoProps = pClass->GetProperties();
	bool			found = false;

	// Check LP ckey definition against FDO.
	for ( int i = 0; i < pFdoProps->GetCount(); i++ ) {

		FdoPropertyP		pFdoProp = pFdoProps->GetItem(i);

		// Skip properties other than data properties.
		if ( pFdoProp->GetPropertyType() != FdoPropertyType_DataProperty )
			continue;

		FdoDataPropertyDefinition*	pFdoDataProp = (FdoDataPropertyDefinition *)(pFdoProp.p);
		FdoSmLpDataPropertyP	pLpDataProp = mProperties->FindItem( pFdoDataProp->GetName() )->SmartCast<FdoSmLpDataPropertyDefinition>(true);

		// Match by column name.
		if ( wcscmp( pCkeyPh->GetColumnName(), pLpDataProp->GetColumnName() ) != 0 )
			continue;

		FdoPtr<FdoPropertyValueConstraint>	pValConstr = pFdoDataProp->GetValueConstraint();

		// Check the status of the property. If anything changed then the constraint will be dropped.
		if ( pValConstr ) {
					
			found = ( pFdoDataProp->GetElementState() == FdoSchemaElementState_Added ||
				      pFdoDataProp->GetElementState() == FdoSchemaElementState_Unchanged  );
		}

		// If FDO property doesn't have a constraint then it has been removed. The constraint
		// will be dropped.

		break;
	}

	return found;
}	

void FdoSmLpClassBase::DropCkeys()
{
	// Check the contraints still match LP vs. FDO

	FdoClassDefinitionP		pRootClass = FDO_SAFE_ADDREF(mFdoClass.p);
    FdoSmPhTableP			phTable = mPhDbObject.p->SmartCast<FdoSmPhTable>();

	if ( phTable ) {
   		const FdoSmPhCheckConstraintCollection*	ckeysPhColl = phTable->RefCkeyColl(); 

		for ( int i = 0; i < ckeysPhColl->GetCount(); i++ ) {
			FdoSmPhCheckConstraintP		ckeyPh = ((FdoSmPhCheckConstraintCollection *)ckeysPhColl)->GetItem(i);	

			bool	found = MatchCkey( pRootClass, ckeyPh);

			// Try also the base classes. Walk up the hierarchy.
			FdoClassDefinitionP		pCurrClass = pRootClass;
			FdoClassDefinitionP		pBaseClass;

			while ( !found && ( (pBaseClass = pCurrClass->GetBaseClass()) != NULL ) ) {
				found = MatchCkey( pBaseClass, ckeyPh );
				pCurrClass = pBaseClass;
			}

			// Still not found, add it to the list of constraints to be deleted.
			if ( !found ) {
				FdoStringsP		pDeletedConstraints = phTable->GetDeletedConstraints();
				pDeletedConstraints->Add( ckeyPh->GetName() );
				ckeyPh->SetElementState( FdoSchemaElementState_Deleted );
			}
		}
	}
}

void FdoSmLpClassBase::CreateCkeysFromFdo()
{
	FdoClassDefinitionP			pBaseClass = mFdoClass->GetBaseClass();
	FdoClassDefinitionP			pRootClass = FDO_SAFE_ADDREF(mFdoClass.p);
	FdoPropertiesP				pFdoProps = pRootClass->GetProperties();
	FdoSmLpCheckConstraintsP	pCkeys = GetCheckConstraints();

	// Each data property needs to be checked and added.
	for ( int i = 0; i < pFdoProps->GetCount(); i++ ) {

		FdoPropertyP	pFdoProp = pFdoProps->GetItem(i);

		// Skip properties other than data properties.
		if ( pFdoProp->GetPropertyType() != FdoPropertyType_DataProperty )
			continue;

		// Subsequent calls need a FdoDataPropertyP
		FdoDataPropertyP		pFdoDataProp = (FdoDataPropertyDefinition*)pFdoProps->GetItem(i);
		FdoSmLpDataPropertyP	pLpDataProp = mProperties->FindItem( pFdoDataProp->GetName() )->SmartCast<FdoSmLpDataPropertyDefinition>(true);				

		if ( pLpDataProp ) {
			FdoPtr<FdoPropertyValueConstraint>	pValConstr = pFdoDataProp->GetValueConstraint();

			// Generate a clause. Ignore the empty ones.
			if ( pValConstr ) {
			
				FdoStringP clause = GetCkeyClause( pLpDataProp->GetColumnName(), pFdoDataProp );
				if ( clause != L"" ) {
					FdoSmLpCheckConstraintP	lpCkey = new FdoSmLpCheckConstraint( L"", pLpDataProp->GetColumnName(), L"", clause );
					pCkeys->Add( lpCkey );
				}
			}
		}
	}

	// Add base class collection of check constraints
	if ( mBaseClass ) {
		FdoSmLpCheckConstraintsP   pLpCheckConstraintsB = mBaseClass->GetCheckConstraints();
		for ( int j = 0; j < pLpCheckConstraintsB->GetCount(); j++ ) {
			FdoSmLpCheckConstraintP		pLpCkey = pLpCheckConstraintsB->GetItem(j);
			pCkeys->Add( pLpCkey );
		}	
	}
}

void FdoSmLpClassBase::FinalizeUkeys()
{
	bool bMerge = false;

	// Handle the unique constraints properties. 
    if ( mFdoClass && ((GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo()) ) 
	{
		CreateUkeysFromFdo();
	}
    else if ( mFdoClass ) 
	{
		// Save the unique keys to delete.
		DropUkeys();
			
		// Rebuild the Lp constraints. 
		CreateUkeysFromFdo();

		bMerge = true;
	}

	if ( mPhDbObject ) {
		CreateUkeys( bMerge);
	}	
}

void FdoSmLpClassBase::FinalizeCkeys()
{
	bool bMerge = false;

	// Handle the properties with check constraints. 
    if ( mFdoClass && ((GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo()) ) 
	{
		CreateCkeysFromFdo();
	}
    else if ( mFdoClass ) 
	{
		// Save the check constraints to delete.
		DropCkeys();
			
		// Rebuild the Lp constraints. 
		CreateCkeysFromFdo(); 

		bMerge = true;
	}

	if ( mPhDbObject ) {
		CreateCkeys( bMerge );
	}
}

/*
void FdoSmLpClassBase::FinalizeIndexes()
{
    const FdoSmLpIndexCollection* pBaseIndexes = mpBaseClass->GetIndexes();

	for ( int i = 0; i < pBaseIndexes->GetCount(); i++ ) 
	{
		const FdoSmLpIndex* pBaseIndex = pBaseIndexes->RefItem(i);
				
		// For each base index, find the index on this class. 
		FdoSmLpIndex* pIndex = mIndexes.FindItem( pBaseIndex->GetName() );				

		if ( pIndex ) {
			// Found, so make the defining class for the index the same
			// as that of the base index.
			pIndex->SetInherited( pBaseIndex );
		}
		else {
            // Not found, so create the inherited index.
            pIndex = pBaseIndex->CreateInherited( this );
            mIndexes.Add( pIndex );
            pIndex->Release();
		}
	}
}
*/
void FdoSmLpClassBase::FinalizePhDbObject()
{

    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhViewP viewObj;

    if ( mBaseClass && Get_TableMapping() == FdoSmOvTableMappingType_BaseTable ) {
        // Set the table to the base class table when base table 
        // mapping is in effect.

        if ( (mDbObjectName.GetLength() > 0) && 
            (GetDbObjectQName().ICompare(mBaseClass->GetDbObjectQName()) != 0) &&
            (GetElementState() != FdoSchemaElementState_Deleted)
        ) {
            // Error if class table already set and it is not the
            // base class table.
//          AddBaseClassTableError();
        }
        else {
            mDbObjectName = mBaseClass->GetDbObjectName();
            mOwner = mBaseClass->GetOwner();
            mDatabase = mBaseClass->GetDatabase();
            FdoSmLpDbObjectP lpObject = mBaseClass->GetDbObject();
            if ( lpObject )
                mPhDbObject = lpObject->GetDbObject();
        }
    }
    else {
        if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {

            if ( mTableMapping != FdoSmOvTableMappingType_BaseTable ) {
                if ( mDbObjectName == L"" ) 
                    SetDbObjectName( L"" /* cause generation of unique name*/ );

                // Skip table name creation only if already exists and was 
                // specified through an override. 
                FdoSmPhDbObjectP dbObject;
                if (pPhysical->GetOwner()->GetHasMetaSchema())
                    dbObject = pPhysical->FindDbObject( mDbObjectName, L"", L"", false );
                else
                    dbObject = pPhysical->FindDbObject( mDbObjectName, mOwner, L"", false );

                if ( dbObject )
                    mDbObjectName = dbObject->GetName();
                else
                    mDbObjectName = pPhysical->GetDcDbObjectName( mDbObjectName );

                if ( mbIsFixedDbObject &&  dbObject ) {
                    // When db Object is fixed, always latch onto db object,
                    // don't create a new one.
                    mPhDbObject = dbObject;
                }
                else if ( mbHasFdoIdentity ) {

                    // No table is created if there are no id properties. Classes without id
	                // are only used as object property types.

                    if ( mRootDbObjectName.GetLength() > 0 ) 
                        // foreign table, so create a view around it.
                        mPhDbObject = NewView( mDbObjectName, mDatabase, mOwner, mRootDbObjectName );
                    else
                        // non-foreign so just create the table.
                        mPhDbObject = NewTable( mDbObjectName, mNewPkeyName );
                   
                    // Indicate that Schema Manager created the class table or 
                    // view on foreign table.
                    mbIsDbObjectCreator = true;
                }
            }
        }
        else {
            // Load the class table

            mPhDbObject = FindPhDbObject();
            if ( mPhDbObject && (mOwner != L"") ) {
                viewObj = mPhDbObject->SmartCast<FdoSmPhView>();
                if ( viewObj && !viewObj->GetRootObject() ) {
                    viewObj->SetRootObject( pPhysical->FindDbObject(mRootDbObjectName, mOwner, mDatabase, false) );
                }
            }
        }
    }

    if ( mPhDbObject )
        mDbObjectName = mPhDbObject->GetName();
}

void FdoSmLpClassBase::FinalizeDbObjects()
{
    mDbObjects = new FdoSmLpDbObjectCollection();

	// Candidate table list
	FdoSmLpDbObjectsP dbObjects = new FdoSmLpDbObjectCollection();

	// Class table is 0 distance away from itself.
	mDbObject->SetPathDist(0);

	// Class table always in tables list
	mDbObjects->Add(mDbObject);
	// Class table always in candidates list
	dbObjects->Add(mDbObject);

	/* Finalize the containing table for each Data Property. The join path between
	 * the containing table and the class table is calculated during finalization.
	 */

	for ( int i = 0; i < mProperties->GetCount(); i++ ) {
		FdoSmLpDataPropertyP pProp =
            mProperties->GetItem(i)->SmartCast<FdoSmLpDataPropertyDefinition>(true);

		if ( pProp ) {
			FdoSmPhDbObjectP phDbObject = pProp->GetContainingDbObject();

			if ( phDbObject ) {
				FdoSmLpDbObjectP lpDbObject = mDbObjects->FindItem(phDbObject->GetName());
				
				if ( !lpDbObject )
					lpDbObject = FinalizeDbObject( phDbObject, dbObjects, 0, pProp );

				// Try to set a default literal for the property if it can't be 
				// joined back to the class table.
				if ( !lpDbObject ||(lpDbObject->GetPathDist() < 0) ) {
					pProp->SetDefaultValue(true);
				}

			}
		}
	}

	// tables for ancestor classes might contain properties so make them candidates.

	FdoSmLpClassDefinitionP pCurrClass = mBaseClass;
	if ( !pCurrClass )
		pCurrClass = GetMetaClass();

	FdoSmLpClassDefinitionP pNextClass;

	// Walk up the class ancestry.

	while ( pCurrClass ) {
		FdoSmLpDbObjectP pBaseLpDbObject = pCurrClass->GetDbObject();
		
		if ( pBaseLpDbObject ) {
			FdoSmPhDbObjectP basePhDbObject = pBaseLpDbObject->GetDbObject();

			if ( basePhDbObject && (!dbObjects->RefItem(basePhDbObject->GetName())) ) {
				// Create a wrapper for the ancestor class table. 
				FdoSmLpDbObjectP dbObject = FinalizeNewDbObject( basePhDbObject, dbObjects, 1, true, NULL );
			}
		}
		
		pNextClass = pCurrClass->GetBaseClass();

		if ( pNextClass ) 
			pCurrClass = pNextClass;
		else
			// Properties can also be in MetaClass tables.
			pCurrClass = pCurrClass->GetMetaClass();
	}
}

FdoSmLpDbObjectP FdoSmLpClassBase::FinalizeDbObject( 
	FdoSmPhDbObjectP pPhDbObject, 
	FdoSmLpDbObjectsP dbObjects, 
	int iLevel,
	const FdoSmLpDataPropertyDefinition* pProp 
)
{
	// Check if this table has already been visited.
	FdoSmLpDbObjectP pLpDbObject = dbObjects->FindItem( pPhDbObject->GetName() );

	if ( !pLpDbObject ) {
		// If not then do the "new" table stuff
		pLpDbObject = FinalizeNewDbObject( pPhDbObject, dbObjects, iLevel, false, pProp );
	}

	if ( iLevel == 0 ) {
		// Level 0 indicates that this is the containing table for a data property
		// Make sure it gets added to the tables list if no there already
		if ( !mDbObjects->RefItem(pLpDbObject->GetName()) ) {
			mDbObjects->Add( pLpDbObject );

			// Check if this table is a class table but not the class table for this class
			// and that it is not a metaclass table
			if ( (pLpDbObject->GetIsClasstable()) && (pLpDbObject != mDbObject) &&
				pProp && 
				(wcscmp( pProp->RefDefiningClass()->RefLogicalPhysicalSchema()->GetName(), FdoSmLpSchema::mMetaClassSchemaName) != 0) ) 
				// If class properties in multiple class tables then 
				// table mapping type is definitely "class".
				SetTableMapping( FdoSmOvTableMappingType_ClassTable );

			// Log an error if there is no join path from this table to the class table.
			// Without a join path, the data properties contained in this table can't be
			// selected.
			if ( (pLpDbObject->GetPathDist() < 0) &&
                (GetElementState() != FdoSchemaElementState_Deleted) ) {
                // There is currently no join between a non-feature class and 
                // F_ClassDefinition, so no error in this case.
//                if ( (GetClassType() != FdoClassType_Class) || 
//                        (FdoStringP("F_CLASSDEFINITION").ICompare(pLpDbObject->GetName()) != 0) )  
//    				AddPathNotFoundError( pLpDbObject->GetName() );
                }
		}

	}

	return( pLpDbObject );
}

FdoSmLpDbObjectP FdoSmLpClassBase::FinalizeNewDbObject( 
	FdoSmPhDbObjectP pPhDbObject, 
	FdoSmLpDbObjectsP dbObjects, 
	int iLevel, 
	bool bClassTable,
	const FdoSmLpDataPropertyDefinition* pProp 
)
{
	// Add this table to the candidates list so we know it has been already 
	// visited.

	FdoSmLpDbObjectP lpDbObject = new FdoSmLpDbObject(pPhDbObject->GetName(), pPhDbObject, mProperties, bClassTable, this);

	dbObjects->Add(lpDbObject);

	// Find the join path back to the class table. There may be a single direct join
	// or we may have to join in intermediate tables.

	// Get all dependencies that reference this table as the Fkey table.
	const FdoSmPhDependencyCollection* pDeps = pPhDbObject->GetDependenciesUp();

	FdoSmLpDbObjectP pPathDbObject;
	const FdoSmPhDependency* pPathDep = NULL;
	// Find the join path with the fewest intermediate tables.
	int iMinDist = 1000000;

	// Check each dependency to see if it leads to a join path
	for ( int i = 0; i < pDeps->GetCount(); i++ ) {
		const FdoSmPhDependency* pDep = pDeps->RefItem(i);

		// Consider only cardinality 1 dependencies because collections of data
		// properties are not supported.
		if ( pDep->GetCardinality() == 1 ) {
			FdoSmLpDbObjectP pCurrDbObject;

			// Get the primary key table and Finalize it.
            FdoSmPhMgrP      pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
            FdoSmPhDbObjectP pDepDbObject;
            if (pPhysical->GetOwner()->GetHasMetaSchema())
			    pDepDbObject = pPhysical->FindDbObject( pDep->GetPkTableName() );
            else
			    pDepDbObject = pPhysical->FindDbObject( pDep->GetPkTableName(), mOwner );

			if ( pDepDbObject ) 
				pCurrDbObject = FinalizeDbObject( pDepDbObject, dbObjects, iLevel + 1, pProp );
			
			// If it was successfully finalized, check if it has a join path to the class table and 
			// it's the shortest path encountered so far.
			if ( pCurrDbObject && (pCurrDbObject->GetPathDist() >= 0) && (pCurrDbObject->GetPathDist() < iMinDist) ) {
				// If so then it is the current candidate for the next table in the 
				// join path.
				iMinDist = pCurrDbObject->GetPathDist();
				pPathDbObject = (pCurrDbObject->GetPathDist() == 0) ? mDbObject : pCurrDbObject;
				pPathDep = pDep;
			}
		}
	}

	if ( pPathDbObject ) {
		// A join path to the class table was found. Create a link between this table
		// and the next table in the path.

		lpDbObject->SetTargetDbObject( pPathDbObject );
		// This table is one join further away from the class table than the next
		// table.
		lpDbObject->SetPathDist( pPathDbObject->GetPathDist() + 1 );

		// Set the join columns from the depencency between the next table and this table.

		// First the columns from this table for joining to the next table.

		FdoSmPhColumnListP sSrcCols = pPathDep->GetFkColumnNames();
		FdoSmPhColumnListP sTargCols = pPathDep->GetPkColumnNames();

		if ( sSrcCols->GetCount() == 0 || ( sSrcCols->GetCount() != sTargCols->GetCount() ) ) {
			if ( GetElementState() != FdoSchemaElementState_Deleted ) 
				AddJoinColCountError( pPathDbObject->GetName(), lpDbObject->GetName() );
			lpDbObject->SetPathDist(-1);
		}

		for ( int i = 0; i < sSrcCols->GetCount(); i++ ) {
			FdoSmPhColumnP pCol = pPhDbObject->GetColumns()->FindItem( sSrcCols->GetString(i) );

			if ( pCol ) {
				lpDbObject->AddSourceColumn( pCol );
			}
			else {
				// Error if dependency column not in this table.
				if ( GetElementState() != FdoSchemaElementState_Deleted ) 
					AddSrcColNotFoundError( pPathDbObject->GetName(), lpDbObject->GetName(), sSrcCols->GetString(i) );
				lpDbObject->SetPathDist(-1);
			}
		}

		// Secondly the columns from the next table for joining to the this table.

		for ( int i = 0; i < sTargCols->GetCount(); i++ ) {
			FdoSmPhColumnP pCol = pPathDbObject->GetDbObject()->GetColumns()->FindItem( sSrcCols->GetString(i) );

			if ( pCol ) {
				lpDbObject->AddTargetColumn( pCol );
			}
			else {
				// Error if dependency column not in next table.
				if ( GetElementState() != FdoSchemaElementState_Deleted ) 
					AddTargColNotFoundError( pPathDbObject->GetName(), lpDbObject->GetName(), sSrcCols->GetString(i) );
				lpDbObject->SetPathDist(-1);
			}
		}
	}
	else {
		// There may not be an explicit dependency to join a class table to this class's
		// class table. In this case, join them using their primary key columns.
        if ( FdoStringP(L"F_FEATURE").ICompare(lpDbObject->GetName()) == 0 ) {
            bool hasIdCols = true;
            for ( int i = 0; i < mIdentityProperties->GetCount(); i++ ) { 
                if ( !(FdoSmLpDataPropertyP(mIdentityProperties->GetItem(i))->GetColumn()) ) {
                    hasIdCols = false;
                    break;
                }
            }

            // Set the join columns only if they all
            // exist.
            if ( hasIdCols ) {

                // When the table is F_Feature then try to join to this class's feature id column.
		        FdoSmLpDataPropertyP pFeatIdProp = GetFeatIdProperty();
    			
		        if ( pFeatIdProp && pFeatIdProp->GetColumn() && ( pFeatIdProp->RefContainingDbObject()->GetQName() == mDbObject->GetDbObject()->GetQName() ) )  {
			        lpDbObject->SetPathDist(1);
			        lpDbObject->SetTargetDbObject(mDbObject);
			        lpDbObject->AddSourceColumn( lpDbObject->GetPkeyColumns()->GetItem(0) );
			        lpDbObject->AddTargetColumn( GetFeatIdProperty()->GetColumn() );
                }
            }
            else {
		        // Otherwise try to join to this class's identity property columns.
		        if ( bClassTable && (mIdentityProperties->GetCount() > 0) && (lpDbObject->GetPkeyColumns()->GetCount() == mIdentityProperties->GetCount()) ) {
			        lpDbObject->SetPathDist(1);
			        lpDbObject->SetTargetDbObject(mDbObject);
			        for ( int i = 0; i < lpDbObject->GetPkeyColumns()->GetCount(); i++ ) 
				        lpDbObject->AddSourceColumn( lpDbObject->GetPkeyColumns()->GetItem(i) );
			        for ( int i = 0; i < mIdentityProperties->GetCount(); i++ ) 
				        lpDbObject->AddTargetColumn( FdoSmLpDataPropertyP(mIdentityProperties->GetItem(i))->GetColumn() );
                }
            }
        }
    }

	return( lpDbObject );
}
/*
FdoSmLpJoinTreeNode* FdoSmLpClassBase::AddJoinTreeNode( FdoSmLpJoinTreeNodeCollection& joinNodes, const FdoSmLpDbObject* pDbObject ) const
{
	// If not already visited then we're done.
	FdoSmLpJoinTreeNode* pNode = joinNodes.RefItem( pTable->GetName() );

	if ( !pNode ) {
		// If not then add it to the candidates list.

		pNode = new FdoSmLpJoinTreeNode( pTable );
		joinNodes.Add(pNode);

		const FdoSmLpDbObject* pTargetTable = pTable->GetTargetTable();

		// If this table has a target table then create a node for the target.
		// Add this node as a source node of the target node ( which builds the tree ).
		if ( pTargetTable ) {
			FdoSmLpJoinTreeNode* pTargetNode = AddJoinTreeNode( joinNodes, pTargetTable );
			pTargetNode->GetSourceNodes()->Add(pNode);
			pTargetNode->Release();
		}
	}
	else {
		pNode->AddRef();
	}

	return(pNode);
}
*/
FdoSmLpPropertyP FdoSmLpClassBase::MatchInheritedProp( const FdoSmLpPropertyDefinition* pBaseProp, FdoSmLpPropertiesP pProps )
{
	FdoSmLpPropertyP                pInheritedProp;
    bool                            baseIsFeatId = false;

	// FeatId base properties not always matched by name
	if ( pBaseProp->GetIsFeatId() && (pProps == mProperties) ) {
        baseIsFeatId = true;

		// First check id properties. If there is a single int64 id property,
        // with autogenerated values,
		// then it becomes the FeatId property for this class, and inherits
		// from the FeatId base property.
		if ( GetIdentityProperties()->GetCount() == 1 ) {
			FdoSmLpDataPropertyP pIdProp = GetIdentityProperties()->GetItem(0);
		
			if ((pIdProp->GetDataType() == FdoDataType_Int64) && pIdProp->GetIsAutoGenerated() )
				pInheritedProp = pIdProp.p->SmartCast<FdoSmLpPropertyDefinition>();
		}

		if ( pInheritedProp == NULL ) {
			// Otherwise, if this class has a FeatId property match to it.
			// This can happen when the class is loaded from the current datastore.
			try {
				pInheritedProp = FdoSmLpDataPropertyDefinitionCollection::FindFeatIDProperty( mProperties ).p->SmartCast<FdoSmLpPropertyDefinition>();
			}
			catch ( ... )
			{
			}
		}
	}

	// Not FeatID property or not yet matched, match by name.
	if ( pInheritedProp == NULL ) 
		pInheritedProp = pProps->FindItem( pBaseProp->GetName() );	

    // Can't add the FeatId system column to a foreign table view since FDO
    // cannot alter a foreign table.
//    if ( (pInheritedProp == NULL) && baseIsFeatId && pBaseProp->GetIsSystem() && (mOwner.GetLength() > 0) )
//        AddForeignNoFeatIdError( pBaseProp );

	return( pInheritedProp );
}

bool FdoSmLpClassBase::ColumnNameUsed( FdoSmPhDbObjectP dbObject, const FdoSmLpPropertyDefinition* pProp, FdoString* columnName )
{
	bool bUsed = true;

	// Check if a property already uses this column name.
	const FdoSmLpSimplePropertyDefinition* pMatchedProp = 		 
		FdoSmLpSimplePropertyDefinition::ColName2Property( mProperties, columnName );

	// It's alright if it is already used by the property that will own the column. 
	if ( pMatchedProp && pProp && (wcscmp(pMatchedProp->GetName(), pProp->GetName()) == 0) )
			pMatchedProp = NULL;

	if ( !pMatchedProp ) {
		// If we are still finalizing then we have to check the base or metaclass properties
		// to see if any of them use this column name.
		if ( GetState() != FdoSmObjectState_Final ) {
			if ( mBaseClass ) {
				pMatchedProp = 		 
					FdoSmLpSimplePropertyDefinition::ColName2Property( mBaseClass->GetProperties(), columnName );
			}
			else {
				if ( GetMetaClass() ) 
					pMatchedProp = 		 
						FdoSmLpSimplePropertyDefinition::ColName2Property( GetMetaClass()->GetProperties(), columnName );
			}

			// It's alright to use this column name if the base property is this property.
            if ( pMatchedProp && pProp && 
                    ((wcscmp(pMatchedProp->GetName(), pProp->GetName()) == 0) ||
                     (pMatchedProp->GetIsFeatId() && pProp->GetIsFeatId())       // Derived FeatId property doesn't
                                                                                 // always have same name so match
                                                                                 // on IsFeatId flag.
                )
            )
				pMatchedProp = NULL;
		}

		if ( !pMatchedProp ) {
			// Final check to be sure. If the column has a table, make sure it is not 
			// already in the table.
			if ( (!dbObject) || (!dbObject->GetColumns()->RefItem( columnName )) )
				// Name not in use
				bUsed = false;
		}
	}

	return(bUsed);
}

void  FdoSmLpClassBase::BreakObjPropTargets( 
	FdoSmLpPropertiesP pProps
)
{
    // The following loop breaks a circular reference between the class 
	// table and object properties, so that these objects can be deleted.
	for ( int i = 0; i < pProps->GetCount(); i++ ) {
		const FdoSmLpObjectPropertyDefinition* pProp = 
			FdoSmLpObjectPropertyDefinition::Cast(pProps->RefItem(i));

		// Skip non-finalized properties. There is no circular reference when the 
		// object property is not final. We also don't want to cause it to finalize.
		if ( pProp && (pProp->GetState() ==  FdoSmObjectState_Final) ) {
			const FdoSmLpClassDefinition* pClass = pProp->RefTargetClass();

			if ( pClass && (pClass->GetState() ==  FdoSmObjectState_Final) ) {
				FdoSmLpDbObjectP pLpDbObject = ((FdoSmLpClassDefinition*)pClass)->GetDbObject();

				if ( pLpDbObject ) 
					pLpDbObject->SetTargetDbObject((FdoSmLpDbObject*) NULL);
			}
		}
	}
}

void FdoSmLpClassBase::AddBaseClassLoopError( const FdoSmLpClassDefinition* pBaseClass )
{
	GetErrors()->Add( FdoSmErrorType_BaseClassLoop, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
        		FDO_NLSID(FDOSM_130),
		    	(FdoString*) pBaseClass->GetQName(), 
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddBaseClassMissingError()
{
	GetErrors()->Add( FdoSmErrorType_BaseClassMissing, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_131),
				(FdoString*) mBaseClassName, 
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddBaseClassTypeError( const FdoSmLpClassDefinition* pBaseClass )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_233),
				(FdoString*) GetQName(),
				(FdoString*) pBaseClass->GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddBaseNoSchemaError( FdoString* baseClassName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_236),
				baseClassName,
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddBaseClassDeleteError( const FdoSmLpClassDefinition* pBaseClass )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_132),
				(FdoString*) pBaseClass->GetQName(), 
				(FdoString*) GetQName()
			)
		)
	);

	if ( GetElementState() == FdoSchemaElementState_Unchanged )
		FdoSmSchemaElement::SetElementState(FdoSchemaElementState_Modified);
}


void FdoSmLpClassBase::AddDuplicateIdPosnError( int position )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_133),
				(FdoString*) GetQName(),
				position
			)
		)
	);
}

void FdoSmLpClassBase::AddIdMissingError( const FdoSmPhColumn* pColumn)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_134),
				(FdoString*) GetQName(), 
				(FdoString*) pColumn->GetName()
			)
		)
	);
}

void FdoSmLpClassBase::AddIdDifferentFromBaseError( void )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_135),
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddJoinColCountError( FdoStringP pkTableName, FdoStringP fkTableName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_136),
    			(FdoString*) pkTableName,
	    		(FdoString*) fkTableName
			)
		)
	);
}

void FdoSmLpClassBase::AddSrcColNotFoundError( FdoStringP pkTableName, FdoStringP fkTableName, FdoStringP colName )
{
	GetErrors()->Add( FdoSmErrorType_ColumnMissing, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_137),
				(FdoString*) pkTableName,
				(FdoString*) fkTableName,
				(FdoString*) colName
			)
		)
	);
}

void FdoSmLpClassBase::AddTargColNotFoundError( FdoStringP pkTableName, FdoStringP fkTableName, FdoStringP colName )
{
	GetErrors()->Add( FdoSmErrorType_ColumnMissing, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_138),
				(FdoString*) pkTableName,
				(FdoString*) fkTableName,
				(FdoString*) colName
			)
		)
	);
}

void FdoSmLpClassBase::AddPathNotFoundError( FdoStringP tableName )
{
	GetErrors()->Add( FdoSmErrorType_NoJoinPath, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_139),
				(FdoString*) tableName,
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddClassTypeChangeError( FdoClassType newType )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_140),
				(FdoString*) GetQName(),
                (FdoString*) FdoSmLpClassTypeMapper::Type2String( GetClassType() ),
				(FdoString*) FdoSmLpClassTypeMapper::Type2String( newType )
			)
		)
	);
}

void FdoSmLpClassBase::AddBaseClassChangeError( FdoStringP newBaseClassName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_141),
				(FdoString*) GetQName(), 
				(FdoString*) mBaseClassName, 
				(FdoString*) newBaseClassName
			)
		)
	);
}

void FdoSmLpClassBase::AddAbstractChangeError()
{
	if ( GetIsAbstract() ) {
		GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_142),
				    (FdoString*)(GetQName())
			    )
            )
		);
	}
	else {
		GetErrors()->Add( FdoSmErrorType_Other, 
            FdoSchemaException::Create(
                FdoSmError::NLSGetMessage(
				    FDO_NLSID(FDOSM_143),
				    (FdoString*)(GetQName())
			    )
            )
		);
	}
}
void FdoSmLpClassBase::AddPropExistsError( FdoString* propName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_144),
				propName,
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpClassBase::AddPropNotExistsError( FdoString* propName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_145),
				(FdoString*)(GetQName()),
				propName
			)
		)
	);
}

void FdoSmLpClassBase::AddGeomPropError( FdoString* propName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_234),
				propName,
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpClassBase::AddFdoDbIdMismatchError( FdoStringsP& fdoIds, FdoStringsP& dbIds )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_146),
				(FdoString*)(GetQName()),
				(FdoString*)(fdoIds->ToString()),
				(FdoString*)(dbIds->ToString())
			)
		)
	);
}

void FdoSmLpClassBase::AddModIdPropsError( FdoStringsP& fdoIds, FdoStringsP& dbIds )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_147),
				(FdoString*)(GetQName()),
				(FdoString*)(dbIds->ToString()),
				(FdoString*)(fdoIds->ToString())
			)
		)
	);
}

void FdoSmLpClassBase::AddGenericError( FdoString* specificError )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_186),
				(FdoString*)(GetQName()),
				specificError
			)
		)
	);
}

void FdoSmLpClassBase::AddNullIdPropError(const FdoSmLpDataPropertyDefinition* pIdProp)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_264),
				(FdoString*)(pIdProp->GetQName())
			)
		)
	);
}

void FdoSmLpClassBase::AddReadOnlyIdPropError(const FdoSmLpDataPropertyDefinition* pIdProp)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_267),
				(FdoString*)(pIdProp->GetQName())
			)
		)
	);
}

void FdoSmLpClassBase::AddBaseClassTableError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_307),
                (FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpClassBase::AddTableCharError( FdoString* tableName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_313),
                (FdoString*) GetQName(),
				tableName
			)
		)
	);
}

void FdoSmLpClassBase::AddTableLengthError( FdoString* tableName, FdoSize tableMaxSize )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_314),
                (FdoString*) GetQName(),
                tableMaxSize,
				tableName
			)
		)
	);
}

void FdoSmLpClassBase::AddTableReservedError( FdoString* tableName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_315),
                (FdoString*) GetQName(),
				tableName
			)
		)
	);
}

void FdoSmLpClassBase::AddColCharError( 
    const FdoSmLpPropertyDefinition* pProp, 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_302),
                pProp ? (FdoString*)(pProp->GetQName()) : L"{none}",
				columnName
			)
		)
	);
}

void FdoSmLpClassBase::AddColLengthError( 
    const FdoSmLpPropertyDefinition* pProp, 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_303),
                pProp ? (FdoString*)(pProp->GetQName()) : L"{none}",
                GetLogicalPhysicalSchema()->GetPhysicalSchema()->DbObjectNameMaxLen(),
				columnName
			)
		)
	);
}

void FdoSmLpClassBase::AddColReservedError( 
    const FdoSmLpPropertyDefinition* pProp, 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_304),
                pProp ? (FdoString*)(pProp->GetQName()) : L"{none}",
				columnName
			)
		)
	);
}

void FdoSmLpClassBase::AddColUsedError( 
    const FdoSmLpPropertyDefinition* pProp, 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_305),
				columnName,
                (FdoString*)(pProp->GetQName())
			)
		)
	);
}

void FdoSmLpClassBase::AddTableNameChangeError( 
    FdoString* newTableName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_306),
				(FdoString*)GetQName(),
                GetDbObjectName(),
                newTableName
			)
		)
	);
}

void FdoSmLpClassBase::AddForeignNoFeatIdError( const FdoSmLpPropertyDefinition* pFeatId )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_405),
				(FdoString*)GetQName(),
                pFeatId->GetName()
			)
		)
	);
}

void FdoSmLpClassBase::XMLSerialize( FILE* xmlFp, int ref  ) const
{
	if ( ref == 0 ) {
		fprintf( xmlFp, "<class xsi:type=\"%ls\" name=\"%ls\" description=\"%ls\"\n baseClass=\"%ls\" abstract=\"%s\"\n tableName=\"%ls\" tblCreator=\"%s\" fixedTbl=\"%s\" >\n",
                (FdoString*) FdoSmLpClassTypeMapper::Type2String(GetClassType()), 
				GetName(), GetDescription(),
				RefBaseClass() ? RefBaseClass()->GetName() : L"", 
				GetIsAbstract() ? "True" : "False",
				GetDbObjectName(),
                IsDbObjectCreator() ? "True" : "False",
                GetIsFixedDbObject() ? "True" : "False"
		);

		XMLSerializeSubElements( xmlFp, ref );

        FdoStringP tableMapping;

        switch ( this->GetTableMapping() ) {
        case FdoSmOvTableMappingType_BaseTable:
            tableMapping = L"Base";
            break;
        case FdoSmOvTableMappingType_ConcreteTable:
            tableMapping = L"Concrete";
            break;
        }

		fprintf( xmlFp, "<tableMapping>%ls</tableMapping>\n",
            (FdoString*) tableMapping
        );

		fprintf( xmlFp, "<identityProperties>\n" );
		for ( int i = 0; i < RefIdentityProperties()->GetCount(); i++ )
			RefIdentityProperties()->RefItem(i)->XMLSerialize(xmlFp, 1);
		fprintf( xmlFp, "</identityProperties>\n" );

		fprintf( xmlFp, "<properties>\n" );
		for ( int i = 0; i < RefProperties()->GetCount(); i++ )
			RefProperties()->RefItem(i)->XMLSerialize(xmlFp, ref);
		fprintf( xmlFp, "</properties>\n" );
/*
        if ( mIndexes.GetCount() > 0 ) {
		    fprintf( xmlFp, "<indexes>\n" );
		    for ( int i = 0; i < mIndexes.GetCount(); i++ )
			    mIndexes.RefItem(i)->XMLSerialize(xmlFp, ref);
		    fprintf( xmlFp, "</indexes>\n" );
        }
*/
		if ( RefDbObject() ) 
			RefDbObject()->XMLSerialize(xmlFp, ref);

		if ( RefDbObjects() ) {
			fprintf( xmlFp, "<tables>\n" );

			for ( int i = 0; i < RefDbObjects()->GetCount(); i++ )
				RefDbObjects()->RefItem(i)->XMLSerialize(xmlFp,1);

			fprintf( xmlFp, "</tables>\n" );
		}

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );

		fprintf( xmlFp, "</class>\n" );
	}
	else {
		fprintf( xmlFp, "<class xsi:type=\"%ls\" name=\"%ls\" />\n",
                (FdoString*) FdoSmLpClassTypeMapper::Type2String(GetClassType()), 
				GetName() 
		);
	}

}

FdoSmLpClassBase::Capabilities::Capabilities( 
    const FdoSmLpClassBase* pClass
) :
    mSupportsLocking(false),
    mSupportsLongTransactions(false),
    mLockTypes(NULL),
    mLockTypeCount(0)
{
    const FdoSmLpDbObject* pLpDbObject = pClass->RefDbObject();

    // Determine class capabilities from class database object
    if ( pLpDbObject ) {
        const FdoSmPhDbObject* pPhDbObject = pLpDbObject->RefDbObject();

        if ( pPhDbObject ) {
            mSupportsLocking = (pPhDbObject->GetLockingMode() != NoLtLock);
            mSupportsLongTransactions = (pPhDbObject->GetLtMode() != NoLtLock);

            const FdoLockType* lockTypes = pPhDbObject->GetLockTypes( mLockTypeCount );
            if ( mLockTypeCount > 0 ) {
                mLockTypes = new FdoLockType[mLockTypeCount];
                memcpy( mLockTypes, lockTypes, mLockTypeCount*sizeof(FdoLockType) );
            }
        }
    }
}

FdoSmLpClassBase::Capabilities::~Capabilities()
{
    if (mLockTypes)
        delete [] mLockTypes;
}

bool FdoSmLpClassBase::Capabilities::SupportsLocking() const
{
    return mSupportsLocking;
}

bool FdoSmLpClassBase::Capabilities::SupportsLongTransactions() const
{
    return mSupportsLongTransactions;
}

const FdoLockType* FdoSmLpClassBase::Capabilities::GetLockTypes(FdoInt32& size) const
{
    size = mLockTypeCount;
	return mLockTypes;
}

FdoSmLpClassDefinition::FdoSmLpClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
	FdoSmLpClassBase(classReader, parent)
{
	// Load the class properties 

	FdoSmPhClassPropertyReaderP propReader = classReader->CreateClassPropertyReader();

	while ( propReader->ReadNext() ) {
		FdoSmLpPropertyP newProp = CreateProperty( propReader );
        if (newProp) {
		    FdoStringP propName = newProp->GetName();

		    // Separate class properties from nested properties.
		    // Nested properties are sub-properties of the class's
		    // object properties.
		    if ( propName.Contains( L"." ) )
			    GetNestedProperties()->Add( newProp );
		    else
			    GetProperties()->Add( newProp );
        }
	}

    // The reader above will have quietly skipped any recognised ordinate columns 
    // that were expected to go into geometric properties.  The reader cannot return
    // property attributes for the two or three ordinate columns at once.  So, we manually
    // search for them here.
    FdoSmPhDbObjectP obj = this->FindPhDbObject();
    bool geomFromOrdsWanted = obj==NULL ? false : obj->GetManager()->IsGeometryFromOrdinatesWanted();
    if (geomFromOrdsWanted) {
        FdoSmPhColumnsP columns = obj->GetColumns();
        FdoSmPhColumnP preExisting = columns->FindItem(DefaultGeometricPropertyName);
        if (preExisting != NULL)
            geomFromOrdsWanted = false;
    }
    if (geomFromOrdsWanted) {
        FdoSmLpSchemaP schema = GetLogicalPhysicalSchema();
        FdoSmPhColumnsP columns = obj->GetColumns();
        FdoSmPhColumnP columnX = columns->FindItem(L"X");
        if (columnX == NULL)
            columnX = columns->FindItem(L"x");
        if (columnX != NULL) {
            FdoSmPhColumnP columnY = columns->FindItem(L"Y");
            if (columnY == NULL)
                columnY = columns->FindItem(L"y");
            FdoSmPhColumnP columnZ = columns->FindItem(L"Z");
            if (columnZ == NULL)
                columnZ = columns->FindItem(L"z");
            if (columnY != NULL) {
                FdoGeometricPropertyP gpd = FdoGeometricPropertyDefinition::Create(L"Geometry", L"");
                gpd->SetHasMeasure(false);
                gpd->SetHasElevation((bool)(columnZ != NULL));
				gpd->SetSpatialContextAssociation(L"Default");
                FdoSmLpPropertyP newProp = schema->CreateGeometricProperty(
                    gpd, columnX->GetName(), columnY->GetName(), columnZ == NULL ? L"" : columnZ->GetName(), true, this );
                GetProperties()->Add( newProp );
            }
        }
    }

/*
    // Load the indexes

    FdoSmPhClassIndexReaderP pIndexReader = classReader->GetClassIndexReader();

    // Prime the index readers with one read.
    pIndexReader->ReadNext();

    while ( !pIndexReader->IsEOF() ) {
        FdoSmPhIndexPropertyReaderP pIndexPropReader = pIndexReader->GetPropertyReader();

        FdoSmLpIndex* pIndex = new FdoSmLpIndex( 
            pIndexReader,
            this
        );

        while ( pIndexPropReader->ReadNext() ) {
            pIndex->AddProperty( pIndexPropReader->GetAttributeName() );
        }

        mIndexes.Add(pIndex);
        pIndex->Release();

        // pIndexPropReader->ReadNext() also positions at the next index so 
        // no need to call pIndexReader->ReadNext() here.
    }

	// Load the Schema Attribute Dictionary
*/
	FdoSmPhClassSADReaderP pSADReader = classReader->GetClassSADReader();
	LoadSAD(pSADReader);

}

FdoSmLpClassDefinition::FdoSmLpClassDefinition(
    FdoClassDefinition* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
    FdoSmLpClassBase( pFdoClass, bIgnoreStates, parent )
{
}

FdoSmLpClassDefinition::FdoSmLpClassDefinition(
    FdoString* name,
    FdoString* description,
    FdoSmLpSchemaElement* parent,
    FdoSmLpClassDefinitionP pSrcClass,
    FdoSmOvTableMappingType tableMapping,
    FdoString* tableName,
    FdoSmPhDbObjectP table,
    FdoSchemaElementState elementState
) : 
	FdoSmLpClassBase(name, description, parent, pSrcClass, tableMapping, tableName, table, elementState, parent->GetIsFromFdo())
{
}

FdoSmLpPropertyP FdoSmLpClassDefinition::CreateProperty(FdoSmPhClassPropertyReaderP propReader)
{
    FdoSmLpSchemaP schema = GetLogicalPhysicalSchema();
    bool bFound; 

    FdoStringP dataTypeString = propReader->GetDataType(); 
    FdoStringP colTypeString = propReader->GetColumnType();

	// Check if the property type is a Data type.
	FdoDataType dataType = FdoSmLpDataTypeMapper::String2Type( dataTypeString, &bFound );
		
	if ( bFound ) {
		// It is, so this is a data property
		return schema->CreateDataProperty( propReader, this );
	}
	else {
		// Otherwise, check if it is a Geometry type.
		if ( dataTypeString.IsNumber() )
			return schema->CreateGeometricProperty( propReader, this );
        else if( colTypeString.ICompare(L"Association") == 0 )
			return schema->CreateAssociationProperty( propReader, this );
        else
			return schema->CreateObjectProperty( propReader, this );
	}

    return (FdoSmLpPropertyDefinition*) NULL;
}

FdoSmLpPropertyP FdoSmLpClassDefinition::CreateProperty(
    FdoPropertyDefinition* pFdoProp, 
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates,
    FdoSchemaElementState elementState
)
{
    FdoSmLpSchemaP schema = GetLogicalPhysicalSchema();
    FdoSmLpPropertyP lpProp;

    switch ( pFdoProp->GetPropertyType() ) {
    case FdoPropertyType_AssociationProperty:
        lpProp = schema->CreateAssociationProperty( 
            (FdoAssociationPropertyDefinition*) pFdoProp,
            bIgnoreStates,
            this
        );
        break;

    case FdoPropertyType_DataProperty:
        lpProp = schema->CreateDataProperty( 
            (FdoDataPropertyDefinition*) pFdoProp,
            bIgnoreStates,
            this
        );
        break;

    case FdoPropertyType_GeometricProperty:
        lpProp = schema->CreateGeometricProperty( 
            (FdoGeometricPropertyDefinition*) pFdoProp,
            bIgnoreStates,
            this
        );
        break;

    case FdoPropertyType_ObjectProperty:
        lpProp = schema->CreateObjectProperty( 
            (FdoObjectPropertyDefinition*) pFdoProp,
            bIgnoreStates,
            this
        );
        break;
    }

    lpProp->Update( pFdoProp, elementState, pClassOverrides, bIgnoreStates );

    return lpProp;
}

