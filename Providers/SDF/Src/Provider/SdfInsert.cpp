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
#include "SdfInsert.h"

#include <FdoSpatial.h>
#include "SdfConnection.h"
#include "DataIO.h"
#include "DataDb.h"
#include "KeyDb.h"
#include "SdfSimpleFeatureReader.h"
#include "PropertyIndex.h"
#include "SdfDataValidator.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs an Insert command using the specified connection
SdfInsert::SdfInsert(SdfConnection* connection)
: SdfCommand<FdoIInsert>(connection)
{
    m_connection = connection;
    m_ClassName = NULL;
	m_ValidationFlag = SdfDataValidationType_None;
    m_Properties = FdoPropertyValueCollection::Create();
	m_NeedSync = true;
}


// default destructor
SdfInsert::~SdfInsert()
{
    FDO_SAFE_RELEASE(m_connection);
    FDO_SAFE_RELEASE(m_ClassName);
    FDO_SAFE_RELEASE(m_Properties);
}

//-------------------------------------------------------
// FdoIInsert implementation
//-------------------------------------------------------


// Gets the name of the class to be operated upon as an Identifier.
FdoIdentifier* SdfInsert::GetFeatureClassName()
{
    // addref the class name because we own it
    FDO_SAFE_ADDREF(m_ClassName);
    return m_ClassName;
}


// Sets the name of the class to be operated upon as an Identifier.
void SdfInsert::SetFeatureClassName(FdoIdentifier* value)
{
    FDO_SAFE_RELEASE(m_ClassName);

    m_ClassName = value;

    FDO_SAFE_ADDREF(m_ClassName);

	// Initialize the validation flag
	m_ValidationFlag = SdfDataValidationType_None;
	if( m_ClassName != NULL )
	{
		FdoFeatureSchema* schema = m_connection->GetSchema();
        if (!schema)
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_96_SCHEMA_NOT_SET)));

		FdoPtr<FdoClassCollection> classes = schema->GetClasses();
		m_ValidationFlag = SdfDataValidator::ValidationFlag( FdoPtr<FdoClassDefinition>(classes->GetItem(m_ClassName->GetName()) ) );
	}

	m_NeedSync = true;
}


// Sets the name of the class to be operated upon as an Identifier.
void SdfInsert::SetFeatureClassName(FdoString* value)
{
    FDO_SAFE_RELEASE(m_ClassName);

    m_ClassName = NULL;

    if (value != NULL)
        m_ClassName = FdoIdentifier::Create(value);

	// Initialize the validation flag
	m_ValidationFlag = SdfDataValidationType_None;
	if( m_ClassName != NULL )
	{
		FdoFeatureSchema* schema = m_connection->GetSchema();
        if (!schema)
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_96_SCHEMA_NOT_SET)));
		FdoPtr<FdoClassCollection> classes = schema->GetClasses();
		m_ValidationFlag = SdfDataValidator::ValidationFlag( FdoPtr<FdoClassDefinition>(classes->GetItem(m_ClassName->GetName()) ) );
	}

	m_NeedSync = true;
}


// Gets the PropertyValueCollection that specifies the names and values
// of the properties for the instance to be inserted.  Returns the list
// of properties and their values.
FdoPropertyValueCollection* SdfInsert::GetPropertyValues()
{
    // addref the collection because we own it
    FDO_SAFE_ADDREF(m_Properties);
    return m_Properties;
}


// Gets the BatchParameterValueCollection that can be used for optimized
// batch inserts of multiple features with a single insert command.  Batch
// inserts can be performed by using Parameters for each of the property
// values, then adding collections of parameter values to the
// BatchParameterValueCollection.  Each ParameterValueCollection in the
// BatchParameterValueCollection should contain one ParameterValue for
// each of the parameters specified for property values.
FdoBatchParameterValueCollection* SdfInsert::GetBatchParameterValues()
{
    // this command has no parameters
    return NULL;
}


