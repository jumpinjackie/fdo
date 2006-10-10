 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include <Sm/Lp/SchemaCollection.h>
#include <Sm/Ph/SchemaReader.h>
#include <Sm/Error.h>

FdoSmLpSchemaCollection::FdoSmLpSchemaCollection(FdoSmPhMgrP physicalSchema, FdoSmLpSpatialContextsP spatialContexts) :
	mPhysicalSchema(physicalSchema),
    mSpatialContexts(spatialContexts),
    mSchemasLoaded(false)
{
}

FdoSmLpSchemaCollection::~FdoSmLpSchemaCollection(void)
{
}
/*
FdoSmLpSchemaP CreateSchema( FdoSmPhSchemaReaderP rdr )
{
    return new (rdr, mPhysicalSchema, FDO_SAFE_ADDREF(this));
}
*/
const FdoSmLpClassDefinition* FdoSmLpSchemaCollection::FindClass( FdoStringP schemaName, FdoStringP className ) const
{
	const FdoSmLpSchema* pSchema = NULL;
	const FdoSmLpClassDefinition* pFoundClass = NULL;

	if ( schemaName.GetLength() > 0 ) {
		// Check the given schema if schema name specified
		pSchema = RefItem(schemaName);
		if ( pSchema ) 
            pFoundClass = pSchema->RefClass(className);
	}
	else {
		// Otherwise, find the first class in any schema.
		const FdoSmLpClassDefinition* pClass = NULL;

		for ( int i = 0; i < GetCount(); i++ ) {
			pSchema = RefItem(i);

			if ( pSchema ) {
				pClass = pSchema->RefClasses()->RefItem(className);

				if ( pClass ) {

					if ( pFoundClass ) {
						// Found a second class so exception.

                        throw FdoSchemaException::Create( 
                            FdoSmError::NLSGetMessage(
                                FDO_NLSID(FDOSM_174), 
									(FdoString*) className, 
									pFoundClass->RefLogicalPhysicalSchema()->GetName(),
									pSchema->GetName()
                            )
                        );
					}

					pFoundClass = pClass;
				}
			}
		}
	}

	return(pFoundClass);
}

const FdoSmLpClassDefinition* FdoSmLpSchemaCollection::FindClass(FdoInt64 classId) const
{
	const FdoSmLpSchema* pSchema;
	const FdoSmLpClassDefinition* pClass = NULL;
	bool found = false;

	for (int i = 0; i < GetCount() && !found; i++)	{
		pSchema = RefItem(i);
		if (pSchema)	{
			for (int j = 0; j < pSchema->RefClasses()->GetCount() && !found; j++)	{
				pClass = pSchema->RefClasses()->RefItem(j);
				if (pClass->GetId() == classId)
					found = true;
			}
		}
	}
	return pClass;
}

/*
FdoSmLpJoinTreeNode* FdoSmLpSchemaCollection::GetJoinTree( FdoStringP schemaName, FdoStringP className, const FdoStringsP& properties ) const
{
	const FdoSmLpClassDefinition* pClass = FindClass( schemaName, className );

	if ( !pClass ) 
		return(NULL);

	return ( pClass->GetJoinTree( properties ) );
}
*/

FdoSmLpQClassesP FdoSmLpSchemaCollection::TableToClasses( 
    FdoStringP tableName, 
    FdoStringP ownerName,
    FdoStringP databaseName
) const
{
    FdoInt32            idx;
    FdoSmLpQClassesP    classes = new FdoSmLpQClassCollection();

    for ( idx = 0; idx < GetCount(); idx++ ) {
        RefItem(idx)->TableToClasses( classes, tableName, ownerName, databaseName );
    }

    return classes;
}

FdoSmLpSchemaP FdoSmLpSchemaCollection::CreateSchema(
	FdoFeatureSchema* pFeatSchema,
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoSmLpSchemaP schema = NewSchema( pFeatSchema, bIgnoreStates );

    schema->Update( 
        pFeatSchema, 
        bIgnoreStates ? FdoSchemaElementState_Added : pFeatSchema->GetElementState(), 
        pOverrides,
        bIgnoreStates 
    );

    return schema;
}

void FdoSmLpSchemaCollection::Commit()
{
	for ( int i = 0; i < GetCount(); i++ ) {
		FdoSmLpSchemaP schema = GetItem(i);
		schema->Commit( true );
	}
}

FdoSchemaExceptionP FdoSmLpSchemaCollection::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FDO_SAFE_ADDREF(pFirstException);

	for ( int i = 0; i < GetCount(); i++ )
		pException = RefItem(i)->Errors2Exception(pException);

	return pException;
}


void FdoSmLpSchemaCollection::Load()
{
    int i;

	if ( (mPhysicalSchema != NULL) && !mSchemasLoaded ) {
        mSchemasLoaded = true;

        // Check if there is a config doc
        FdoFeatureSchemasP schemas = mPhysicalSchema->GetConfigSchemas();
        FdoSchemaMappingsP mappings = mPhysicalSchema->GetConfigMappings();
        FdoStringP providerName = mPhysicalSchema->GetProviderName();

        if ( schemas ) {
            // There is a config doc so load schemas from it first.
            for ( i = 0; i < schemas->GetCount(); i++ ) {
                FdoFeatureSchemaP schema = schemas->GetItem(i);
                FdoPhysicalSchemaMappingP mapping;
                
                if ( mappings ) 
                    mapping = mappings->GetItem(
                        providerName, 
                        schema->GetName() 
                    );

                FdoSmLpSchemaP lpSchema = NewSchema( schema, true );

                // Elements from config doc are never modified so mark them as Unchanged.
                lpSchema->Update(
                    schema, 
                    FdoSchemaElementState_Unchanged, 
                    mapping,
                    true
                );

                Add( lpSchema );
            }
        }

        // Next, read schemas from datastore
        FdoSmPhSchemaReaderP rdr = mPhysicalSchema->CreateSchemaReader();

        while ( rdr->ReadNext() ) {
            FdoSmLpSchemaP rdrSchema = NewSchema(rdr);
            // Skip any schemas already loaded from config doc.
            // Config doc takes precedence.
            if ( !schemas || (!FindItem(rdrSchema->GetName())) )
    		    this->Add( NewSchema(rdr) );
	    }
	}
}


void FdoSmLpSchemaCollection::XMLSerialize( FdoString* sFileName ) const
{
	FILE* xmlFp = fopen( (const char*) FdoStringP(sFileName), "w" );

	fprintf( xmlFp, "<?xml version=\"1.0\" standalone=\"yes\"?>\n" );
    fprintf( xmlFp, "<schemas xmlns:xsi=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http:/www.autodesk.com/isd/fdo/GenericLogicalPhysical\" >\n" );
	
	for ( int i = 0; i < GetCount(); i++ ) {
		RefItem(i)->XMLSerialize(xmlFp, 0);
	}
	
	fprintf( xmlFp, "</schemas>\n" );

	fclose(xmlFp);

}
