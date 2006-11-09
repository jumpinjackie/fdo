// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "SdfSelect.h"
#include "SdfConnection.h"
#include "SdfQueryOptimizer.h"
#include "SdfSimpleFeatureReader.h"
#include "FilterExecutor.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs a Select command using the specified connection
SdfSelect::SdfSelect(SdfConnection* connection)
: SdfFeatureCommand<FdoISelect>(connection)
{
    m_properties = FdoIdentifierCollection::Create();
}

// default constructor
SdfSelect::SdfSelect()
: SdfFeatureCommand<FdoISelect>(NULL)
{
}

// default destructor
SdfSelect::~SdfSelect()
{
    FDO_SAFE_RELEASE(m_properties);
}

//-------------------------------------------------------
// FdoISelect implementation
//-------------------------------------------------------


// Gets the IdentifierCollection that holds the list of property names to
// return with the result.  If empty all properties of the specified class
// are returned.
FdoIdentifierCollection* SdfSelect::GetPropertyNames()
{
    // addref the collection because we own it
    FDO_SAFE_ADDREF(m_properties);
    return m_properties;
}


// Gets the LockType value (see "Feature Locking Commands").
FdoLockType SdfSelect::GetLockType()
{
    // no lock support for SDF
    return FdoLockType_Exclusive;
}


// Sets the LockType value (see "Feature Locking Commands").
void SdfSelect::SetLockType(FdoLockType value)
{
    // no lock support for SDF
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_40_NO_FEATURE_LOCKING)));
}


// Gets the LockStrategy value (see "Feature Locking Commands").
FdoLockStrategy SdfSelect::GetLockStrategy()
{
    // no lock support for SDF
    return FdoLockStrategy_All;
}


// Sets the LockStrategy value (see "Feature Locking Commands").
void SdfSelect::SetLockStrategy(FdoLockStrategy value)
{
    // no lock support for SDF
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_40_NO_FEATURE_LOCKING)));
}


// Executes the select command and returns a reference to an IFeatureReader.
FdoIFeatureReader* SdfSelect::Execute()
{
    // verify connection
    if (m_connection == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_39_NO_CONNECTION)));

    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    // verify the feature class name is specified
    if (m_className == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_41_NULL_FEATURE_CLASS)));

    //get feature class
    FdoPtr<FdoClassDefinition> clas = FdoPtr<FdoClassCollection>(
        m_connection->GetSchema()->GetClasses())->GetItem(m_className->GetName());

    //get the R-Tree for this feature class... 
    SdfRTree* rt = m_connection->GetRTree(clas);
    recno_list* features = NULL;
    KeyDb* keys = m_connection->GetKeyDb(clas);

    FdoFilter* rdrFilter = m_filter;
    bool disposeFilter = false;

	m_connection->FlushAll( clas );

    // Need to re-synch the Rtree root note as it may have changed by a different connection
    // since the Rtree was initialized
    if( rt )
	    rt->UpdateRootNode();

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

    
    PropertyIndex* pi = m_connection->GetPropertyIndex(clas);

    //check for computed properties
    FdoPtr<FdoPropertyDefinitionCollection> pdc = ProcessComputedIdentifiers(pi, clas, rdrFilter, features);
	FdoIFeatureReader* ret = new SdfSimpleFeatureReader(m_connection, clas, rdrFilter, features, m_properties, pdc);
    //FdoIFeatureReader* ret = new SdfSimpleFeatureReader(m_connection, clas, rdrFilter, features, NULL, NULL);


    //we are responsible for calling Release() once on the optimized
    //filter returned by the SdfQueryOptimizer
    if (disposeFilter)
        FDO_SAFE_RELEASE(rdrFilter);

    return ret;
}


// Executes the select command and returns a reference to an IFeatureReader.
FdoIFeatureReader* SdfSelect::ExecuteWithLock()
{
    // no lock support for SDF
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_40_NO_FEATURE_LOCKING)));
}


// When executing the operation ExecuteWithLock lock conflicts may occur.
// Those lock conflicts are reported.  The function GetLockConflicts returns
// a lock conflict reader providing access to the list of lock conflicts
// that occurred during the execution of the operation.
FdoILockConflictReader* SdfSelect::GetLockConflicts()
{
    // no lock support for SDF
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_40_NO_FEATURE_LOCKING)));
}


