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
#include "SdfDeletingFeatureReader.h"

#include "RTree.h"
#include "BinaryWriter.h"
#include "DataIO.h"
#include "KeyDb.h"
#include "DataDb.h"
#include "SdfConnection.h"


SdfDeletingFeatureReader::SdfDeletingFeatureReader(SdfConnection* connection, FdoClassDefinition* classDef, FdoFilter* filter, recno_list* features)
: SdfSimpleFeatureReader(connection, classDef, filter, features, NULL, NULL)
{
    m_rtree = connection->GetRTree(classDef);
    m_keys = connection->GetKeyDb(classDef);
    m_dbData = connection->GetDataDb(classDef);

    FdoPtr<FdoPropertyDefinition> gpd = PropertyIndex::FindGeomProp(m_class);

    if (gpd)
        m_geomPropName = gpd->GetName();
    else
        m_geomPropName = NULL;

}

SdfDeletingFeatureReader::SdfDeletingFeatureReader( SdfConnection* connection, FdoClassDefinition* classDef, SdfSimpleFeatureReader & reader ):
SdfSimpleFeatureReader( reader )
{
	m_rtree = connection->GetRTree(classDef);
    m_keys = connection->GetKeyDb(classDef);
    m_dbData = connection->GetDataDb(classDef);

    FdoPtr<FdoPropertyDefinition> gpd = PropertyIndex::FindGeomProp(m_class);

    if (gpd)
        m_geomPropName = gpd->GetName();
    else
        m_geomPropName = NULL;
}

SdfDeletingFeatureReader::~SdfDeletingFeatureReader()
{
}

void SdfDeletingFeatureReader::Dispose()
{
    delete this;
}


//overloaded ReadNext() deletes features after positioning reader 
//This function works in two steps. Calls to ReadNext() build up 
//lists of feature record numbers and keys that need to be deleted
//On the last call to ReadNext(), we go over the lists and do 
//the actual deletion. The reason for this is explained further
//below in the comments inside the function
bool SdfDeletingFeatureReader::ReadNext()
{
    //call superclass to navigate to next feature
    bool ret = SdfSimpleFeatureReader::ReadNext();

    //are we done iterating? Then go and actually delete the features
    if (!ret)
    {
        //delete from feature SQLiteTable and R-Tree
        for (unsigned i=0; i<m_recsToDelete.size(); i++)
        {
            REC_NO r = m_recsToDelete[i];
            m_dbData->DeleteFeature(r);

            if (m_rtree && m_geomPropName )
            {
                Bounds b = m_boundsToDelete[i];
				if (!Bounds::IsUndefined(b))
				{
					SQLiteData rtkey(&r, sizeof(REC_NO));
                
					//now perform the R-Tree delete
					m_rtree->Delete(b, rtkey);
				}
            }

        }

        //delete key from KeyDb
        for (unsigned i=0; i<m_keysToDelete.size(); i++)
        {
            BinaryWriter* wrt = m_keysToDelete[i];

            SQLiteData key(wrt->GetData(), wrt->GetDataLen());

            m_keys->DeleteKey(&key);

            delete wrt;
        }

        return false;
    }



    //we cannot delete features while we have a cursor which is
    //reading feature data -- this is because a feature key entry in the 
    //KeyDb for example can be on the same database page as the feature data
    //in that case we cannot delete the feature key while still holding on
    //to the feature data cursor. So instead we make lists of feature keys
    //we need to delete and do it after we are done with the reading cursor.

    //remember record number that we will kill
    m_recsToDelete.push_back(m_currentFeatureRecno);

    //remember key we will delete
    BinaryWriter* wrtkey = new BinaryWriter(16);
    DataIO::MakeKey(m_class, this, *wrtkey);
    m_keysToDelete.push_back(wrtkey);
    
    //remember bounds of feature if we are deleting from the R-Tree
    if (m_rtree && m_geomPropName && !IsNull(m_geomPropName))
    {
        //this gets the bounds of the feature to delete from the R-Tree
		FdoPtr<FdoByteArray> fgf = GetGeometry(m_geomPropName);
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
		FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();
		Bounds bounds(envelope->GetMinX(), envelope->GetMinY(), envelope->GetMaxX(), envelope->GetMaxY());

		m_boundsToDelete.push_back(bounds);
    }
	else
	{
		Bounds bounds;
		m_boundsToDelete.push_back(bounds);
	}

    return true;
}

