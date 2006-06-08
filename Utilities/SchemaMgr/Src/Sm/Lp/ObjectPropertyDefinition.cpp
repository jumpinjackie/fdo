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
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Ph/DependencyCollection.h>
#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/DependencyWriter.h>
#include <Sm/Error.h>

FdoSmLpObjectPropertyDefinition::FdoSmLpObjectPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader, 
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpPropertyDefinition(propReader, parent),
	mObjectType(FdoObjectType_Value),
	mOrderType(FdoOrderType_Ascending),
	mClassName( propReader->GetDataType() ),
	mpClass(NULL),
    mTableMapping(FdoSmLpPropertyMappingType_Concrete),
	mpDependency(NULL),
    mbFixedDbObject(propReader->GetIsFixedColumn()),
    mbDbObjectCreator(propReader->GetIsColumnCreator()),
    mRootDbObjectName(propReader->GetRootObjectName())
{
}

FdoSmLpObjectPropertyDefinition::FdoSmLpObjectPropertyDefinition(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpPropertyDefinition(pFdoProp, bIgnoreStates, parent),
	mObjectType(pFdoProp->GetObjectType()),
	mOrderType(pFdoProp->GetOrderType()),
	mpClass(NULL),
    mTableMapping(FdoSmLpPropertyMappingType_Concrete),
	mpDependency(NULL),
    mbFixedDbObject(false),
    mbDbObjectCreator(false)
{
}

FdoSmLpObjectPropertyDefinition::FdoSmLpObjectPropertyDefinition(
    FdoPtr<FdoSmLpObjectPropertyDefinition> pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpPropertyDefinition(
        pBaseProperty.p->SmartCast<FdoSmLpPropertyDefinition>(), 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    ),
	mObjectType(pBaseProperty->GetObjectType()),
	mOrderType(pBaseProperty->GetOrderType()),
	mClassName( pBaseProperty->GetFeatureClassName() ),
	mpClass( pBaseProperty->GetClass() ),
	mIdentityPropertyName(pBaseProperty->GetIdentityPropertyName() ),
	mpDependency(NULL),
    mbFixedDbObject(false),
    mbDbObjectCreator(false)
{
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhDbObjectP pPhDbObject = pPhysical->FindDbObject(pTargetClass->GetDbObjectName());

    if ( FdoSmLpPropertyMappingSingle::Cast(pBaseProperty->RefMappingDefinition()) )
        mTableMapping = FdoSmLpPropertyMappingType_Single;
    else
        mTableMapping = FdoSmLpPropertyMappingType_Concrete;

    // When creating from a nested property, copy the fixedtable and tablecreator
    // flags from the nested property. This ensures these flags are properly set
    // for existing object properties that are sub-properties of other object
    // properties.
    if ( bInherit || FdoStringP(pBaseProperty->GetName()).Contains(L".") ) {
        mbFixedDbObject = pBaseProperty->GetIsFixedDbObject();
        if ( !bInherit) 
            mbDbObjectCreator = pBaseProperty->GetIsDbObjectCreator();
    }

	switch ( GetElementState() ) {
    case FdoSchemaElementState_Added:
        break;

    default:
        // Object properties with Single Table are always in the containing
        // class's table. 
        if ( mTableMapping == FdoSmLpPropertyMappingType_Single )
            SetContainingDbObject( pPhDbObject, pTargetClass->GetDbObjectName() );

	}
}

FdoSmLpObjectPropertyDefinition::~FdoSmLpObjectPropertyDefinition(void)
{
}


void FdoSmLpObjectPropertyDefinition::SetInherited( const FdoSmLpPropertyDefinition* pBaseProp )
{
	bool		same = true;

    FdoSmLpPropertyDefinition::SetInherited( pBaseProp );
  	
    Finalize();

	if ( (pBaseProp->GetElementState() != FdoSchemaElementState_Deleted) &&
		 (GetElementState() != FdoSchemaElementState_Deleted) ) {

		const FdoSmLpObjectPropertyDefinition* pBaseObjProp = 
			FdoSmLpObjectPropertyDefinition::Cast( pBaseProp );

		if ( pBaseObjProp ) {

			// Inherited property can't redefine the base property. Check if it
			// redefines it by checking if any characteristics are different.
			if ( (wcscmp(mClassName, pBaseObjProp->GetFeatureClassName()) != 0) ||
				 (wcscmp(mIdentityPropertyName, pBaseObjProp->GetIdentityPropertyName()) != 0) ||
				 (mObjectType != pBaseObjProp->GetObjectType()) )
				same = false;

            // Also check if the identity property specified by FDO is 
            // different from the current one ( identity property change
            // attempted ).
            if ( (GetElementState() == FdoSchemaElementState_Added) &&
				 (wcscmp(mFdoIdPropName, pBaseObjProp->GetIdentityPropertyName()) != 0) )
				same = false;

			if ( same && (mObjectType != FdoObjectType_Value) &&
					(GetOrderType() != pBaseObjProp->GetOrderType()) )
				same = false;

			// Inherited property redefines base, log an error
			if ( !same ) 
				AddRedefinedError( pBaseObjProp );
		}		 
	}
}

/*
FdoSmLpPropertyDefinition* FdoSmLpObjectPropertyDefinition::CreateInherited( FdoSmLpClassDefinition* pSubClass ) const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	return(new FdoSmLpObjectPropertyDefinition( (FdoSmLpObjectPropertyDefinition*) this, pSubClass, L"", L"", true ) );
}

FdoSmLpPropertyDefinition* FdoSmLpObjectPropertyDefinition::CreateCopy( 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName,
    FdoOracleOvPropertyDefinition* pPropOverrides
) const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	return(
        new FdoSmLpObjectPropertyDefinition( 
            (FdoSmLpObjectPropertyDefinition*) this, 
            pTargetClass, 
            logicalName, 
            physicalName, 
            false,
            pPropOverrides
        ) 
    );
}
*/
FdoPropertyType FdoSmLpObjectPropertyDefinition::GetPropertyType() const
{
	return FdoPropertyType_ObjectProperty;
}

FdoObjectType FdoSmLpObjectPropertyDefinition::GetObjectType() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

    return mObjectType;
}

FdoOrderType FdoSmLpObjectPropertyDefinition::GetOrderType() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

    return mOrderType;
}

