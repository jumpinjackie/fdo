#ifndef FDOSMLPCLASSDEFINITION_H
#define FDOSMLPCLASSDEFINITION_H		1
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

#include <Fdo/Commands/Schema/PhysicalClassMapping.h>
#include <Sm/Lp/SchemaElement.h>
#include <Sm/Lp/DbObjectCollection.h>
#include <Sm/Lp/PropertyDefinitionCollection.h>
#include <Sm/Lp/DataPropertyDefinitionCollection.h>
#include <Sm/Lp/UniqueConstraintCollection.h>
#include <Sm/Lp/CheckConstraintCollection.h>

/*
#include <Sm/Lp/IndexCollection.h>
#include <Sm/Lp/JoinTreeNode.h>
*/
#include <Sm/Ph/ClassReader.h>
#include <Sm/Ph/ClassWriter.h>
#include <Utilities/SchemaMgr/Overrides/TableMappingType.h>

class FdoSmLpSchemaCollection;

// ClassBase is the virtual abstract base class for FdoSmLpClassDefinition and
// FdoSmLpGrdClassDefinition. It's main purpose is to allow FdoSmLpClassDefinition
// to be a non-virtual base class of FdoSmLpClass and FdoSmLpFeatureClass. This
// allows Schema Manager callers to avoid having to dynamic cast to convert from
// FdoSmLpClassDefinition to FdoSmLpClass or FdoSmLpFeatureClass.
//
// A side effect is that fdoSmLpGrdClassDefinition (and provider-specific class
// definitions are not based on FdoSmLpClassDefinition). However, this is not
// a problem since these classes would not normally be accessed outside the 
// Schema Manager.
class FdoSmLpClassBase : public FdoSmLpSchemaElement
{
public:

    /// The following class contains the table capabilities
    class Capabilities
    {
    public:
        Capabilities( 
            const FdoSmLpClassBase* pClass
        );
        virtual ~Capabilities();

    /// True if table rows can be locked.
        bool SupportsLocking() const;
    /// True if table rows can be versioned.
        bool SupportsLongTransactions() const;

        const FdoLockType* GetLockTypes(FdoInt32& size) const;

    private:
        bool                    mSupportsLocking;
        bool                    mSupportsLongTransactions;
        FdoLockType*            mLockTypes;
        FdoInt32                mLockTypeCount;
    };

    ~FdoSmLpClassBase(void);

    /// Abstract operation that must be implemented by derived classes to return
    /// the concrete class type.
	virtual FdoClassType GetClassType() const = 0;

    /// Gets the base ClassObject this class is derived from.
	const FdoSmLpClassDefinition* RefBaseClass() const;

    /// Gets the base class without tripping a Finalize.
	FdoPtr<FdoSmLpClassDefinition> GetBaseClass();

    /// Gets the MetaClass for this class. The MetaClass is the 
    /// class from the F_MetaClass schema that references the class's
    /// class type.
	const FdoSmLpClassDefinition* RefMetaClass() const;

    FdoPtr<FdoSmLpClassDefinition> GetMetaClass();

    /// Gets the value of the isAbstract attribute. Classes marked as abstract
    /// cannot be created via FDO commands and cannot be used as properties of
    /// another class.
    bool GetIsAbstract() const;

    /// Returns a DataPropertyDefinitionCollection containing references to the
    /// data properties that can be used to uniquely identify instances of the
    /// class.
    /// Return value is always valid (never null).
    const FdoSmLpDataPropertyDefinitionCollection* RefIdentityProperties() const;

    /// Returns a PropertyCollection containing the properties
    /// for this class. Includes inherited properties.
    /// Return value is always valid (never null).
    const FdoSmLpPropertyDefinitionCollection* RefProperties() const;

    /// Returns the collection of nested properties. These are the sub-properties
    /// for object properties for this class.
    /// Return value is always valid (never null).
    const FdoSmLpPropertyDefinitionCollection* RefNestedProperties() const;

    /// Returns the collection of UNIQUE constraints
	const FdoSmLpUniqueConstraintCollection* RefUniqueConstraints() const;

