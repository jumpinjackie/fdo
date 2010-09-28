#ifndef FDOSMLPSCHEMACOLLECTION_H
#define FDOSMLPSCHEMACOLLECTION_H		1
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

#include <Sm/Lp/Schema.h>
#include <Sm/NamedCollection.h>
#include <Sm/Lp/SpatialContextMgr.h>

class FdoSmLpObjectPropertyDefinition;
class FdoSmLpAssociationPropertyDefinition;

// Collection of the Feature Schemas ( in LogicalPhysical form )
// from the current datastore.
class FdoSmLpSchemaCollection : public FdoSmNamedCollection<FdoSmLpSchema>
{
public:
    /// Create an empty collection.
	FdoSmLpSchemaCollection(void);

    /// Create a populated collection.
	//
    /// Parameters:
    /// 	physicalSchema: Physical Schema Manager. All Feature Schemas in 
    ///          the current database are added to this collection.
    ///     scMgr : Spatial Context Manager (for retrieving spatial contexts)
	FdoSmLpSchemaCollection( FdoSmPhMgrP physicalSchema, FdoSmLpSpatialContextMgrP scMgr );

	~FdoSmLpSchemaCollection(void);

    // returns true if allowed to create tables and columns from added elements
    // in this feature schema collection. 
    // returns true only if both of the following are true:
    //
    //  - Provider supports applying schema to non-FDO datastore.
    //  See CanApplySchemaWithoutMetaSchema().
    //  - Physical object creation has been turned on
    //  See SetCreatePhysicalObjects().

    virtual bool CanCreatePhysicalObjects() const;

    // Returns true if the current provider supports applying feature
    // schemas to datastores without FDO MetaSchema.
    // Returns false by default. Providers that support apply to non-FDO
    // datastore must override this function in their specific implementation.
    virtual bool CanApplySchemaWithoutMetaSchema() const;

    // Sets whether tables and columns can be created from elements in this
    // Feature Schema collection. The main purpose of this function is to 
    // ensure that LogicalPhysical schema collection only modifies the 
    // physical schema when an ApplySchema command is in progress.
    void SetCreatePhysicalObjects( bool createPhysicalObjects );

    // Returns true if  tables and columns can be created from elements in 
    // this Feature Schema collection.
    bool GetCreatePhysicalObjects() const;

    FdoSmPhMgrP GetPhysicalSchema()
    {
        return mPhysicalSchema;
    }

    // Get the Spatial Context Manager for LogicalPhysical Spatial Contexts
    FdoSmLpSpatialContextMgrP GetSpatialContextMgr()
    {
        return mSpatialContextMgr;
    }

    /// Finds a class object by name. Returns NULL if the class is not
    /// in any schemas in the current database.
	//
    /// Parameters:
    /// 	schemaName: the class's containing schema.
    /// 		if L"" then all schemas are searched. An exception is 
    /// 		thrown if schemaName is L"" and the class is in 
    /// 		multiple schemas.
    /// 	className: the name of the class to find.
	const FdoSmLpClassDefinition* FindClass( FdoStringP schemaName, FdoStringP className ) const;

    /// Finds a class object by id. Returns NULL if the class is not 
    /// in any schemas.
	//
    /// Parameters:
    /// 	class id

	const FdoSmLpClassDefinition* FindClass(FdoInt64 classId) const;

    // Converts this collection of logical-physical schemas into a collection
    // of FDO (logical) schemas.
    //
    // Parameters:
    //      schemaName: if specified, convert and return only the specified schema plus 
    //          any it depends on. If "" then convert all schemas.
    FdoFeatureSchemasP GetFdoSchemas( FdoStringP schemaName);
    
    /// Given a table, returns the list of classes for which this table
    /// is the class table.
	//
    /// Parameters:
    /// 	tableName: the table to check
    /// 	ownerName: the datastore where table resides,
    ///          default is current datastore.
    /// 	databaseName: the database where table resides,
    ///          default is current database.