FdoString* FdoSmLpObjectPropertyDefinition::GetFeatureClassName() const
{
	return mClassName;
}

const FdoSmLpClassDefinition* FdoSmLpObjectPropertyDefinition::RefClass() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	return mpClass;
}

FdoSmLpClassDefinitionP FdoSmLpObjectPropertyDefinition::GetClass()
{
	return FDO_SAFE_ADDREF(mpClass);
}

FdoStringP FdoSmLpObjectPropertyDefinition::GetIdentityPropertyName() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

    return mIdentityPropertyName;
}

const FdoSmLpDataPropertyDefinition* FdoSmLpObjectPropertyDefinition::RefIdentityProperty() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	return (FdoSmLpDataPropertyDefinition*) mpIdentityProperty;
}

FdoSmLpDataPropertyP FdoSmLpObjectPropertyDefinition::GetIdentityProperty()
{
	return(mpIdentityProperty);
}

void FdoSmLpObjectPropertyDefinition::SetIdentityProperty( FdoSmLpDataPropertyP idProp )
{
    mpIdentityProperty = idProp;
}

void FdoSmLpObjectPropertyDefinition::SetIdentityPropertyName( FdoString* idPropName )
{
    mIdentityPropertyName = idPropName;
}

void FdoSmLpObjectPropertyDefinition::SetMappingDefinition( FdoSmLpPropertyMappingP mappingDefinition )
{
    mpMappingDefinition = mappingDefinition;
}

void FdoSmLpObjectPropertyDefinition::SetTableMapping( FdoSmLpPropertyMappingType tableMapping )
{
    mTableMapping = tableMapping;
}

const FdoSmLpPropertyMappingDefinition* FdoSmLpObjectPropertyDefinition::RefMappingDefinition() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	return(mpMappingDefinition);
}

const FdoSmLpClassDefinition* FdoSmLpObjectPropertyDefinition::RefTargetClass() const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	return( mpMappingDefinition ? mpMappingDefinition->RefTargetClass() : NULL );
}

const FdoSmLpClassDefinition* FdoSmLpObjectPropertyDefinition::RefPkClass() const
{
    return ((FdoSmLpObjectPropertyDefinition*) this)->GetPkClass();
}

FdoSmLpClassDefinitionP FdoSmLpObjectPropertyDefinition::GetPkClass()
{
    // Class containing primary key table usually the containing class.
    FdoSmLpClassDefinitionP pPkClass = FDO_SAFE_ADDREF( (FdoSmLpClassDefinition*) RefParentClass());

    FdoSmLpObjectPropertyClass* pOpClass = 
        dynamic_cast<FdoSmLpObjectPropertyClass*>((FdoSmLpClassDefinition*) pPkClass);

    if ( pOpClass ) {
        // Containing class is an object property class. If it's property uses single 
        // table mapping that primary key table is the lowest container of the containing
        // class, that does not use single table mapping.
        FdoSmLpObjectPropertyP pObProp = 
            pOpClass->GetObjectProperty();

        if ( pObProp ) {
            const FdoSmLpPropertyMappingSingle* pMapping =
                FdoSmLpPropertyMappingSingle::Cast( pObProp->RefMappingDefinition() );

            if ( pMapping ) 
                // Single table mapping used so continue walking up to find the primary key
                // class.
                return( pObProp->GetPkClass() );
        }
    }

    return( pPkClass );
}
	
FdoStringP FdoSmLpObjectPropertyDefinition::GetDefaultDbObjectName() const
{
    return mDefaultDbObjectName;
}

FdoStringP FdoSmLpObjectPropertyDefinition::GetRootDbObjectName() const
{
    return mRootDbObjectName;
}

void FdoSmLpObjectPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
	FdoSmLpPropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

	// Superclass Update logs property type conflict so just silently skip validation
	// at this level.
	if ( pFdoProp->GetPropertyType() == FdoPropertyType_ObjectProperty ) {
		FdoObjectPropertyDefinition*		pFdoObjProp = 
			(FdoObjectPropertyDefinition*) pFdoProp;

		FdoStringP					        fdoTypeClassName;
		FdoPtr<FdoClassDefinition>			pFdoTypeClass = pFdoObjProp->GetClass();
		FdoPtr<FdoDataPropertyDefinition>	pFdoIdProp = pFdoObjProp->GetIdentityProperty();;

		// Changing local id not yet supported.
		// Since current identity property is on the type class, it is 
		// not known until Finalize() is called. Therefore, just save away
		// the new identity property name for validation during Finalize().
		if ( (FdoDataPropertyDefinition*) pFdoIdProp )
			mFdoIdPropName =  pFdoIdProp->GetName();

		if ( (FdoClassDefinition*) pFdoTypeClass ) {
			FdoFeatureSchemaP pTypeSchema = pFdoTypeClass->GetFeatureSchema();
			if ( (FdoFeatureSchema*) pTypeSchema ) {
				fdoTypeClassName = FdoStringP( FdoFeatureSchemaP( pTypeSchema )->GetName() ) + L":" +
							pFdoTypeClass->GetName();
				}
			else {
				AddTypeNoSchemaError( pFdoTypeClass->GetName() );
				fdoTypeClassName = pFdoTypeClass->GetName();
			}
		}

        // Single property mapping allowed only for Value properties
        if ( (mTableMapping == FdoSmLpPropertyMappingType_Single) &&
             (pFdoObjProp->GetObjectType() != FdoObjectType_Value) )
             AddSingleCollectionError();

        if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
			// Ref the local id property.
			mIdentityPropertyName = mFdoIdPropName;

			// Ref the name of the class defining the object property type.
			mClassName = fdoTypeClassName;

			// Value properties cannot have local identity properties
			if ( (pFdoObjProp->GetObjectType() == FdoObjectType_Value) && pFdoIdProp ) 
				AddValueHasIdError();
        }
        else if ( GetElementState() == FdoSchemaElementState_Modified ) {

			// Changing type class not yet supported.
			if ( wcscmp ( mClassName, fdoTypeClassName ) != 0 ) {
				AddTypeClassChangeError( fdoTypeClassName );
			}

            // Some validations cannot be done until this property is finalized.
            // Save away the fdo definition for validation
            mFdoProp = FDO_SAFE_ADDREF(pFdoObjProp);
		}
	}
}

void FdoSmLpObjectPropertyDefinition::SynchPhysical(bool bRollbackOnly)
{	
	Finalize();

    // Proceed only if there is a target class and this is not an inherited property.
    // Inherited object properties have the same table as their base property, so the 
    // table is synchronized when the base property is synchronized.
	if ( mpMappingDefinition && (GetParent() == RefDefiningClass()) )
		((FdoSmLpClassDefinition*)mpMappingDefinition->RefTargetClass())->SynchPhysical(bRollbackOnly);
}
   
