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
#include "ShpQueryOptimizer.h"
#include <Constants.h>

#include <float.h>
#include <math.h>

#include <algorithm>
#include <functional>

#include <FdoGeometry.h>
#include <FdoSpatial.h>
#include <Int32Value.h>
#include <DoubleValue.h>
#include <StringValue.h>
#include <BooleanValue.h>

// Used to determine if two double precision numbers are about equal
#define SHP_GLOBAL_TOLERANCE 1.0e-10 

ShpQueryOptimizer::ShpQueryOptimizer()
{    
}

ShpQueryOptimizer* ShpQueryOptimizer::Create(FdoIReader* reader, FdoIdentifierCollection* selected )
{
    ShpFeatureReader* my_reader = (ShpFeatureReader *)reader;
    FdoPtr<ShpConnection>  connection = my_reader->GetConnection();
    FdoPtr<FdoClassDefinition> classDef = ShpSchemaUtilities::GetLogicalClassDefinition (connection, my_reader->GetClassName(), NULL);

    FdoPtr<FdoDataPropertyDefinitionCollection> idProps = classDef->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem(0);

    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition(connection, classDef->GetName());
    ShpSpatialIndex* ssi = lpClass->GetPhysicalFileSet()->GetSpatialIndex();

    return new ShpQueryOptimizer(reader, classDef, selected, ssi); 
}

ShpQueryOptimizer::ShpQueryOptimizer(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* selected, ShpSpatialIndex* rtree ):
    FdoCommonFilterExecutor (reader, selected)
{
    ShpFeatureReader* my_reader = (ShpFeatureReader *)reader;
    m_Connection = my_reader->GetConnection();

    m_Class = classDef; 
    FDO_SAFE_ADDREF (m_Class.p);

    m_RTree = rtree;
    mProperties = classDef->GetProperties ();

    FdoPtr<FdoDataPropertyDefinitionCollection> idProps = classDef->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem(0);

    m_LogicalIdentityPropertyName = idProp->GetName();
}

ShpQueryOptimizer::~ShpQueryOptimizer()
{    
    for (retno_lists::iterator iter = m_FeatidLists.begin(); iter != m_FeatidLists.end(); iter++)
    {
        delete *iter;
    }
}

void ShpQueryOptimizer::Dispose()
{
    delete this;
}

FdoPropertyDefinition* ShpQueryOptimizer::GetPropInfo (FdoString* name)
{
    return (mProperties->FindItem (name));
}