// Executes the insert command and returns a reference to an IFeatureReader.
// Some feature providers can generate automatic identity values for features.
// This will happen automatically as the features are inserted.  The returned
// IFeatureReader allows the client to obtain the automatic identity property
// value(s) of newly inserted object(s).  The returned feature reader at a
// minimum will read the unique identity properties of the objects just
// inserted.  Multiple objects will be returned through the reader in the
// case of a batch insert.
FdoIFeatureReader* SdfInsert::Execute()
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

    FdoFeatureSchema* schema = m_connection->GetSchema();

    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    FdoPtr<FdoClassDefinition> clas = classes->GetItem(m_ClassName->GetName());

	if( clas->GetIsAbstract() )
		throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_71_ABSTRACT_CLASS)));

	if( m_NeedSync )
	{
		m_connection->FlushAll( clas, true );
	
		// Re-synch with the on-disk id pool
		m_connection->ReSyncData();
		m_NeedSync = false;
	}

    PropertyIndex* pi = m_connection->GetPropertyIndex(clas);

	if( m_ValidationFlag != SdfDataValidationType_None )
		SdfDataValidator::Validate( m_connection, clas, m_Properties, m_ValidationFlag, false );
    //construct feature key
    BinaryWriter wrtkey(64);

    KeyDb* keydb = m_connection->GetKeyDb(clas);
    SQLiteData key(NULL, 0);
    
    //if there is no autogen ID, we need to ensure given key is unique
    //before inserting feature data into the main database
    if (!pi->HasAutoGen())
    {
        //serialize key record
        //we will keep track of this key so we don't have to 
        //serialize it again once we need to do the insert
        DataIO::MakeKey(clas, pi, m_Properties, wrtkey, 0);

        key.set_data(wrtkey.GetData());
        key.set_size(wrtkey.GetDataLen());
        
        //check if key exists
        if (keydb->KeyExists(&key))
            throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_54_KEY_NOT_UNIQUE)));
    }
    
    //insert feature data into main feature database
	DataDb  *dataDb = m_connection->GetDataDb(clas);
    REC_NO recno = dataDb->InsertFeature(clas, pi, m_Properties);

    //if insert failed we will get 0 back for the
    //feature record number.
    if (recno == 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_11_ERROR_INSERTING_FEATURE)));

    if (pi->HasAutoGen())
    {
        //The returned recno is the unique ID of the 
        //feature. We will need to use it as the autogenerated
        //property value when inserting the key in the key db.

        //serialize key record
        DataIO::MakeKey(clas, pi, m_Properties, wrtkey, recno);

        key.set_data(wrtkey.GetData());
        key.set_size(wrtkey.GetDataLen());
    }

    //insert key into key db
    keydb->InsertKey(&key, recno);
       
    
    //now we need to insert into R-Tree if needed
    SdfRTree* rt = m_connection->GetRTree(clas);

    //only if the class is a feature class w/ geometry it has an R-Tree of course...
    if (rt)
    {
        //get the geometry property and the value of the geometry
        FdoPtr<FdoGeometricPropertyDefinition> gpd = PropertyIndex::FindGeomProp(clas);

        if (gpd.p)
        {
            FdoPtr<FdoPropertyValue> gpv = m_Properties->FindItem(gpd->GetName());
			if( gpv != NULL )
			{
				FdoPtr<FdoGeometryValue> gpgv = (FdoGeometryValue*)gpv->GetValue();

				if( gpgv != NULL )
				{
					FdoPtr<FdoByteArray> fgf = gpgv->GetGeometry();

					if (fgf)
					{
						Bounds bounds;

						FdoSpatialUtility::GetExtents(fgf, bounds.minx, bounds.miny, bounds.maxx, bounds.maxy);

						_ASSERT(bounds.maxx >= bounds.minx);
						_ASSERT(bounds.maxy >= bounds.miny);

						SQLiteData rtkey(&recno, sizeof(REC_NO));

						rt->Insert(bounds, 0, rtkey);
					}
				}
			}
        }
    }

    //return a feature reader with the new feature in it
    recno_list* list = new recno_list;
    list->push_back(recno);

	if( keydb->NeedsAFlush() ||
		dataDb->NeedsAFlush() ||
		(rt && rt->NeedsAFlush() ) )
	{
		m_connection->GetDataBase()->begin_transaction();
		keydb->Flush();
		dataDb->Flush();
		if( rt ) 
			rt->Flush();
		m_connection->GetDataBase()->commit();
	}
    return new SdfSimpleFeatureReader(m_connection, clas, NULL, list, NULL, NULL);
}


