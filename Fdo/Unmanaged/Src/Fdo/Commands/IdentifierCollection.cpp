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
#include <Fdo/Commands/IdentifierCollection.h>
#include "StringUtility.h"


class FdoIdentifierCollectionMap
{
public:
    // A map keyed by name, to help performance of large collections.
    std::map<FdoStringP,FdoIdentifier*> *mpNameMap;

public:
    FdoIdentifierCollectionMap() : mpNameMap(NULL)
    {
    }

    ~FdoIdentifierCollectionMap()
    {
        if (mpNameMap)
        {
            delete mpNameMap;
            mpNameMap = NULL;
        }
    }
};


FdoIdentifierCollection* FdoIdentifierCollection::Create()
{
	return new FdoIdentifierCollection();
}

void FdoIdentifierCollection::Dispose()
{
    delete this;
}

FdoIdentifier* FdoIdentifierCollection::GetItem(FdoInt32 index)
{
    return FdoCollection<FdoIdentifier, FdoCommandException>::GetItem(index);
}

FdoIdentifier* FdoIdentifierCollection::GetItem(FdoString* name)
{
    FdoIdentifier* item = FindItem( name );
    if ( !item ) 
        throw FdoCommandException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_38_ITEMNOTFOUND),
                name
            )
        );

    return(item);
}

FdoIdentifier* FdoIdentifierCollection::FindItem(FdoString* name)
{
    // trigger the building of a map when the collection reaches the threshold size.
    ((FdoIdentifierCollection*) this)->InitMap();

    FdoIdentifier* obj = NULL;

    if ( mpMap->mpNameMap ) {
        // Accessing the map is faster for large collections, so use it if built.
        obj = GetMap(name);

        // If the found object's name is the same as the given name
        // then we're done. Otherwise, this object's name has changed
        // and a linear search is needed to find the requested object.
        if ( (obj != NULL) && (Compare(obj->GetText(), name) != 0) )
            FDO_SAFE_RELEASE( obj );
    }

    if ( obj == NULL ) {
        // No map so do linear search.
        for ( FdoInt32 i = 0; i < FdoCollection<FdoIdentifier, FdoCommandException>::GetCount(); i++ ) {
            FdoIdentifier* obj = GetItem(i);

            if ( Compare(name, obj->GetText()) == 0 )
                return(obj);

            FDO_SAFE_RELEASE(obj);
        }
    }

    return (obj);
}

void FdoIdentifierCollection::SetItem(FdoInt32 index, FdoIdentifier* value)
{
    CheckDuplicate( value, index );

    // Remove the old item from the map 
    if ( mpMap->mpNameMap ) 
        RemoveMapAt(index);

    // Add the new item to the map
    if ( mpMap->mpNameMap && value ) 
        InsertMap( value );

    // Set the new item in the collection.
    FdoCollection<FdoIdentifier, FdoCommandException>::SetItem(index, value);
}

FdoInt32 FdoIdentifierCollection::Add( FdoIdentifier* value)
{
    CheckDuplicate( value, -1 );

    // Insert the new item in the map
    if ( mpMap->mpNameMap && value ) {
        InsertMap(value);
    }

    // add it to the list.
    return( FdoCollection<FdoIdentifier, FdoCommandException>::Add(value) );
}

void FdoIdentifierCollection::Insert( FdoInt32 item, FdoIdentifier* value)
{
    CheckDuplicate( value, -1 );

    // Insert the new item in the map
    if ( mpMap->mpNameMap ) {
        InsertMap(value);
    }

    // Insert it in the list
    return( FdoCollection<FdoIdentifier, FdoCommandException>::Insert(item, value) );
}

void FdoIdentifierCollection::Clear()
{
    // Clear the map
    if ( mpMap->mpNameMap ) {
        delete mpMap->mpNameMap;
        mpMap->mpNameMap = NULL;
    }

    // Clear the list
    FdoCollection<FdoIdentifier, FdoCommandException>::Clear();
}

void FdoIdentifierCollection::Remove(const FdoIdentifier* value)
{
    // Remove the item from the map.
    if ( mpMap->mpNameMap ) 
        RemoveMap( value );

    // Remove it from the list
    FdoCollection<FdoIdentifier, FdoCommandException>::Remove(value);
}

void FdoIdentifierCollection::RemoveAt(FdoInt32 index)
{
    // Remove the item from the map.
    if ( mpMap->mpNameMap ) 
        RemoveMapAt(index);

    // Remove it from the list
    FdoCollection<FdoIdentifier, FdoCommandException>::RemoveAt(index);
}

bool FdoIdentifierCollection::Contains(const FdoIdentifier* value)
{
    // trigger the building of a map when the collection reaches the threshold size.
    ((FdoIdentifierCollection*) this)->InitMap();

    if ( mpMap->mpNameMap )
    {
        // If map is built, use it since it is faster. 
        FdoPtr <FdoIDisposable> temp = GetMap (((FdoIdentifier*)value)->GetText());
        bool ret = (temp != NULL);
        return (ret);
    }
    else // Otherwise, linear search
    {
        FdoString * valueName = ((FdoIdentifier*)value)->GetText();
        FdoInt32 count = FdoCollection<FdoIdentifier, FdoCommandException>::GetCount();
        bool ret = false;
        for (FdoInt32 i = 0; !ret && i < count; i++)
        {
            FdoPtr<FdoIdentifier> item = GetItem(i);
            FdoString * itemName = item->GetText();
            ret = (Compare(itemName, valueName)==0);
        }
        return ret;
    }
}