    FdoSmLpQClassesP TableToClasses( 
        FdoStringP tableName, 
        FdoStringP ownerName = L"",
        FdoStringP databaseName = L"",
        bool cacheOnly = true
    ) const;    
/*
    /// Given a class and list of data properties. This function returns a Join Tree of all
    /// tables that a select of the given properties must include.
    /// See FdoSmLpClassDefinition::GetJoinTree() for more information about the
    /// join tree.
	//
    /// Returns NULL if the class was not found or the class has no class table.
	//
    /// Parameters:
    /// 	schemaName: the class's containing schema.
    /// 		if L"" then all schemas are searched.
    /// 	className: the name of the class to find.
    /// 	properties: a list of data properties. The join tree will contain all tables
    /// 		that need to be joined into a select statement that selects these properties.
	FdoSmLpJoinTreeNode* GetJoinTree( FdoStringP schemaName, FdoStringP className, const FdoStringsP& properties ) const;
*/
    /// Constructs an instance of a LogicalPhysical Feature Schema 
    /// Definition from an FDO feature schema
	//
    /// Parameters:
    /// 	pfeatSchema: the FDO feature schema
    /// 	pClassOverrides: schema overrides for the feature schema
    ///      bIgnoreStates: true if the element state on the FDO feature schema is 
    ///          to be ignored.
    FdoSmLpSchemaP CreateSchema(
		FdoFeatureSchema* pFeatSchema,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
	);

    /// Post outstanding modifications to the current database.
	virtual void Commit();

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    void Load();

    /// Output the schemas to an XML file.
    /// Mainly for unit testing.
	void XMLSerialize( FdoString* sFileName ) const;

    /// Retrieve the collection of available feature class names
    FdoStringCollection* GetClassNames(FdoStringP schemaName);

    /// Retrieve the schema collection for containg the requested schema and feature classes
    FdoFeatureSchemasP GetFdoSchemasEx(FdoStringP schemaName, FdoStringCollection* classNames);

    /// Retrieve the collection of available schema names
    FdoStringCollection* GetSchemaNames();

protected:
    /// LogicalPhysical Feature Schema constructors,
    /// Each provider must implement.
    virtual FdoSmLpSchemaP NewSchema( FdoSmPhSchemaReaderP rdr ) = 0;
    virtual FdoSmLpSchemaP NewSchema(
		FdoFeatureSchema* pFeatSchema,
        bool bIgnoreStates
	) = 0;


private:
    //////////////////////////////////////////////////////////////////////////
    // Implementation Helper classes

    class SchemaCollection : public FdoSmNamedCollection<FdoSmLpSchema>
    {
    public:
        SchemaCollection():FdoSmNamedCollection<FdoSmLpSchema>() {;}

        virtual void Dispose() { delete this; }

		void AddReference(const FdoSmLpSchema* pSchema)
        {
            if (!Contains(pSchema))
                Add((FdoSmLpSchema*)pSchema);    // cast drops const
        }

        void Merge(SchemaCollection& x)
        {
            for (int i=0; i < x.GetCount(); i++)
            {
                FdoPtr<FdoSmLpSchema>   pItem = x.GetItem(i);

                // see if this item is already in this collection
                bool    bFound = false;
                for (int j=0; j < GetCount(); j++)
                {
					FdoPtr<FdoSmLpSchema> pCurrItem = GetItem(j);
                    if ( (FdoSmLpSchema*) pCurrItem == (FdoSmLpSchema*) pItem )
                    {
                        bFound = true;
                        break;
                    }
                }

                // skip the metaclass schema (not exposed to FDO).
                if ( !bFound && (wcscmp(pItem->GetName(), FdoSmPhMgr::mMetaClassSchemaName) != 0) )
                    Add(pItem);
            }
        }
    };

    class ClassCollection : public FdoSmNamedCollection<FdoSmLpClassDefinition>
    {
    public:
        ClassCollection():FdoSmNamedCollection<FdoSmLpClassDefinition>() {;}

        virtual void Dispose() {delete this; }

        void AddReference(const FdoSmLpClassDefinition* pClassDef)
        {
            if (!Contains(pClassDef))
                Add((FdoSmLpClassDefinition*)pClassDef);
        }