    /// Returns the collection of CHECK constraints
	const FdoSmLpCheckConstraintCollection*  RefCheckConstraints() const;

    /// Get modifyable versions of the Properties, Id Properties and Nested Properties.
    /// Used only by special wrapper classes, to set up property lists.
    FdoSmLpDataPropertiesP GetIdentityProperties();
    FdoSmLpPropertiesP GetProperties();
    FdoSmLpPropertiesP GetNestedProperties();
	FdoSmLpUniqueConstraintsP GetUniqueConstraints();
	FdoSmLpCheckConstraintsP  GetCheckConstraints();

/*
    const FdoSmLpIndexCollection* GetIndexes() const;
*/
    /// Returns the class table name. L"" if the class has no table.
	FdoString* GetDbObjectName() const;

    /// Returns the foreign table name when class table is actually a view
    /// on a foreign table.
    /// Returns L"" if class table is not a view.
	FdoString* GetRootDbObjectName() const;

    /// Returns the class table name. L"" if the class has no table.
	FdoStringP GetDbObjectQName( 
        bool includeDefaultOwner = false // true: always prepend with owning datastore.
    /// false: leave owner off if it is the current one.
    ) const;

    /// Returns the main table for this class. NULL if class has no table
    /// or table doesn't exist. Call GetTableName() to determine whether
    /// the class has a table.
	const FdoSmLpDbObject* RefDbObject() const;

    /// Finds the database object (class table) for this class definition.
    /// If the class is not being modified, and the class table is an 
    /// invalid view, the root table for the view is returned instead.
    FdoSmPhDbObjectP FindPhDbObject();

    /// Returns true if the table name was specified by schema overrides.
    bool GetIsFixedDbObject() const
    {
        return mbIsFixedDbObject;
    }

    /// Returns a collection containing the main table 
    /// and any other tables that store properties for 
    /// this class. Does not include object property tables.
    /// Return value is always valid (never null).
	const FdoSmLpDbObjectCollection* RefDbObjects() const;

    /// Get the table mapping for this class.
    /// Determines how class properties are mapped to
    /// schema tables.
	FdoSmOvTableMappingType GetTableMapping() const;

    /// Get the table mapping as specified by schema overrides
    FdoString* GetOvTableMapping() const
    {
        return mOvTableMapping;
    }

    /// Get the name of the database for the class table
    FdoString* GetDatabase() const
    {
        return mDatabase;
    }

    /// Get the name of the owning schema of the class table
    FdoString* GetOwner() const
    {
        return mOwner;
    }

    /// Get a fully qualified class name ( "[schemaname]:[classname]" ).
	virtual FdoStringP GetQName() const;

    /// For a given set of data property names, this function returns all tables that a 
    /// select statement must include to select the properties.
    /// The tables are returned in a tree structure with the class table at the root node.
    /// The root node contains a SourceNodes collection listing all tables that must be
    /// joined directly to the class table. Each Source Node can also have a SourceNodes
    /// list. Leaf nodes are always for tables containing at least one of the given properties.
    /// Non-Leaf nodes may contain some of the given properties or they might just 
    /// represent intermediate tables that need to be joined in.
    /// Therefore, by traversing all branches of the tree, you find all the tables that
    /// need to be joined in to select the properties.
	//
    /// Each Source Node has a table object that indicates how to join to the table at
    /// the parent node. The table object has:
    /// \li \c sourceColumns: join columns at the source node.
    /// \li \c targetColumns: join columns at the parent (target) node.
	//
    /// The Root Node also has a missingProperties collection containing any properties
    /// not in any of the nodes in the tree. These are usually properties not defined for
    /// this class or properties whose containing table has no join path back to the 
    /// class table.
	//
    /// \note
    /// Only base properties from MetaClasses can be found in the non-root nodes. 
/*
	FdoSmLpJoinTreeNodeP GetJoinTree( const FdoStringsP& properties ) const;

    /// Get the Schema Mappings for this class
	//
    /// Parameters:
    /// 	bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      The Schema Mappings.
    ///      NULL if there are no mappings.
*/
    virtual FdoPhysicalClassMappingP GetSchemaMappings( bool bIncludeDefaults ) const;

