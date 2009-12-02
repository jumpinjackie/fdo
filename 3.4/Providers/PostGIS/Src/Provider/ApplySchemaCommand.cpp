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
    if (mFeatureSchema != schema)
    {
        mFeatureSchema = schema;
        FDO_SAFE_ADDREF(mFeatureSchema.p);
    }
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

ov::ClassDefinition* ApplySchemaCommand::GetClassDefinition(FdoStringP className) const
{
    ov::ClassDefinition* phClass = NULL;
    try
    {
        FdoPtr<FdoIdentifier> cid(FdoIdentifier::Create(className));
        SchemaDescription* schemaDesc = mConn->DescribeSchema();
        if (schemaDesc && schemaDesc->IsDescribed()) 
        {
            phClass = schemaDesc->FindClassMapping(cid);
        }
    }
    catch (FdoException* e)
    {
        // XXX - mloskot: Is this enough diagnostic to kill the exception
        //       and return NULL here ?
        FDOLOG_WRITE(L"ERROR: Describe operation for '%ls' failed. %ls"
            , static_cast<FdoString*>(className)
            , static_cast<FdoString*>(e->ToString()));

        phClass = NULL;
        e->Release();
    }
    return phClass;
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
    {
        FdoPtr<FdoClassCollection> featureClasses(mFeatureSchema->GetClasses());
        FdoInt32 const classesSize = featureClasses->GetCount();
     
        // NOTE - Eric Barby: First pass to check if table exist before creating it
        // or to check if table is empty before dropping it
        for (FdoInt32 i = 0; i < classesSize; i++)
        {
            FdoPtr<FdoClassDefinition> classDef(featureClasses->GetItem(i));
            switch ( classDef->GetElementState() )
            {
            case FdoSchemaElementState_Added:
                
                // NOTE - Eric Barby: Test if table exist before creating it
                if (!TestingCreateTable(GetClassDefinition(classDef->GetName())))
                {
                    FdoStringP msg = FdoStringP::Format(
                        L"[PostGIS] ApplySchemaCommand: Add class error! class '%ls' already exist!",
                        classDef->GetName());
                    FDOLOG_WRITE("ERROR: %ls", static_cast<FdoString*>(msg));
                    throw FdoCommandException::Create(msg);
                }
                break;

            case FdoSchemaElementState_Deleted:
                
                // NOTE - Eric Barby: Test if table is empty before dropping it
                if (!TestingDropTable(GetClassDefinition(classDef->GetName())))
                {
                    FdoStringP msg = FdoStringP::Format(
                        L"[PostGIS] ApplySchemaCommand: Delete class '%ls' error! table in not empty!",
                        classDef->GetName());
                    FDOLOG_WRITE("ERROR: %ls", static_cast<FdoString*>(msg));
                    throw FdoCommandException::Create(msg);
                }
                break;
            }
        }

        for (FdoInt32 i = 0; i < classesSize; i++)
        {
            FdoPtr<FdoClassDefinition> classDef(featureClasses->GetItem(i));
            switch ( classDef->GetElementState() )
            {
            case FdoSchemaElementState_Added:
                CreateTable(classDef);
                break;
            case FdoSchemaElementState_Deleted:
                DropTable(classDef);
                break;
            case FdoSchemaElementState_Detached:
                break;            
            case FdoSchemaElementState_Modified:
                AlterTable(classDef);
                break;
            case FdoSchemaElementState_Unchanged:
                break;
            }
        }

        mFeatureSchema->AcceptChanges();
    }

    {
        FdoPtr<FdoClassCollection> featClasses(mFeatureSchema->GetClasses());
        FdoInt32 const classesSize = featClasses->GetCount();
        for (FdoInt32 i = 0; i < classesSize; i++)
        {
            FdoPtr<FdoClassDefinition> classDef(featClasses->GetItem(i));
            if (FdoSchemaElementState_Unchanged != classDef->GetElementState())
            {
                std::string tableName(static_cast<char const*>(FdoStringP(classDef->GetName()).Lower()));
                FDOLOG_WRITE("Error of ClassDefinition State of '%s'", tableName.c_str());
            }
        }
    }
 }

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

