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
#ifndef SDF_TABLEREFORMATTER_H
#define SDF_TABLEREFORMATTER_H

#include "DataDb.h"
#include "KeyDb.h"
#include "RTree.h"
#include "PropertyIndex.h"

class SdfConnection;

// This class handles the updating of table rows when the class definition for the 
// table has been changed.
// Currently, only the data table needs updating. However, rtree and key table modifications
// might be required in the future if more types of class definition modifications are
// supported.
class TableReformatter: public FdoIDisposable
{
public:

    TableReformatter( 
        FdoString* name,                // data table name
        SdfConnection* connection,      // current connection
        DataDb* origData,               // current data table.
        KeyDb* origKeys,                // current key table
        SdfRTree* origRTree,            // current spatial index
        FdoFeatureSchema* newSchema     // new schemas (with updated class definitions).
    );
    virtual ~TableReformatter();

    // Update the blobs for all rows in the data table to be consistent with the 
    // new definition for the class.
    void Reformat();

    // Rollback any changes performed by Reformat.
    void Rollback();

    // When ModClassId is set, references to classId are updated when blobs are reformatted
    bool GetModClassId();
    void SetModClassid( bool modClassId );

	// When new nullable properties are added, the data blob need to be updated to insert the new null values
    bool GetAddedProperties();
    void SetAddedProperties( bool addedProps );

    FdoString* GetName();

    bool CanSetName() 
    {
        return false;
    }

    static const FdoInt32 stateInitial = 0;
    static const FdoInt32 stateFinal = 1;

protected:
    /// 0-argument constructor to please FdoPtr::operator->() :
    TableReformatter () { }

    virtual void Dispose()
    {
        delete this;
    }
private: 

    SQLiteTable* OpenBackupTable( bool bCreate );
    void DropBackupTable();
    FdoStringP GetBackupTableName();

    FdoStringP mName;
    SdfConnection* mConnection;
    FdoFeatureSchema* mNewSchema;

    DataDb* mOrigData;
    DataDb* mTempData;

    KeyDb* mOrigKeys;
    KeyDb* mTempKeys;

    SdfRTree* mOrigRTree;
    SdfRTree* mTempRTree;

    FdoStringP mOrigName;
    FdoStringP mNewName;
    FdoStringP mTempName;
    FdoStringP mDropName;

    bool mModClassIds;
    bool mConvertData;
    bool mConvertKeys;
    bool mConvertRTree;
	bool mNewProperties;

    FdoInt32 mState;

    stdext::hash_map<void*, void*> m_newPropertyIndexes;
};

/// \brief
/// TableReformatterP is a FdoPtr on TableReformatter, provided for convenience.
typedef FdoPtr<TableReformatter> TableReformatterP;

class TableReformatterCollection: public FdoNamedCollection<TableReformatter,FdoException> 
{
public:
    TableReformatterCollection()
	{
	}

    virtual ~TableReformatterCollection()
    {
    }

protected:
    virtual void Dispose()
    {
        delete this;
    }
};

/// \brief
/// TableReformattersP is a FdoPtr on TableReformatterCollection, provided for convenience.
typedef FdoPtr<TableReformatterCollection> TableReformattersP;

#endif

