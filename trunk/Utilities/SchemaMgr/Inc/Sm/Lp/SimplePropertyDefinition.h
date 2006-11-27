#ifndef FDOSMLPSIMPLEPROPERTYDEFINITION_H
#define FDOSMLPSIMPLEPROPERTYDEFINITION_H		1
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


#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/PropertyDefinition.h>
#include <Sm/Lp/PropertyDefinitionCollection.h>
#include <Sm/Ph/Column.h>

class FdoSmLpTable;

// SimplePropertyDefinition derives from PropertyDefinition and represents types
// of properties that map to a single column.
class FdoSmLpSimplePropertyDefinition : public FdoSmLpPropertyDefinition
{
public:

    /// Returns a bool value that indicates if this properties value can be
    /// null.
    bool GetNullable() const;

    /// Gets the name of the corresponding Database column for this property.
    /// L"" if the property has no column.
	FdoString* GetColumnName() const;

    /// Returns the corresponding column name in the foreign table, when 
    /// containing table is actually a view on a foreign table.
    /// Returns L"" if containing table is not a view.
	FdoString* GetRootColumnName() const;

    /// Gets the column definition for this property. The column
    /// definition indicates where the property is stored in the 
    /// current datastore.
    /// NULL if the property has no column or the column doesn't exist.
    /// Call GetColumnName() to see if the property has a column.
	const FdoSmPhColumn* RefColumn() const;

    FdoSmPhColumnP GetColumn();

    /// Returns true if column name was specified by a schema override
    bool GetIsFixedColumn() const
    {
        return mbFixedColumn;
    }

    /// Returns true if column was created by Schema Manager.
    bool GetIsColumnCreator() const
    {
        return mbColumnCreator;
    }

    /// Sets the name of the corresponding Database column for this property.
    /// L"" if the property has no column.
	void SetColumnName( FdoString* columnName );

    /// Sets the name of the corresponding column in the foreign table.
    /// Called only when the containing table is actually a view on a 
    /// foreign table.
	void SetRootColumnName( FdoString* columnName );

    /// Sets the column definition for this property. The column
    /// definition indicates where the property is stored in the 
    /// current datastore.
    /// NULL if the property has no column or the column doesn't exist.
	void SetColumn( FdoSmPhColumnP pColumn );

    /// Set whether column name was specified by a schema override
    void SetIsFixedColumn( bool bFixedColumn ) 
    {
        mbFixedColumn = bFixedColumn;
    }

    /// Set whether column was created by Schema Manager.
    void SetIsColumnCreator( bool bColumnCreator ) 
    {
        mbColumnCreator = bColumnCreator;
    }

    /// Convenience function for casting a property to a simple property.
    /// Returns NULL if the property is not a data property.
	static const FdoSmLpSimplePropertyDefinition* Cast( const FdoSmLpPropertyDefinition* src )
	{
		return( dynamic_cast<const FdoSmLpSimplePropertyDefinition*>(src) );
	}

    /// Set the modification state of this element
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Given a property collection and a column, this function finds the 
    /// the property for the column.
	//
    /// Parameters:
    /// 	pProperties: the property collection.
    /// 	columnName: name of the column to search for.
	static const FdoSmLpSimplePropertyDefinition* ColName2Property( 
		const FdoSmLpPropertyDefinitionCollection* pProperties,
		FdoStringP columnName 
	);


    /// Creates a column for this property. The column is added to the given
    /// table.
	void CreateColumn( FdoSmPhDbObjectP dbObject );

    /// Returns true if this is a foreign column ( containing table
    /// is a view on a foreign table ).
	bool ColumnIsForeign();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmLpSimplePropertyDefinition() {}

    /// Loads a SimplePropertyDefinition from the current datastore
    //
    /// Parameters:
    /// 	propReader: reader for obtaining the DataPropertyDefinition attributes.
    /// 	parent: the containing class.
    FdoSmLpSimplePropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);
    
    /// Creates a SimplePropertyDefinition from an FDO Feature Schema element.
    //
    /// Parameters:
    /// 	pFdoProp: the FDO element.
    ///      pPropOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO element is 
    ///          to be ignored.
    /// 	parent: the containing class. 
    FdoSmLpSimplePropertyDefinition(
        FdoPropertyDefinition* pFdoProp, 
//        FdoOracleOvPropertyDefinition* pPropOverrides,
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    /// Constructs an instance of a SimplePropertyDefinition from the given parameters.
    /// Used mainly by CreateInherited() and CreateCopy().
    //
    /// Parameters:
    /// 	pBaseProperty: initialize the new property from this base property.
    /// 	pTargetClass: the class that will contain the new property.
    /// 	logicalName: The name for the new property.
    /// 		If L"" then the new property has the same name as this
    /// 		property.
    /// 	physicalName: the name of the new property's column.
    /// 		If L"" then the column is the same as the base property column name.
    /// 	bInherit:
    /// 		True: the new property inherits from the base property.
    /// 		False: the new property is just a copy of the base property.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
	FdoSmLpSimplePropertyDefinition(
		FdoPtr<FdoSmLpSimplePropertyDefinition> pBaseProperty, 
		FdoSmLpClassDefinition* pTargetClass, 
		FdoStringP logicalName, 
		FdoStringP physicalName, 
		bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
	);

    /// Sets the nullability of this property.
    void SetNullable( bool bNullable );

    /// Makes this property an inherited property.
	//
    /// Parameters:
    /// 	pBaseProp: the property to inherit from.
	virtual void SetInherited( const FdoSmLpPropertyDefinition* pBaseProp );

    /// Constructs a new column for this property.
    //
    /// Parameters:
    /// 	dbObject: put the column in this database object.
    /// 	columnName: the name for the new column.
	virtual FdoSmPhColumnP NewColumn( FdoSmPhDbObjectP dbObject, FdoStringP columnName ) = 0;

    /// Finalize this property by generating its column.
    virtual void Finalize();

private:
	FdoStringP mColumnName;

    FdoStringP mRootColumnName;

    FdoStringP mCandRootColumnName;

	FdoSmPhColumnP mColumn;

    bool mbFixedColumn;

    bool mbColumnCreator;

    bool mbNullable;

};

typedef FdoPtr<FdoSmLpSimplePropertyDefinition> FdoSmLpSimplePropertyP;

#endif


