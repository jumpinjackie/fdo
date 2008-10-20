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
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Ph/SchemaReader.h>
//#include <Sm/Ph/SADReader.h>
//#include <Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h>
#include <Sm/Lp/Class.h>
#include <Sm/Lp/FeatureClass.h>
#include <Sm/Lp/ClassTypeMapper.h>
#include <Sm/Error.h>

FdoSmLpSchema::FdoSmLpSchema(FdoSmPhSchemaReaderP rdr,  FdoSmPhMgrP physicalSchema, FdoSmLpSchemaCollection* schemas) : 
	FdoSmLpSchemaElement(rdr->GetName(), rdr->GetDescription() ),
	mPhysicalSchema(physicalSchema),
	mpSchemas(schemas),
	mbSchemaLoaded(false),
    mbSADLoaded(false),
	mTableMapping(FdoSmOvTableMappingType_Default)
{
	// this is the schema 
	SetLogicalPhysicalSchema(FDO_SAFE_ADDREF(this));
	mClasses = new FdoSmLpClassCollection();

	// Read schema-wide default overrides:
    SetDatabase(rdr->GetDatabase());
    SetOwner(rdr->GetOwner());
}

FdoSmLpSchema::FdoSmLpSchema(
	FdoFeatureSchema* pFeatSchema, 
    bool bIgnoreStates,
	FdoSmPhMgrP physicalSchema,  
	FdoSmLpSchemaCollection* schemas
) :
	FdoSmLpSchemaElement(pFeatSchema->GetName(), pFeatSchema->GetDescription(), NULL, true ),
	mPhysicalSchema(physicalSchema),
	mpSchemas(schemas),
	mbSchemaLoaded(false),
    mbSADLoaded(false),
    mTableMapping(FdoSmOvTableMappingType_Default)
{
	// this is the schema 
	SetLogicalPhysicalSchema(FDO_SAFE_ADDREF(this));
	mClasses = new FdoSmLpClassCollection();
}	

FdoSmLpSchema::~FdoSmLpSchema(void)
{
}

const FdoSmLpClassCollection* FdoSmLpSchema::RefClasses() const
{
	// Cast this to allow behind-the-scenes loading of this schema.
	((FdoSmLpSchema*) this)->LoadSchema();

    return mClasses;
}

const FdoSmLpClassDefinition* FdoSmLpSchema::RefClass(FdoStringP className) const
{
	// Cast this to allow behind-the-scenes loading of this schema.
	return (FdoSmLpClassDefinition*)(((FdoSmLpSchema*) this)->LoadClass(className));
}

const FdoSmLpSAD* FdoSmLpSchema::RefSAD() const
{
    if ( !mbSADLoaded )
    {
        FdoSmPhSADReaderP pSADReader = new FdoSmPhSADReader( FdoSmPhMgr::SchemaType, mPhysicalSchema, GetName() );
	    ((FdoSmLpSchema*)this)->LoadSAD(pSADReader);

        (bool) mbSADLoaded = true;
    }

	return FdoSmLpSchemaElement::RefSAD();
}

const FdoSmPhMgr* FdoSmLpSchema::RefPhysicalSchema() const
{
    return ((FdoSmLpSchema*)this)->GetPhysicalSchema();
}

FdoSmPhMgrP FdoSmLpSchema::GetPhysicalSchema()
{
    return mPhysicalSchema;
}

const FdoSmLpSchemaCollection* FdoSmLpSchema::RefSchemas() const
{
    return mpSchemas;
}

FdoSmLpSchemasP FdoSmLpSchema::GetSchemas()
{
    return FDO_SAFE_ADDREF(mpSchemas);
}

FdoPhysicalSchemaMappingP FdoSmLpSchema::GetSchemaMappings( bool bIncludeDefaults ) const
{
    return (FdoPhysicalSchemaMapping*) NULL;
}