//returns a collection of property definitions corresponding to 
//computed identifiers added by the user to the Select command's
//identifier collection
FdoPropertyDefinitionCollection* SdfSelect::ProcessComputedIdentifiers(PropertyIndex* pi, 
                FdoClassDefinition* clas, FdoFilter* rdrFilter, recno_list* &features)
{
    SdfSimpleFeatureReader* temprdr = NULL; //can't use FdoPtr because of Linux build
    FilterExecutor* tempfe = NULL;
    FdoPropertyDefinitionCollection* pdc = NULL;

    //check for any computed properties
    //those need special handling -- we need to find out their data type
    //and then create placeholder property definitions for the feature 
    //class to be returned with the feature reader.
    //The scheme used to find the data type of a computed property is to 
    //actually evaluate the expression for one feature and see what comes out
    //of the expression engine. It may be a kludge, but it sure beats writing
    //large amounts of code.
    for (int i=0; i<m_properties->GetCount(); i++)
    {
        FdoPtr<FdoIdentifier> ident = m_properties->GetItem(i);

        //see if the property is in the class' collection,
        //otherwise it must be a computed property
        PropertyStub* ps = pi->GetPropInfo(ident->GetName());

        if (ps == NULL)
        {
            //it is a computed property
            FdoComputedIdentifier* cident = dynamic_cast<FdoComputedIdentifier*>(ident.p);
            if (cident == NULL)
                throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_57_INVALID_PROPERTY_NAME_ARG), ident->GetName()));

            if (!temprdr)
            {
                //we need to passin the identifier collection to the feature reader
                //because there might be computed identifiers which depend on other
                //computed identifiers
                temprdr = new SdfSimpleFeatureReader(m_connection, clas, rdrFilter, features, m_properties, NULL);
                if (!temprdr->ReadNext())
                {
                    //query returned no features... skip computed identifiers stuff
                    //and return nothing
                    if (tempfe)
                        ((FdoIExpressionProcessor*)tempfe)->Release();

					// get a copy of the features object
					recno_list rec;
					bool bFeaturesNULL = (features == NULL);
					if (!bFeaturesNULL)
						for (unsigned int i=0; i<features->size(); i++)
							rec.push_back(features->at(i));
					// the follow deletes the feature object
                    FDO_SAFE_RELEASE(temprdr);
					// restores the features object
					if (!bFeaturesNULL)
					{
						recno_list* rec_list = new recno_list;
						for (unsigned int i=0; i<rec.size(); i++)
							rec_list->push_back(rec[i]);
						features = rec_list;
					}
                    return NULL;
                }
            }

            if (!tempfe)
                tempfe = new FilterExecutor(temprdr, pi, m_properties, clas);

            if (!pdc)
                pdc = FdoPropertyDefinitionCollection::Create(NULL);

            FdoPtr<FdoExpression> expr = cident->GetExpression();

            expr->Process(tempfe);

            FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(cident->GetName(), NULL);

            switch(tempfe->GetResultType())
            {
            case Dvt_Int64:
                tempfe->Reset();
                dpd->SetDataType(FdoDataType_Int64);
                break;
            case Dvt_String:
                tempfe->Reset();
                dpd->SetDataType(FdoDataType_String);
                break;
            case Dvt_Boolean:
                tempfe->Reset();
                dpd->SetDataType(FdoDataType_Boolean);
                break;
            case Dvt_Double:
                tempfe->Reset();
                dpd->SetDataType(FdoDataType_Double);
                break;
            case Dvt_DateTime:
                tempfe->Reset();
                dpd->SetDataType(FdoDataType_DateTime);
                break;
            default:
                tempfe->Reset();
                dpd->SetDataType(FdoDataType_String);
                break;
            }

            pdc->Add(dpd);
        }
    }

    if (tempfe)
        ((FdoIExpressionProcessor*)tempfe)->Release();

	if (temprdr)
	{
		// get a copy of the features object
		recno_list rec;
		bool bFeaturesNULL = (features == NULL);
		if (!bFeaturesNULL)
			for (unsigned int i=0; i<features->size(); i++)
				rec.push_back(features->at(i));
		// the follow deletes the feature object
		FDO_SAFE_RELEASE(temprdr);

		// restores the features object
		if (!bFeaturesNULL)
		{
			recno_list* rec_list = new recno_list;
			for (unsigned int i=0; i<rec.size(); i++)
				rec_list->push_back(rec[i]);
			features = rec_list;
		}
	}
    
    return pdc;
}


