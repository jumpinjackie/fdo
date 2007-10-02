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
#ifndef _SdfIndexedScrollableFeatureReader_H_
#define _SdfIndexedScrollableFeatureReader_H_

#include "SdfScrollableFeatureReader.h"

class SdfIndexedScrollableFeatureReader : public SdfSimpleFeatureReader
{

public:
    // constructs an SdfFeatureReader using the specified
    // connection, class definition, and Sdf reader
    SDF_API SdfIndexedScrollableFeatureReader( SdfConnection* connection, FdoClassDefinition* classDef, 
					FdoIdentifierCollection* selectIdents, FdoPropertyDefinitionCollection* computedProps, REC_NO *table, int tableSize  );

protected:
    // default destructor
    SDF_API virtual ~SdfIndexedScrollableFeatureReader();

  

protected:
    // dispose this object
    SDF_API virtual void Dispose();

public:

	SDF_API virtual int Count();

	SDF_API virtual bool ReadFirst();

	SDF_API virtual bool ReadLast();

    // Advances the reader to the next item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadFirst or ReadLast to begin accessing any data.
    SDF_API virtual bool ReadNext();

	// Advances the reader to the previous item and returns true if there is
    // another object to read or false if reading is complete.  The default
    // position of the reader is prior to the first item.  Therefore you
    // must call ReadFirst or ReadLast to begin accessing any data.
    SDF_API virtual bool ReadPrevious();

	SDF_API virtual bool ReadAt(FdoPropertyValueCollection* key);

	SDF_API virtual bool  ReadAtIndex( unsigned int recordindex );

	SDF_API unsigned int IndexOf(FdoPropertyValueCollection* key);

protected:

	virtual void InitDataReader();

private:
	void InitCurrentKey();

    DataDb* m_Sourcedata;
	KeyDb* m_Sourcekey;

	FdoPtr<FdoClassDefinition> m_SrcClass;

	FdoPtr<SdfScrollableFeatureReader> m_IndexReader;

	REC_NO	*m_IndexTable;

	int m_TableSize;

	int m_CurrentIndex;

    bool m_IdSingleAutogen;
};

#endif
