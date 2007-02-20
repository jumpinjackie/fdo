// 
//  
//  Copyright (C) 2004-2007  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfDelete.h"
#include "BinaryReader.h"
#include "SdfConnection.h"
#include "SdfQueryOptimizer.h"
#include "SdfDeletingFeatureReader.h"
#include "DataDb.h"
#include "PropertyIndex.h"
#include "FdoCommonFilterExecutor.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs a Delete command using the specified connection
SdfDelete::SdfDelete(SdfConnection* connection)
: SdfFeatureCommand<FdoIDelete>(connection)
{
}


// default destructor
SdfDelete::~SdfDelete()
{
}


//-------------------------------------------------------
// FdoIDelete implementation
//-------------------------------------------------------


// Executes the delete command and returns the number of instances deleted.
FdoInt32 SdfDelete::Execute()
{
    // verify connection
    if (m_connection == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_39_NO_CONNECTION)));

    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    bool readOnly = m_connection->GetReadOnly();
    if (readOnly)
		throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

    // verify the feature class name is specified
    if (m_className == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_41_NULL_FEATURE_CLASS)));

    //get feature class
    FdoPtr<FdoClassDefinition> clas = FdoPtr<FdoClassCollection>(
        m_connection->GetSchema()->GetClasses())->FindItem(m_className->GetName());
    if( clas == NULL )
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_75_CLASS_NOTFOUND), m_className->GetName()));

    if( m_filter != NULL )
	{
		FdoPtr<FdoIFilterCapabilities> filterCaps = m_connection->GetFilterCapabilities();
        FdoCommonFilterExecutor::ValidateFilter( clas, m_filter, NULL, filterCaps );
	}

	m_connection->FlushAll( clas, true );

	bool followAssociation = RequiresAssociationHandling( clas );

    //get the R-Tree for this feature class... 
    SdfRTree* rt = m_connection->GetRTree(clas);
    recno_list* features = NULL;
    KeyDb* keys = m_connection->GetKeyDb(clas);

    // Need to re-synch the Rtree root note as it may have changed by a different connection
    // since the Rtree was initialized
	if( rt )
	    rt->UpdateRootNode();

    FdoPtr<FdoFilter>rdrFilter = m_filter;
	FDO_SAFE_ADDREF( m_filter );

    //if we have a filter, perform any spatial and key queries if necessary
    //using the query optimizer
    if (m_filter)
    {
        SdfQueryOptimizer* zer = new SdfQueryOptimizer(rt, keys, clas);
        m_filter->Process(zer);

        features = zer->GetResult();

        rdrFilter = zer->GetOptimizedFilter();

        //need this cast because of multiple inheritance
        ((FdoIFilterProcessor*)zer)->Release();
    }

    FdoPtr<SdfDeletingFeatureReader>delrdr = new SdfDeletingFeatureReader(m_connection, clas, rdrFilter, features);

    int bodyCount = 0;
    while (delrdr->ReadNext()) 
	{
		if( followAssociation )
			bodyCount += FollowAssociatedObjects( clas, delrdr );

		bodyCount++;
	}

    return bodyCount;
}


// Deleting objects might result in lock conflicts if objects to be deleted
// are not exclusively locked for the user attempting to delete the object.
// In this case a lock conflict report is generated.  The function
// GetLockConflicts returns a lock conflict reader providing access to the
// list of lock conflicts that occurred during the execution of the delete
// operation.
FdoILockConflictReader* SdfDelete::GetLockConflicts()
{
    // no lock support for SDF+
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_40_NO_FEATURE_LOCKING)));
}

bool SdfDelete::RequiresAssociationHandling( FdoClassDefinition* cls )
{
	FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties();

	for(int i=0; i<properties->GetCount(); i++ )
	{
		FdoPtr<FdoPropertyDefinition>prop = properties->GetItem( i );
		if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
		{
			FdoAssociationPropertyDefinition *aprop = (FdoAssociationPropertyDefinition *) prop.p;
			if( ! aprop->GetIsReadOnly() )
			{
				if( aprop->GetDeleteRule() != FdoDeleteRule_Break )
					return true;
			}
		}
	}
	// Check base properties
	FdoPtr<FdoReadOnlyPropertyDefinitionCollection> base_properties = cls->GetBaseProperties();
	for(int i=0; i<base_properties->GetCount(); i++ )
	{
		FdoPtr<FdoPropertyDefinition>prop = base_properties->GetItem( i );
		if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
		{
			FdoAssociationPropertyDefinition *aprop = (FdoAssociationPropertyDefinition *) prop.p;
			if( ! aprop->GetIsReadOnly() )
			{
				if( aprop->GetDeleteRule() != FdoDeleteRule_Break )
					return true;
			}
		}
	}

	return false;
}

int SdfDelete::FollowAssociatedObjects( FdoClassDefinition* cls, SdfSimpleFeatureReader* reader )
{
	int count = 0;
	FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties();
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> base_properties = cls->GetBaseProperties();
	for(int k=0; k<2; k++) // Do this twice; one for the class properties and the second for the base properties.
	{
		int prop_count = ( k == 0 )?properties->GetCount():base_properties->GetCount();
		for(int i=0; i<prop_count; i++ )
		{
			FdoPtr<FdoPropertyDefinition>prop = ( k == 0 )?properties->GetItem( i ):base_properties->GetItem( i );
			if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
			{
				FdoAssociationPropertyDefinition *aprop = (FdoAssociationPropertyDefinition *) prop.p;
				if( aprop->GetIsReadOnly() || reader->IsNull ( aprop->GetName() ) )
						continue;

				FdoPtr<SdfSimpleFeatureReader>objReader = (SdfSimpleFeatureReader*)reader->GetFeatureObject( aprop->GetName() );
				if( objReader.p != NULL  )
				{
					FdoPtr<FdoClassDefinition>assoc_class = aprop->GetAssociatedClass();
					FdoPtr<SdfDeletingFeatureReader>delrdr = new SdfDeletingFeatureReader(m_connection, assoc_class, *objReader );
					if( objReader->ReadNext() )
					{
						if( aprop->GetDeleteRule() == FdoDeleteRule_Prevent )
							throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_69_DELETE_RULE_VIOLATION), aprop->GetName() ));
						
						count += FollowAssociatedObjects( assoc_class, objReader );
					
						if( aprop->GetDeleteRule() == FdoDeleteRule_Cascade )
							while (delrdr->ReadNext())  count++;
					}
				}
			}
		}
	}
	return count;
}


