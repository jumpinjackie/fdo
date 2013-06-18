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
#include "SdfDataReader.h"




class SdfDistinctDataReader : public SdfDataReader
{
public:

    SDF_API SdfDistinctDataReader(FdoIFeatureReader* reader, FdoIdentifierCollection* idents);
    SDF_API ~SdfDistinctDataReader();

protected:

    SDF_API virtual void Dispose();

public:

    /// <summary>Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    SDF_API virtual bool ReadNext();

    /// <summary>Closes the FdoIDataReader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void Close();

    //------------------------------------------------------------
    // SdfDataReader
    //------------------------------------------------------------

    SDF_API virtual BinaryReader* GetBinaryReader();

private:

    void RunQuery();

    SQLiteTable* m_db;
    SQLiteCursor* m_dbc;
    SQLiteData* m_currentKey;
    SQLiteData* m_currentData;
    BinaryReader* m_binReader;

    FdoIdentifierCollection* m_idents;
    FdoClassDefinition* m_class;
    FdoIFeatureReader* m_reader;

};

