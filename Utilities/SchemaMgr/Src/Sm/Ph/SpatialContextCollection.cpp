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
#include "Sm/Ph/SpatialContextCollection.h"
#include <FdoCommonStringUtil.h>

FdoSmPhSpatialContextCollection::FdoSmPhSpatialContextCollection(void)
{
    mIdMap = FdoDictionary::Create();
}

FdoSmPhSpatialContextCollection::~FdoSmPhSpatialContextCollection(void)
{
}

FdoSmPhSpatialContextP FdoSmPhSpatialContextCollection::FindItemById( FdoInt64 scid )
{
    FdoSmPhSpatialContextP sc;

    // Use ID Map to convert id to name and then look up sc by name

    FdoDictionaryElementP elem = mIdMap->FindItem( FdoCommonStringUtil::Int64ToString(scid) );

    if ( elem ) {
        FdoStringP scName = elem->GetValue();

        sc = FindItem(scName);
    }

    return sc;
}

FdoInt32 FdoSmPhSpatialContextCollection::Add( FdoSmPhSpatialContext* value)
{
    FdoInt32 ret = FdoSmNamedCollection<FdoSmPhSpatialContext>::Add(value);

    // Keep id map synchronized
    AddToIdMap(value);

    return ret;
}

FdoInt32 FdoSmPhSpatialContextCollection::FindExistingSC( FdoSmPhSpatialContextP sc )
{
	FdoInt32 index = -1;

	for (int i = 0; i < this->GetCount() && (index == -1); i++ )
	{
		FdoSmPhSpatialContextP  sc1 = this->GetItem(i);

		if ( ( sc1->GetSrid() == sc1->GetSrid() ) &&
			 ( wcscmp(sc1->GetCoordinateSystem(), sc->GetCoordinateSystem()) == 0 ) &&
			 ( wcscmp(sc1->GetCoordinateSystemWkt(), sc->GetCoordinateSystemWkt()) == 0 ) &&
			 ( sc1->GetXYTolerance() == sc->GetXYTolerance() ) &&
			 ( sc1->GetZTolerance() == sc->GetZTolerance() ) )
		{
			FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			FdoPtr<FdoIGeometry>          geom = gf->CreateGeometryFromFgf( sc->GetExtent() );
			FdoPtr<FdoIEnvelope>		  env = geom->GetEnvelope();
			FdoPtr<FdoIGeometry>          geom1= gf->CreateGeometryFromFgf( sc1->GetExtent() );
			FdoPtr<FdoIEnvelope>		  env1 = geom1->GetEnvelope();

			bool found = ( env1->GetMinX() == env->GetMinX() &&
						   env1->GetMaxX() == env->GetMaxX() &&
						   env1->GetMinY() == env->GetMinY() &&
						   env1->GetMaxY() == env->GetMaxY() );
			if ( found )
				index = i;
		}
	}
	return (index);
}

void FdoSmPhSpatialContextCollection::AddToIdMap( FdoSmPhSpatialContext* sc )
{
    if ( sc->GetId() > -1 ) {
        FdoStringP idKey = FdoCommonStringUtil::Int64ToString( sc->GetId() );
        FdoStringP idVal = sc->GetName();

        FdoDictionaryElementP elem = FdoDictionaryElement::Create( idKey, idVal );

        mIdMap->Add( elem );
    }
}

void FdoSmPhSpatialContextCollection::RemoveFromIdMap( FdoSmPhSpatialContext* sc )
{
    FdoStringP idKey = FdoCommonStringUtil::Int64ToString( sc->GetId() );
    FdoInt32 ix = mIdMap->IndexOf( idKey );

    if ( ix > -1 ) 
        mIdMap->RemoveAt( ix );
}
