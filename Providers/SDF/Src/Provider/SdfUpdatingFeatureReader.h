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
#include "SdfSimpleFeatureReader.h"
#include "RTree.h"

class KeyDb;
class DataDb;
class BinaryWriter;

class SdfUpdatingFeatureReader : public SdfSimpleFeatureReader
{

public:
    // constructs an SdfFeatureReader using the specified
    // connection, class definition, and Sdf reader
    SDF_API SdfUpdatingFeatureReader(SdfConnection* connection, FdoClassDefinition* classDef, 
        FdoFilter* filter, recno_list* features, FdoPropertyValueCollection* pvc);

protected:
    // default destructor
    SDF_API virtual ~SdfUpdatingFeatureReader();

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
    DataDb* m_data;

    FdoPropertyValueCollection* m_propVals;
    bool m_bUpdateKey;
    bool m_bUpdateGeom;
    FdoString* m_geomPropName;

};