bool FdoIdentifierCollection::Contains(FdoString* name)
{
    FdoIdentifier* item = FindItem(name);
    bool found = (item != NULL);

    FDO_SAFE_RELEASE(item);

    return(found);
}

FdoInt32 FdoIdentifierCollection::IndexOf(const FdoIdentifier* value)
{
    return(FdoCollection<FdoIdentifier, FdoCommandException>::IndexOf(value));
}

FdoInt32 FdoIdentifierCollection::IndexOf(FdoString* name)
{
    if (name == NULL)
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    FdoInt32    size = FdoCollection<FdoIdentifier, FdoCommandException>::GetCount();
    for (FdoInt32 i = 0; i < size; i++)
    {
        FdoPtr<FdoIdentifier> pitem = GetItem(i);
        if (pitem != NULL && pitem->GetText() != (FdoString*) NULL && Compare(name, pitem->GetText()) == 0) {
            return i;
        }
    }

    return(-1);
}


FdoIdentifierCollection::FdoIdentifierCollection( bool caseSensitive )
{
    mbCaseSensitive = caseSensitive;
    mpMap = new FdoIdentifierCollectionMap();
}

FdoIdentifierCollection::~FdoIdentifierCollection(void)
{
    if (mpMap) 
    {
        delete mpMap;
        mpMap = NULL;
    }
}

int FdoIdentifierCollection::Compare( FdoString* str1, FdoString* str2 ) const
{
    if ( mbCaseSensitive )
        return ( wcscmp(str1, str2) );

// Try case-insensitive comparison.
#ifdef _WIN32
    return ( _wcsicmp(str1, str2) );
#else
    return ( wcscasecmp(str1, str2) );
#endif
}

void FdoIdentifierCollection::CheckDuplicate( FdoIdentifier* item, FdoInt32 index )
{
    FdoPtr<FdoIdentifier> foundItem1 = FindItem( item->GetText() );
    FdoPtr<FdoIdentifier> foundItem2;

    if ( index >= 0 ) {
        foundItem2 = GetItem(index);
    }

    if ( (foundItem1 !=NULL) && (foundItem1.p != foundItem2.p ) ) { 
        throw FdoCommandException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_45_ITEMINCOLLECTION),
                (FdoString*) item->GetText()
            )
        );
    }
}

void FdoIdentifierCollection::InitMap()
{
    // Build the map if not already built and the collection has hit the threshold
    if ( !mpMap->mpNameMap && ( FdoCollection<FdoIdentifier, FdoCommandException>::GetCount() > FDO_COLL_MAP_THRESHOLD ) ) {
        mpMap->mpNameMap = new std::map<FdoStringP,FdoIdentifier*>();

        // Put all the current elements into the map
        for ( FdoInt32 i = (FdoCollection<FdoIdentifier, FdoCommandException>::GetCount() - 1); i >= 0; i-- ) 
            InsertMap( FdoPtr<FdoIdentifier>(GetItem(i)) );
    }
}

void FdoIdentifierCollection::InsertMap( FdoIdentifier* value ) const
{
    // Add an element to the map. Elements are keyed by name, which may or may not be case sensitive.
    // Case insensitive names are stored in lower case.
    if ( mbCaseSensitive ) 
        mpMap->mpNameMap->insert( std::pair<FdoStringP,FdoIdentifier*> ( value->GetText(), value ) );
    else
        mpMap->mpNameMap->insert( std::pair<FdoStringP,FdoIdentifier*> ( FdoStringP(value->GetText()).Lower(), value ) );            
}

void FdoIdentifierCollection::RemoveMapAt( FdoInt32 index )
{
    // Remove the old item from the map 
    FdoIdentifier* pItem = FdoCollection<FdoIdentifier, FdoCommandException>::GetItem(index);

    if ( pItem ) {
        RemoveMap( pItem );
        pItem->Release();
    }
}

void FdoIdentifierCollection::RemoveMap( const FdoIdentifier* value )
{
    // handle the current case sensitivity of the element name.
    if ( mbCaseSensitive ) 
        mpMap->mpNameMap->erase( FdoStringP( ((FdoIdentifier*)value)->GetText() ) );
    else
        mpMap->mpNameMap->erase( FdoStringP( ((FdoIdentifier*)value)->GetText() ).Lower() );            
}

FdoIdentifier* FdoIdentifierCollection::GetMap( FdoString* name ) const
{
    FdoIdentifier* pItem = NULL;

    std::map <FdoStringP,FdoIdentifier*> :: const_iterator iter;

    if ( mbCaseSensitive )
        iter = mpMap->mpNameMap->find( FdoStringP(name) );
    else 
        // Case sensitive names are stored in lower case.
        iter = mpMap->mpNameMap->find( FdoStringP(name).Lower() );

    if ( iter != mpMap->mpNameMap->end() ) { 
        pItem = (FdoIdentifier*) iter->second;
        FDO_SAFE_ADDREF(pItem);
    }

    return( pItem );
}
