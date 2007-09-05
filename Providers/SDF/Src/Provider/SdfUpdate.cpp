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
#include "SdfUpdate.h"
#include "SdfConnection.h"
#include "SdfUpdatingFeatureReader.h"
#include "SdfQueryOptimizer.h"
#include "DataDb.h"
#include "FdoExpressionEngine.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs an Update command using the specified connection
SdfUpdate::SdfUpdate(SdfConnection* connection)
: SdfFeatureCommand<FdoIUpdate>(connection)
{
    m_properties = FdoPropertyValueCollection::Create();
}


// default destructor
SdfUpdate::~SdfUpdate()
{
    FDO_SAFE_RELEASE(m_properties);
}


//-------------------------------------------------------
// FdoIUpdate implementation
//-------------------------------------------------------


// Gets the StringDictionary that specifies the names and values of the
// properties to be updated.
FdoPropertyValueCollection* SdfUpdate::GetPropertyValues()
{
    // addref the collection because we own it
    FDO_SAFE_ADDREF(m_properties);
    return m_properties;
}


// Executes the update command and returns the number of modified instances.
FdoInt32 SdfUpdate::Execute()
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
        FdoExpressionEngine::ValidateFilter( clas, m_filter, NULL, filterCaps );
	}
	m_connection->FlushAll( clas, true );

    

    //get the R-Tree for this feature class... 
    SdfRTree* rt = m_connection->GetRTree(clas);
    recno_list* features = NULL;
    KeyDb* keys = m_connection->GetKeyDb(clas);

    // Need to re-synch the Rtree root note as it may have changed by a different connection
    // since the Rtree was initialized
	if( rt )
	    rt->UpdateRootNode();

    FdoFilter* rdrFilter = m_filter;
    bool disposeFilter = false;

    //if we have a filter, perform any spatial and key queries if necessary
    //using the query optimizer
    if (m_filter)
    {
        SdfQueryOptimizer* zer = new SdfQueryOptimizer(rt, keys, clas);
        m_filter->Process(zer);

        features = zer->GetResult();

        rdrFilter = zer->GetOptimizedFilter();
        disposeFilter = true;

        //need this cast because of multiple inheritance
        ((FdoIFilterProcessor*)zer)->Release();
    }

    SdfUpdatingFeatureReader* updaterdr = new SdfUpdatingFeatureReader(m_connection, clas, rdrFilter, features, m_properties);

    int bodyCount = 0;

    while (updaterdr->ReadNext()) bodyCount++;

    updaterdr->Release();

    //we are responsible for calling Release() once on the optimized
    //filter returned by the SdfQueryOptimizer
    if (disposeFilter)
        FDO_SAFE_RELEASE(rdrFilter);

    
    return bodyCount;
}


// Updating objects might result in lock conflicts if objects to be updated
// are not exclusively locked for the user attempting to update the object.
// In this case a lock conflict report is generated.  The function
// GetLockConflicts returns a lock conflict reader providing access to the
// list of lock conflicts that occurred during the execution of the update
// operation.
FdoILockConflictReader* SdfUpdate::GetLockConflicts()
{
    // no lock support for SDF+
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_40_NO_FEATURE_LOCKING)));
}