    /// Gets the internal datastore class id number. Note that this id does not get
    /// exposed through FDO.
	FdoInt64 GetId() const;

    /// Get the property that holds the feature id for objects of this class.
    /// NULL if no feature id property.
	const FdoSmLpDataPropertyDefinition* RefFeatIdProperty() const;
	FdoSmLpDataPropertyP GetFeatIdProperty();

    const Capabilities* GetCapabilities() const;

    bool GetIsDbObjectCreator() const 
    {
        return mbIsDbObjectCreator;
    }

    /// Convert a column name to one that is datastore-acceptable and 
    /// not already in use.
	//
    /// Parameters:
    /// 	dbObject - the database object where the column will reside.
    /// 		if NULL then this column is not intended to be physically created.
    /// 	pProp - the property that the column will be assigned to
    /// 	columnName - the column name to convert.
    ///      bFixed - true if the column name was set through mapping overrides.
	FdoStringP UniqueColumnName( 
        FdoSmPhDbObjectP dbObject, 
        const FdoSmLpPropertyDefinition* pProp, 
        const wchar_t* columnName,
        bool bFixed = false
    );

    /// Returns true if this type of class is allowed to contain
    /// geometric properties.
	virtual bool SupportsGeometry() const { return(false);} 

    /// Generates and adds this class's physical mappings to the given 
    /// schema override set.
    virtual bool AddSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const
    {
        return false;
    }

    /// Adds the classes physical mappings to the given class mappings.
    virtual bool SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
    {
        return false;
    }

