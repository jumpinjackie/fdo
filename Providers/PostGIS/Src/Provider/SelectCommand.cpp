//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "SelectCommand.h"
#include "FeatureReader.h"
#include "FilterProcessor.h"
#include "Connection.h"
#include "PgCursor.h"
#include "PgUtility.h"
#include "PostGIS/FdoPostGisOverrides.h"

#include <cassert>
#include <string>

namespace fdo { namespace postgis {

SelectCommand::SelectCommand(Connection* conn) :
    Base(conn),
    mProperties(NULL),
    mOrderingProperties(NULL),
    mOrderingOption(FdoOrderingOption_Ascending)
{
    // idle
}

SelectCommand::~SelectCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SelectCommand::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIBaseSelect interface
///////////////////////////////////////////////////////////////////////////////

FdoIdentifierCollection* SelectCommand::GetPropertyNames()
{
    if (NULL == mProperties)
    {
        mProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mProperties.p);
    return mProperties.p;
}
 	
FdoIdentifierCollection* SelectCommand::GetOrdering()
{
    if (NULL == mOrderingProperties)
    {
        mOrderingProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mOrderingProperties.p);
    return mOrderingProperties.p;
}

void SelectCommand::SetOrderingOption(FdoOrderingOption option)
{
    FDOLOG_MARKER("SelectCommand::+SetOrderingOption");
    FDOLOG_WRITE("Ordering: %d", option);

    mOrderingOption = option;
}

FdoOrderingOption SelectCommand::GetOrderingOption()
{
    return mOrderingOption;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISelect interface
///////////////////////////////////////////////////////////////////////////////

FdoLockType SelectCommand::GetLockType()
{
    FDOLOG_MARKER("SelectCommand::+GetLockType");

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"GetLockType"));

    return FdoLockType_None;
}

void SelectCommand::SetLockType(FdoLockType value)
{
    FDOLOG_MARKER("SelectCommand::+SetLockType");

    FdoStringP msg(FdoStringP::Format(L"SetLockType($ls)", value));

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).",
        static_cast<FdoString*>(msg)));
}

FdoLockStrategy SelectCommand::GetLockStrategy()
{
    FDOLOG_MARKER("SelectCommand::+GetLockStrategy");

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"GetLockStrategy"));

    return FdoLockStrategy_All;
}

void SelectCommand::SetLockStrategy(FdoLockStrategy value)
{
    FDOLOG_MARKER("SelectCommand::+SetLockStrategy");

    FdoStringP msg(FdoStringP::Format(L"SetLockStrategy($ls)", value));

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).",
        static_cast<FdoString*>(msg)));
}
    