void ShpQueryOptimizer::ProcessSpatialCondition(FdoSpatialCondition& filter)
{ 
    FdoPtr<FdoExpression> expr = filter.GetGeometry();

    //right now we only handle literal Geometry values
    //Any other expression will cause exception.
    FdoGeometryValue* geomRightVal = dynamic_cast<FdoGeometryValue*>(expr.p);

    if ( geomRightVal == NULL)
        throw FdoCommandException::Create ( NlsMsgGet(SHP_LITERAL_GEOM_VALUES_ONLY, "Only literal geometry values supported"));

    FdoPtr<FdoByteArray>  geomRightFgf = geomRightVal->GetGeometry();

    BoundingBox             searchArea;
    FdoSpatialUtility::GetExtents( geomRightFgf, searchArea.xMin, searchArea.yMin, searchArea.xMax, searchArea.yMax);

    FdoPtr<FdoGeometricPropertyDefinition> gpd = FindGeomProp(m_Class);
    FdoPtr<FdoIdentifier> idname = filter.GetPropertyName();

    if (0 != wcscmp (gpd->GetName (), idname->GetName ()))
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_104_READER_PROPERTY_NOT_SELECTED), idname->GetName ()));

    if (m_RTree)
    {
        // For each bounding box build a sorted list of candidates (primary filter) and save it.
        // This ensures that the RTree search is done just once per a spatial filter.
        // The filter is identified by it's bounding box.
        
        FdoSpatialOperations spatialOp = filter.GetOperation();

        if ( spatialOp != FdoSpatialOperations_EnvelopeIntersects &&
            spatialOp != FdoSpatialOperations_Within &&
            spatialOp != FdoSpatialOperations_Inside &&
            spatialOp != FdoSpatialOperations_Intersects &&
            spatialOp != FdoSpatialOperations_Contains && 
            spatialOp != FdoSpatialOperations_Disjoint ) 
            throw FdoException::Create (NlsMsgGet(SHP_SI_OPERATOR_NOT_SUPPORTED, "The spatial operation %1$ls is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoSpatialOperationsToString (spatialOp))));

        int     nStatus;
        size_t  filterNum;
        bool    filter_found = false;

        // Try to reuse a list of candidates.
        for ( filterNum = 0; filterNum < m_FeatidLists.size() && !filter_found; filterNum++ )
        {
            BoundingBox  box = m_FeatidLists[filterNum]->searchArea;  

            filter_found = ( AreEqual( box.xMin, searchArea.xMin ) &&
                            AreEqual( box.yMin, searchArea.yMin ) &&
                            AreEqual( box.xMax, searchArea.xMax ) &&
                            AreEqual( box.yMax, searchArea.yMax ) );
        }
         
        if ( filter_found )
        {
            // Filter found, reuse it.
            filterNum--;
        }
        else
        {
            // Filter not found (first time), build one.
            m_RTree->InitializeSearch (&searchArea, SHP_SI_SEARCH_ALL_TYPES);

            // Initialize an entry in the list of results
            interval_res*    results = new interval_res;

            results->op = ShpComparisonOperation_In;
            recno_list*  retFeatNum = &results->queryResults;

            results->searchArea.xMin = searchArea.xMin;
            results->searchArea.yMin = searchArea.yMin;
            results->searchArea.xMax = searchArea.xMax;
            results->searchArea.yMax = searchArea.yMax;

            bool            done = false;
            do
            {
                unsigned long   offset;
                BoundingBoxEx     extents;

                nStatus = m_RTree->GetNextObject (offset, extents);
                switch (nStatus)
                {
                    case SHP_SI_SEARCH_COMPLETE:
                        done = true;
                        break;
                    case SHP_OK:
                        retFeatNum->push_back( offset );
                        break;
                    default:
                        throw FdoException::Create (NlsMsgGet(SHP_UNDEFINED_ERROR, "Error code '%1$d' returned.", nStatus ));
                        break;
                }
            }
            while ( !done );

            // Sort the list and add it to the global list of results
            std::sort(retFeatNum->begin(), retFeatNum->end(), std::less<FdoInt32>());

            m_FeatidLists.push_back( results );
            filterNum = m_FeatidLists.size() - 1;
        }

        // Get the current list of candidates, matching this filter
        recno_list*    featidPrimarySel = &m_FeatidLists[ filterNum ]->queryResults;

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geomRight = gf->CreateGeometryFromFgf( geomRightFgf );

        FdoInt32  featNum = m_reader->GetInt32(L"FeatId");
        featNum--;  //featids are 0-based internally but 1-based externally (outside FDO API)

        bool  ret = std::binary_search( featidPrimarySel->begin( ), featidPrimarySel->end( ), featNum, std::less<FdoInt32>() );
        
        if ( ret && spatialOp != FdoSpatialOperations_EnvelopeIntersects )
        {
            FdoPtr<FdoByteArray>  geomLeftFgf = m_reader->GetGeometry( L"Geometry" );
            FdoPtr<FdoIGeometry>  geomLeft = gf->CreateGeometryFromFgf( geomLeftFgf );
            ret = FdoSpatialUtility::Evaluate( geomLeft, spatialOp, geomRight);
        }

        m_retvals.push_back( ObtainBooleanValue (false, ret) );
    }
}

//----------------------------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------------------------

FdoGeometricPropertyDefinition* ShpQueryOptimizer::FindGeomProp(FdoClassDefinition* classDef)
{
    if (classDef->GetClassType() != FdoClassType_FeatureClass)
        return NULL;

    FdoPtr<FdoGeometricPropertyDefinition> gpd = ((FdoFeatureClass*)classDef)->GetGeometryProperty();
    FdoPtr<FdoClassDefinition> base = FDO_SAFE_ADDREF(classDef);

    //navigate up class hierarchy until we find a geom property or 
    //until we reach a NULL base class, in which case the geom prop is NULL
    while (    gpd.p == NULL 
           && (base = base->GetBaseClass()) != NULL )
        gpd = ((FdoFeatureClass*)base.p)->GetGeometryProperty();

    return FDO_SAFE_ADDREF(gpd.p);
}

bool ShpQueryOptimizer::AreEqual(double/*&*/ d1, double/*&*/ d2)
{
    if (fabs(d1 - d2) <= SHP_GLOBAL_TOLERANCE)
        return true;

    return false;
}