	//Copy updates from corresponding FDO class. 
    //
    /// Parameters:
    ///      pFdoClass: the FDO class.
    ///      elementState: the modification state for this class definition.
    ///      pClassOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO class is 
    ///          to be ignored.
	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pOverrides,
        bool bIgnoreStates
    ); 

	//Copy updates from corresponding FDO class. 
    //Providers must override to pick out the class overrides
    //from the given schema override set.
    //
    /// Parameters:
    ///      pFdoClass: the FDO class.
    ///      elementState: the modification state for this class definition.
    ///      pOverrides: schema override set for class's feature schema.
    ///      bIgnoreStates: true if the element state on the FDO class is 
    ///          to be ignored.
	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ) = 0;

    /// Synchronize the class table and columns with this class and its properties.
    //
    /// Parameters:
    //
    ///      bRollbackOnly - 
    ///          true - synchronize only if a change to this class has been rolled back since
    ///              the previous synchronization.
    ///          false - always synchronize this class.
	virtual void SynchPhysical(bool bRollbackOnly = true);

    /// Set the modification state of this element
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Post outstanding modifications to the current datastore.
	virtual void Commit( bool fromParent = false );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Serialize this class to an XML file.
    /// Primarily for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

	virtual void XMLSerializeSubElements(  FILE* xmlFp, int ref ) const
	{
	}

    /// Create a property for this class from the given FDO property definition.
    virtual FdoSmLpPropertyP CreateProperty(
        FdoPropertyDefinition* pFdoProp, 
        FdoPhysicalClassMapping* pOverrides,
        bool bIgnoreStates,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpClassBase() {}

    /// Constructs an instance of a ClassBase from a class reader
    FdoSmLpClassBase(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);
	
    /// Constructs an instance of a ClassBase from an FDO element
    /// and Fdo to physical mapping overrides.
    //
    /// Parameters:
    /// 	pFdoClass: the FDO class.
    ///      pClassOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO class is 
    ///          to be ignored.
    /// 	parent: the parent element ( usually an FdoSmLpSchema ).
    FdoSmLpClassBase(
        FdoClassDefinition* pFdoClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

    /// Constructs an instance of a ClassBase from the given attributes.
	//
    /// Parameters:
    /// 	name: class name
    /// 	description: class description
    /// 	parent: class objects parent ( usually an FdoSmLpSchema object )
    /// 	pSrcClass: When this is an object property class, pSrcClass represents the 
    /// 		object property class for the associated object property's base property.
    /// 	tableMapping: property to table mapping type
    /// 	tableName: class table name
    /// 	table: class table. Null if class has no table or table
    /// 		does not exist.
    ///      fromFdo: true if this object came directly from an FDO Feature Schema.

	FdoSmLpClassBase(
        FdoString* name,
        FdoString* description,
        FdoSmLpSchemaElement* parent,
        FdoPtr<FdoSmLpClassDefinition> pSrcClass,
        FdoSmOvTableMappingType tableMapping,
        FdoString* tableName,
        FdoSmPhDbObjectP table,
        FdoSchemaElementState elementState,
        bool fromFdo
    );

    const FdoSmLpClassDefinition* RefSrcClass() const
    {
        return mSrcClass;
    }

    /// Get table mapping type without finalizing this class.
	FdoSmOvTableMappingType Get_TableMapping() const
    {
        return mTableMapping;
    }

    /// Called just before a property is inherited to this class.
    /// Returns true if it can be inherited.
    /// Returns false if it must be skipped.
    /// Base function always returns true.
    virtual bool CanInherit( const FdoSmLpPropertyDefinition* pProp );

    /// Get modifyable version of the Class table.
	FdoSmLpDbObjectP GetDbObject();

    /// Get a class writer for adding class definitions.
    /// Returns a class writer with fields already set.
    /// This function is virtual so that different class types 
    /// can set their specific fields.
    virtual FdoSmPhClassWriterP GetPhysicalAddWriter();

    /// Get a class writer for modifying class definitions.
    /// Returns a class writer with fields already set.
    /// This function is virtual so that different class types 
    /// can set their specific fields.
    virtual FdoSmPhClassWriterP GetPhysicalModifyWriter();

    // Called by GetPhysicalAddWriter() and GetPhysicalModifyWriter().
    // Can be overridden to write any necessary schema options for the class.
    virtual void SetPhysicalOptionsWriter( FdoSmPhClassWriterP classWriter );

    /// Create a new table with the given table name.
	virtual FdoSmPhDbObjectP NewTable( 
        FdoString* tableName, 
        FdoString* pkeyName
    );

    /// Create a new view on the given foreign table.
	virtual FdoSmPhDbObjectP NewView( 
        FdoString* viewName, 
        FdoString* database,
        FdoString* owner,
        FdoString* tableName
    );

    /// Check if a foreign table or column name is valid (not too long).
    void ValidateForeignObjectName( FdoStringP foreignObjectName );

    /// Returns true if this class owns its table. If the class owns the table then
    /// the table will be dropped when the class is deleted.
    virtual bool IsDbObjectCreator() const;
 
    /// Returns true if this class owns its table. If the class owns the table then
    /// the table will be dropped when the class is deleted.
    virtual bool Is_DbObjectCreator() const;
 
    /// Returns true if objectName is a valid object name in the current datastore.
    /// The bFromConfigFile input argument should be set to true iff the 
    /// given object was originally defined from a configuration file.
	bool VldDbObjectName( FdoStringP objectName, bool bFromConfigFile );

    /// Add a primary key to the class table if it doesn't already have one.
    /// The primary key columns are set to the identity property columns.
	void CreatePkey();

    /// Add unique constraints to the class table if it doesn't already have one.
	void CreateUkeys();

    /// Add constraints to the class table.
	void CreateCkeys();

    /// Returns a CHECK clause
	FdoStringP GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp );

    /// Callback to allow providers to customize a new primary key that was added to the 
    /// class table.
    virtual void NewPkey( FdoSmPhTableP table );

    /// Returns the list of identity properties that are in the datastore (have columns).
	FdoStringsP GetDbIds();

    void SetIsFixedDbObject( bool bFixed )
    {
        mbIsFixedDbObject = bFixed;
    }

    /// Sets the current table mapping type
    void SetTableMapping( FdoSmOvTableMappingType mappingType );

    /// Records the table mapping type specified by Schema Overrides
    void SetOvTableMapping( FdoString* mappingType )
    {
        mOvTableMapping = mappingType;
    }

    /// Sets whether this class created its class talbe.
    void SetIsDbObjectCreator( bool bCreator );

    /// Set name of database containing class table
    void SetDatabase( FdoString* database );

    /// Set name of owning schema containing class table
    void SetOwner( FdoString* owner );

    /// Set name class table
    void SetDbObjectName( FdoStringP objectName );

    /// Set class table primary key name (new classes only).
    void SetNewPkeyName( FdoStringP newPkeyName )
    {
        mNewPkeyName = newPkeyName;
    }

    /// Finalize this class. Resolves base class and property to column.
    /// Base class resolution is not done in the constructor in case
    /// two schemas reference classes in each other. Class objects are
    /// constructed during schema loading so we don't want to resolve 
    /// base classes until at least one of the schemas has finished 
    /// loading.
	void Finalize();

    /// Callback invoked by Finalize when it is finished processing but just
    /// before setting the object state to Final. 
    /// Derived classes can override to add extra finalization code.
    /// TODO: add a PreFinalize callback when the need arises.
    virtual void PostFinalize();

    /// Finalize the properties ( nested and not nested ) for this class. Inherit
    /// the properties from the base class to this class.
	//
    /// Parameters:
    /// 	pBaseProps: properties from the base class.
    /// 	pProps: the properties for this class. An inherited copy of 
    /// 		each base property is added to this collection.
	virtual void FinalizeProps( 
		const FdoSmLpPropertyDefinitionCollection* pBaseProps,
		FdoSmLpPropertiesP pProps
	);

    /// Finalize the identity properties for this class.
	virtual void FinalizeIdProps();

    /// Finalize the unique keys for this class.
	virtual void FinalizeUkeys();

    /// Finalize the check keys for this class.
	virtual void FinalizeCkeys();

/*
    /// Finalize the indexes for this class.
	virtual void FinalizeIndexes();
*/
    /// Finalize the physical database object for this class.
	virtual void FinalizePhDbObject();

    /// Finalize the list of tables containing this class's data properties.
	virtual void FinalizeDbObjects();

    /// Finalize a class property table or table on the join path to the 
    /// class table.
	//
    /// Parameters:
    /// 	pPhTable: the table to finalize
    /// 	tables: list of tables that already have been finalized
    /// 	iLevel: recursion level.
    /// 		0 - this is a table for a property
    /// 		>0 - this a an intermediate table on the join path between a 
    /// 		  property table and the class table.
    /// 	pProp: when iLevel is 0 then this is a property contained in this table.
	FdoSmLpDbObjectP FinalizeDbObject( 
		FdoSmPhDbObjectP phDbObject, 
		FdoSmLpDbObjectsP dbObjects, 
		int iLevel,
		const FdoSmLpDataPropertyDefinition* pProp 
	);

    /// Finalize a class property table that has not yet been finalized.
	//
    /// Parameters:
    /// 	pPhTable: the table to finalize
    /// 	tables: list of tables that already have been finalized
    /// 	iLevel: recursion level.
    /// 		0 - this is a table for a property
    /// 		>0 - this a an intermediate table on the join path between a 
    /// 		  property table and the class table.
    /// 	bClassTable: 
    /// 		true: this is a class table ( for this class or an ancestor or metaclass ).
    /// 		false: otherwise.
    /// 	pProp: when iLevel is 0 then this is a property contained in this table.
	FdoSmLpDbObjectP FinalizeNewDbObject( 
		FdoSmPhDbObjectP phTable, 
		FdoSmLpDbObjectsP dbObjects, 
		int iLevel, 
		bool bClassTable,
		const FdoSmLpDataPropertyDefinition* pProp 
	);

    void AddTableNameChangeError( 
        const wchar_t* newTableName
    );

private:
/*
    /// Visit a join tree node. Create it if it doesn't yet exist.
    /// Link it with its parent node if it's table has a target table.
	//
    /// Parameters:
    /// 	joinNodes: list of nodes already visited.
    /// 	pTable: the table for this node.
	FdoSmLpJoinTreeNode* AddJoinTreeNode( FdoSmLpJoinTreeNodeCollection& joinNodes, const FdoSmLpTable* pTable ) const;
*/
    /// Match the given base property to the property in this class that
    /// inherits from it.
    /// Returns the inherited property if one was found.
	FdoSmLpPropertyP MatchInheritedProp( const FdoSmLpPropertyDefinition* pBaseProp, FdoSmLpPropertiesP pProps );

    /// Check if a column name is already in use by this class or the given table. 
	//
    /// Returns true if the column is referenced by an explicit or inherited property 
    /// in this class or the given database object.

    /// Parameters:
    /// 	dbObject - the database object to check.
    /// 		if NULL then this column is not intended to be physically created.
    /// 	pProp - the property that the column will be assigned to the function returns
    /// 		false if this is the only property referencing the column.
    /// 	columnName - the column name to check for references.
	bool ColumnNameUsed( FdoSmPhDbObjectP dbObject, const FdoSmLpPropertyDefinition* pProp, const wchar_t* columnName );

	void BreakObjPropTargets( 
		FdoSmLpPropertiesP pProps
	);

    /// Various helpers for logging errors in the physical schema.
	void AddBaseClassLoopError( const FdoSmLpClassDefinition* pBaseClass );
	void AddBaseClassMissingError(  void );
	void AddBaseClassDeleteError( const FdoSmLpClassDefinition* pBaseClass );
	void AddBaseClassTypeError( const FdoSmLpClassDefinition* pBaseClass );
	void AddBaseNoSchemaError( const wchar_t* baseClassName );
	void AddDuplicateIdPosnError( int position );
    void AddIdMissingError( const FdoSmPhColumn* pColumn);
	void AddIdDifferentFromBaseError( void );
	void AddJoinColCountError( FdoStringP pkTableName, FdoStringP fkTableName );
	void AddSrcColNotFoundError( FdoStringP pkTableName, FdoStringP fkTableName, FdoStringP colName );
	void AddTargColNotFoundError( FdoStringP pkTableName, FdoStringP fkTableName, FdoStringP colName );
	void AddPathNotFoundError( FdoStringP tableName );
	void AddClassTypeChangeError( FdoClassType newType );
	void AddBaseClassChangeError( FdoStringP newBaseClassName );
	void AddAbstractChangeError();
	void AddPropExistsError( const wchar_t* propName );
	void AddPropNotExistsError( const wchar_t* propName );
	void AddGeomPropError( const wchar_t* propName );
	void AddFdoDbIdMismatchError( FdoStringsP& fdoIds, FdoStringsP& dbIds );
	void AddModIdPropsError( FdoStringsP& fdoIds, FdoStringsP& dbIds );
	void AddGenericError( const wchar_t* specificError );
	void AddNullIdPropError(const FdoSmLpDataPropertyDefinition* pIdProp);
	void AddReadOnlyIdPropError(const FdoSmLpDataPropertyDefinition* pIdProp);
    void AddBaseClassTableError();
    void AddTableCharError( const wchar_t* tableName );
    void AddTableLengthError( const wchar_t* tableName, FdoSize tableMaxSize ); 
    void AddTableReservedError( const wchar_t* tableName ); 
    void AddColCharError( 
        const FdoSmLpPropertyDefinition* pProp, 
        const wchar_t* columnName
    );
    void AddColLengthError( 
        const FdoSmLpPropertyDefinition* pProp, 
        const wchar_t* columnName
    );
    void AddColReservedError( 
        const FdoSmLpPropertyDefinition* pProp, 
        const wchar_t* columnName
    );
    void AddColUsedError( 
        const FdoSmLpPropertyDefinition* pProp, 
        const wchar_t* columnName
    );
    void AddForeignNoFeatIdError( const FdoSmLpPropertyDefinition* pFeatId );

    /// Various members. See corresponding getter functions.

	bool mIsAbstract;

	FdoClassDefinitionP mFdoClass;

    FdoSmLpPropertiesP mProperties;

    FdoSmLpPropertiesP mNestedProperties;

	FdoSmLpDataPropertiesP mIdentityProperties;

	FdoSmLpUniqueConstraintsP mUniqueConstraints;

	FdoSmLpCheckConstraintsP  mCheckConstraints;

/*
    FdoSmLpIndexCollection mIndexes;
*/
	FdoSmPhDbObjectP mPhDbObject;
	FdoSmLpDbObjectP mDbObject;

    /// Name of class table (or view when table is foreign schema)
	FdoStringP mDbObjectName;

    /// When class "table" is actually a view, this is the foreign table that the
    /// view is based on.
	FdoStringP mRootDbObjectName;

    FdoStringP mNewPkeyName;

    bool mbIsFixedDbObject;

    bool mbIsDbObjectCreator;

	FdoSmLpDbObjectsP mDbObjects;

    /// The resultant table mapping (after applying defaulting rules)
	FdoSmOvTableMappingType mTableMapping;

    /// The mapping specified through Schema Overrides, if any.
    FdoStringP mOvTableMapping;

	FdoPtr<FdoSmLpClassDefinition> mBaseClass;

	FdoStringP mBaseClassName;

	FdoPtr<FdoSmLpClassDefinition> mSrcClass;

	FdoInt64 mId;

    FdoStringP mDatabase;

    FdoStringP mOwner;

    Capabilities* mCapabilities;

    /// This is set when the FDO and Database versions of the id 
    /// properties need to be compared.
	bool mbCheckIds;
    /// Stores the FDO version of the id properties by name
	FdoStringsP mFdoIds;

    bool mbHasFdoIdentity;
};

