#ifndef FDORDBMSLPINDEX_H
#define FDORDBMSLPINDEX_H		1
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

#include "SchemaElement.h"
#include "DataPropertyDefinitionCollection.h"
#include "../Physical/Index.h"
#include "../Physical/ClassIndexReader.h"
#include "../../DbiConnection.h"
#include <Rdbms/Override/Oracle/OracleOvUniqueKeyDefinition.h>
#include <Rdbms/Override/Oracle/OracleOvIndexDefinition.h>

class FdoRdbmsLpDataPropertyDefinitionCollection;

// This describes the relationship between a class and a Index.
class FdoRdbmsLpIndex : public FdoRdbmsLpSchemaElement
{
public:
    /// Constructs an instance of a Logical-Physical Index object from the 
    /// metaschema of the current datastore.
    /// 
    /// Parameters:
    /// 	pReader: reads the indexes from the datastore.
    /// 	parent: the index's containing class.
    FdoRdbmsLpIndex(
		FdoRdbmsPhClassIndexReader* pReader, 
		FdoRdbmsLpSchemaElement* parent
	);

    /// Constructs an instance of a Logical-Physical Index object from a 
    /// Unique Key schema override.
    /// 
    /// Parameters:
    /// 	pUniqueKey: the Schema Override.
    /// 	parent: the index's containing class.
    FdoRdbmsLpIndex(
		FdoOracleOvUniqueKeyDefinition* pOvUniqueKey, 
		FdoRdbmsLpSchemaElement* parent
	);

    /// Constructs an instance of a Logical-Physical Index object from a 
    /// Non-Unique Index schema override.
    /// 
    /// Parameters:
    /// 	pUniqueKey: the Schema Override.
    /// 	parent: the index's containing class.
    FdoRdbmsLpIndex(
		FdoOracleOvIndexDefinition* pOvIndex, 
		FdoRdbmsLpSchemaElement* parent
	);

    /// Constructs an instance of a Logical-Physical Index object from a 
    /// base index
    /// 
    /// Parameters:
    /// 	pBaseIndex: the base index.
    /// 	pTargetClass: the new index's containing class.
    ///      bInherit:
    ///          true: inherit the new index from the base index.
    ///          false: just copy the new index from the base index.
    FdoRdbmsLpIndex(
		const FdoRdbmsLpIndex* pBaseIndex, 
        FdoRdbmsLpClassDefinition* pTargetClass,
        bool bInherit
	);

	~FdoRdbmsLpIndex(void);

    /// Returns true if this is a unique index.
    bool GetIsUnique() const
    {
        return mbIsUnique;
    }

    /// Returns a PropertyCollection containing all class properties 
    /// whose columns are in this Index.
    /// Return value is always valid (never NULL).
    const FdoRdbmsLpDataPropertyDefinitionCollection* GetProperties() const
    {
        return &mProperties;
    }

    /// Same as previous function, except that a modifiable collection is returned.
    FdoRdbmsLpDataPropertyDefinitionCollection* Get_Properties()
    {
        return &mProperties;
    }

    /// Returns the Physical Index definition.
    /// NULL if the Index doesn't exist in the current datastore.
	const FdoRdbmsPhIndex* GetIndex() const;

    /// Get a modifiable version of the Index.
	FdoRdbmsPhIndex* Get_Index();

    /// When this is an inherited index, this function returns 
    /// the index that it is based on.
    /// Returns NULL if this index is not inherited.
    const FdoRdbmsLpIndex* GetBaseIndex() const 
    {
        return mpBaseIndex;
    }

    /// When this is a copied index this function returns 
    /// the index that it is based on.
    /// Returns NULL if this index was not copied from another index.
    const FdoRdbmsLpIndex* GetSrcIndex() const 
    {
        return mpSrcIndex;
    }

    /// When this is a copied or inherited index, this function returns 
    /// the index that it was directly copied or inherited from. This function
    /// differs from GetBaseIndex() and GetSrcIndex() since these return
    /// the original index that it was inherited or copied from.
    /// Returns NULL if this index was not copied or inherited from another index.
    const FdoRdbmsLpIndex* GetPrevIndex() const 
    {
        return mpPrevIndex;
    }

