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
#include "ShpFeatIdQueryEvaluator.h"
#include <Constants.h>

#include <float.h>
#include <math.h>

#include <algorithm>
#include <functional>

#include <Geometry.h>
#include <Spatial.h>
#include <Int32Value.h>
#include <DoubleValue.h>
#include <StringValue.h>
#include <BooleanValue.h>

// Used to determine if two double precision numbers are about equal
#define SHP_GLOBAL_TOLERANCE 1.0e-10 
#ifndef _WIN32
#define max(a,b)             ((a) > (b) ? (a) : (b))
#endif

ShpFeatIdQueryEvaluator::ShpFeatIdQueryEvaluator()
{    
}

ShpFeatIdQueryEvaluator* ShpFeatIdQueryEvaluator::Create(FdoIReader* reader, FdoIdentifierCollection* selected )
{
    ShpFeatureReader* my_reader = (ShpFeatureReader *)reader;
    FdoPtr<ShpConnection>  connection = my_reader->GetConnection();
    FdoPtr<FdoClassDefinition> classDef = ShpSchemaUtilities::GetLogicalClassDefinition (connection, my_reader->GetClassName(), NULL);

    FdoPtr<FdoDataPropertyDefinitionCollection> idProps = classDef->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem(0);

    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition(connection, classDef->GetName());
    ShpSpatialIndex* ssi = lpClass->GetPhysicalFileSet()->GetSpatialIndex();

    return new ShpFeatIdQueryEvaluator(reader, classDef, selected, ssi); 
}

ShpFeatIdQueryEvaluator::ShpFeatIdQueryEvaluator(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* selected, ShpSpatialIndex* rtree ):
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
    m_MergedFeatidList = NULL;
}

ShpFeatIdQueryEvaluator::~ShpFeatIdQueryEvaluator()
{    

    for (retno_lists::iterator iter = m_FeatidLists.begin(); iter != m_FeatidLists.end(); iter++)
    {
        interval_res* x = *iter;
        delete x;
    }

    if ( m_MergedFeatidList )
        delete m_MergedFeatidList;
}


void ShpFeatIdQueryEvaluator::Dispose()
{
    delete this;
}

void ShpFeatIdQueryEvaluator::SetOperation( eShapeQueryOptimizerOp op )
{
    m_CurrOperation = op;
}

bool ShpFeatIdQueryEvaluator::IsFeatidQuery( )
{
    return m_IsFeatidQuery;
}

retno_lists* ShpFeatIdQueryEvaluator::GetFeatidLists()
{
    return &m_FeatidLists;
}

logical_op_list* ShpFeatIdQueryEvaluator::GetFeatidFilterOpsList()
{
    return &m_LogicalOpsList;
}

recno_list*  ShpFeatIdQueryEvaluator::GetMergedFeatidList()
{
    return m_MergedFeatidList;
}

FdoPropertyDefinition* ShpFeatIdQueryEvaluator::GetPropInfo (FdoString* name)
{
    return (mProperties->FindItem (name));
}

void ShpFeatIdQueryEvaluator::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
   
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();

    // Initialize an entry in the list of results
    m_LogicalOpsList.push_back( filter.GetOperation() );

    left->Process (this);
    right->Process(this);
}

void ShpFeatIdQueryEvaluator::ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> right = filter.GetOperand ();

    // Initialize an entry in the list of results
    m_LogicalOpsList.push_back( ShpUnaryLogicalOperation_Not );
    right->Process(this);
}

void ShpFeatIdQueryEvaluator::ProcessComparisonCondition(FdoComparisonCondition& filter)
{

    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();

    FdoIdentifier* ident = dynamic_cast<FdoIdentifier*>(left.p);
    FdoDataValue* val = dynamic_cast<FdoDataValue*>(right.p);

    // Initialize an entry in the list of results
    interval_res*    results = new interval_res;
    results->op = filter.GetOperation(); 
    recno_list*  retFeatNum = &results->queryResults;

    FdoInt32    featid = ProcessInt32Expression( right );
    retFeatNum->push_back( featid-1 );  // featids are 1-based to FDO users, 0-based internally
    m_FeatidLists.push_back( results );
}

