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

#include <FdoCommonReader.h>

class FdoWmsSpatialExtentsAggregateReader : public FdoCommonReader<FdoIDataReader>
{
public:

    FdoWmsSpatialExtentsAggregateReader(FdoWmsConnection* con, FdoIdentifier* className, FdoString* aliasName);
    virtual ~FdoWmsSpatialExtentsAggregateReader();

protected:

    virtual void Dispose();

public:

    // Overriden FdoCommonReader methods:
    virtual FdoInt32 GetItemCount(void);
    virtual FdoString* GetItemName(FdoInt32 i);
    virtual FdoInt32 GetItemIndex(FdoString* itemName);
    virtual FdoPropertyType GetItemType(FdoInt32 i);
    virtual FdoDataType GetItemDataType(FdoInt32 i);
    virtual void GetGeometryForCache(FdoIdentifier *itemName, FdoByteArray **byteArray, bool *bIsNull);
    virtual bool ReaderHasData(void);

    // Overriden FdoIReader methods:
    virtual bool ReadNext();
    virtual void Close();

private:

    FdoPtr<FdoIPolygon> m_Extents;  // the geometry that store the spatial extents
    FdoInt32 m_ReaderIndex;  // the current row we are on (-1 == before first row, 0 == first row, etc)
    FdoStringP m_AliasName;
};