bool ApplySchemaCommand::TestingCreateTable(ov::ClassDefinition* phClass) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-check before CreateTable");
    
    if (NULL == phClass)
    {
        return true;
    }

    // 
    // before CREATE TABLE command
    // 
    FdoStringP sql = FdoStringP::Format(
        L"SELECT schemaname, tablename FROM pg_tables WHERE schemaname='%ls' AND tablename ='%ls'"
        , static_cast<FdoString*>(phClass->GetSchemaName())
        , static_cast<FdoString*>(phClass->GetTableName() ));

    FdoPtr<FdoISQLCommand> cmd = static_cast<FdoISQLCommand*>(mConn->CreateCommand(FdoCommandType_SQLCommand));
    assert(NULL != cmd);

    cmd->SetSQLStatement(sql);
    FdoPtr<FdoISQLDataReader> reader = cmd->ExecuteReader();
    assert(NULL != reader);

    bool hasTuples = reader->ReadNext();
    return !hasTuples;
}

bool ApplySchemaCommand::TestingDropTable(ov::ClassDefinition* phClass) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-check before DropTable");

    assert(NULL != mFeatureSchema);
    if (NULL == phClass)
        return false;
    
    //
    // before DROP TABLE command
    // 
    FdoStringP sql = FdoStringP::Format(L"SELECT COUNT(*) AS nb FROM %ls",
        static_cast<FdoString*>(phClass->GetTableName()));

    FdoPtr<FdoISQLCommand> cmd = static_cast<FdoISQLCommand*>(mConn->CreateCommand(FdoCommandType_SQLCommand));
    assert(NULL != cmd);

    cmd->SetSQLStatement(sql);
    FdoPtr<FdoISQLDataReader> reader = cmd->ExecuteReader();
    assert(NULL != reader);

    bool hasData = false;
    if (reader->ReadNext()) 
    {
        if (reader->GetInt16(L"nb") > 0)
            hasData = true;
    }

    return !hasData;
}

void ApplySchemaCommand::AlterTable(FdoPtr<FdoClassDefinition> classDef) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-AlterTable");

    int nbPropModified=0;
    ov::ClassDefinition* phClass;
    phClass = this->GetClassDefinition(classDef->GetName());
    if (NULL == phClass)
    {
      CreateTable(classDef);
      return;
      /*
        FdoStringP msg = FdoStringP::Format(
            L"[PostGIS] ApplySchemaCommand: Alter class '%s' error! ClassDefinition not fount!",
            classDef->GetName());
        FDOLOG_WRITE("ERROR: %s", static_cast<FdoString*>(msg));
        throw FdoCommandException::Create(msg);
        */
    }
    
    std::string schemaName(static_cast<char const*>(phClass->GetSchemaName()));
    std::string tableName (static_cast<char const*>(phClass->GetTableName()));

    std::string sqlAlter("ALTER TABLE ");
    sqlAlter += tableName;

    FdoPtr<FdoPropertyDefinitionCollection> props(classDef->GetProperties());
    if (NULL != props && props->GetCount() > 0)
    {
        //
        // Read properties and parse details: name, data type, size.
        // 
        FdoInt32 const propsSize = props->GetCount();
        for (FdoInt32 j = 0; j < propsSize; j++)
        {
            FdoPtr<FdoPropertyDefinition> propDef(props->GetItem(j));
            if (FdoPropertyType_DataProperty == propDef->GetPropertyType())
            {
                FdoSchemaElementState propState = propDef->GetElementState();
                FdoStringP propName(propDef->GetName());
                std::string sqlType(details::PgTypeFromFdoProperty(propDef));
                if (sqlType.empty())
                {
                    throw FdoCommandException::Create(L"ApplySchemaCommand::Execute: Unkown data property type"); 
                }
                switch(propState)
                {
                  case FdoSchemaElementState_Added:
                    if(nbPropModified > 0) sqlAlter += ',';
                    sqlAlter += " ADD COLUMN ";
                    sqlAlter += static_cast<char const*>(propName);
                    sqlAlter += " " + sqlType;
                    nbPropModified++;
                    break;
                  case FdoSchemaElementState_Deleted:
                    if(nbPropModified > 0) sqlAlter += ',';
                    sqlAlter += " DROP COLUMN ";
                    sqlAlter += static_cast<char const*>(propName);
                    nbPropModified++;
                    break;
                }

            }
        }
    }
    if(nbPropModified)
    {
      FDOLOG_WRITE("SQL:\n\t%s", sqlAlter.c_str());
      mConn->PgExecuteCommand(sqlAlter.c_str());
      //
      // Update a Logical & Physical mapping of the feature class
      //
      mConn->ResetSchema();
    }
}

