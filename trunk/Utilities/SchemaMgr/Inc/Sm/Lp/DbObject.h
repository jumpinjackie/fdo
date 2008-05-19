#ifndef FDOSMLPDBOBJECT_H
#define FDOSMLPDBOBJECT_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/SchemaElement.h>
#include <Sm/Ph/DbObject.h>

class FdoSmLpPropertyDefinitionCollection;

// This describes the relationship between a class and a table.
class FdoSmLpDbObject : public FdoSmLpSchemaElement
{
public:
    /// Constructs an instance of a Table object.
    /// 
    /// Parameters:
    /// 	name: table name
    /// 	connection: DBI interface wrapper for accessing
    /// 		the database. Used to retrieve the table
    /// 		attributes from this database.
    /// 	properties: the properties for the corresponding class.
    FdoSmLpDbObject(
		FdoString* objectName, 
		FdoSmPhDbObjectP physicalObject, 
		FdoPtr<FdoSmLpPropertyDefinitionCollection> properties, 
		bool bClassTable, 
		FdoSmLpSchemaElement* parent
	);

	~FdoSmLpDbObject(void);

    /// Returns a collection of the columns defining the primary key for the table.
    /// Return value is always valid (never NULL).
	FdoSmPhColumnsP GetPkeyColumns();

    /// Returns a PropertyCollection containing all class properties 
    /// whose columns are in this table.
    /// Return value is always valid (never NULL).
    const FdoSmLpPropertyDefinitionCollection* RefProperties() const;

    /// Returns the name of this table's primary key
    /// L"" if there is no primary key.
	FdoString* GetPkeyName() const;

    /// Returns the Physical table definition.
    /// NULL if the table doesn't exist in the current database.
	const FdoSmPhDbObject* RefDbObject() const;

    /// Get a modifiable version of the table.
	FdoSmPhDbObjectP GetDbObject();

    /// The next three functions get info on how to join this table to the 
    /// class table.

    /// Get the next table in the join path. The next table can be the class table or 
    /// an intermediate table between this table and the class table.
	const FdoSmLpDbObject* RefTargetDbObject() const;

    /// Get the columns from this table that join to the next table.
	const FdoSmPhColumnCollection* RefSourceColumns() const;

    /// Get the columns from the next table that join to this table.
	const FdoSmPhColumnCollection* RefTargetColumns() const;

    /// Get the distance along the join path to the class table.
	//
    /// Returns:
    /// <0 - there is no join path to the class table.
    /// 0 - this is the class table.
    ///  1 - the next table is the class table.
    ///  2 - the next table is the intermediate table between this table and the
    /// 	class table.
    ///  >2 - there are multiple intermediate tables.

	int GetPathDist() const;

    /// Returns true when there is a join path to the class table.
	bool GetPathFound() const
	{
		return( miPathDist >= 0 );
	}

    /// Returns true if this is the class table or a class table of one of the
    /// ancestor classes.
	bool GetIsClasstable() const;

    /// Add the given source column
	void AddSourceColumn( FdoSmPhColumnP pColumn );

    /// Add the given target column.
	void AddTargetColumn( FdoSmPhColumnP pColumn );

    /// Set the target table.
	void SetTargetDbObject( FdoPtr<FdoSmLpDbObject> dbObject );

    /// Set the distance along the join path.
	void SetPathDist( int iDist );

    /// Set whether this is a class table.
	void SetIsClassTable( bool bClassTable );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Serialize this table to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

  protected:
     //Unused constructor needed only to build on Linux
    FdoSmLpDbObject() {}

  private:
    FdoPtr<FdoSmLpPropertyDefinitionCollection> mProperties;

	FdoSmPhColumnsP mSourceColumns;

	FdoSmPhColumnsP mTargetColumns;

	FdoPtr<FdoSmLpDbObject> mTargetDbObject;

	int miPathDist;

	bool mbClassTable;

	FdoSmPhColumnsP mEmptyPkey;

	FdoSmPhDbObjectP mPhDbObject;

};

typedef FdoPtr<FdoSmLpDbObject> FdoSmLpDbObjectP;

#endif


