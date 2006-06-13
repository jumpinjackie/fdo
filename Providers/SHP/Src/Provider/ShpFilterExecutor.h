/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SHPFILTEREXECUTOR_H
#define SHPFILTEREXECUTOR_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <FdoCommonFilterExecutor.h>

#include "ShpSpatialIndex.h"

#include <vector>

typedef std::vector<FdoInt32> recno_list;

// Extra operations used for liniaring the parse tree
#define ShpComparisonOperation_In         -1
#define ShpUnaryLogicalOperation_Not      -1
#define ShpLogicalOperation_None          -2

typedef struct  {
    int             op;             //FdoComparisonOperations + ShpComparisonOperation_In
	int				depth;			// Index in the flatten parse tree
    FdoIGeometry*   geom;
    BoundingBox     searchArea;
    recno_list      queryResults;
} interval_res;

typedef std::vector<interval_res*> retno_lists;

// Flags to control the QueryOptimizer execution
enum eShapeQueryOptimizerOp
{
    SHP_QUERY_OPTIMIZER_OP_IS_FEATID,      // Test if it is a "featId" based query
    SHP_QUERY_OPTIMIZER_OP_GET_RESULTS,    // Evaluate each subfilter and get the results into vectors
    SHP_QUERY_OPTIMIZER_OP_DO_EXECUTE      // Normal execution flow, calls base class methods, not optimized
};

typedef std::vector<int>  logical_op_list;   // FdoBinaryLogicalOperations + ShpLogicalOperation_None

typedef std::vector<int>  left_right_op_list; // 0 (left) or 1 (right)

class ShpFilterExecutor :
    public FdoCommonFilterExecutor
{
protected:
	ShpFilterExecutor() {};
    ShpFilterExecutor (FdoIReader* reader, FdoIdentifierCollection* compIdents);

    FdoPtr<FdoPropertyDefinitionCollection> mProperties;

public:
    virtual ~ShpFilterExecutor (void);

    static ShpFilterExecutor* Create (FdoIReader* featureReader, FdoIdentifierCollection* compIdents);

    virtual FdoPropertyDefinition* GetPropInfo (FdoString* name);
};

#endif // SHPFILTEREXECUTOR_H

