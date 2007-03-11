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
#include "DescribeSchemaCommand.h"
#include "SpatialContextCollection.h"
#include "PgGeometryColumn.h"
#include "PgSpatialTablesReader.h"
#include "PgTableColumnsReader.h"
#include "PgUtility.h"
#include "PostGIS/FdoPostGisOverrides.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

namespace fdo { namespace postgis {

DescribeSchemaCommand::DescribeSchemaCommand(Connection* conn)
    : mConn(conn)
{
    FDO_SAFE_ADDREF(mConn.p);
}

DescribeSchemaCommand::~DescribeSchemaCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void DescribeSchemaCommand::Dispose()
{
    FDOLOG_MARKER("DescribeSchemaCommand::#Dispose()");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommand interface
///////////////////////////////////////////////////////////////////////////////

FdoIConnection* DescribeSchemaCommand::GetConnection()
{
    FDO_SAFE_ADDREF(mConn.p);
    return mConn.p;
}

FdoITransaction* DescribeSchemaCommand::GetTransaction()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void DescribeSchemaCommand::SetTransaction(FdoITransaction* value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoInt32 DescribeSchemaCommand::GetCommandTimeout()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void DescribeSchemaCommand::SetCommandTimeout(FdoInt32 value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoParameterValueCollection* DescribeSchemaCommand::GetParameterValues()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void DescribeSchemaCommand::Prepare()
{
    assert(!"NOT IMPLEMENTED");
}

void DescribeSchemaCommand::Cancel()
{
    assert(!"NOT IMPLEMENTED");
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDescribeSchema interface
///////////////////////////////////////////////////////////////////////////////

FdoString* DescribeSchemaCommand::GetSchemaName()
{
    return mSchemaName;
}

void DescribeSchemaCommand::SetSchemaName(FdoString* name)
{
    assert(NULL != name);
    mSchemaName = name;
}

FdoFeatureSchemaCollection* DescribeSchemaCommand::Execute()
{
    FDOLOG_MARKER("DescribeSchemaCommand::=Execute");
    
    FdoPtr<FdoFeatureSchemaCollection> featSchemas = NULL;
    featSchemas = FdoFeatureSchemaCollection::Create(NULL);

    ov::PhysicalSchemaMapping::Ptr phSmMapping = NULL;
    phSmMapping = ov::PhysicalSchemaMapping::Create();

    SpatialContextCollection::Ptr spContexts(new SpatialContextCollection());
    
    //
    // Create FDO classes from PostGIS-enabled spatial tables
    //
    FdoPtr<FdoFeatureSchema> featSchema(FdoFeatureSchema::Create(L"FdoPostGIS", L""));
    featSchemas->Add(featSchema.p);

    FdoPtr<FdoClassCollection> featClasses(featSchema->GetClasses());
    ov::ClassCollection::Ptr phClasses(phSmMapping->GetClasses());
    
    //
    // Process every table to FDO class
    //
    PgSpatialTablesReader::Ptr stReader(new PgSpatialTablesReader(mConn.p));
    stReader->Open();
    
    while (stReader->ReadNext())
    {
        // TODO: Fetch all geometries, but not only the first one - default
        PgSpatialTablesReader::columns_t::size_type const geometryIdx = 0;
        PgSpatialTablesReader::columns_t geometryColumns(stReader->GetGeometryColumns());
        PgGeometryColumn::Ptr geomColumn = geometryColumns[geometryIdx];
        
        ////////////////// GENERATE SPATIAL CONTEXT //////////////////
        
        SpatialContext::Ptr spContext = NULL;
        FdoInt32 srid = geomColumn->GetSRID();
        if (srid >= 0)
        {
            FdoStringP spContextName = FdoStringP::Format(L"PostGIS_%d", srid);
            spContext = spContexts->FindItem(spContextName);
            if (NULL == spContext)
            {
                FDOLOG_WRITE(L"Created spatial context: %s",
                    static_cast<FdoString*>(spContextName));
                    
                spContext = CreateSpatialContext(spContextName, geomColumn);
                spContexts->Insert(0, spContext);
            }
        }
        else
        {
            // The CS is unknown, so default spatial context is used
            spContext = spContexts->FindItem(SpatialContextDefaultName);
        }
        
        ////////////////// CALCULATE SPATIAL EXTENT //////////////////
        
        // TODO
        
        ////////////////// GENERATE CLASS DEFINITION //////////////////

        // TODO: Do we need to have geometry column name in FDO class name?
        FdoStringP fdoClassName = FdoStringP::Format(L"%s~%s",
            static_cast<FdoString*>(stReader->GetSchemaName()),
            static_cast<FdoString*>(stReader->GetTableName()));
        assert(!featClasses->FindItem(fdoClassName));
        
        // TODO: Use table COMMENT as a class description
        FdoPtr<FdoFeatureClass> featClass = FdoFeatureClass::Create(fdoClassName, L"");      

        // Physical mapping for the feature class
        ov::ClassDefinition::Ptr classDef = ov::ClassDefinition::Create();
        classDef->SetName(fdoClassName);
        
        FdoPtr<FdoPropertyDefinitionCollection> pdc = featClass->GetProperties();

        FDOLOG_WRITE(L"Created feature class: %s", static_cast<FdoString*>(fdoClassName));
        
        ////////////////// CREATE GEOMETRY PROPERTY //////////////////
        
        FdoInt32 fdoGeomType = geomColumn->GetGeometryType();
        
        FdoPtr<FdoGeometricPropertyDefinition> geomPropDef = NULL;
        geomPropDef = FdoGeometricPropertyDefinition::Create(
            geomColumn->GetName(), geomColumn->GetDescription());                        
            
        geomPropDef->SetGeometryTypes(fdoGeomType);  
        if (NULL != spContext)
        {
            geomPropDef->SetSpatialContextAssociation(spContext->GetName());
        }
        pdc->Add(geomPropDef);
        featClass->SetGeometryProperty(geomPropDef);
        
        FDOLOG_WRITE(L"+ geometric property: %s",
            static_cast<FdoString*>(geomColumn->GetName()));

        ////////////////// CREATE DATA PROPERTIES //////////////////
        
        PgTableColumnsReader::Ptr tcReader  = new PgTableColumnsReader(
            mConn, stReader->GetSchemaName(), stReader->GetTableName());
        tcReader->Open();
        while(tcReader->ReadNext())
        {
            FdoPtr<FdoDataPropertyDefinition> datPropDef = NULL;
            datPropDef = FdoDataPropertyDefinition::Create(
                tcReader->GetColumnName(), tcReader->GetColumnDescription());
                
            FDOLOG_WRITE(L"+ data property: %s",
                static_cast<FdoString*>(tcReader->GetColumnName()));
    
            FdoDataType dataType = tcReader->GetColumnType();
            datPropDef->SetDataType(dataType);
            int size = tcReader->GetColumnSize();
            datPropDef->SetLength(size);
            int precision = tcReader->GetColumnPrecision();
            datPropDef->SetPrecision(precision);
            int scale = tcReader->GetColumnScale();
            datPropDef->SetPrecision(scale);
            
            pdc->Add(datPropDef);
        }
        tcReader->Close();
        
        //
        // Add Feature Class and Class Definition to collections
        //
        featClasses->Add(featClass);
        phClasses->Add(classDef);
        
    } // while
    
    stReader->Close();
    
    assert(NULL != featSchemas);
    assert(NULL != phClasses);
    
    FDO_SAFE_ADDREF(featSchemas.p);
    return featSchemas.p;
}

///////////////////////////////////////////////////////////////////////////////
// Private operations interface
///////////////////////////////////////////////////////////////////////////////

SpatialContext* DescribeSchemaCommand::CreateSpatialContext(
    FdoStringP spContextName, PgGeometryColumn::Ptr column)
{
    FDOLOG_MARKER("DescribeSchemaCommand::-CreateSpatialContext");

    //
    // Query for SRS details
    //
    
    std::string srid;
    try
    {
        srid = boost::lexical_cast<std::string>(column->GetSRID());
    }
    catch (boost::bad_lexical_cast& e)
    {
        srid = "-1";
        FDOLOG_WRITE("Number to string conversion failed: %s", e.what());
        assert(!"FIX HANDLING INVALID SRID");
    }
    
    std::string sql("SELECT srtext FROM spatial_ref_sys WHERE srid = " + srid);
    
    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);    
    if (PGRES_TUPLES_OK != PQresultStatus(pgRes.get()))
    {
        // TODO: Replace with exception
        assert(!"SRS NOT FOUND");
    }
    assert(1 == PQntuples(pgRes.get()));

    //
    // Generate spatial context details
    // 
    
    SpatialContext::Ptr spContext(new SpatialContext());
    spContext->SetName(spContextName);

    int const nfield = PQfnumber(pgRes.get(), "srtext");
    std::string wkt(PQgetvalue(pgRes.get(), 0, nfield));
    
    // Use substring between first quotes ("") as the SRS name
    std::string wktName("UNKNOWN");
    std::string::size_type pos1 = wkt.find_first_of('"') + 1;
    std::string::size_type pos2 = wkt.find_first_of(',') - 1;
    if (pos1 != std::string::npos && pos2 != std::string::npos)
    {
        wktName = wkt.substr(pos1, pos2 - pos1);
    }
    
    FdoStringP csName(wktName.c_str());
    spContext->SetCoordSysName(csName);

    FdoStringP csWkt(wkt.c_str());
    spContext->SetCoordinateSystemWkt(csWkt);

    FDO_SAFE_ADDREF(spContext.p);
    return spContext.p;
}

}} // namespace fdo::postgis
