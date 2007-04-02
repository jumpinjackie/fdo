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
#include "Connection.h"
#include "PostGisProvider.h"
#include "SchemaDescription.h"
#include "SpatialContextCollection.h"
#include "PgGeometryColumn.h"
#include "PgSpatialTablesReader.h"
#include "PgTableColumnsReader.h"
#include "PgUtility.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

SchemaDescription::SchemaDescription()
    : mLogicalSchemas(NULL),
      mSchemaMapping(NULL),
      mSpatialContexts(NULL),
      mIsDescribed(false)
{
}

SchemaDescription::~SchemaDescription()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SchemaDescription::Dispose()
{
    //FDOLOG_MARKER("SchemaDescription::#Dispose");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// SchemaDescription interface
///////////////////////////////////////////////////////////////////////////////

SchemaDescription* SchemaDescription::Create()
{
    SchemaDescription::Ptr sd(new SchemaDescription());
    
    FDO_SAFE_ADDREF(sd.p);
    return sd.p;
}

bool SchemaDescription::IsDescribed() const
{
    return mIsDescribed;
}

void SchemaDescription::SetLogicalSchemas(FdoFeatureSchemaCollection* logicalSchemas)
{
    mLogicalSchemas = logicalSchemas;
    FDO_SAFE_ADDREF(mLogicalSchemas.p);
}

void SchemaDescription::SetSchemaMapping(ov::PhysicalSchemaMapping* phSchemaMapping)
{
    mSchemaMapping = phSchemaMapping;
    FDO_SAFE_ADDREF(mSchemaMapping.p);
}

void SchemaDescription::SetSpatialContexts(SpatialContextCollection* spContexts)
{
    mSpatialContexts = spContexts;
    FDO_SAFE_ADDREF(mSpatialContexts.p);
}

FdoFeatureSchemaCollection* SchemaDescription::GetLogicalSchemas() const
{
    FDO_SAFE_ADDREF(mLogicalSchemas.p);
    return mLogicalSchemas.p;
}

ov::PhysicalSchemaMapping* SchemaDescription::GetSchemaMapping() const
{
    FDO_SAFE_ADDREF(mSchemaMapping.p);
    return mSchemaMapping.p;
}

SpatialContextCollection* SchemaDescription::GetSpatialContexts() const
{
    FDO_SAFE_ADDREF(mSpatialContexts.p);
    return mSpatialContexts.p;
}

FdoClassDefinition* SchemaDescription::FindClassDefinition(FdoIdentifier* id)
{
    assert(NULL != id);
    
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas(GetLogicalSchemas());
    FdoPtr<FdoClassDefinition> classDef;

    if (NULL != logicalSchemas && logicalSchemas->GetCount() > 0)
    {
        FdoStringP name(id->GetText());
        FdoPtr<FdoIDisposableCollection> featClasses(logicalSchemas->FindClass(name));
        if (NULL != featClasses)
        {
            classDef = static_cast<FdoClassDefinition*>(featClasses->GetItem(0));
        }
    }

    FDO_SAFE_ADDREF(classDef.p);            
    return classDef.p;
}

ov::ClassDefinition* SchemaDescription::FindClassMapping(FdoIdentifier* id)
{
    assert(NULL != id);
    
    // TODO: What to do if none class found?
    return mSchemaMapping->FindByClassName(id->GetName());
}


void SchemaDescription::DescribeSchema(Connection* conn, FdoString* schemaName)
{
    Connection::Ptr mConn(conn);
    FDO_SAFE_ADDREF(mConn.p);
    
    mIsDescribed = false;
    
    // Collection of Logical / Physical Schemas
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas = NULL;
    logicalSchemas = FdoFeatureSchemaCollection::Create(NULL);

    ov::PhysicalSchemaMapping::Ptr schemaMapping = NULL;
    schemaMapping = ov::PhysicalSchemaMapping::Create();

    // Create collection of Spatial Contexts with default context included
    SpatialContextCollection::Ptr spContexts = NULL;
    spContexts = new SpatialContextCollection();
    //SpatialContext::Ptr spContextDefault(new SpatialContext());
    //spContexts->Insert(0, spContextDefault);

    // TODO: How does the schema name work?
    //       There may be problems with feature commands,
    //       because class ID is formatted as FdoPostGIS:mydatastore~mytable.

    FdoPtr<FdoFeatureSchema> featSchema(FdoFeatureSchema::Create(L"FdoPostGIS", L""));
    logicalSchemas->Add(featSchema.p);

    FdoPtr<FdoClassCollection> featClasses(featSchema->GetClasses());
    ov::ClassCollection::Ptr phClasses(schemaMapping->GetClasses());

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

        FdoInt32 srid = geomColumn->GetSRID();
        if (srid <= 0)
        {
            // Use WGS 84 if SRS not specified
            srid = 4326;
            FDOLOG_WRITE("Use default spatial context with SRID = %d", srid);
        }
        FdoStringP spContextName = FdoStringP::Format(L"PostGIS_%d", srid);
        
        SpatialContext::Ptr spContext = NULL;
        spContext = spContexts->FindItem(spContextName);
        if (NULL == spContext)
        {
            FDOLOG_WRITE(L"Created spatial context: %s",
                static_cast<FdoString*>(spContextName));

            spContext = CreateSpatialContext(mConn, spContextName, geomColumn);
            spContexts->Insert(0, spContext);
        }

        ////////////////// CALCULATE SPATIAL EXTENT //////////////////

        // TODO: Do we want to calculate bbox at all?

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

        FdoGeometryType geomType = geomColumn->GetGeometryType();

        FdoPtr<FdoGeometricPropertyDefinition> geomPropDef = NULL;
        geomPropDef = FdoGeometricPropertyDefinition::Create(
            geomColumn->GetName(), geomColumn->GetDescription());                        

        // General geometry type mask
        FdoInt32 geometricType = 
            FdoGeometricType_Point|FdoGeometricType_Curve|FdoGeometricType_Surface;

        // Try to match specific geometry type
        if (FdoGeometryType_Point == geomType
            || FdoGeometryType_MultiPoint == geomType)
        {
            geometricType = FdoGeometricType_Point;
        }
        else if (FdoGeometryType_LineString == geomType
                 || FdoGeometryType_MultiLineString == geomType)
        {
            geometricType = FdoGeometricType_Curve;
        }
        else if (FdoGeometryType_Polygon == geomType
                 || FdoGeometryType_MultiPolygon == geomType)
        {
            geometricType = FdoGeometricType_Surface;
        }

        geomPropDef->SetGeometryTypes(geometricType);  
        if (NULL != spContext)
        {
            geomPropDef->SetSpatialContextAssociation(spContext->GetName());
        }
        pdc->Add(geomPropDef);
        featClass->SetGeometryProperty(geomPropDef);

        FDOLOG_WRITE(L"+ geometric property: %s",
            static_cast<FdoString*>(geomColumn->GetName()));

        ////////////////// CREATE DATA PROPERTIES //////////////////

        PgTableColumnsReader::Ptr tcReader =
            new PgTableColumnsReader(
                mConn, stReader->GetSchemaName(), stReader->GetTableName());
        tcReader->Open();
        
        while(tcReader->ReadNext())
        {
            FdoPtr<FdoDataPropertyDefinition> datPropDef = NULL;
            datPropDef = FdoDataPropertyDefinition::Create(
                tcReader->GetColumnName(), tcReader->GetColumnDescription());

            FDOLOG_WRITE(L"+ data property: %s",
                static_cast<FdoString*>(tcReader->GetColumnName()));

            FdoDataType const dataType = tcReader->GetColumnType();
            datPropDef->SetDataType(dataType);
            int const size = tcReader->GetColumnSize();
            datPropDef->SetLength(size);
            int const precision = tcReader->GetColumnPrecision();
            datPropDef->SetPrecision(precision);
            int const scale = tcReader->GetColumnScale();
            datPropDef->SetScale(scale);
            bool const isNullable = tcReader->GetColumnNullability();
            datPropDef->SetNullable(isNullable);

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


    //
    // Compose the Schema Description aggregation
    //
    assert(NULL != logicalSchemas);
    assert(NULL != phClasses);

    SetLogicalSchemas(logicalSchemas);
    SetSchemaMapping(schemaMapping);
    SetSpatialContexts(spContexts);
    
    mIsDescribed = true;
}


///////////////////////////////////////////////////////////////////////////////
// Private operations interface
///////////////////////////////////////////////////////////////////////////////

SpatialContext* SchemaDescription::CreateSpatialContext(Connection* conn,
                                                        FdoStringP spContextName,
                                                        PgGeometryColumn* column)
{
    FDOLOG_MARKER("SchemaDescription::-CreateSpatialContext");
    
    Connection::Ptr mConn(conn);
    FDO_SAFE_ADDREF(mConn.p);
    
    PgGeometryColumn::Ptr geomColumn(column);
    FDO_SAFE_ADDREF(geomColumn.p);

    //
    // Query for SRS details
    //

    FdoInt32 srid = -1;
    std::string sridText;
    try
    {
        srid = geomColumn->GetSRID();
        if (srid <= 0)
        {
            // Use WGS 84 if SRS not specified
            srid = 4326;
        }
        sridText = boost::lexical_cast<std::string>(srid);
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("Type conversion failed: %s", e.what());
        assert(!"FIX HANDLING INVALID SRID");
    }

    std::string sql("SELECT srtext FROM spatial_ref_sys WHERE srid = " + sridText);

    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);    
    if (PGRES_TUPLES_OK != PQresultStatus(pgRes.get()) || PQntuples(pgRes.get()) < 1)
    {
        FdoStringP msg = FdoStringP::Format(L"The Spatial Reference System for SRID=%s not found.",
            sridText.c_str());
        throw FdoException::Create(static_cast<FdoString*>(msg));
    }

    //
    // Generate spatial context details
    // 

    SpatialContext::Ptr spContext(new SpatialContext());
    spContext->SetSRID(srid);
    spContext->SetName(spContextName);
    spContext->SetDescription(L"");    

    // TODO: Replace this hacks with new function like:
    // CoordinateSystemNameFromWKT
    // CoordinateSystemDescFromWKT
    int const nfield = PQfnumber(pgRes.get(), "srtext");
    std::string wkt(PQgetvalue(pgRes.get(), 0, nfield));
    assert(!wkt.empty());

    // Use substring between first quotes ("") as the SRS name
    std::string wktName("UNKNOWN");
    std::string::size_type pos1 = wkt.find_first_of('"') + 1;
    std::string::size_type pos2 = wkt.find_first_of(',') - 1;
    if (pos1 != std::string::npos && pos2 != std::string::npos)
    {
        wktName = wkt.substr(pos1, pos2 - pos1);
    }

    FdoStringP csName(wktName.c_str());
    spContext->SetCoordinateSystem(csName);

    FdoStringP csWkt(wkt.c_str());
    spContext->SetCoordinateSystemWkt(csWkt);
    
    FDOLOG_WRITE(L"CRS: %s", static_cast<FdoString*>(csName));
    FDOLOG_WRITE(L"WKT: %s", static_cast<FdoString*>(csWkt));

    FDO_SAFE_ADDREF(spContext.p);
    return spContext.p;
}


}} // namespace fdo::postgis