void FdoSmLpObjectPropertyDefinition::Commit( bool fromParent )
{
	// FdoSmLpPropertyDefinition::Commit is not called, since this
	// function handles the delete case. It needs to use the classid for 
	// the top class to delete or modify the F_AttributeDefinition row.

	FdoSmPhMgrP	                    pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhPropertyWriterP          pWriter = pPhysical->GetPropertyWriter();
    FdoSmPhDependencyWriterP        pDepWriter = pPhysical->GetDependencyWriter();

	// Ref the containing class
	FdoSmLpClassDefinition*		    pClass = (FdoSmLpClassDefinition*) RefParentClass();

	// Ref the highest level containing class. Different from containing class
	// when this property is nested within another object property.
	FdoSmLpClassDefinition*		    pTopClass = (FdoSmLpClassDefinition*) RefTopProperty()->RefParentClass();
    int								rowsProcessed =0;
	const FdoSmLpDbObject*	        pLpTable = NULL;
    const FdoSmLpDbObject*          pTargTable = NULL;
    FdoStringP				        fullName = GetName();
	FdoStringP				        pkTableName;
	FdoStringP				        fkTableName;
    bool                            bDepInherited;
//TODO
//    char*					        pUser = pConn->RefUser();
//	FdoStringP				        dbUser(pUser);
	FdoStringP				        dbUser(L"fdo_user");
    FdoStringP                      colPrefix = L"n/a";
    FdoSmLpPropertyMappingSingleP pSingleMapping = 
        mpMappingDefinition.p->SmartCast<FdoSmLpPropertyMappingSingle>();

//TODO
//	delete[] pUser;
	
	// table and full property name are defined in the target class. 
	if ( RefTargetClass() ) {
		pLpTable = RefTargetClass()->RefDbObject();
		fkTableName = RefTargetClass()->GetDbObjectName();
		// knock off outermost class name.
		fullName = FdoStringP( RefTargetClass()->GetName() ).Right(L"." );		
	}

	// Figure out the pkTable ( table for containing class ).
	if ( pLpTable ) {
		pTargTable = pLpTable->RefTargetDbObject();

		if ( pTargTable )
			pkTableName = pTargTable->GetName();
	}

	if ( pkTableName.GetLength() == 0 ) 
		// This happens when the containing class has no id properties and therefore
		// no table. Just use containing class table name in this case.
		pkTableName = RefParentClass()->GetDbObjectName();

    // Check if relation to pkTable is inherited from an ancestor property.
    bDepInherited = IsPkTableInherited( this, pkTableName );

    if ( pSingleMapping ) 
        colPrefix = pSingleMapping->GetPrefix();

	switch ( GetElementState() ) {
	case FdoSchemaElementState_Added:
		// Skip inherited properties when doing concrete property mapping. 
        // They can be auto-generated from their base properties.
		if ( pSingleMapping || (pClass == RefDefiningClass()) ) {

			// Create the attribute definition

            if ( mpMappingDefinition )
                mpMappingDefinition->WriteDb( pWriter );

            pWriter->SetTableName( GetContainingDbObjectName() );
			pWriter->SetClassId( pTopClass->GetId() );
			pWriter->SetName( fullName );
            pWriter->SetColumnName( colPrefix );
            pWriter->SetDataType( RefClass() ? RefClass()->GetQName() : L"" );
			pWriter->SetColumnType( L"table"  );
			pWriter->SetIsNullable( true );
			pWriter->SetIsFeatId( GetIsFeatId() );
			pWriter->SetIsSystem( GetIsSystem() );
			pWriter->SetIsReadOnly(	GetReadOnly() );
			pWriter->SetUser( dbUser );
			pWriter->SetDescription( GetDescription() );
            pWriter->SetIsFixedColumn( mbFixedDbObject );
            pWriter->SetIsColumnCreator( mbDbObjectCreator );
            pWriter->SetRootObjectName( mRootDbObjectName );
			pWriter->Add();
		}

		// Create the attribute dependency row, which shows how the 
		// containing class table and the object property table are joined.
		// Also stores cardinality.

		// Attribute dependency rows for inherited properties are created,
		// since the primary key table for an inherited property is 
		// different from that of the base property.
        //
        // This create step is skipped if:
        //  - contain class has no table
        //  - containing class table and containing table are the same
        //      (single table mapping)
        //  - relationship to pkTable is inherited from an ancestor property.

		if ( (pkTableName.GetLength() > 0) && 
            (pkTableName.ICompare(fkTableName) != 0) &&
            (!bDepInherited) 
        ) {
			FdoSmPhColumnListP srcColNames = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
			FdoSmPhColumnListP targColNames = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
			FdoStringP idColName;

			if ( pLpTable ) {
				// Gather the pkey and fkey columns if the containing class has a table.
				const FdoSmPhColumnCollection* pSrcCols = pLpTable->RefSourceColumns();
				const FdoSmPhColumnCollection* pTargCols = pLpTable->RefTargetColumns();

				for ( int i = 0; i < pSrcCols->GetCount(); i++ )
					srcColNames->Add( pSrcCols->RefItem(i)->GetName() );

				for ( int i = 0; i < pTargCols->GetCount(); i++ )
					targColNames->Add( pTargCols->RefItem(i)->GetName() );
			}

			if ( mpIdentityProperty )
				idColName = mpIdentityProperty->GetColumnName();

			// Create the dependency row.

            pDepWriter->SetPkTableName( pkTableName );
			pDepWriter->SetPkColumnNames( targColNames );
            pDepWriter->SetFkTableName( fkTableName );
			pDepWriter->SetFkColumnNames( srcColNames );
			pDepWriter->SetCardinality(	GetObjectType() == FdoObjectType_Value ? 1 : -1 );
			pDepWriter->SetIdentityColumn( idColName );
            pDepWriter->SetOrderType(
                (mObjectType == FdoObjectType_OrderedCollection) ?
					(( mOrderType == FdoOrderType_Descending ) ? L"d" : L"a") :
					L""
			);
            pDepWriter->Add();
		}

		break;

	case FdoSchemaElementState_Deleted:
		// Delete the attribute definition.

        pWriter->Delete( pTopClass->GetId(), fullName );
		// Delete the dependency row if containing class and this 
        // property each have a table. Skip the delete if the relationship to 
        // pkTable is inherited from an ancestor property.


		if ( (pkTableName.GetLength() > 0) && 
             (fkTableName.GetLength() > 0) &&
             (!bDepInherited) 
        )
            pDepWriter->Delete( pkTableName, fkTableName );

		break;

	case FdoSchemaElementState_Modified:
		pWriter->SetDescription( GetDescription() );
        pWriter->Modify( pTopClass->GetId(), fullName );

        break;
	}


	// Commit any nested object properties if this property is error free.
	// Nested inherited object properties with concrete table mapping are not committed. They
	// can be derived from both the f_AttributeDefinition and 
	// f_AttributeDependencies rows for the base property.
	if ( (GetErrors()->GetCount() == 0) && (pSingleMapping || (pClass == RefDefiningClass())) && RefTargetClass() ) 
		((FdoSmLpObjectPropertyClass*) RefTargetClass())->Commit();
}

FdoSchemaExceptionP FdoSmLpObjectPropertyDefinition::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	FdoSchemaExceptionP pException = FdoSmLpSchemaElement::Errors2Exception(pFirstException);

	// Don't check target class if we already have an error. It might be a circular reference
	// error so proceeding could cause an infinite loop.
    if ( ((FdoSmLpObjectPropertyDefinition*) this)->GetErrors()->GetCount() == 0 ) {
        if ( RefTargetClass() ) 
		    pException = RefTargetClass()->Errors2Exception(pException);

        if ( RefMappingDefinition() )
		    pException = RefMappingDefinition()->Errors2Exception(pException);
    }

	return pException;
}