const FdoSmLpClassDefinition* FdoSmLpSchema::FindClass( FdoStringP className, bool searchAllSchemas ) const
{
	FdoStringP					        schemaName;
	FdoStringP					        localClassName;
	const FdoSmLpClassDefinition*      	pFoundClass = NULL;

	// Check if class name qualified by schema name.
	FdoStringP leftString = className.Left( L":" );
	if ( leftString.GetLength() == className.GetLength() ) {
		// Not qualified
		localClassName = className;
	}
	else {
		// Qualified so split into schema name and class name
		if ( leftString.GetLength() > 0 ) 
			schemaName = leftString;

		localClassName = className.Right( L":" );
	}

    if( wcscmp(schemaName, GetName()) == 0 )
    {
        pFoundClass = (FdoSmLpClassDefinition*)(((FdoSmLpSchema*) this)->LoadClass( localClassName ));
        if ( pFoundClass )
            return( pFoundClass );
    }

    // May be it's a non qualified metaclass: F_Feature, F_Class or F_ClassDefinition
    if( ( ((const wchar_t*)schemaName)[0] == '\0' || wcscmp(schemaName,FdoSmPhMgr::mMetaClassSchemaName) == 0 ) && 
        (wcscmp(localClassName,FdoSmLpClassDefinition::ClassClassName ) == 0 ||
         wcscmp(localClassName,FdoSmLpClassDefinition::ClassDefinitionName ) == 0 ||
         wcscmp(localClassName,FdoSmLpClassDefinition::FeatureClassName ) == 0 )
         )
    {
        pFoundClass = mpSchemas->FindClass( FdoSmPhMgr::mMetaClassSchemaName, localClassName );
        if ( pFoundClass )
            return( pFoundClass );
    }

    ((FdoSmLpSchema*) this)->LoadSchema();
    // Search this schema if it is the class's schema or class not qualified by 
	// schema name.
	if ( (schemaName.GetLength() == 0) || (wcscmp(schemaName, GetName()) == 0) ) 
		pFoundClass = mClasses->RefItem( localClassName );

	// If not in this schema, search all schemas.
	if ( !pFoundClass && searchAllSchemas) 
		pFoundClass = mpSchemas->FindClass( schemaName, localClassName );
	
	return( pFoundClass );
}

void FdoSmLpSchema::TableToClasses( 
    FdoSmLpQClassesP& classes,
    FdoStringP tableName, 
    FdoStringP ownerName ,
    FdoStringP databaseName,
    bool cacheOnly
) const
{
    FdoInt32 idx;

    if (!cacheOnly)
        RefClasses();

    for ( idx = 0; idx < mClasses->GetCount(); idx++ ) {
        const FdoSmLpClassDefinition* pClass = mClasses->RefItem(idx);

        const FdoSmPhDbObject* smPhDbObject = NULL;
        const FdoSmLpDbObject* smLpDbObject = pClass->RefDbObject();

        if (smLpDbObject != NULL)
            smPhDbObject = smLpDbObject->RefDbObject();

        FdoSmPhOwnerP owner = ((FdoSmLpSchema *)this)->GetPhysicalSchema()->FindOwner(ownerName, databaseName);

        if ( smPhDbObject != NULL ) {
            if ((tableName.ICompare(pClass->GetDbObjectName()) == 0) &&
                (FdoStringP(owner->GetName()).ICompare(smPhDbObject->GetParent()->GetName()) == 0) &&
                (databaseName.ICompare(smPhDbObject->GetParent()->GetParent()->GetName()) == 0)) {
                    FdoSmLpQClassDefinitionP qClass = new FdoSmLpQClassDefinition( pClass );
                    classes->Add( qClass );
            }
        }
    }
}

