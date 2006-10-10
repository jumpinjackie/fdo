#ifndef FDORDBMSDESCRIBESCHEMA_H
#define FDORDBMSDESCRIBESCHEMA_H        1
#ifdef _WIN32
#pragma once
#endif
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

#include <map>
#include "Inc/Util/string.h"
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsConnection.h"

class FdoSmLpObjectPropertyDefinition;
class FdoSmLpGeometricPropertyDefinition;
class FdoSmLpAssociationPropertyDefinition;

// The DescribeSchema command describes the feature schemas available
// from the connection. The describe schema command can describe a
// single schema or all schemas available from the connection. The
// Execute operation returns a FeatureSchemaCollection object which
// is described in the FDO Feature Schema specification.
class FdoRdbmsDescribeSchemaCommand : public FdoRdbmsCommand<FdoIDescribeSchema>
{
    friend class FdoRdbmsConnection;
    friend class FdoRdbmsFeatureReader;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDescribeSchemaCommand(const FdoRdbmsDescribeSchemaCommand &right);

    // Constructs a default instance of a DescribeSchema command.
    FdoRdbmsDescribeSchemaCommand();

    // Constructs an instance of a DescribeSchema command using
    // the specified arguments.
    FdoRdbmsDescribeSchemaCommand(FdoIConnection* connection);

    // Constructs an instance of a DescribeSchema command using
    // the specified arguments.
    FdoRdbmsDescribeSchemaCommand(DbiConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~FdoRdbmsDescribeSchemaCommand();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDescribeSchemaCommand & operator=(const FdoRdbmsDescribeSchemaCommand &right);

    // Gets the name of the schema to describe. This is optional,
    // if not specified execution of the command will describe all schemas.
    virtual const wchar_t* GetSchemaName();

    // Sets the name of the schema to describe. This is optional, if not
    // specified execution of the command will describe all schemas.
    virtual void SetSchemaName(const wchar_t* value);

    // Executes the describe schema command and returns a
    // FeatureSchemaCollection. If a schema name is given that has
    // references to another schema, the dependent schemas will
    // be returned as well. If the specified schema name does not exist
    // the Execute method will throw an exception.
    virtual FdoFeatureSchemaCollection* Execute();

protected:
    DbiConnection*                        mRdbmsConnection;
    FdoStringP                            mSchemaName;

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

    MappingCollection               mMappingClass;
    MappingCollection               mMappingPropDef;

	FdoPtr<FdoIConnection>			mFdoConnection;
};

#endif  // FDORDBMSDESCRIBESCHEMA_H