void FdoSmLpObjectPropertyDefinition::FindDependency(const FdoSmLpClassDefinition* pParent)
{
	// Ref the f_attributedependencies row corresponding to this
	// object property.

	const FdoSmPhDbObject* pContainingTable = RefContainingDbObject();

	if ( pContainingTable ) {
		// Ref dependencies referencing the containing table.
		const FdoSmPhDependencyCollection* pDeps = 
			pContainingTable->GetDependenciesUp();

		// Look for the dependency referenced by the table for the containing class.
		for ( int i = 0; i < pDeps->GetCount(); i++ ) {
			const FdoSmPhDependency* pDep = pDeps->RefItem(i);
#ifdef _WIN32
			if ( _wcsicmp( pDep->GetPkTableName(), pParent->GetDbObjectName() ) == 0 ) {
#else
			if ( wcscasecmp( pDep->GetPkTableName(), pParent->GetDbObjectName() ) == 0 ) {
#endif
				SetDependency( pDep );
				break;
			}
		}
	}
	else {
		// Containing class has no table but may still have a dependency row. Ref the 
		// dependency directly. Look for the one from the contain class's table name to the
		// containing table name.
		FdoSmPhDependencyReaderP rdr = new FdoSmPhDependencyReader( pParent->GetDbObjectName(), GetContainingDbObjectName(), true, GetLogicalPhysicalSchema()->GetPhysicalSchema() );

		if ( rdr->ReadNext() ) {
			// Dependency was allocated so put it in mpMyDependency so it gets freed.
			mpMyDependency = rdr->GetDependency((FdoSmPhSchemaElement*) NULL);
			SetDependency( mpMyDependency );
	
		}
	}

	return;
}

void FdoSmLpObjectPropertyDefinition::SetDependency( const FdoSmPhDependency* pDependency )
{
	mpDependency = pDependency;

	/* Derive the object type. Cardinality tells us whether
	 * it is a collection property
	 */

	mObjectType = FdoObjectType_Value;
	mOrderType  = FdoOrderType_Ascending;

	if ( (mpDependency) && (mpDependency->GetCardinality() != 1) ) {
		// Order type tells us whether collection is ordered.
		if ( mpDependency->GetOrderType().GetLength() > 0 ) {
			mObjectType = FdoObjectType_OrderedCollection;

			// For ordered collection, check if descending. All other types of 
			// object property default to ascending.
			if ( wcscmp( mpDependency->GetOrderType(), L"d" ) == 0 ) 
				mOrderType = FdoOrderType_Descending;
		}
		else {
			mObjectType = FdoObjectType_Collection;
		}
	}
}

bool FdoSmLpObjectPropertyDefinition::IsPkTableInherited( 
    const FdoSmLpObjectPropertyDefinition* pProp, 
    FdoStringP pkTableName 
)
{
	FdoStringP				        pkPrevTableName;
    bool                            bDepInherited = false;

    // Ref the table for the class containing the property that this one
    // is directly base on, if any.
    const FdoSmLpObjectPropertyDefinition* pPrevProp = 
        FdoSmLpObjectPropertyDefinition::Cast( pProp->RefPrevProperty() );

    if ( pPrevProp ) {
        if ( pPrevProp->RefTargetClass() ) {
		    const FdoSmLpDbObject* pLpTable = pPrevProp->RefTargetClass()->RefDbObject();

            if ( pLpTable ) {
    		    const FdoSmLpDbObject* pTargTable = pLpTable->RefTargetDbObject();

	    	    if ( pTargTable )
                    pkPrevTableName = pTargTable->GetName(); 
            }
        }

        if ( pkTableName.ICompare(pkPrevTableName) == 0 ) {
            // PK table for base property the same as PK table for this property,
            // so dependency is inherited.
            bDepInherited = true;
        }
        else {
            // Not the same so walk further up the inheritance tree.
            bDepInherited = IsPkTableInherited( pPrevProp, pkTableName );
        }
    }

    return(bDepInherited );
}

void FdoSmLpObjectPropertyDefinition::Finalize()
{
    if ( GetState() != FdoSmObjectState_Final) {
        FdoSmLpPropertyDefinition::Finalize();

        /* Resolve dependencies. Only existing properties with concrete 
         * table mapping have dependency rows.
         */

        if ( (mTableMapping == FdoSmLpPropertyMappingType_Concrete) && (GetElementState() != FdoSchemaElementState_Added) ) {
	        // Get the dependency from the containing class table this object property's table
	        // from the MetaData.
	        FindDependency( RefParentClass() );

	        // If not found, inherit the dependency from the base property.
	        if ( (!mpDependency) && RefBaseProperty() )
		        mpDependency = ((const FdoSmLpObjectPropertyDefinition*) RefPrevProperty())->RefDependency();
        }
		    
        /* Find the referenced class */

		if ( (mClassName.GetLength() > 0) && !mpClass )
			mpClass = (FdoSmLpClassDefinition*)(this->RefLogicalPhysicalSchema()->FindClass(mClassName));

		/* Resolve the table mapping type */

		if ( mpClass && !ReferenceLoop() ) {

			if ( GetElementState() != FdoSchemaElementState_Deleted ) {
				// Error if type class is being deleted but this object property is not.
				if ( mpClass->GetElementState() == FdoSchemaElementState_Deleted )
					AddRefClassDeleteError( mpClass );

				// Error if type class is abstract ( not allowed by FDO spec ).
				if ( mpClass->GetIsAbstract() ) 
					AddRefClassAbstractError( mpClass);

				// The FDO spec does not make restrictions as to the class type.
				// However, it would not be easy to support Feature classes as 
				// Object Property classes. There are issues regarding
				// what to do with FeatId's, Geometric Properties and whether 
				// these object properties should have F_Feature rows.
				if ( mpClass->GetClassType() != FdoClassType_Class ) 
					AddRefClassFeatureError( mpClass );
			}

            const FdoSmLpClassDefinition*	pParentClass = RefPkClass();
            
            // Establish this object property's containing table.
            FinalizeTable(pParentClass);

			switch (mTableMapping) {
				case FdoSmLpPropertyMappingType_Single:
					SetSingleMapping();
					break;

				case FdoSmLpPropertyMappingType_Class:
					if ( GetElementState() != FdoSchemaElementState_Deleted )
						AddClassMappingError();
					break;

				case FdoSmLpPropertyMappingType_Concrete:
					SetConcreteMapping();
					break;
  			}

            // It is an error to add an object property to a class table if the 
            // containing class is base table mapped ( class's base class is in same table) and
            // any sub-property is not nullable. Such a not-nullable property 
            // prevents any objects for the containing class's base table from being added.

            if ( mpMappingDefinition &&
                (mTableMapping == FdoSmLpPropertyMappingType_Single ) &&
                (pParentClass->GetTableMapping() == FdoSmOvTableMappingType_BaseTable) &&
                (GetElementState() != FdoSchemaElementState_Deleted)
            ) {
                const FdoSmLpClassDefinition* pTargetClass = mpMappingDefinition->RefTargetClass();

                if ( pTargetClass ) {
                    int i;

                    for ( i = 0; i < pTargetClass->RefProperties()->GetCount(); i++ ) {
                        const FdoSmLpDataPropertyDefinition* pSubProp =
                            FdoSmLpDataPropertyDefinition::Cast( pTargetClass->RefProperties()->RefItem(i) );

                        if ( pSubProp && 
                            (pSubProp->GetElementState() != FdoSchemaElementState_Deleted) &&
                            (!pSubProp->GetNullable()) &&
                            (wcscmp(GetContainingDbObjectName(), pSubProp->GetContainingDbObjectName()) == 0)
                        ) {

                            AddSubPropNotNullError( pParentClass, pSubProp );
                        }
                    }
                }
            }
		}
		else {
			if ( GetElementState() != FdoSchemaElementState_Deleted )
				AddRefClassMissingError(mClassName);
		}

		// Now that the mapping definition has been created, a number of illegal
        // modification checks can be made.

		if ( GetElementState() == FdoSchemaElementState_Modified ) {
			// Changing local id not yet supported
			if ( wcscmp ( mIdentityPropertyName, mFdoIdPropName ) != 0 ) {
				AddIdPropChangeError( mFdoIdPropName );
			}

            if ( mFdoProp ) {
                // Changing type between value, collection or ordered collection
			    // not supported.
			    if ( mObjectType != mFdoProp->GetObjectType() )
				    AddObjTypeChangeError( mFdoProp->GetObjectType() );

			    // Changing order type not supported.
			    // Order type not applicable for valid property so skip check in this
			    // case.
			    if ( (mObjectType != FdoObjectType_Value) && (mOrderType != mFdoProp->GetOrderType()) )
				    AddOrderChangeError( mFdoProp->GetOrderType() );

                // Don't hang on to FDO Feature Schema elements longer than is 
                // necessary.
                mFdoProp = NULL;
            }
        }
	}
}

void FdoSmLpObjectPropertyDefinition::FinalizeTable( const FdoSmLpClassDefinition*	pParentClass )
{
  	FdoSmPhMgrP				pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
   	FdoStringP       		parentTable;
    FdoStringP  	        classTable;

    // Make sure object property class is finalized before accessing
    // table name.
    mpClass->RefDbObject();

    // Generate default containing table name by containing the table name for the defining class
    // and the table name for the type class. Use the foreign table names
    // if class maps onto a foreign schema.
    parentTable = (wcslen(pParentClass->GetRootDbObjectName()) > 0) ?
                    pParentClass->GetRootDbObjectName() :
                    pParentClass->GetDbObjectName();
    classTable = (wcslen(mpClass->GetRootDbObjectName()) > 0) ?
                    mpClass->GetRootDbObjectName() :
                    mpClass->GetDbObjectName();

    // Generate default object property table name from parent and class
    // table substrings. On SqlServer, this ensures that user name doesn't
    // get into the generated name multiple times.
    FdoStringP substParentTable = (wcslen(pParentClass->GetRootDbObjectName()) > 0) ?
                    pParentClass->GetSubstRootDbObjectName() :
                    pParentClass->GetSubstDbObjectName();
    FdoStringP substClassTable = (wcslen(mpClass->GetRootDbObjectName()) > 0) ?
                    mpClass->GetSubstRootDbObjectName() :
                    mpClass->GetSubstDbObjectName();

    mDefaultDbObjectName = substParentTable + L"_" + substClassTable;

    if ( GetIsFromFdo() || (GetElementState() == FdoSchemaElementState_Added) ) {
        // Generate table for new property
    	FdoSmPhDbObjectP                    pTargetTable ;
        FdoStringP                          targetTable;
        // Object Properties are always in the same datastore as their defining
        // classes.
        FdoStringP                          targetOwner = RefDefiningClass()->GetOwner();
        FdoStringP                          targetDatabase = RefDefiningClass()->GetDatabase();
        FdoStringP                          targetPkeyName;
        FdoStringP                          targetTablespace;
	    const FdoSmLpPropertyDefinition*    pTopBaseProp = RefTopProperty()->RefBaseProperty();
 
        // Tables or views always created in current owner.
      	FdoSmPhOwnerP		    pOwner = pPhysical->GetOwner();

        if ( mTableMapping == FdoSmLpPropertyMappingType_Single ) {
            // For single mapping, object property is in the containing class's table.
            pTargetTable = pPhysical->FindDbObject( parentTable );
		    SetContainingDbObject( pTargetTable, parentTable );
        }
        else {
            // For concrete mapping, object property is in its own table.
            // Do this only for non-inherited properties. Inherited properties inherit
    	    // the containing table from the base properties.
            if ( (mTableMapping == FdoSmLpPropertyMappingType_Concrete) && (!pTopBaseProp) ) {
                targetTable = GetOvTableName();
                if ( targetTable.GetLength() > 0 ) 
                    mbFixedDbObject = true;

                if ( targetOwner.GetLength() > 0 ) {
                    // Object Property is in a foreign table.
                    // Foreign table name is specified by overrides or defaults to default table name.
                    if ( targetTable.GetLength() == 0 ) {
                        mRootDbObjectName = mDefaultDbObjectName;
/* TODO
                        ValidateForeignObjectName( mRootDbObjectName );
*/
                    }
                    else {
                        mRootDbObjectName = targetTable;
                    }

                    // Name of view around foreign table defaults to foreign table name.
                    // Adjust name to be unique.
                    targetTable = pOwner->UniqueDbObjectName( mRootDbObjectName );
                    mbFixedDbObject = true;
                }

                if ( mbFixedDbObject ) {
  		            // Create actual table only if there are id properties.
                    if ( pParentClass->RefIdentityProperties()->GetCount() > 0 ) {
                        if ( mRootDbObjectName.GetLength() == 0 ) {
                            // Non-foreign table so just create it
                            pTargetTable = pPhysical->FindDbObject( targetTable );
                            // Create fixed table only if it doesn't yet exist 
                            if ( !pTargetTable ) {
                                targetTable = pPhysical->GetDcDbObjectName(targetTable);
                                pTargetTable = pPhysical->FindDbObject( targetTable );
                                if ( !pTargetTable ) {
    				                pTargetTable = NewTable( pOwner, targetTable );
                                    mbDbObjectCreator = true;
                                }
                            }
                        }
                        else {
                            // Foreign table so create view around it.
                            pTargetTable = NewView( pOwner, targetTable, targetDatabase, targetOwner, mRootDbObjectName );
                            mbDbObjectCreator = true;
                        }
                    }
                    else {
                        targetTable = pPhysical->GetDcDbObjectName(targetTable);
                    }

			        SetContainingDbObject( pTargetTable, targetTable );
                }
                else {
                    // table name not overridden so autogenerate it.
	                // Create a containing table for a new Object Property

                    if ( (parentTable.GetLength() > 0) && 
     			         (classTable.GetLength() > 0) 
					) {
					    FdoStringP targetTable = pOwner->UniqueDbObjectName( mDefaultDbObjectName );

						// Create actual table only if there are id properties.
                        if ( pParentClass->RefIdentityProperties()->GetCount() > 0 ) {
						    pTargetTable = NewTable( pOwner, targetTable )->SmartCast<FdoSmPhDbObject>();
                            mbDbObjectCreator = true;
                        }

						SetContainingDbObject( pTargetTable, targetTable );
					}
					else {
					    SetContainingDbObject( (FdoSmPhDbObject*) NULL, L"" );
					}
                }
            }
        }
    }
    else {
        // If object property already exists, do the reverse processing,
        // determine table mapping type .
    
        const FdoSmLpObjectPropertyDefinition* pPrevProp = 
            FdoSmLpObjectPropertyDefinition::Cast( RefPrevProperty() );

        // Skip nested properties, their table mapping is already determined.
        if ( !wcschr( GetName(), '.' ) ) {
            // No base. Object property will have a table so determine
            // table mapping from the table name.
            if ( wcscmp( GetContainingDbObjectName(), parentTable ) == 0 ) {
                // Object property is embedded in the defining class's table.
		        mTableMapping = FdoSmLpPropertyMappingType_Single;
		    }
		    else {
		        if ( wcscmp( GetContainingDbObjectName(), mpClass->GetDbObjectName() ) == 0 ) 
			        // Object property is kept in the referenced class table.
			        mTableMapping = FdoSmLpPropertyMappingType_Class;
		        else
			        // Object property has its own table.
			        mTableMapping = FdoSmLpPropertyMappingType_Concrete;
            }
        }
    }
}

FdoSmPhDbObjectP FdoSmLpObjectPropertyDefinition::NewTable( 
    FdoSmPhOwnerP owner,
    FdoString* tableName
)
{
	FdoSmPhTableP table = owner->CreateTable( tableName );

    // table inherits long transaction mode from current owner.
    // Object Properties cannot be locked.
    table->SetLtMode( owner->GetLtMode() );    

    return table.p->SmartCast<FdoSmPhDbObject>();
}

FdoSmPhDbObjectP FdoSmLpObjectPropertyDefinition::NewView( 
    FdoSmPhOwnerP viewOwner,
    FdoString* viewName, 
    FdoString* database,
    FdoString* owner,
    FdoString* tableName
)
{
	return( viewOwner->CreateView(viewName, database, owner, tableName).p->SmartCast<FdoSmPhDbObject>() );
}


bool FdoSmLpObjectPropertyDefinition::ReferenceLoop()
{
    const FdoSmLpObjectPropertyDefinition* currProp = this;

    // Walk up the object property nesting.
    while ( currProp != NULL ) {
        FdoSmLpObjectPropertyClass* pOpClass = 
            dynamic_cast<FdoSmLpObjectPropertyClass*>((FdoSmLpSchemaElement*) (currProp->RefParentClass()) );
    
        if ( pOpClass ) {
            currProp = pOpClass->RefObjectProperty();

            if ( currProp->RefClass() == mpClass ) { 
                // Found this property's class so there is a reference loop.
                this->AddReferenceLoopError();
                return( true );
            }
        }
        else {
            currProp = NULL;
        }
    }

    return( false );
}

void FdoSmLpObjectPropertyDefinition::AddClassMappingError()
{
	GetErrors()->Add( FdoSmErrorType_ObjectPropertyMapping, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_188),
				RefDefiningClass()->GetName(), 
				GetName()
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddReferenceLoopError()
{
	GetErrors()->Add( FdoSmErrorType_ObjectPropertyLoop, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_190),
				GetFeatureClassName(),	
				RefDefiningClass()->GetName(), 
				GetName()
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddRefClassMissingError(FdoString* refClassName)
{
	GetErrors()->Add( FdoSmErrorType_ObjectPropertyLoop, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_238),
				refClassName,	
				(FdoString*)(GetQName())
			)
		)
	);

}