void FdoSmLpSchema::Update(
    FdoFeatureSchema* pFeatSchema,
	FdoSchemaElementState elementState, 
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
	FdoSmLpClassDefinitionP			pLpClass;
    FdoInt32                        idx;

    if ( GetSchemas()->CanCreatePhysicalObjects() ) {
        FdoSmPhOwnerP owner = GetPhysicalSchema()->GetOwner();
        if ( (!owner) || !(owner->GetHasMetaSchema()) ) {
            if ( elementState == FdoSchemaElementState_Added ) 
                // No MetaSchema so must create corresponding physical schema
                CreatePhysicalSchema( owner );
      
            if ( elementState == FdoSchemaElementState_Deleted ) 
                // No MetaSchema so must delete corresponding physical schema
                DeletePhysicalSchema( owner );
        }
    }

    // Skip load from RDBMS when loading from config doc. Classes in config doc take
    // precedence over this in RDBMS so load from config doc first.
    //
    // However, when doing an ApplySchema, need to load from RDBMS first so that
    // updates from FDO are merged into RDBMS classes.
    if ( (!GetIsFromFdo()) || (elementState != FdoSchemaElementState_Unchanged) ) 
    	LoadSchema();
	
    FdoSmLpSchemaElement::Update(pFeatSchema, elementState, bIgnoreStates);

	if ( GetElementState() != FdoSchemaElementState_Deleted ) {

        // Handle updates to classes. Skipped when deleting schema since classes
		// are already marked for delete at this point.
		FdoClassesP pFeatClasses = pFeatSchema->GetClasses();

		for ( int i = 0; i < pFeatClasses->GetCount(); i++ ) {
			FdoClassDefinitionP pFeatClass = pFeatClasses->GetItem(i);
            FdoSchemaElementState fdoClassState = FdoSchemaElementState_Unchanged;

            if ( bIgnoreStates ) {
                // When ignoring element states, the operation to perform depends 
                // on whether the class already in LogicalPhysical schema.
                idx = mClasses->IndexOf(pFeatClass->GetName());
                if ( idx >= 0 ) {
                    // Class is in LogicalPhysical schema.
                    if ( elementState == FdoSchemaElementState_Unchanged ) {
                        // When Fdo element unchanged, it must be from a config doc.
                        // In this case it replaces any pre-existing version of the class.
                        mClasses->RemoveAt( idx );
                    }
                    else {
                        // Otherwise, doing an ApplySchema so modify the class
                        fdoClassState = FdoSchemaElementState_Modified;
                    }
                }
                else {
                    if ( elementState != FdoSchemaElementState_Unchanged )
                        // Class not yet in LogicalPhysical Schema and is not
                        // from config doc. Add it.
                        fdoClassState = FdoSchemaElementState_Added;
                }
            }
            else {
                fdoClassState = pFeatClass->GetElementState();
            }

            if ( (fdoClassState == FdoSchemaElementState_Added) || GetIsFromFdo() ) {

                // Can't add class if it already exists
				if ( mClasses->RefItem(pFeatClass->GetName()) ) {
					AddClassExistsError( pFeatClass );
				}
				else {
                    // Create the new LogicalPhysical class from the FDO class.
        		    pLpClass = CreateClassDefinition( 
                        pFeatClass, 
                        pOverrides, 
                        bIgnoreStates,
                        fdoClassState
                    );
		    	    mClasses->Add( pLpClass );
				}
            }
            else { // let class figure out what to do when it is unchanged.
				pLpClass = mClasses->FindItem( pFeatClass->GetName() );

				if ( pLpClass ) {
					pLpClass->Update( 
                        pFeatClass, 
                        fdoClassState,
                        pOverrides,
                        bIgnoreStates
                    );
				}
				else {
					if ( fdoClassState != FdoSchemaElementState_Deleted )
						// Can't modify class not in database.
						AddClassNotExistsError( pFeatClass->GetName() );
				}
			}
		}
	}
}

void FdoSmLpSchema::SynchPhysical(bool bRollbackOnly)
{
	for ( int i = 0; i < RefClasses()->GetCount(); i++ )
		mClasses->GetItem(i)->SynchPhysical(bRollbackOnly);
}
 
