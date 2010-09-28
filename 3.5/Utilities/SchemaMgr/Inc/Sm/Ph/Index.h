#ifndef FDOSMPHINDEX_H
#define FDOSMPHINDEX_H		1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/IndexType.h>

class FdoSmPhMgr;
class FdoSmPhTable;

// Index represents a table index that was created by the Schema Manager
class FdoSmPhIndex : virtual public FdoSmPhDbObject
{
public:
    /// Returns true this Index is unique.
	bool GetIsUnique() const
    {
        return mIsUnique;
    }

    /// Get the database object to which this index applies.
    FdoSmPhDbObjectP GetDbObject() const
    {
        return FDO_SAFE_ADDREF(mpDbObject);
    }

    /// Add a column to this index.
    void AddColumn( FdoSmPhColumnP pColumn );

    /// Calculate a weight for this index, based on the size of its columns.
    long GetWeight();
	
    /// Serialize the Index to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

    /// When chosing the identity columns, reject any index whose columns are not compact enough
    /// (whose weight exceeds this value).
    static const long mMaxWeight = 5000;

    /// weight for date columns (arbitrarily chosen to make them a bit more 
    /// expensive than simpler types of columns such as strings).
    static const long mDateWeight = 50;

protected:
    /// unused constructor only for building on Linux
    FdoSmPhIndex() {}

    /// Constructs an instance of a Index object.
    /// 
    /// Parameters:
    /// 	name: Index name
    ///      pParent: the index's database object (object that contains the index columns).
    /// 	isUnique: true if the index is unique, false if non-unique
    ///      elementState: Indicates whether the index is new or already exists.
    FdoSmPhIndex(
        FdoStringP name, 
        FdoSmPhDbObject* pParent,
        bool isUnique,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

	~FdoSmPhIndex(void);

    /// Overrides base function to do dependency checks against index's table 
    /// rather than owner (datastore).
    virtual bool CheckCommitDependencies( bool fromParent = false, bool isBeforeParent = false );

    /// Called when this index is deleted, removes this index from its table's
    /// collection. 
    virtual void Discard();

    /// Override column reader creator to return empty reader.
    /// Parent table handles the adding of the right columns to each index.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader();
    
private:

    void AddColumnNoexistError( FdoString* pColName );

    bool mIsUnique;

    /// Not smart pointer in order to avoid circular reference.
    FdoSmPhDbObject* mpDbObject;

    FdoSmPhColumnsP mColumns;
};

typedef FdoPtr<FdoSmPhIndex> FdoSmPhIndexP;

#endif


