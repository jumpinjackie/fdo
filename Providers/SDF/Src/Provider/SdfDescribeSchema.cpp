// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfDescribeSchema.h"
#include "SdfConnection.h"
#include "SchemaDb.h"
#include "FdoCommonSchemaUtil.h"
#include <algorithm>

#ifndef _MSC_VER
#define _wcsicmp wcscasecmp
#endif

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs a DescribeSchema command using the specified connection
SdfDescribeSchema::SdfDescribeSchema(SdfConnection* connection)
: SdfCommand<FdoIDescribeSchema>(connection)
{
    m_schemaName = NULL;
    m_classNames = NULL;
}


// default destructor
SdfDescribeSchema::~SdfDescribeSchema()
{
    FDO_SAFE_RELEASE(m_classNames); 
    if (m_schemaName != NULL)
    {
        // use free() since we allocated with malloc via wcsdup()
        free(m_schemaName);
        m_schemaName = NULL;
    }
}

//-------------------------------------------------------
// FdoIDescribeSchema implementation
//-------------------------------------------------------


// Gets the name of the schema to describe.  This is optional,
// if not specified execution of the command will describe all schemas.
FdoString* SdfDescribeSchema::GetSchemaName()
{
    return m_schemaName;
}


// Sets the name of the schema to describe.  This is optional, if not
// specified execution of the command will describe all schemas.
void SdfDescribeSchema::SetSchemaName(FdoString* value)
{
    if (m_schemaName != NULL)
    {
        // use free() since we allocated with malloc via wcsdup()
        free(m_schemaName);
        m_schemaName = NULL;
    }

    if (value != NULL)
#ifdef _WIN32
        m_schemaName = _wcsdup(value);
#else
        m_schemaName = wcsdup(value);
#endif
}


// Gets the names of the classes to retrieve. This is optional,
// if not specified execution of the command will describe all classes.
// If the class name is not qualified, and the schema name is not specified,
// the requested class from all schemas will be described.
// The class names specified serve only as a hint.  Use of the hint
// during command execution is provider dependent.  Providers that 
// will not use the hint will describe the schema for all classes.
FdoStringCollection* SdfDescribeSchema::GetClassNames()
{
    return FDO_SAFE_ADDREF(m_classNames);
}

// Sets the name of the classes to retrieve. This is optional, if not
// specified execution of the command will describe all classes.
// If the class name is not qualified, and the schema name is not specified,
// the requested class from all schemas will be described.
// The class names specified serve only as a hint.  Use of the hint
// during command execution is provider dependent.  Providers that 
// will not use the hint will describe the schema for all classes.
void SdfDescribeSchema::SetClassNames(FdoStringCollection* value)
{
    FDO_SAFE_RELEASE(m_classNames); 
    m_classNames = FDO_SAFE_ADDREF(value); 
}

// Executes the describe schema command and returns a FeatureSchemaCollection.
// If a schema name is given that has references to another schema, the dependent
// schemas will be returned as well.  If the specified schema name does not exist
// the Execute method will throw an exception.
FdoFeatureSchemaCollection* SdfDescribeSchema::Execute()
{
    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    // create the collection to return
    FdoPtr<FdoFeatureSchemaCollection> schemaCollection = FdoFeatureSchemaCollection::Create(NULL);

    // Re-read a fresh copy of the schema from the SDF file; this is significant
    // if the caller has changed the spatial contexts in the file since doing ApplySchema:
    FdoFeatureSchema* schema = m_connection->GetSchema(GetSchemaName());
    if (schema != NULL)
    {
        FdoPtr<FdoFeatureSchema> sch = FdoFeatureSchema::Create(schema->GetName(), schema->GetDescription());
        schemaCollection->Add(sch);
        FdoPtr<FdoClassCollection> clssDest = sch->GetClasses();
        int cnt = (m_classNames == NULL) ? 0 : m_classNames->GetCount();
        if (cnt != 0)
        {
            FdoPtr<FdoClassCollection> clss = schema->GetClasses();
            for (int i = 0; i < cnt; i++)
            {
                FdoPtr<FdoIdentifier> idClass = FdoIdentifier::Create(m_classNames->GetString(i));
                FdoPtr<FdoClassDefinition> cls = clss->GetItem(idClass->GetName());
                FdoPtr<FdoClassDefinition> fc_copy = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(cls);                
                clssDest->Add(fc_copy);
            }
        } 
        else
        {
            FdoPtr<FdoClassCollection> clss = schema->GetClasses();
            cnt = clss->GetCount();
            for (int i = 0; i < cnt; i++)
            {
                FdoPtr<FdoClassDefinition> cls = clss->GetItem(i);
                FdoPtr<FdoClassDefinition> fc_copy = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(cls);                
                clssDest->Add(fc_copy);
            }
        }
        sch->AcceptChanges();
    }
    return schemaCollection.Detach();
}

FdoStringCollection* SdfGetSchemaNames::Execute()
{
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    if (m_schemaNames != NULL)
        return m_schemaNames;

    m_schemaNames = FdoStringCollection::Create();

    FdoFeatureSchema* schema = m_connection->GetSchema();
    m_schemaNames->Add(schema->GetName());

    return FDO_SAFE_ADDREF(m_schemaNames);
}

static bool Compare(const wchar_t* _Left, const wchar_t* _Right)
{
    return _wcsicmp(_Left, _Right) < 0;
}

FdoStringCollection* SdfGetClassNames::Execute()
{
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    if (m_clsNames != NULL)
        return m_clsNames;

    FdoFeatureSchema* schema = m_connection->GetSchema(m_schName.c_str());
    m_clsNames = FdoStringCollection::Create();
    
    if (schema != NULL)
    {
        FdoPtr<FdoClassCollection> clss = schema->GetClasses();
        std::vector<FdoString*> vclss;
        int cnt = clss->GetCount();
        if (cnt > 0)
        {
            std::wstring qName((FdoString*)schema->GetQualifiedName());
            qName.append(L":");
            for (int i = 0; i < cnt; i++)
            {
                FdoPtr<FdoClassDefinition> cls = clss->GetItem(i);
                vclss.push_back(cls->GetName());
            }
            std::sort(vclss.begin(), vclss.end(), Compare);
            for (int i = 0; i < cnt; i++)
            {
                std::wstring clsqName = qName + vclss.at(i);
                m_clsNames->Add(clsqName.c_str());
            }
        }
    }
    return FDO_SAFE_ADDREF(m_clsNames);
}