void FdoSmLpSchema::SetElementState(FdoSchemaElementState elementState)
{
	FdoSmSchemaElement::SetElementState( elementState );

	// When a schema is deleted, all of its classes must also be deleted.
	if ( elementState == FdoSchemaElementState_Deleted ) {
        if ( GetSchemas()->CanCreatePhysicalObjects() ) {
            FdoSmPhOwnerP owner = GetPhysicalSchema()->GetOwner();
            if ( (!owner) || !(owner->GetHasMetaSchema()) ) {
                // No MetaSchema so must delete corresponding physical schema
                DeletePhysicalSchema( owner );
            }
        }

		if ( RefClasses() ) {
			for ( int i = 0; i < mClasses->GetCount(); i++ ) 
				FdoSmLpClassDefinitionP(mClasses->GetItem(i))->SetElementState(elementState);
		}
	}

}

void FdoSmLpSchema::Commit( bool fromParent )
{
    FdoSmPhSchemaWriterP            pWriter;
	
	switch ( GetElementState() ) {
	case FdoSchemaElementState_Added:
        pWriter = GetPhysicalAddWriter();
	    pWriter->Add();

        break;

	case FdoSchemaElementState_Deleted:
        pWriter = GetPhysicalSchema()->GetSchemaWriter();
        pWriter->Delete( GetName() );
		break;

	case FdoSchemaElementState_Modified:
        pWriter = GetPhysicalSchema()->GetSchemaWriter();
		pWriter->SetDescription( GetDescription() );
        pWriter->Modify( GetName() );

		break;
	}

	if ( mClasses ) {
		for ( int i = 0; i < mClasses->GetCount(); i++ ) 
			FdoSmLpClassDefinitionP(mClasses->GetItem(i))->Commit( fromParent );
	}

	CommitSAD(FdoSmPhMgr::SchemaType);
}

FdoSmPhSchemaWriterP FdoSmLpSchema::GetPhysicalAddWriter()
{
    char*							pUser = "fdo_user";//TODO pConn->GetUser();
	FdoStringP				        dbUser(pUser);

    FdoSmPhSchemaWriterP pWriter = GetPhysicalSchema()->GetSchemaWriter();

    pWriter->SetName( GetName() );
    pWriter->SetDescription( GetDescription() );
    pWriter->SetUser( dbUser );
    pWriter->SetDatabase( GetDatabase() );
    pWriter->SetOwner( GetOwner() );

//TODO	delete[] pUser;

    return pWriter;
}

FdoSmLpClassDefinitionP FdoSmLpSchema::CreateClassDefinition(FdoSmPhClassReaderP classReader)
{
    switch ( FdoSmLpClassTypeMapper::String2Type(classReader->GetClassType()) ) {
    case FdoClassType_Class:
        return CreateClass( classReader);
    
    case FdoClassType_FeatureClass:
        return CreateFeatureClass( classReader );
    }

    throw FdoSchemaException::Create( 
        FdoSmError::NLSGetMessage(
            FDO_NLSID(FDOSM_127), 
            (FdoString*) classReader->GetClassType()
        )
    );

    return (FdoSmLpClassDefinition*) NULL;
}

FdoSmLpClassDefinitionP FdoSmLpSchema::CreateClassDefinition(
    FdoClassDefinition* pFdoClass, 
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates,
    FdoSchemaElementState elementState
)
{
    FdoSmLpClassDefinitionP lpClass;

    switch ( pFdoClass->GetClassType() ) {
    case FdoClassType_Class:
        lpClass = CreateClass( (FdoClass*) pFdoClass, bIgnoreStates );
        break;
    
    case FdoClassType_FeatureClass:
        lpClass = CreateFeatureClass( (FdoFeatureClass*) pFdoClass, bIgnoreStates );
        break;
    
    default:
        throw FdoSchemaException::Create( 
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_125), 
                (FdoString*) pFdoClass->GetQualifiedName()
            )
        );
    }

    lpClass->Update( pFdoClass, elementState, pOverrides, bIgnoreStates );
    return lpClass;
}

void FdoSmLpSchema::CreatePhysicalSchema(
    FdoSmPhOwnerP owner
)
{
    AddCreateNoMetaError( owner );
}

