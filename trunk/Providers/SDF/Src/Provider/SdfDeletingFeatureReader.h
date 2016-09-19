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
#include <vector>
#include "SdfSimpleFeatureReader.h"
#include "RTree.h"

class KeyDb;
class DataDb;
class BinaryWriter;

class SdfDeletingFeatureReader : public SdfSimpleFeatureReader
{

public:
    // constructs an SdfFeatureReader using the specified
    // connection, class definition, and Sdf reader
    SDF_API SdfDeletingFeatureReader(SdfConnection* connection, FdoClassDefinition* classDef, FdoFilter* filter, recno_list* features);

	SdfDeletingFeatureReader( SdfConnection* connection, FdoClassDefinition* classDef, SdfSimpleFeatureReader & reader );

protected:
    SDF_API SdfDeletingFeatureReader() {};
    // default destructor
    SDF_API virtual ~SdfDeletingFeatureReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

protected:
    // dispose this object
    SDF_API virtual void Dispose();


public:

    // Advances the reader to the next item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadNext to begin accessing any data.
    SDF_API virtual bool ReadNext();

private:

    SdfRTree* m_rtree;
    KeyDb* m_keys;
    FdoString* m_geomPropName;

    recno_list m_recsToDelete;
    std::vector<Bounds> m_boundsToDelete;
    std::vector<BinaryWriter*> m_keysToDelete;
};


