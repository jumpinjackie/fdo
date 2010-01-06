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
#include <Sm/Ph/CoordinateSystemCollection.h>
#include <FdoCommonStringUtil.h>

FdoSmPhCoordinateSystemCollection::FdoSmPhCoordinateSystemCollection(void)
{
    mIdMap = FdoDictionary::Create();
}

FdoSmPhCoordinateSystemCollection::~FdoSmPhCoordinateSystemCollection(void)
{
}

FdoSmPhCoordinateSystemP FdoSmPhCoordinateSystemCollection::FindItemById( FdoInt64 srid )
{
    FdoSmPhCoordinateSystemP coordSys;

    // Use ID Map to convert id to name and then look up coordinate system by name

    FdoDictionaryElementP elem = mIdMap->FindItem( FdoCommonStringUtil::Int64ToString(srid) );

    if ( elem ) {
        FdoStringP csName = elem->GetValue();

        coordSys = FindItem(csName);
    }

    return coordSys;
}

FdoSmPhCoordinateSystemP FdoSmPhCoordinateSystemCollection::FindItemByWkt( FdoStringP wkt )
{
    // TODO: try putting wkt's in a map to see if performance improves. However, 
    // wkt's are long strings so needs to be tried to see if helps performance.

    FdoInt32 idx;
    FdoSmPhCoordinateSystemP coordSys;

    for ( idx = 0; idx < GetCount(); idx++ ) {
        FdoSmPhCoordinateSystemP item = GetItem( idx );
        if ( wkt == item->GetWkt() ) {
            coordSys = item;
            break;
        }
    }

    return coordSys;
}

FdoInt32 FdoSmPhCoordinateSystemCollection::Add( FdoSmPhCoordinateSystem* value)
{
    FdoInt32 ret = FdoSmNamedCollection<FdoSmPhCoordinateSystem>::Add(value);

    // Keep id map synchronized
    AddToIdMap(value);

    return ret;
}


void FdoSmPhCoordinateSystemCollection::AddToIdMap( FdoSmPhCoordinateSystem* coordSys )
{
    if ( coordSys->GetSrid() > -1 ) {
        FdoStringP idKey = FdoCommonStringUtil::Int64ToString( coordSys->GetSrid() );
        FdoStringP idVal = coordSys->GetName();

        FdoDictionaryElementP elem = FdoDictionaryElement::Create( idKey, idVal );

        mIdMap->Add( elem );
    }
}

void FdoSmPhCoordinateSystemCollection::RemoveFromIdMap( FdoSmPhCoordinateSystem* coordSys )
{
    FdoStringP idKey = FdoCommonStringUtil::Int64ToString( coordSys->GetSrid() );
    FdoInt32 ix = mIdMap->IndexOf( idKey );

    if ( ix > -1 ) 
        mIdMap->RemoveAt( ix );
}