void FdoSmLpObjectPropertyDefinition::AddRefClassDeleteError(const FdoSmLpClassDefinition* pRefClass)
{
	GetErrors()->Add( FdoSmErrorType_ObjectPropertyLoop, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_191),
				(FdoString*)(pRefClass->GetQName()),	
				(FdoString*)(GetQName())
			)
		)
	);

	// Set the state of this element so that the error gets pick up by ApplySchemaCommand.
	// This is a bit of a stretch but deleting the type class can be thought of as 
	// modifying this property.
	if ( GetElementState() == FdoSchemaElementState_Unchanged )
		FdoSmLpSchemaElement::SetElementState(FdoSchemaElementState_Modified);
}

void FdoSmLpObjectPropertyDefinition::AddRefClassAbstractError(const FdoSmLpClassDefinition* pRefClass)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_202),
				(FdoString*)(pRefClass->GetQName()),	
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddRefClassFeatureError(const FdoSmLpClassDefinition* pRefClass)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_263),
				(FdoString*)(pRefClass->GetQName()),	
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddTypeClassChangeError( FdoStringP fdoTypeClassName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_192),
				(FdoString*) GetQName(), 
				(FdoString*) mClassName, 
				(FdoString*) fdoTypeClassName
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddIdPropChangeError( FdoString* fdoIdPropName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_193),
				(FdoString*)(GetQName()), 
				(FdoString*) mIdentityPropertyName, 
				fdoIdPropName
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddObjTypeChangeError( FdoObjectType fdoObjectType  )
{
	// Todo: show object types in message.
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_194),
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddOrderChangeError( FdoOrderType fdoOrderType  )
{
	// Todo: show object types in message.
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_195),
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddValueHasIdError()
{
	// Todo: show object types in message.
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_247),
				(FdoString*)(GetQName())
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddTypeNoSchemaError( FdoString* typeClassName )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_259),
				typeClassName,
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddSingleCollectionError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_301),
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpObjectPropertyDefinition::AddSubPropNotNullError(
    const FdoSmLpClassDefinition* pContainingClass,
    const FdoSmLpDataPropertyDefinition* pDataProp
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_197),
			    (FdoString*) (GetQName() + L"." + pDataProp->GetName()),
                (FdoString*) pContainingClass->GetQName()
            )
		)
	);
}

