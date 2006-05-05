#ifndef ShpFeatIdQueryEvaluator_H
#define ShpFeatIdQueryEvaluator_H
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
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "stdafx.h"
#include <FdoCommonFilterExecutor.h>

class ShpFeatureReader;

///<summary>
///</summary>

class ShpFeatIdQueryEvaluator : public FdoCommonFilterExecutor
{
    friend class ShpFeatureReader;

public:
    ShpFeatIdQueryEvaluator();

    static ShpFeatIdQueryEvaluator* Create(FdoIReader* reader, FdoIdentifierCollection* compIdents);
    ~ShpFeatIdQueryEvaluator();

    virtual FdoPropertyDefinition* GetPropInfo (FdoString* name);

protected:
    ShpFeatIdQueryEvaluator(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* compIdents, ShpSpatialIndex* rtree );
    
    FdoPtr<FdoPropertyDefinitionCollection> mProperties;

    virtual void Dispose();

public:

    //--------------------------------------------------------------------
    // FdoIFilterProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    virtual void ProcessInCondition(FdoInCondition& filter);

    virtual void ProcessNullCondition(FdoNullCondition& filter);

    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);


    //--------------------------------------------------------------------
    // FdoIExpressionProcessor : base class
    //--------------------------------------------------------------------

public:
    
    void                    SetOperation( eShapeQueryOptimizerOp op );
    bool                    IsFeatidQuery();
    retno_lists*            GetFeatidLists();
    logical_op_list*        GetFeatidFilterOpsList();
    recno_list*             GetMergedFeatidList();
    bool                    MergeFeatidLists( size_t maxAllowedSize, int maxRecords );

private:

    //helper functions
    bool                    AreEqual( double d1, double d2 );
    FdoInt32                ProcessInt32Expression( FdoExpression *exp );
    recno_list*             FeatidListsUnion( recno_list* left, recno_list* right );
    recno_list*             FeatidListsIntersection( recno_list* left, recno_list* right );
    recno_list*             FeatidListNegate( recno_list* list, int maxRecords);
    size_t                  EvaluateMergedListSize( int maxRecords );

    FdoGeometricPropertyDefinition* FindGeomProp(FdoClassDefinition* classDef);

    FdoPtr<ShpConnection>   m_Connection;
    FdoPtr<FdoClassDefinition> m_Class;
    FdoStringP              m_LogicalIdentityPropertyName;
    bool                    m_IsFeatidQuery;
    eShapeQueryOptimizerOp  m_CurrOperation;
    ShpSpatialIndex*        m_RTree;  
    retno_lists             m_FeatidLists;
    logical_op_list         m_LogicalOpsList;
    recno_list*             m_MergedFeatidList;
};
#endif