FdoIFeatureReader* SelectCommand::Execute()
{
    FDOLOG_MARKER("SelectCommand::+Execute");
    
    //////////////////////////////////////////////////////////////
    // TODO: Refactor this big function into smaller procedures
    //////////////////////////////////////////////////////////////

    try
    {
        //
        // Get Logical Schema
        //
        FdoPtr<FdoFeatureSchemaCollection> logicalSchemas;
        logicalSchemas = mConn->GetLogicalSchema();

        FDOLOG_WRITE(L"Number of logical schemas: %d", logicalSchemas->GetCount());

        //
        // Get Feature Class definition
        //
        FdoPtr<FdoClassDefinition> classDef = NULL;
        FdoPtr<FdoIdentifier> classIdentifier = GetFeatureClassName();
        assert(NULL != classIdentifier);
        FdoStringP classId = classIdentifier->GetText();

        FDOLOG_WRITE(L"Logical schema name: %s",
            static_cast<FdoString*>(classIdentifier->GetSchemaName()));
        FDOLOG_WRITE(L"Class name: %s",
            static_cast<FdoString*>(classIdentifier->GetName()));

        // Find definition of the feature class
        FdoPtr<FdoIDisposableCollection> featureClasses = NULL;
        featureClasses = logicalSchemas->FindClass(classId);
        if (NULL == featureClasses)
        {
            FDOLOG_WRITE(L"ERROR: Logical schema for '%s' not found",
                static_cast<FdoString*>(classId));
            return NULL;
        }

        FDOLOG_WRITE(L"Number of feature schemas: %d", featureClasses->GetCount());

        if (featureClasses->GetCount() <= 0)
        {
            FDOLOG_WRITE(L"ERROR: No class definition found for schema '%s'",
                static_cast<FdoString*>(classId));
            return NULL;
        }

        // We have a single-element collection here
        classDef = static_cast<FdoClassDefinition*>(featureClasses->GetItem(0));
        assert(NULL != classDef);
        FdoStringP className = classDef->GetName();

        FDOLOG_WRITE(L"Class definition for: %s", static_cast<FdoString*>(className));

        //
        // Get Physical Schema details
        //
        ov::PhysicalSchemaMapping::Ptr schemaMapping;
        schemaMapping = mConn->GetPhysicalSchemaMapping();

        FDOLOG_WRITE(L"Schema mapping for: %", schemaMapping->GetName());

        ov::ClassDefinition::Ptr phClassDef;
        phClassDef = schemaMapping->FindByClassName(className);
        if (NULL == phClassDef)
        {
            FDOLOG_WRITE(L"ERROR: Physical schema for '%s' not found",
                static_cast<FdoString*>(className));
            return NULL;
        }

        // NOTE: Schema and table names are wrapped with double quotes
        std::string tablePath(static_cast<char const*>(phClassDef->GetTablePath()));

        FDOLOG_WRITE("Table: %s", tablePath.c_str());

        //
        // Read properties definition to SQL columns
        //
        FdoInt32 currentSrid = 0;
        std::string sqlColumns("");
        FdoPtr<FdoPropertyDefinitionCollection> props = classDef->GetProperties();
        
        FDOLOG_WRITE("Number of properties: %d", props->GetCount());

        int const propsCount = props->GetCount();
        for (int propIdx = 0; propIdx < propsCount; propIdx++)
        {
            FdoPtr<FdoPropertyDefinition> propDef(props->GetItem(propIdx));

            assert(FdoPropertyType_DataProperty == propDef->GetPropertyType()
                || FdoPropertyType_GeometricProperty == propDef->GetPropertyType());

            // Find SRID of geometric property
            if (FdoPropertyType_GeometricProperty == propDef->GetPropertyType())
            {
                // TODO: It won't work with multiple-geometric properties
                FdoGeometricPropertyDefinition* geom = NULL;
                geom = static_cast<FdoGeometricPropertyDefinition*>(propDef.p);

                FdoString* csName = geom->GetSpatialContextAssociation();
                SpatialContextCollection::Ptr spContexts(mConn->GetSpatialContexts());
                SpatialContext::Ptr spc(spContexts->FindItem(csName));
                if (NULL != spc)
                {
                    currentSrid = spc->GetSRID();
                }

                FDOLOG_WRITE(L"\t+ %s (SRID=%d)", propDef->GetName(), currentSrid);
            }

            // Add property to columns list
            FdoStringP propName(propDef->GetName());

            if (propIdx > 0)
            {
                sqlColumns += ',';
            }
            sqlColumns += tablePath;
            sqlColumns += '.';
            sqlColumns += static_cast<char const*>(propName);

            FDOLOG_WRITE("\t- %s", static_cast<char const*>(propName));
        }

        //
        // Process filter conditions
        //
        std::string sqlWhere("");

        if (NULL != mFilter)
        {
            FDOLOG_WRITE("Configuring filter");

            FilterProcessor::Ptr proc(new FilterProcessor(currentSrid));
            mFilter->Process(proc);
            sqlWhere = proc->GetFilterStatement();
        }

        //
        // Compose SQL query with filters and ordering clauses if available
        //

        // 1 - Basic query
        std::string sql("SELECT " + sqlColumns + " FROM " + tablePath);

        // 2 - WHERE clause
        if (!sqlWhere.empty())
        {
            sql.append(" WHERE " + sqlWhere);
        }

        // 3 - ORDER BY clause
        FdoPtr<FdoIdentifierCollection> orderingColumns = GetOrdering();
        FdoInt32 const orderingCount = orderingColumns->GetCount();
        if (orderingCount > 0)
        {
            std::string sep("");
            sql.append(" ORDER BY ");

            for (FdoInt32 i = 0; i < orderingCount; ++i)
            {
                FdoPtr<FdoIdentifier> id(orderingColumns->GetItem(i));
                FdoStringP name(id->GetName());

                sql.append(sep + static_cast<char const*>(name));

                if (FdoOrderingOption_Ascending == GetOrderingOption())
                    sql.append(" ASC ");
                else
                    sql.append(" DESC ");

                sep = ",";
            }
        }

        FDOLOG_WRITE("SQL statement:\n\t%s", sql.c_str());

        //
        // Declare cursor and create feature reader
        //
        FDOLOG_WRITE("Creating cursor: crsFdoSelectCommand");

        PgCursor::Ptr cursor(NULL);
        cursor = mConn->PgCreateCursor("crsFdoSelectCommand");
        assert(NULL != cursor);

        // Collect bind parameters
        details::pgexec_params_t params;
        Base::PgGenerateExecParams(params);

        // Open new cursor
        cursor->Declare(sql.c_str(), params);
        
        FeatureReader::Ptr reader(new FeatureReader(mConn, cursor, classDef));
    
        FDO_SAFE_ADDREF(reader.p);
        return reader.p;
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("ERROR: The execution of Select command failed.");

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_SELECT_FAILED,
                "The execution of Select command failed."), e);
        e->Release();
        throw ne;
    }
}
    
FdoIFeatureReader* SelectCommand::ExecuteWithLock()
{
    FDOLOG_WRITE("ERROR: Execute with lock unsupported");

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"ExecuteWithLock"));

    return NULL;
}
    
FdoILockConflictReader* SelectCommand::GetLockConflicts()
{
    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"GetLockConflicts"));

    return NULL;
}

}} // namespace fdo::postgis