void FdoSmLpObjectPropertyDefinition::XMLSerialize( FILE* xmlFp, int ref ) const
{
    ((FdoSmLpObjectPropertyDefinition*) this)->Finalize();

	fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" description=\"%ls\"\n objectType=\"%s\" className=\"%ls\" idColumn=\"%ls\" order=\"%s\"\n fixedCol=\"%s\" >\n",
            (FdoString*) FdoSmLpPropertyTypeMapper::Type2String(GetPropertyType()),
			GetName(), GetDescription(), 
			(GetObjectType() == FdoObjectType_OrderedCollection) ? "Ordered Collection" :
				( (GetObjectType() == FdoObjectType_Collection) ? "Collection" : "Value" ),
			GetFeatureClassName(),
			RefIdentityProperty() ? RefIdentityProperty()->GetColumnName() : L"",
			(GetOrderType() == FdoOrderType_Descending) ? "descending" : "ascending",
            GetIsFixedDbObject() ? "True" : "False"
	);

	if ( ref == 0 ) {
		if ( RefDefiningClass() &&
			RefBaseProperty() ) 
			fprintf( xmlFp, "<Inherited baseClass=\"%ls\" />\n",
				RefDefiningClass()->GetName()
			);

		if ( RefIdentityProperty() ) {
			fprintf( xmlFp, "<idProperty>\n" );
			RefIdentityProperty()->XMLSerialize(xmlFp,1);
			fprintf( xmlFp, "</idProperty>\n" );
		}

		if ( RefMappingDefinition() ) 
			RefMappingDefinition()->XMLSerialize(xmlFp, ref);

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );
	}

	fprintf( xmlFp, "</property>\n" );
}

