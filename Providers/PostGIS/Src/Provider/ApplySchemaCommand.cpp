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
#include "ApplySchemaCommand.h"
#include "Connection.h"
#include "PgGeometry.h"
#include "PgUtility.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/format.hpp>

namespace fdo { namespace postgis {

ApplySchemaCommand::ApplySchemaCommand(Connection* conn) :
    Base(conn),
    mFeatureSchema(NULL),
    mSchemaMapping(NULL),
    mIgnoreStates(true)
{
    FDOLOG_WRITE("ApplySchemaCommand command created");
}

ApplySchemaCommand::~ApplySchemaCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIApplySchema interface
///////////////////////////////////////////////////////////////////////////////

FdoFeatureSchema* ApplySchemaCommand::GetFeatureSchema()
{
    FDO_SAFE_ADDREF(mFeatureSchema.p);
    return mFeatureSchema.p;
}

void ApplySchemaCommand::SetFeatureSchema(FdoFeatureSchema* schema)
{
    mFeatureSchema = schema;
    FDO_SAFE_ADDREF(mFeatureSchema.p);
}

FdoPhysicalSchemaMapping* ApplySchemaCommand::GetPhysicalMapping()
{
    FDO_SAFE_ADDREF(mSchemaMapping.p);
    return mSchemaMapping.p;
}

void ApplySchemaCommand::SetPhysicalMapping(FdoPhysicalSchemaMapping* sm)
{
    mSchemaMapping = sm;
    FDO_SAFE_ADDREF(mSchemaMapping.p);
}

FdoBoolean ApplySchemaCommand::GetIgnoreStates()
{
    return mIgnoreStates;
}

void ApplySchemaCommand::SetIgnoreStates(FdoBoolean ignoreStates)
{
    mIgnoreStates = ignoreStates;
}

void ApplySchemaCommand::Execute()
{
    FDOLOG_MARKER("ApplySchemaCommand::+Execute");

    assert(NULL != mFeatureSchema);

    if (!mIgnoreStates)
    {
        throw FdoCommandException::Create(L"[PostGIS] ApplySchemaCommand requires ignore states parameter as TRUE");
    }

    //
    // Read and parse feature classes to SQL commands
    //
    FdoPtr<FdoClassCollection> featureClasses(mFeatureSchema->GetClasses());

    FdoInt32 const classesSize = featureClasses->GetCount();
    for (FdoInt32 i = 0; i < classesSize; i++)
    {
        FdoPtr<FdoClassDefinition> classDef(featureClasses->GetItem(i));
        FdoPtr<FdoPropertyDefinitionCollection> props(classDef->GetProperties());

        if (NULL != props && props->GetCount() > 0)
        {
            std::string sep;
            std::string sqlColumns;

            //
            // Read properties and parse details: name, data type, size.
            // 
            FdoInt32 const propsSize = props->GetCount();
            for (FdoInt32 j = 0; j < propsSize; j++)
            {
                FdoPtr<FdoPropertyDefinition> propDef(props->GetItem(j));
                if (FdoPropertyType_DataProperty == propDef->GetPropertyType())
                {
                    FdoStringP propName(propDef->GetName());
                    std::string sqlType(details::PgTypeFromFdoProperty(propDef));
                    if (sqlType.empty())
                    {
                        throw FdoCommandException::Create(L"ApplySchemaCommand::Execute: Unkown data property type"); 
                    }

                    sqlColumns += sep;
                    
                    // Ticket #115
                    //sqlColumns += static_cast<char const*>(propName.Lower());
                    sqlColumns += details::QuoteSqlName<std::string>(static_cast<char const*>(propName.Lower()));
                    
                    sqlColumns += " " + sqlType;
                    sep = ",";
                }
            }

            //
            // Read identity properties used to build PRIMARY KEY
            //
            sep = "";
            std::string sqlPrimaryKey;
            FdoPtr<FdoDataPropertyDefinitionCollection> propsIdentity(classDef->GetIdentityProperties());
            FdoInt32 const propsIdSize = propsIdentity->GetCount();
            for (FdoInt32 j = 0; j < propsIdSize; j++)
            {
                FdoPtr<FdoDataPropertyDefinition> propId(propsIdentity->GetItem(j));
                FdoStringP propName(propId->GetName());

                if (sqlPrimaryKey.empty())
                {
                    sqlPrimaryKey = ", PRIMARY KEY(";
                }

                sqlPrimaryKey += sep;

                // Ticket #115: do we need to quote PK names?
                sqlPrimaryKey += static_cast<char const*>(propName.Lower());
                sep = ",";
            }

            if (!sqlPrimaryKey.empty())
            {
                sqlPrimaryKey += ")";
            }

            //
            // CREATE TABLE command
            // 
            std::string tableName(static_cast<char const*>(FdoStringP(classDef->GetName()).Lower()));

            std::string sqlCreate("CREATE TABLE ");
            sqlCreate += details::QuoteSqlName(tableName);
            sqlCreate += " ( ";
            sqlCreate += sqlColumns;
            if (!sqlPrimaryKey.empty())
            {
                sqlCreate += sqlPrimaryKey;
            }
            sqlCreate += " )";

            FDOLOG_WRITE("SQL:\n\t%s", sqlCreate.c_str());

            mConn->PgExecuteCommand(sqlCreate.c_str());

            //
            // Register geometric property as PostGIS geometry column 
            //
            if (FdoClassType_FeatureClass == classDef->GetClassType())
            {
                FdoFeatureClass* featClass = static_cast<FdoFeatureClass*>(classDef.p);

                AddGeometryColumn(tableName, featClass->GetGeometryProperty());

                // Create GiST index for table-column pair.
                CreateSpatialIndex(tableName, featClass->GetGeometryProperty());
            }

            //
            // CREATE SEQUENCE for single column, integral type PRIMARY KEY
            //
            if (1 == propsIdSize)
            {
                FdoPtr<FdoDataPropertyDefinition> propId = propsIdentity->GetItem(0);
                assert(NULL != propId);

                if (propId->GetIsAutoGenerated()
                    && (FdoDataType_Int16 == propId->GetDataType() 
                        || FdoDataType_Int32 == propId->GetDataType()
                        || FdoDataType_Int64 == propId->GetDataType()))
                {
                    CreateSequence(tableName, propId);
                }
            }

            // TODO: Add class description as a COMMENT


        } // if (NULL != props && props->GetCount() > 0)
    }
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void ApplySchemaCommand::AddGeometryColumn(std::string const& table,
                                           FdoPtr<FdoGeometricPropertyDefinition> prop) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-AddGeometryColumn");

    assert(!table.empty());
    assert(NULL != prop);

    std::string column(static_cast<char const*>(FdoStringP(prop->GetName()).Lower()));

    // Use undefined SRID value of -1 by default (see PostGIS manual, 4.2.3.)
    FdoInt32 srid = -1;
    FdoStringP scName = prop->GetSpatialContextAssociation();
    SpatialContextCollection::Ptr spContexts = mConn->GetSpatialContexts();
    SpatialContext::Ptr spContext = spContexts->FindItem(scName);
    if (NULL != spContext)
    {
        srid = spContext->GetSRID();
    }

    // Find spatial dimension and geometry type name
    int dimension = 2;
    bool isXYM = (prop->GetHasMeasure() && !prop->GetHasElevation());

    if (isXYM)
        dimension = 3;
    else if (prop->GetHasElevation() && prop->GetHasMeasure())
        dimension = 4;

    std::string type(ewkb::PgGeometryTypeFromFdoType(prop->GetGeometryTypes(), isXYM));

    // Execute SQL
    std::string sql("SELECT AddGeometryColumn(current_schema()::text,");
    sql += details::QuoteSqlValue(table) + ",";
    sql += details::QuoteSqlValue(column) + ",";
    sql += str(boost::format("%d") % srid);
    sql += "," + details::QuoteSqlValue(type) + ",";
    sql += str(boost::format("%d") % dimension);
    sql += ")";

    FDOLOG_WRITE("SQL:\n\t%s", sql.c_str());

    mConn->PgExecuteCommand(sql.c_str());
}

void ApplySchemaCommand::CreateSpatialIndex(std::string const& table,
                                            FdoPtr<FdoGeometricPropertyDefinition> prop) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-CreateSpatialIndex");