void FdoSmLpSchema::DeletePhysicalSchema(
    FdoSmPhOwnerP owner
)
{
    AddDeleteNoMetaError( owner );
}

FdoSchemaExceptionP FdoSmLpSchema::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	if ( RefClasses() ) {
		for ( int i = 0; i < mClasses->GetCount(); i++ )
			pException = mClasses->RefItem(i)->Errors2Exception(pException);
	}

	return pException;
}


void FdoSmLpSchema::LoadSchema()
{
	if ( !mbSchemaLoaded ) {
		LoadClasses();

		mbSchemaLoaded = true;
	}
    if ( !mbSADLoaded ) {
        FdoSmPhSADReaderP pSADReader = new FdoSmPhSADReader( FdoSmPhMgr::SchemaType, mPhysicalSchema, GetName() );
		LoadSAD(pSADReader);

        mbSADLoaded = true;
    }


}

FdoSmLpClassDefinitionP FdoSmLpSchema::LoadClass(FdoStringP className, FdoString* schemaName )
{
    FdoSmLpClassDefinition* cls = mClasses->FindItem(className);
    if( cls != NULL )
        return cls;
    
    FdoSmPhClassReaderP classReader = mPhysicalSchema->CreateClassReader((schemaName!=NULL&&schemaName[0]!='\0')?schemaName:GetName(),className);

    while ( classReader->ReadNext() ) {
		FdoSmLpClassDefinitionP newClass = CreateClassDefinition( classReader );
        if ( !FdoPtr<FdoSmLpClassDefinition>(mClasses->FindItem(newClass->GetName())) )
    		mClasses->Add( newClass );
	}
    return mClasses->FindItem(className);
}

void FdoSmLpSchema::LoadClasses()
{
	// (Re)load the classes
	FdoSmPhClassReaderP classReader = mPhysicalSchema->CreateClassReader(GetName());

	while ( classReader->ReadNext() ) {
		FdoSmLpClassDefinitionP newClass = CreateClassDefinition( classReader );
        FdoSmLpClassDefinitionP oldClass = mClasses->FindItem(newClass->GetName());
        if ( oldClass == NULL  )
    	    mClasses->Add( newClass );
	}
}

void FdoSmLpSchema::DeleteSchema()
{
	mClasses->Clear();

	DeleteSAD();

	mbSchemaLoaded = false;
    mbSADLoaded = false;
}

void FdoSmLpSchema::AddSchemaExistsError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_222),
				GetName() 
			)
		)
	);
}


void FdoSmLpSchema::AddClassExistsError( FdoClassDefinition* pFdoClass )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_214),
				(FdoString*) pFdoClass->GetQualifiedName() 
			)
		)
	);
}

void FdoSmLpSchema::AddClassNotExistsError( FdoString* className )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_173),
				(FdoString*) GetName(), 
				className
			)
		)
	);
}

void FdoSmLpSchema::AddCreateNoMetaError( FdoSmPhOwnerP owner )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_33),
				GetName(),
                owner ? owner->GetName() : L""
			)
		)
	);
}

void FdoSmLpSchema::AddDeleteNoMetaError( FdoSmPhOwnerP owner )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_34),
				(FdoString*) GetName(),
                owner ? owner->GetName() : L""
			)
		)
	);
}

void FdoSmLpSchema::AddSchemaNameLengthError( FdoString* schemaName, FdoSize maxLen )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_318),
                maxLen,
                schemaName
			)
		)
	);
}

void FdoSmLpSchema::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<schema name=\"%s\" description=\"%s\" >\n",
        (const char*) FdoStringP(GetName()), (const char*) FdoStringP(GetDescription()) );

	if ( ref == 0 ) {
		const FdoSmLpClassCollection* classes = RefClasses();

		if ( classes ) {
			for ( int i = 0; i < classes->GetCount(); i++ ) 
				classes->RefItem(i)->XMLSerialize(xmlFp, ref);
		}

		FdoSmLpSchemaElement::XMLSerialize( xmlFp, ref );
	}

	fprintf( xmlFp, "</schema >\n" );
}