void ApplySchemaCommand::CreateTable(FdoPtr<FdoClassDefinition> classDef) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-CreateTable");

    FdoPtr<FdoPropertyDefinitionCollection> props(classDef->GetProperties());

    // TODO do not use Lower if ClassName starting and ending by ""
    FdoStringP className(static_cast<char const*>(FdoStringP(classDef->GetName()).Lower()));
    std::string   schemaName, tableName;
    // TODO check a SchemaMapping...

    ov::ClassDefinition* phClass;
    phClass = this->GetClassDefinition(classDef->GetName());
    if (phClass) 
    {
        //schemaName = static_cast<char const*>(phClass->GetSchemaName());
        schemaName = static_cast<char const*>(mConn->GetPgCurrentSchema());
        tableName  = static_cast<char const*>(phClass->GetTableName() );
    } 
    else // FIXME: It's recommended to visually indicate scopes with braces.
    if ( className.Contains(L"~"))
    {
        schemaName = className.Left(L"~");
        tableName  = className.Right(L"~");
    } 
    else
    { 
        schemaName = mConn->GetPgCurrentSchema();
        tableName  = className;
        //eric barby - table naming convention without ~
        //className  = FdoStringP(schemaName.c_str()) + L"~" + FdoStringP(tableName.c_str());
        className  = FdoStringP(tableName.c_str());
        classDef->SetName(className);
    }
    if (!schemaName.length())
    {
      schemaName = static_cast<char const*>(phClass->GetSchemaName());
      schemaName = static_cast<char const*>(mConn->GetPgCurrentSchema());
    }

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
                //sqlColumns += details::QuoteSqlName<std::string>(static_cast<char const*>(propName.Lower()));
                sqlColumns += static_cast<char const*>(propName.Lower());                
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
            InsertDummyRecord(tableName, propId);
        }

        // TODO: Add class description as a COMMENT

        // Update Logical & Physical mapping for the feature class
        //mConn->ResetSchema();

    } // if (NULL != props && props->GetCount() > 0)
}


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
    else if(scName.GetLength() > 0)
    { 
        // No spatial context has been created but
        // we got spatial association either PostGIS_srid or srid
        if(scName.Contains(FdoStringP("PostGIS_"))) 
        { 
            scName = scName.Mid(8,-1); 
        }

        if(scName.IsNumber()) 
        { 
            srid = (FdoInt32)scName.ToLong(); 
        } 
        else 
        { 
            FDOLOG_WRITE("ERROR: invalid SpatialContextAssociation %ls, should be PostGIS_srid or srid", static_cast<FdoString*>(scName)); 
        } 
    } 

    // Find spatial dimension and geometry type name
    int dimension = 2;
    bool isXYM = (prop->GetHasMeasure() && !prop->GetHasElevation());

    if (isXYM)
        dimension = 3;
    else if (prop->GetHasElevation() && prop->GetHasMeasure())
        dimension = 4;

    std::string type;
    FdoInt32 length = 0;
    FdoGeometryType* fdoTypes = prop->GetSpecificGeometryTypes(length);
    if (fdoTypes)
        type = ewkb::PgGeometryTypeFromFdoType(fdoTypes, length, isXYM);
    else 
        type = ewkb::PgGeometryTypeFromFdoType(prop->GetGeometryTypes(), isXYM);

    // Execute SQL
    std::stringstream sqlStm; 
    sqlStm << "SELECT AddGeometryColumn(current_schema()::text,"
        << ((std::string)details::QuoteSqlValue(table))  << ","
        << ((std::string)details::QuoteSqlValue(column)) << ","
        <<  srid << ","
        << ((std::string)details::QuoteSqlValue(type)) << ","
        << dimension << ")";

    std::string sql(sqlStm.str());
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
        maxValue = 9223372036854775807LL;

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