    assert(!table.empty());
    assert(NULL != prop);

    // PostGIS Documentation, 4.5.1. GiST Indexes
    // CREATE INDEX <indexname> ON <tablename> USING GIST (<geometryfield> GIST_GEOMETRY_OPS);
    std::string column(static_cast<char const*>(FdoStringP(prop->GetName()).Lower()));
    std::string sql("CREATE INDEX " + table + "_gist ON " + table + " USING GIST (" + column + " GIST_GEOMETRY_OPS)");

    FDOLOG_WRITE("SQL:\n\t%s", sql.c_str());

    mConn->PgExecuteCommand(sql.c_str());
}

void ApplySchemaCommand::CreateSequence(std::string const& table,
                                        FdoPtr<FdoDataPropertyDefinition> prop) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-CreateSequence");

    assert(!table.empty());

    FdoInt32 const incValue = 1;
    FdoInt32 const minValue = 1;
    FdoInt64 maxValue = 0;
    if (FdoDataType_Int16 == prop->GetDataType())
        maxValue = details::ePgTypeSmallintMax;
    else if (FdoDataType_Int32 == prop->GetDataType())
        maxValue = details::ePgTypeIntegerMax;
    else // FdoDataType_Int64
        maxValue = 9223372036854775807;

    std::string column(static_cast<char const*>(FdoStringP(prop->GetName()).Lower()));
    std::string sequence = details::MakeSequenceName(table, column);

    std::string sql("CREATE SEQUENCE " + sequence);
    sql += str(
        boost::format(" INCREMENT %1$d MINVALUE %2$d MAXVALUE %3$ld START %1$d  CACHE %1$d") 
                      % incValue % minValue % maxValue);
    mConn->PgExecuteCommand(sql.c_str());

    // Configure table

    sql = "ALTER TABLE " + table + " ALTER COLUMN " + column + " SET STORAGE PLAIN";
    mConn->PgExecuteCommand(sql.c_str());
    sql = "ALTER TABLE " + table + " ALTER COLUMN " + column + " SET NOT NULL";
    mConn->PgExecuteCommand(sql.c_str());
    sql = "ALTER TABLE " + table + " ALTER COLUMN " + column + " SET DEFAULT nextval(\'" + sequence + "\'::regclass)";
    mConn->PgExecuteCommand(sql.c_str());
}

}} // namespace fdo::postgis