void ShpFeatIdQueryEvaluator::ProcessInCondition(FdoInCondition& filter)
{
    FdoPtr<FdoExpression> left = filter.GetPropertyName();
 
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();
    FdoPtr<FdoValueExpressionCollection> vals = filter.GetValues();

    // Initialize an entry in the list of results
    interval_res*    results = new interval_res;
    results->op = ShpComparisonOperation_In; 
    recno_list*  retFeatNum = &results->queryResults;

    for ( int i = 0; i < vals->GetCount(); i++ )
    {
        FdoPtr<FdoValueExpression> exp = vals->GetItem(i);

        FdoInt32    featid = ProcessInt32Expression( exp );

        retFeatNum->push_back( featid-1 );  // featids are 1-based to FDO users, 0-based internally
    }

    // Sort the list and add it to the global list of results
    std::sort(retFeatNum->begin(), retFeatNum->end(), std::less<FdoInt32>());

    m_FeatidLists.push_back( results );
}

void ShpFeatIdQueryEvaluator::ProcessNullCondition (FdoNullCondition& filter)
{
    // This should be never called since then it would not be a FeatId query
}

void ShpFeatIdQueryEvaluator::ProcessDistanceCondition (FdoDistanceCondition& filter)
{
    // Until it is implentented, let it throw a base class exception
    FdoCommonFilterExecutor::ProcessDistanceCondition( filter );
}

void ShpFeatIdQueryEvaluator::ProcessSpatialCondition(FdoSpatialCondition& filter)
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
        FdoSpatialOperations spatialOp = filter.GetOperation();

        if ( spatialOp != FdoSpatialOperations_EnvelopeIntersects &&
            spatialOp != FdoSpatialOperations_Within &&
            spatialOp != FdoSpatialOperations_Inside &&
            spatialOp != FdoSpatialOperations_Intersects &&
            spatialOp != FdoSpatialOperations_Contains && 
            spatialOp != FdoSpatialOperations_Disjoint ) 
            throw FdoException::Create (NlsMsgGet(SHP_SI_OPERATOR_NOT_SUPPORTED, "The spatial operation %1$ls is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoSpatialOperationsToString (spatialOp))));

        // Build spatial index.
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

            int nStatus = m_RTree->GetNextObject (offset, extents);
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
        size_t filterNum = m_FeatidLists.size() - 1;

        // Get the current list of candidates, matching this filter
        recno_list*    featidPrimarySel = &m_FeatidLists[ filterNum ]->queryResults;

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geomRight = gf->CreateGeometryFromFgf( geomRightFgf );

        // Do secondary filtering
        if ( spatialOp != FdoSpatialOperations_EnvelopeIntersects )
        {
            interval_res*    results = new interval_res;

            results->op = ShpComparisonOperation_In;
            recno_list*  secFilterList = &results->queryResults;

            FdoPtr<ShpLpClassDefinition> shpLpClassDef = ShpSchemaUtilities::GetLpClassDefinition(m_Connection, m_Class->GetName());

            ShpFileSet*  fileSet = shpLpClassDef->GetPhysicalFileSet();

            for ( size_t i = 0; i < featidPrimarySel->size(); i++ )
            {
                ULONG    offset;
                int         length;
                Shape*      shape = NULL; 
                eShapeTypes type;

                FdoInt32  featNum = featidPrimarySel->at(i);

                // Get info from the file.
                fileSet->GetShapeIndexFile()->GetObjectAt( featNum, offset, length);
                shape = fileSet->GetShapeFile()->GetObjectAt (offset, type);

                FdoPtr<FdoByteArray>  geomLeftFgf = shape->GetGeometry ();
                FdoPtr<FdoIGeometry>  geomLeft = gf->CreateGeometryFromFgf( geomLeftFgf );
                delete shape;

                bool  ret = FdoSpatialUtility::Evaluate( geomLeft, spatialOp, geomRight);
                if ( ret )
                    secFilterList->push_back( featNum );
            }

            //delete featidPrimarySel;
            retno_lists::iterator  iter_lists = m_FeatidLists.end();
			--iter_lists;
            delete *iter_lists;  // delete the memory this vector entry points to
            m_FeatidLists.erase( iter_lists );  // delete this vector entry
            m_FeatidLists.push_back( results );
        }
    }
}

