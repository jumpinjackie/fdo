#ifndef FdoSmPhPostGisSCHEMA_H
#define FdoSmPhPostGisSCHEMA_H		1
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

#include <Sm/Ph/DbElement.h>

class FdoSmPhRdPostGisDbSchemaReader;
// This represents a Sql Server schema
class FdoSmPhPostGisSchema : public FdoSmPhDbElement
{
public:
    // Instantiate a schema
	//
    // Parameters:
    // 	name: schema name
    // 	owner: owning datastore datastore.
    //     reader: initialize this schema from this reader.
    //          If set, then this is an existing schema
    //          If NULL, then this schema is not yet in the RDBMS.
    FdoSmPhPostGisSchema(
        FdoStringP name,
        const FdoSmPhOwner* pOwner,
        FdoSmPhRdPostGisDbSchemaReader* reader = NULL
    );

    // Returns the name of this schema's owning user.
	FdoStringP GetUsrName() const;

    // Returns true if this is a system schema.
    bool IsSystem() const
    {
        return mIsFixed || mIsSystem;
    }

  	virtual void SetElementState(FdoSchemaElementState elementState);

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhPostGisSchema();

	virtual ~FdoSmPhPostGisSchema(void);

    /// Remove from parent Owner's collection.
    virtual void Discard();

    // The following 3 post modifications to the RDBMS.
    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

private:
    void AddDeleteSystemSchemaError();
    
    FdoStringP mUserName;
    bool mIsUserDefault;
    bool mIsFixed;
    bool mIsSystem;
};

typedef FdoPtr<FdoSmPhPostGisSchema> FdoSmPhPostGisSchemaP;

#endif