void ApplySchemaCommand::InsertDummyRecord(std::string const& table,
                                        FdoPtr<FdoDataPropertyDefinition> prop) const
{
    FDOLOG_MARKER("ApplySchemaCommand::-InsertDummyRecord");

    assert(!table.empty());
    std::string column(static_cast<char const*>(FdoStringP(prop->GetName()).Lower()));

    std::string sql("INSERT INTO " + table + "(" + column + ") values(0)");
    mConn->PgExecuteCommand(sql.c_str());
}

void ApplySchemaCommand::DropTable(FdoPtr<FdoClassDefinition> classDef) const
{
    assert(NULL != mFeatureSchema);
    FDOLOG_MARKER("ApplySchemaCommand::-DropTable");
    
    //
    // DROP TABLE command
    // 
    ov::ClassDefinition* phClass;
    phClass = this->GetClassDefinition(classDef->GetName());
    if (NULL == phClass)
    {
        FdoStringP msg = FdoStringP::Format(
            L"[PostGIS] ApplySchemaCommand: Delete class '%ls' error! ClassDefinition not fount!",
            classDef->GetName());
        FDOLOG_WRITE("ERROR: %ls", static_cast<FdoString*>(msg));
        throw FdoCommandException::Create(msg);
    }
    
    std::string schemaName(static_cast<char const*>(phClass->GetSchemaName()));
    std::string tableName (static_cast<char const*>(phClass->GetTableName()));

    std::string sqlDrop("DROP TABLE ");
    sqlDrop += tableName;

    FDOLOG_WRITE("SQL:\n\t%s", sqlDrop.c_str());

    mConn->PgExecuteCommand(sqlDrop.c_str());

    FdoPtr<FdoDataPropertyDefinitionCollection> propsIdentity(classDef->GetIdentityProperties());
    FdoInt32 const propsIdSize = propsIdentity->GetCount();
    
    //
    // DROP SEQUENCE for single column, integral type PRIMARY KEY
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
            std::string column(static_cast<char const*>(FdoStringP(propId->GetName()).Lower()));
            std::string sequence = details::MakeSequenceName(tableName, column);

            std::string sql("DROP SEQUENCE " + sequence);
            mConn->PgExecuteCommand(sql.c_str());
        }
    }

    //
    // Drop geometric property as PostGIS geometry column 
    //
    if (FdoClassType_FeatureClass == classDef->GetClassType())
    {
        FdoFeatureClass* featClass = static_cast<FdoFeatureClass*>(classDef.p);
        if (featClass)
        {
            FdoPtr<FdoGeometricPropertyDefinition> propGeom(featClass->GetGeometryProperty());
            std::string geomCol(static_cast<char const*>(FdoStringP(propGeom->GetName()).Lower()));
        
            // DELETE FROM geometry_column WHERE tableName AND geomCol;
            std::string sql("DELETE FROM geometry_columns "
                " WHERE  f_table_schema = '" + schemaName + "'"
                " AND f_table_name = '" + tableName + "'");

            mConn->PgExecuteCommand(sql.c_str());

            // TODO - Eric Barby: DROP Spatial Index ?
        }
    }

    //
    // Update a Logical & Physical mapping of the feature class
    //
    mConn->ResetSchema();

}

}} // namespace fdo::postgis