//----------------------------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------------------------

FdoGeometricPropertyDefinition* ShpFeatIdQueryEvaluator::FindGeomProp(FdoClassDefinition* classDef)
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

// Ported from SDFPlus, author Traian Stanev
recno_list* ShpFeatIdQueryEvaluator::FeatidListsUnion(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return that list as the union of the two
    if (left == NULL)
        return left;

    if (right == NULL)
        return right;

    std::sort(left->begin(), left->end(), std::less<FdoInt32>());
    std::sort(right->begin(), right->end(), std::less<FdoInt32>());

    recno_list::iterator iter1 = left->begin();
    recno_list::iterator iter2 = right->begin();

    recno_list* ret = new recno_list;

    while (iter1 != left->end() || iter2 != right->end())
    {
        if (iter1 == left->end())
            ret->push_back(*iter2++);
        else if (iter2 == right->end())
            ret->push_back(*iter1++);
        else if (*iter2 < *iter1)
            ret->push_back(*iter2++);
        else if (*iter2 > *iter1)
            ret->push_back(*iter1++);
        else
        {
            ret->push_back(*iter1);
            iter1++;
            iter2++;
        }
    }

    //dispose of inputs and return newly allocated list
    delete left;
    delete right;

    return ret;
}

// Ported from SDFPlus, author Traian Stanev
recno_list* ShpFeatIdQueryEvaluator::FeatidListsIntersection(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return the non-null list as the intersection of the two, if one is non-null
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    std::sort(left->begin(), left->end(), std::less<FdoInt32>());
    std::sort(right->begin(), right->end(), std::less<FdoInt32>());  // NO NEEED

    recno_list::iterator iter1 = left->begin();
    recno_list::iterator iter2 = right->begin();

    recno_list* ret = new recno_list;

    while(iter1 != left->end() || iter2 != right->end())
    {
        if (iter1 == left->end())
            break;
        else if (iter2 == right->end())
            break;
        else if (*iter2 < *iter1)
            iter2++;
        else if (*iter2 > *iter1)
            iter1++;
        else
        {
            ret->push_back(*iter1);
            iter1++;
            iter2++;
        }
    }

    //dispose of inputs and return newly allocated list
    delete left;
    delete right;

    return ret;
}

// Ported from SDFPlus, author Traian Stanev
recno_list* ShpFeatIdQueryEvaluator::FeatidListNegate( recno_list* list, int maxRecords)
{
    if (list == NULL)
        return list;

    std::sort(list->begin(), list->end(), std::less<FdoInt32>());

    recno_list* ret = new recno_list;

    for ( FdoInt32 i = 0; i < maxRecords; i++ )
    {
        bool  exists = std::binary_search( list->begin( ), list->end( ), i, std::less<FdoInt32>() );

        if ( !exists )
            ret->push_back( i );
    }

    //dispose of input and return newly allocated list
    delete list;

    return ret;
}

bool ShpFeatIdQueryEvaluator::AreEqual(double/*&*/ d1, double/*&*/ d2)
{
    if (fabs(d1 - d2) <= SHP_GLOBAL_TOLERANCE)
        return true;

    return false;
}