    /// Gets the class that defines this index. When this index
    /// is inherited then the defining and containing ( returned by
    /// GetParent() class will be different.
    /// NULL if the defining class is not in the current database.
    const FdoRdbmsLpClassDefinition* GetDefiningClass() const;

    /// Generates a UniqueKey Schema override from this index.
    /// Returns NULL if this index is not unique.
    virtual FdoOracleOvUniqueKeyP GetUniqueSchemaMappings() const;

    /// Generates a non-unique Index Schema override from this index.
    /// Returns NULL if this index is unique.
    virtual FdoOracleOvIndexP GetNonUniqueSchemaMappings() const;

    /// Gets the qualified name for this index
    /// ( [schemaname]:[classname].[indexname] )
	virtual FdoStringP GetQName() const;

    /// Add a property, by name, to this index
    void AddProperty( FdoString* pPropName );

    /// Add a property, by definition, to this index
    void AddProperty( FdoRdbmsLpDataPropertyDefinition* pProp );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaException* Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Inherit this index to a subclass. Creates a new instance of this 
    /// index, which can be attached to the subclass.
	//
    /// Parameters:
    /// 	pSubClass: inherit to this subclass. Must be a subclass of this
    /// 		index's defining class.
	FdoRdbmsLpIndex* CreateInherited( FdoRdbmsLpClassDefinition* pSubClass ) const;

    /// Create a non-inherited copy of this index. 
	//
    /// Parameters:
    /// 	pTargetClass: The class that will contain the new index.
	FdoRdbmsLpIndex* CreateCopy( FdoRdbmsLpClassDefinition* pTargetClass ) const;

    /// Makes this index an inherited index.
	//
    /// Parameters:
    /// 	pBaseIndex: the index to inherit from.
    void SetInherited( const FdoRdbmsLpIndex* pBaseIndex );

    /// Copy updates from corresponding UniqueKey Schema Mapping.
    //
    /// Parameters
    ///      pOvUniqueKey: the unique key.
    void Update( FdoOracleOvUniqueKeyDefinition* pOvUniqueKey );

    /// Copy updates from corresponding non-unique Index Schema Mapping.
    //
    /// Parameters
    ///      pOvIndexKey: the non-unique index.
    void Update( FdoOracleOvIndexDefinition* pOvIndex );

    /// Copy property list updates from Schema Mapping
    //
    /// Parameters
    ///      propNmes: the property names from the Schema Mapping
    void UpdateProperties( FdoStringsP& propNames );

    /// Post outstanding modifications to the database.
	virtual void Commit();

    /// Serialize this Index to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

  protected:
    /// Get the index property names
    void GetPropertyMappings( FdoStringsP propMappings ) const;
    
    /// Add the given properties to this index.
    void AddProperties( FdoStringsP& propNames );

    /// Finalize this class. Resolves base class and property to column.
    /// Base class resolution is not done in the constructor in case
    /// two schemas reference classes in each other. Class objects are
    /// constructed during schema loading so we don't want to resolve 
    /// base classes until at least one of the schemas has finished 
    /// loading.
	virtual void Finalize();

  private:
    void AddPropNotFoundError( FdoString* pPropName );
    void AddPropAfterFinalError( FdoString* pPropName );
    void AddUniqueChangeError();
    void AddPropChangeError();
    void AddRedefinitionError( const FdoRdbmsLpIndex* pBaseIndex );
    void AddLpUniqueError();
    void AddLpPropColError();
    void AddColNotFoundError( FdoString* pColName );
    void AddPropDeleteError( const FdoRdbmsLpDataPropertyDefinition* pProp);

    FdoRdbmsLpDataPropertyDefinitionCollection mProperties;
    FdoStringsP mPropNames;
	FdoRdbmsPhIndex* mpIndex;
    bool mbIsUnique;

    const FdoRdbmsLpIndex* mpBaseIndex;
    const FdoRdbmsLpIndex* mpSrcIndex;
    const FdoRdbmsLpIndex* mpPrevIndex;
};

#endif


