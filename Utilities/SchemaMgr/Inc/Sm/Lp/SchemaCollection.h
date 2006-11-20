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
#include <Sm/Lp/SpatialContextCollection.h>

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
	FdoSmLpSchemaCollection( FdoSmPhMgrP physicalSchema, FdoSmLpSpatialContextsP spatialContexts );

	~FdoSmLpSchemaCollection(void);

    FdoSmPhMgrP GetPhysicalSchema()
    {
        return mPhysicalSchema;
    }

    FdoSmLpSpatialContextsP GetSpatialContexts()
    {
        return mSpatialContexts;
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
        FdoStringP databaseName = L""
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

    FdoFeatureSchema*               ConvertSchema(const FdoSmLpSchema* pLpSchema, SchemaCollection& aReferenced);
    FdoClassDefinition*             ConvertClassDefinition(const FdoSmLpClassDefinition* pLpClassDef, SchemaCollection& aReferenced);
    FdoDataPropertyDefinition*      ConvertDataPropertyDefinition(const FdoSmLpDataPropertyDefinition* pLpDataPropDef, SchemaCollection& aReferenced);
    FdoObjectPropertyDefinition*    ConvertObjectPropertyDefinition(const FdoSmLpObjectPropertyDefinition* pLpObjPropDef, SchemaCollection& aReferenced);
    FdoGeometricPropertyDefinition* ConvertGeometricPropertyDefinition(const FdoSmLpGeometricPropertyDefinition* pLpGeomPropDef, SchemaCollection& aReferenced);
    FdoAssociationPropertyDefinition* ConvertAssociationPropertyDefinition(const FdoSmLpAssociationPropertyDefinition* pLpAssocPropDef, SchemaCollection& aReferenced );
    void                            ConvertSAD(const FdoSmLpSchemaElement* pLpElement, FdoSchemaElement* pFdoElement);
	void							ConvertConstraints(const FdoSmLpClassDefinition* pLpClassDef, FdoClassDefinition* pFdoClassDef);
	FdoPtr<FdoDataValue>			FixDataValueType( FdoPtr<FdoDataValue> sourceVal, FdoDataType destType );

    MappingCollection               mMappingClass;
    MappingCollection               mMappingPropDef;

    bool        mSchemasLoaded;
	FdoSmPhMgrP mPhysicalSchema;
    FdoSmLpSpatialContextsP mSpatialContexts;
};

typedef FdoPtr<FdoSmLpSchemaCollection> FdoSmLpSchemasP;

#endif


