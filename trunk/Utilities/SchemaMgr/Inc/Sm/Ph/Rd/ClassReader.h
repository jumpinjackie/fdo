#ifndef FDOSMPHRDCLASSREADER_H
#define FDOSMPHRDCLASSREADER_H		1
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

#include <Sm/Ph/Reader.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/Rd/DbObjectReader.h>

// This reader reads all of the classes for a schema, when no MetaSchema
// information is present. It reads each table in the given database schema and 
// automatically wraps a class around it.


class FdoSmPhRdClassReader : public FdoSmPhReader
{
public:
    /// Create and execute the reader
	FdoSmPhRdClassReader(
        FdoSmPhRowsP froms,         // rows and fields to retrieve.
        FdoStringP schemaName,      // name of Feature Schema that will contain the classes
        FdoStringP className,      // name of Feature class 
        FdoSmPhMgrP mgr,            // Physical Schema Manager
        FdoBoolean classifyDefaultTypes = true,// If true, classify only types of objects that are classified by default
                                               // If false, classify all types of object than can be classified
        FdoStringP database = L"",  // Database where RDBMS schema resides (current connection by default)
        FdoStringP owner = L""      // the RDBMS schema (defaults to current schema)
    );

    /// Deactivates the query.
	~FdoSmPhRdClassReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdClassReader() {}

    bool IsOrdinate(FdoSmPhColumnP column);

    // The following two functions control which database objects are reverse-engineered
    // into FDO Class Definitions:

    // Returns true if the type for the given object is one for which an FDO class definition
    // can be generated. 
    // Returns true if the object is a table or view. Sub-classes can override this function
    // to allow RDBMS-specific types to be classified.
    // The classifyDefaultTypes parameter has no effect in this function. However, specific 
    // RDBMS's might have some types that can be classified but are not classified by default.
    // When classifyDefaultTypes is true then this function must return true only for types
    // that can be classified by default. 
    // 
    virtual bool ClassifyObjectType( FdoSmPhDbObjectP dbObject, FdoBoolean classifyDefaultTypes );

    // Returns true if the given database object's corresponding class is in the Feature Schema
    // for this class reader. 
    // No checking is done against the object's type. This is performed by ClassifyObjectType()
    virtual FdoStringP ClassifyObject( FdoSmPhDbObjectP dbObject );
    // Find the main geometric property for the class from the given column collection.
    // Returns the main geometric property, L"" if none could be determined
    // (non geometric properties, or there was a tie for best main geometry).
    // hasGeom is set to true, on return, if the input column has any geometric
    // columns.
    virtual FdoStringP FindGeometryProperty( FdoSmPhColumnsP cols,  bool& hasGeom );

private:
    // This function is now obsolete; override or extend ClassifyObject( FdoSmPhDbObjectP ) instead.
#ifdef _WIN32
    virtual FdoStringP ClassifyObject( FdoStringP objectName, FdoSmPhDbObjType objectType ) sealed;
#else
    virtual FdoStringP ClassifyObject( FdoStringP objectName, FdoSmPhDbObjType objectType );
#endif


    FdoStringP mSchemaName;
    FdoBoolean mClassifyDefaultTypes;

    FdoSmPhOwnerP mOwner;
    FdoSmPhDbObjectsP mDbObjects;
    FdoInt32   mCurrDbObject;

    FdoBoolean      m_IsGeometryFromOrdinatesWanted;
};

// convenient smart pointer
typedef FdoPtr<FdoSmPhRdClassReader> FdoSmPhRdClassReaderP;

#endif


