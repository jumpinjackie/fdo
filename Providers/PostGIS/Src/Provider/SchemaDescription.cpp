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
#include "PgTablesReader.h"
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
    : mIsDescribed(false)
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

void SchemaDescription::ResetSchema()
{

  FDO_SAFE_RELEASE(mLogicalSchemas.p);
  FDO_SAFE_RELEASE(mSchemaMapping.p);
  FDO_SAFE_RELEASE(mSpatialContexts.p);
  mIsDescribed = false;
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
    if (NULL == id)
    {
        throw FdoException::Create(L"The class identifier is NULL.");
    }

    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas(GetLogicalSchemas());
    FdoPtr<FdoClassDefinition> classDef;

    if (NULL != logicalSchemas && logicalSchemas->GetCount() > 0)
    {
        assert(NULL != id);

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
    if (NULL == id)
    {
        FDOLOG_WRITE("ERROR: The class identifier is NULL");
        throw FdoException::Create(L"The class identifier is NULL.");
    }

    // TODO: What to do if none class found?
    assert(NULL != id);
    return mSchemaMapping->FindByClassName(id->GetName());
}

void SchemaDescription::DescribeSchema(Connection* conn, FdoString* schemaName)
{
    FDOLOG_MARKER("SchemaDescription::+DescribeSchema");

    if (NULL == conn)
    {
        FDOLOG_WRITE("ERROR: Connection is NULL");
        throw FdoException::Create(L"The connection is NULL.");
    }

    Connection::Ptr mConn(conn);
    FDO_SAFE_ADDREF(mConn.p);
    
    mIsDescribed = false;
    
    //
    // Prepare schema components
    //

    // Collection of Logical / Physical Schemas
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas;
    logicalSchemas = FdoFeatureSchemaCollection::Create(NULL);

    ov::PhysicalSchemaMapping::Ptr schemaMapping;
    schemaMapping = ov::PhysicalSchemaMapping::Create();

    // TODO: How does the schema name work?
    //       There may be problems with feature commands, because
    //       class ID is formatted as FdoPostGIS:mydatastore~mytable.
    //       Is it a user's responsibility to provide valid class full name?
    FdoPtr<FdoFeatureSchema> featSchema(FdoFeatureSchema::Create(L"FdoPostGIS", L"Default PostGIS Feature Schema"));
    logicalSchemas->Add(featSchema.p);

    FdoPtr<FdoClassCollection> featClasses(featSchema->GetClasses());
    ov::ClassCollection::Ptr phClasses(schemaMapping->GetClasses());

    // Create collection of Spatial Contexts with default context included
    SpatialContextCollection::Ptr spContexts;
    spContexts = new SpatialContextCollection();

    //
    // Process every table to FDO class
    //
    PgTablesReader::Ptr stReader(new PgTablesReader(mConn.p));
    stReader->Open();

    while (stReader->ReadNext())
    {
        ////////////////// CALCULATE SPATIAL EXTENT //////////////////

        // TODO: Do we want to calculate bbox at all?
        // already in stReader->GetGeometryColumns() {
        //     PgTablesReader::EstimateColumnExtent()
        //  or PgTablesReader::SelectColumnExtent()
        // }

        ////////////////// GENERATE CLASS DEFINITION //////////////////
        FdoStringP fdoClassName;
        try
        {

          fdoClassName = stReader->GetTableName();
          assert(!featClasses->FindItem(fdoClassName));

          // TODO: Use table COMMENT as a class description
          FdoPtr<FdoClassDefinition> xClass;
          if (stReader->IsSpatialTable())
          {
              xClass = FdoFeatureClass::Create(fdoClassName, L"");      
              FDOLOG_WRITE(L"Created feature class: %ls", static_cast<FdoString*>(fdoClassName));
          }
          else
          {
              xClass = FdoClass::Create(fdoClassName, L"");      
              FDOLOG_WRITE(L"Created class: %ls", static_cast<FdoString*>(fdoClassName));
          }
          

          FdoPtr<FdoPropertyDefinitionCollection> pdc = xClass->GetProperties();

          if (stReader->IsSpatialTable()) 
          {
            // TODO: Fetch all geometries, but not only the first one - default

            PgTablesReader::columns_t::size_type const geometryIdx = 0;
            PgTablesReader::columns_t geometryColumns(stReader->GetGeometryColumns());
            PgGeometryColumn::Ptr geomColumn = geometryColumns[geometryIdx];

            ////////////////// GENERATE SPATIAL CONTEXT //////////////////

            FdoStringP spContextName(SpatialContextDefaultName);
            FdoInt32 srid = geomColumn->GetSRID();
            if (srid >= 0)
            {
                spContextName = FdoStringP::Format(L"PostGIS_%d", srid);
            }
            
            SpatialContext::Ptr spContext;
            spContext = spContexts->FindItem(spContextName);
            if (NULL == spContext)
            {
                spContext = CreateSpatialContext(mConn, spContextName, geomColumn);
                spContext->SetExtent(static_cast<FdoEnvelopeImpl*>(geomColumn->GetEnvelope()));
                spContexts->Add(spContext);

                FDOLOG_WRITE(L"Created spatial context: %ls",
                    static_cast<FdoString*>(spContextName));
            }

            ////////////////// CREATE GEOMETRY PROPERTY //////////////////

            FdoGeometryType geomType = geomColumn->GetGeometryType();

            FdoPtr<FdoGeometricPropertyDefinition> geomPropDef;
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
            (dynamic_cast<FdoFeatureClass*>(xClass.p))->SetGeometryProperty(geomPropDef);

            FDOLOG_WRITE(L"+ geometric property: %ls",
                static_cast<FdoString*>(geomColumn->GetName()));

          } //if stReader->IsSpatialTable()

          ////////////////// CREATE DATA PROPERTIES //////////////////

          PgTableColumnsReader::Ptr tcReader =
              new PgTableColumnsReader(
                  mConn, stReader->GetSchemaName(), stReader->GetTableName());
          tcReader->Open();
          
          while (tcReader->ReadNext())
          {
              FdoStringP colName(tcReader->GetColumnName());
              if (pdc->FindItem(colName)) 
              { 
                  // NOTE - Eric Barby: Because is out of date after a remove thisTable 'DROP TABLE...'
                  FDOLOG_WRITE("ERROR: Table '%s' PropertyDefinition '%ls' is already in the PropertyDefinitionCollection",
                      stReader->GetTableName(), static_cast<FdoString*>(colName));
              } 
              else 
              {
                  FdoPtr<FdoDataPropertyDefinition> datPropDef;
                  datPropDef = FdoDataPropertyDefinition::Create(
                      colName, tcReader->GetColumnDescription());

                  FDOLOG_WRITE(L"+ data property: %ls",
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

                  // Add default value.
                  if (tcReader->IsDefault())
                  {
                      FdoStringP defaultVal(tcReader->GetDefault());
                      if (defaultVal.GetLength())
                          datPropDef->SetDefaultValue(defaultVal);
                  }

                  // First, it's required to add property to the base collection.
                  // So, it can be add to the identity properties in next step,
                  // if required.
                  pdc->Add(datPropDef);

                  // Retrieve definition of PRIMARY KEY constraint
                  if (tcReader->IsPrimaryKey())
                  {
                      FDOLOG_WRITE(" - PRIMARY KEY");

                      FdoPtr<FdoDataPropertyDefinitionCollection> propsIdentity;
                      propsIdentity = xClass->GetIdentityProperties();
                      assert(NULL != propsIdentity);

                      datPropDef->SetNullable(false);
                      propsIdentity->Add(datPropDef);

                      // NOTE - Eric Barby: 2008-02 add correct sequence information. 
                      // XXX - mloskot: Is this a TODO or just comment for the code below?
                      bool const isSequence = tcReader->IsSequence();
                      datPropDef->SetIsAutoGenerated(isSequence);
                  }
              }
          
          } // while (tcReader->ReadNext())

          tcReader->Close();

          // Physical mapping for the feature class
          ov::ClassDefinition::Ptr classDef = ov::ClassDefinition::Create();
          classDef->SetName(fdoClassName);
          classDef->SetSchemaName(stReader->GetSchemaName());

          //
          // Add Feature Class and Class Definition to collections
          //
          featClasses->Add(xClass);
          phClasses->Add(classDef);
       }
       catch (FdoException* e)
       {
          FDOLOG_WRITE(L"ERROR: Describe operation for '%ls' failed. %ls"
              , static_cast<FdoString*>(fdoClassName)
              , e->GetExceptionMessage());
          e->Release();
       }

    } // while

    stReader->Close();

    //
    // Compose the Schema Description aggregation
    //
    assert(NULL != logicalSchemas);
    assert(NULL != phClasses);
    featSchema->AcceptChanges();

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

    //
    // Determine type of context
    //
    if (0 == spContextName.ICompare(SpatialContextDefaultName))
    {
        SpatialContext::Ptr spDefaultContext(new SpatialContext());
        spDefaultContext->SetName(spContextName);

        FDO_SAFE_ADDREF(spDefaultContext.p);
        return spDefaultContext.p;
    }

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
        sridText = boost::lexical_cast<std::string>(srid);
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("ERROR: Type conversion failed: %s", e.what());
    }

    std::string sql("SELECT srtext FROM spatial_ref_sys WHERE srid = " + sridText);

    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);    
    if (PGRES_TUPLES_OK != PQresultStatus(pgRes.get()) || PQntuples(pgRes.get()) < 1)
    {
        FDOLOG_WRITE("ERROR: The Spatial Reference System for SRID=%s not found", 
            sridText.c_str());

        FdoStringP tmp = sridText.c_str();
        FdoStringP msg = FdoStringP::Format(L"The Spatial Reference System for SRID=%s not found.",
            static_cast<FdoString const*>(tmp));
        throw FdoException::Create(static_cast<FdoString*>(msg));
    }

    //
    // Generate spatial context details
    // 
    SpatialContext::Ptr spContext(new SpatialContext());
    spContext->SetSRID(srid);
    spContext->SetName(spContextName);
    spContext->SetDescription(L""); 
    spContext->SetExtent(geomColumn->GetEnvelope());

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

    FDOLOG_WRITE(L"CRS: %ls", static_cast<FdoString*>(csName));
    FDOLOG_WRITE(L"WKT: %ls", static_cast<FdoString*>(csWkt));

    FDO_SAFE_ADDREF(spContext.p);
    return spContext.p;
}


}} // namespace fdo::postgis
