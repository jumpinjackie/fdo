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
#ifndef _SdfScrollableFeatureReader_H_
#define _SdfScrollableFeatureReader_H_

#include "SdfSimpleFeatureReader.h"
#include "RTree.h"

class KeyDb;
class DataDb;
class BinaryWriter;

class SdfScrollableFeatureReader : public SdfSimpleFeatureReader
{

public:
    // constructs an SdfFeatureReader using the specified
    // connection, class definition, and Sdf reader
    SdfScrollableFeatureReader( SdfConnection* connection, FdoClassDefinition* classDef );
	SdfScrollableFeatureReader( SdfConnection* connection, FdoClassDefinition* classDef, SdfConnection* srcConnection, FdoClassDefinition* srcClassDef );

protected:
    // default destructor
    virtual ~SdfScrollableFeatureReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

protected:
    // dispose this object
    virtual void Dispose();


public:

	virtual int Count();

	virtual bool ReadFirst();

	virtual bool ReadLast();

    // Advances the reader to the next item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadFirst or ReadLast to begin accessing any data.
    virtual bool ReadNext();

	// Advances the reader to the previous item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadFirst or ReadLast to begin accessing any data.
    virtual bool ReadPrevious();

	virtual bool ReadAt(FdoPropertyValueCollection* key);

	virtual bool ReadAtIndex( unsigned int recordindex );

	unsigned int IndexOf(FdoPropertyValueCollection* key);

	// Internal function.
	SQLiteData* GetCurrentKey();
protected:

	virtual void InitDataReader();

private:
    DataDb* m_data;

	bool	m_validCursorPosition;
};

#endif