// Process an expression by invoking the handler of the concrete expression class
FdoInt32 ShpFeatIdQueryEvaluator::ProcessInt32Expression( FdoExpression *exp )
{
	if( exp == NULL )
        throw FdoException::Create (NlsMsgGet(SHP_EXPRESSION_NULL_PARAMETER, "Null parameter." ));
 
    FdoDataValue *dVal = static_cast<FdoDataValue *>((FdoExpression*)exp);

	if( dVal == NULL || dVal->GetDataType() != FdoDataType_Int32)
        throw FdoException::Create (NlsMsgGet(SHP_EXPRESSION_INVALID_PARAMETER_TYPE, "Invalid parameter type." ));
    
    return (dynamic_cast<FdoInt32Value*>(dVal))->GetInt32();
}

///////////////////////////////////////////////////////////////////////////////
bool  ShpFeatIdQueryEvaluator::MergeFeatidLists( size_t maxAllowedSize, int maxRecords )
{

    // Evaluate in advance the size of the resulting list
    size_t   aproxListSize = EvaluateMergedListSize( maxRecords );

    //printf("[MergeFeatidLists] maxAllowedSize = %d  aproxListSize %d\n", maxAllowedSize, aproxListSize);

    // Quit if too large
    if ( aproxListSize > maxAllowedSize )
        return false;

    //Results available in lists;
    retno_lists::iterator       iter_comp_op = m_FeatidLists.end();
    logical_op_list::iterator   iter_logical_op  = m_LogicalOpsList.end();
    int                         curr_logical_op = ShpLogicalOperation_None;

    // Evaluate each filter
    for (; iter_comp_op != m_FeatidLists.begin(); )
    {
        iter_comp_op--;

        interval_res*       curr_filter = *iter_comp_op; // rename
        int                 curr_comp_op = curr_filter->op;
        recno_list*         featid_list = &curr_filter->queryResults;

        // Unary logical operator NOT? Process and advance.
        if ( curr_logical_op == ShpUnaryLogicalOperation_Not )
        {
            m_MergedFeatidList = FeatidListNegate( m_MergedFeatidList, maxRecords );
            if ( iter_logical_op != m_LogicalOpsList.begin() )
                curr_logical_op = (*--iter_logical_op);
        }

        if ( featid_list->size() != 0 )
        {
            FdoInt32            featId = (*featid_list->begin());

            recno_list*         tmp_list = new recno_list;

            switch ( curr_comp_op )
            {
            case FdoComparisonOperations_EqualTo : 
                tmp_list->push_back( featId );
                break;
            case FdoComparisonOperations_NotEqualTo : 
                for ( FdoInt32 i = 0; i < maxRecords; i++ )
                {
                    if ( i != featId )
                        tmp_list->push_back( i );
                }
                break;
            case FdoComparisonOperations_GreaterThan : 
                for ( FdoInt32 i = featId + 1; i < maxRecords; i++ )
                    tmp_list->push_back( i );
                break;
            case FdoComparisonOperations_GreaterThanOrEqualTo : 
                for ( FdoInt32 i = featId; i < maxRecords; i++ )
                    tmp_list->push_back( i );
                break;
            case FdoComparisonOperations_LessThan : 
                for ( FdoInt32 i = 0; i < featId; i++ )
                    tmp_list->push_back( i );
                break;
            case FdoComparisonOperations_LessThanOrEqualTo : 
                for ( FdoInt32 i = 0; i <= featId; i++ )
                    tmp_list->push_back( i );
                break;
            case ShpComparisonOperation_In: // In or Spatial query candidates
                for ( recno_list::iterator iter = featid_list->begin(); iter != featid_list->end(); iter++)
                    tmp_list->push_back( *iter );
                break;
            case FdoComparisonOperations_Like :
            default:
                throw FdoException::Create (L"Invalid comparison operation type");
            }

            // Use current binary logical operation to merge the current list
            switch ( curr_logical_op )
            {
            case FdoBinaryLogicalOperations_And:
                m_MergedFeatidList = FeatidListsIntersection( m_MergedFeatidList, tmp_list );
                break;
            case FdoBinaryLogicalOperations_Or:
                m_MergedFeatidList = FeatidListsUnion( m_MergedFeatidList, tmp_list );
                break;
            case ShpLogicalOperation_None:
                m_MergedFeatidList = tmp_list;  // No merging if first time.     
                break;
            default:
                throw FdoException::Create (L"Invalid logical operation type");
            }
        }


        // Advance in the list of logical operations
        if ( iter_logical_op != m_LogicalOpsList.begin() )
            curr_logical_op = (*--iter_logical_op);
    }

    if ( curr_logical_op == ShpUnaryLogicalOperation_Not )
        m_MergedFeatidList = FeatidListNegate( m_MergedFeatidList, maxRecords );

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
size_t ShpFeatIdQueryEvaluator::EvaluateMergedListSize( int maxRecords )
{
    ////Results available in lists;
    //retno_lists*        intervals = mFilterExecutor->GetFeatidLists();
    //logical_op_list*    logical_ops = mFilterExecutor->GetFeatidFilterOpsList();

    retno_lists::iterator       iter_comp_op = m_FeatidLists.end();
    logical_op_list::iterator   iter_logical_op  = m_LogicalOpsList.end();
    int                         curr_logical_op = ShpLogicalOperation_None;

    size_t     aproxListSize = 0;

    // Evaluate each filter
    for (; iter_comp_op != m_FeatidLists.begin(); )
    {
        iter_comp_op--;

        interval_res*       curr_filter = *iter_comp_op; // rename
        int                 curr_comp_op = curr_filter->op;
        recno_list*         featid_list = &curr_filter->queryResults;


        // Handle NOT operator and advance 
        if ( curr_logical_op == ShpUnaryLogicalOperation_Not )
        {
            aproxListSize = maxRecords - aproxListSize;
            if ( iter_logical_op != m_LogicalOpsList.begin() )
                curr_logical_op = (*--iter_logical_op);
        }

        // The result set may be empty (say a spatial query returning nothing)
        size_t  currListSize = featid_list->size();
        size_t  intervalSize = 0;

        if ( currListSize != 0 )
        {
            FdoInt32            featId = (*featid_list->begin());

            switch ( curr_comp_op )
            {
            case FdoComparisonOperations_EqualTo : 
                intervalSize = 1;
                break;
            case FdoComparisonOperations_NotEqualTo : 
                currListSize = maxRecords - featId;
                break;
            case FdoComparisonOperations_GreaterThan : 
                intervalSize = maxRecords - featId - 1;
                break;
            case FdoComparisonOperations_GreaterThanOrEqualTo : 
                intervalSize = maxRecords - featId;
                break;
            case FdoComparisonOperations_LessThan : 
                intervalSize = featId;
                break;
            case FdoComparisonOperations_LessThanOrEqualTo : 
                intervalSize = featId + 1;
                break;
            case ShpComparisonOperation_In : // In or Spatial query candidates
                intervalSize = currListSize;
                break;
            case FdoComparisonOperations_Like :
            default:
                throw FdoException::Create (L"Invalid comparison operation type");
            }
        }

        // Use current binary logical operation to proceed or not
        switch ( curr_logical_op )
        {
        case FdoBinaryLogicalOperations_And:
            aproxListSize = max( aproxListSize, intervalSize );
            break;
        case FdoBinaryLogicalOperations_Or:
            aproxListSize = aproxListSize + intervalSize; 
            break;
        case ShpLogicalOperation_None:
            aproxListSize = intervalSize;     
            break;
        default:
            throw FdoException::Create (L"Invalid logical operation type");
        }

        // Advance in the list of logical operations.
        if ( iter_logical_op != m_LogicalOpsList.begin() )
            curr_logical_op = (*--iter_logical_op);
    }

    return aproxListSize;
}