        void Merge(ClassCollection& x)
        {
            for (int i = 0; i < x.GetCount(); i++)
            {
                FdoPtr<FdoSmLpClassDefinition> pItem = x.GetItem(i);

                // see if this item is already in this collection
                bool bFound = false;
                for (int j = 0; j < GetCount(); j++)
                {
                    FdoPtr<FdoSmLpClassDefinition> pCurrItem = GetItem(j);
                    if ( (FdoSmLpClassDefinition*) pCurrItem == (FdoSmLpClassDefinition*) pItem )
                    {
                        bFound = true;
                        break;
                    }
                }
                
                if (!bFound)
                    Add(pItem);
            }
        }
    };

    class SchemaElementCollection
    {
    public:
        SchemaCollection schemas;
        ClassCollection classes;
        void Merge(SchemaElementCollection& x)
        {
            schemas.Merge(x.schemas);
            classes.Merge(x.classes);
        }
    };

    class MapItem : public FdoSmDisposable
    {
    public:
        virtual void Dispose() { delete this; }

        const FdoSmLpSchemaElement*  pFrom;
        FdoSchemaElement*               pTo;
    };
	class MappingCollection : public FdoSmDisposable
    {
    public:
        MappingCollection() {;}

        virtual void Dispose() { delete this; }

		void Add(const FdoSmLpSchemaElement* pFrom, FdoSchemaElement* pTo)
        {
			mMap.insert( std::pair<const FdoSmLpSchemaElement*,const FdoSchemaElement*>(pFrom, pTo ) );
        }

        void* Map(const FdoSmLpSchemaElement* pFrom)
        {
			std::map <const FdoSmLpSchemaElement*,const FdoSchemaElement*> :: const_iterator iter;
            if (pFrom)
            {
				iter = mMap.find( pFrom );
				if ( iter != mMap.end() ) 
					return( (FdoSmLpSchemaElement*) iter->second );
            }

            return NULL;
        }

        void Clear()
        {
            mMap.clear();
        }

	private:
		std::map<const FdoSmLpSchemaElement*,const FdoSchemaElement*> mMap;
    };

    FdoFeatureSchema*               ConvertSchema(const FdoSmLpSchema* pLpSchema, SchemaElementCollection& aReferenced);
    FdoDataPropertyDefinition*      ConvertDataPropertyDefinition(const FdoSmLpDataPropertyDefinition* pLpDataPropDef, SchemaCollection& aReferenced);
    FdoObjectPropertyDefinition*    ConvertObjectPropertyDefinition(const FdoSmLpObjectPropertyDefinition* pLpObjPropDef, SchemaElementCollection& aReferenced);
    FdoGeometricPropertyDefinition* ConvertGeometricPropertyDefinition(const FdoSmLpGeometricPropertyDefinition* pLpGeomPropDef, SchemaCollection& aReferenced);
    FdoAssociationPropertyDefinition* ConvertAssociationPropertyDefinition(const FdoSmLpAssociationPropertyDefinition* pLpAssocPropDef, SchemaElementCollection& aReferenced);
    void                            ConvertSAD(const FdoSmLpSchemaElement* pLpElement, FdoSchemaElement* pFdoElement);
	void							ConvertConstraints(const FdoSmLpClassDefinition* pLpClassDef, FdoClassDefinition* pFdoClassDef);
	FdoPtr<FdoDataValue>			FixDataValueType( FdoPtr<FdoDataValue> sourceVal, FdoDataType destType );

    FdoFeatureSchema*               ConvertSchema(const FdoSmLpSchema* pLpSchema, const FdoSmLpClassDefinition* pLpClassDef, SchemaElementCollection& aReferenced);
    FdoClassDefinition*              ConvertClassDefinition(const FdoSmLpClassDefinition* pLpClassDef, SchemaElementCollection& aReferenced);

    MappingCollection               mMappingClass;
    MappingCollection               mMappingPropDef;
    MappingCollection               mMappingSchema;

    bool        mSchemasLoaded;
	FdoSmPhMgrP mPhysicalSchema;
    FdoSmLpSpatialContextMgrP mSpatialContextMgr;

    bool        mCreatePhysicalObjects;

    int        mFoundCount;
};

typedef FdoPtr<FdoSmLpSchemaCollection> FdoSmLpSchemasP;

#endif