class FdoSmLpClassDefinition : public virtual FdoSmLpClassBase
{
public:
    /// Create a property for this class from the given property reader
    virtual FdoSmLpPropertyP CreateProperty( FdoSmPhClassPropertyReaderP propReader );

    /// Create a property for this class from the given FDO property definition.
    virtual FdoSmLpPropertyP CreateProperty(
        FdoPropertyDefinition* pFdoProp, 
        FdoPhysicalClassMapping* pOverrides,
        bool bIgnoreStates,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

  	static const FdoStringP ClassDefinitionName;
	static const FdoStringP ClassClassName;
	static const FdoStringP FeatureClassName;
	static const FdoStringP DefaultGeometricPropertyName;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpClassDefinition() {}

    /// Constructs an instance of a ClassDefinition from a class reader
    FdoSmLpClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);
	
    /// Constructs an instance of a ClassDefinition from an FDO element
    /// and Fdo to physical mapping overrides.
    //
    /// Parameters:
    /// 	pFdoClass: the FDO class.
    ///      pClassOverrides: Fdo to physical mapping overrides.
    ///      bIgnoreStates: true if the element state on the FDO class is 
    ///          to be ignored.
    /// 	parent: the parent element ( usually an FdoSmLpSchema ).
    FdoSmLpClassDefinition(
        FdoClassDefinition* pFdoClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

    /// Constructs an instance of a Class Definition from the given attributes.
    /// Usually called by special wrapper classes such as FdoSmLpObjectPropertyClass.
	//
    /// Parameters:
    /// 	name: class name
    /// 	description: class description
    /// 	parent: class objects parent ( usually an FdoSmLpSchema object )
    /// 	pSrcClass: When this is an object property class, pSrcClass represents the 
    /// 		object property class for the associated object property's base property.
    /// 	tableMapping: property to table mapping type
    /// 	tableName: class table name
    /// 	table: class table. Null if class has no table or table
    /// 		does not exist.

    FdoSmLpClassDefinition(
        FdoString* name,
        FdoString* description,
        FdoSmLpSchemaElement* parent,
        FdoPtr<FdoSmLpClassDefinition> pSrcClass,
        FdoSmOvTableMappingType tableMapping,
        FdoString* tableName,
        FdoSmPhDbObjectP table,
        FdoSchemaElementState elementState
    );

};

typedef FdoPtr<FdoSmLpClassDefinition> FdoSmLpClassDefinitionP;

#endif


