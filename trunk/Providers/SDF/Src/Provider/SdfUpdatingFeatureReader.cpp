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
#include "PropertyIndex.h"
#include "SdfUpdatingFeatureReader.h"

#include <FdoSpatial.h>
#include "RTree.h"
#include "KeyDb.h"
#include "SdfConnection.h"
#include "DataIO.h"
#include "DataDb.h"

#include "SdfDataValidator.h"

SdfUpdatingFeatureReader::SdfUpdatingFeatureReader(SdfConnection* connection, 
                                                   FdoClassDefinition* classDef, 
                                                   FdoFilter* filter, 
                                                   recno_list* features, 
                                                   FdoPropertyValueCollection* pvc)
: SdfSimpleFeatureReader(connection, classDef, filter, features, NULL, NULL)
{
    m_rtree = connection->GetRTree(classDef);
    m_keys = connection->GetKeyDb(classDef);
    m_data = connection->GetDataDb(classDef);

    m_propVals = pvc;

    //determine whether the user is updating any identity properties -- we would
    //need to update the KeyDb in that case
    FdoPtr<FdoDataPropertyDefinitionCollection> idprops = DataIO::FindIDProps(classDef);

    m_bUpdateKey = false;

    for (int i=0; i<m_propVals->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue> pv = m_propVals->GetItem(i);

        FdoString* name = FdoPtr<FdoIdentifier>(pv->GetName())->GetName();

        FdoPtr<FdoDataPropertyDefinition> dummy = idprops->FindItem(name);
		if( dummy != NULL )
		{
			m_bUpdateKey = true;
			break;
		}
    }

	int validationFlag = SdfDataValidator::ValidationFlag( classDef );
	if(  validationFlag != SdfDataValidationType_None )
		SdfDataValidator::Validate( m_connection, classDef, pvc, validationFlag, true );

    //determine whether the user is changing the geometry property -- we would
    //need to update the R-Tree in that case
    m_bUpdateGeom = false;

    if (m_class->GetClassType() == FdoClassType_FeatureClass)
    {
        FdoPtr<FdoPropertyDefinition> gpd = PropertyIndex::FindGeomProp(m_class);
        
        if (gpd)
        {
            m_geomPropName = gpd->GetName();

            FdoPtr<FdoPropertyValue> dummy = m_propVals->FindItem(m_geomPropName);

            if (dummy.p)
                m_bUpdateGeom = true;
        }
    }
}

SdfUpdatingFeatureReader::~SdfUpdatingFeatureReader()
{
}

void SdfUpdatingFeatureReader::Dispose()
{
    delete this;
}


//overloaded ReadNext() updates features after positioning reader 
bool SdfUpdatingFeatureReader::ReadNext()
{
    //call superclass to navigate to next feature
    bool ret = SdfSimpleFeatureReader::ReadNext();

    //are we done?
    if (!ret)
        return false;

    //if there is an identity property in the value collection,
    //the key database will need updating
    if (m_bUpdateKey)
    {
        //old key val
        BinaryWriter wrt(16);
        DataIO::MakeKey(m_class, this, wrt);

        //new key val
        BinaryWriter wrt2(16);
        DataIO::UpdateKey(m_class, m_propVals, this, wrt2);

        //update key only if the key is actually changing
        int len1 = wrt.GetDataLen();
        int len2 = wrt2.GetDataLen();
        int len = min(len1, len2);
        if ((len1 != len2) || 0 != memcmp(wrt.GetData(), wrt2.GetData(), len))
        {
            SQLiteData oldkey(wrt.GetData(), wrt.GetDataLen());
            SQLiteData newkey(wrt2.GetData(), wrt2.GetDataLen());

            //make sure user did not rename a key to equal an 
            //existing key
            if (m_keys->KeyExists(&newkey))
                throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_54_KEY_NOT_UNIQUE)));

            //remove old and insert new key
            m_keys->DeleteKey(&oldkey);
            m_keys->InsertKey(&newkey, m_currentFeatureRecno);
        }

    }


    //if geometry changes we need to update R-Tree as well
    if (m_bUpdateGeom)
    {
        SQLiteData rtkey(&m_currentFeatureRecno, sizeof(REC_NO));
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        if (!IsNull(m_geomPropName))
        {
            //this gets the bounds of the feature to delete from the R-Tree
            FdoPtr<FdoByteArray> fgf = GetGeometry(m_geomPropName);

			Bounds oldbounds;
			FdoSpatialUtility::GetExtents(fgf, oldbounds.minx, oldbounds.miny, oldbounds.maxx, oldbounds.maxy);

			//remove old record/bounds from R-Tree
			m_rtree->Delete(oldbounds, rtkey);
        }

        //now compute the new bounds for the feature
		FdoPtr<FdoGeometryValue> gv;
		FdoPtr<FdoPropertyValue>geomProp = m_propVals->FindItem(m_geomPropName);
		if( geomProp != NULL )
			gv = dynamic_cast<FdoGeometryValue*>(geomProp->GetValue());

        if (gv)
        {
            FdoPtr<FdoByteArray> fgf = gv->GetGeometry();

			if (fgf)
			{
				Bounds newbounds;
				FdoSpatialUtility::GetExtents(fgf, newbounds.minx, newbounds.miny, newbounds.maxx, newbounds.maxy);

				//add new bounds to RTree
				m_rtree->Insert(newbounds, 0, rtkey);
			}
        }
    }


    //finally update current feature record,
    //by overwriting the current record   
    BinaryWriter wrtdata(256);

    DataIO::UpdateDataRecord(m_class, m_propIndex, m_propVals, this, wrtdata);
    SQLiteData data(wrtdata.GetData(), wrtdata.GetDataLen());

    m_dbData->UpdateFeature(m_currentFeatureRecno, &data);

	if( m_keys->NeedsAFlush() ||
		m_dbData->NeedsAFlush() ||
		(m_rtree && m_rtree->NeedsAFlush() ) )
	{
		m_connection->GetDataBase()->begin_transaction();
		m_keys->Flush();
		m_dbData->Flush();
		if( m_rtree )
			m_rtree->Flush();
		m_connection->GetDataBase()->commit();
	}
    return true;
}

