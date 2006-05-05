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
#include "PropertyIndex.h"
#include <vector>

#include "RTree.h"
#include "KeyDb.h"

typedef std::vector<recno_list*> qo_retval_stack;
typedef std::vector<FdoFilter*> filter_stack;

//typedef std::vector<int> idprop_indices;
//typedef std::vector<idprop_indices*> idprop_stack;


///<summary>
///Processes Filter tree and looks for spatial queries which can be evaluated
///by searching the feature class R-Tree. In the end the result is a set
///of feature record numbers that we need to consider for that query
///For example if the Filter contains a spatial condition which limits features
///to a given extent, we only need to read and execute the full query on those
///particular features
///This class also looks for conditions on feature key (identity properties) 
///and uses the KeyDb database to look up record numbers based on feature key 
///queries.
///It can produce a modified FdoFilter based on the conditions which were
///not examined by the query optimizer itself. This filter can then be applied
///to the output features of the query optimizer to complete the filtering
///operation.
///</summary>
//TODO: get rid of the multiple inheritance!!!
class SdfQueryOptimizer : public FdoIFilterProcessor, public FdoIExpressionProcessor
{

public:

    SDF_API SdfQueryOptimizer(SdfRTree* rtree, KeyDb* keys, FdoClassDefinition* classDef);
    SDF_API ~SdfQueryOptimizer();

protected:

    SDF_API virtual void Dispose();

public:

    //--------------------------------------------------------------------
    // FdoIFilterProcessor
    //--------------------------------------------------------------------

    SDF_API virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    SDF_API virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    SDF_API virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    SDF_API virtual void ProcessInCondition(FdoInCondition& filter);

    SDF_API virtual void ProcessNullCondition(FdoNullCondition& filter);

    SDF_API virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    SDF_API virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);


    //--------------------------------------------------------------------
    // FdoIExpressionProcessor
    //--------------------------------------------------------------------

    SDF_API virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    SDF_API virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    SDF_API virtual void ProcessFunction(FdoFunction& expr);

    SDF_API virtual void ProcessIdentifier(FdoIdentifier& expr);

	SDF_API virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr) { return; }

    SDF_API virtual void ProcessParameter(FdoParameter& expr);

    SDF_API virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    SDF_API virtual void ProcessByteValue(FdoByteValue& expr);

    SDF_API virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    SDF_API virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    SDF_API virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    SDF_API virtual void ProcessInt16Value(FdoInt16Value& expr);

    SDF_API virtual void ProcessInt32Value(FdoInt32Value& expr);

    SDF_API virtual void ProcessInt64Value(FdoInt64Value& expr);

    SDF_API virtual void ProcessSingleValue(FdoSingleValue& expr);

    SDF_API virtual void ProcessStringValue(FdoStringValue& expr);

    SDF_API virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    SDF_API virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    SDF_API virtual void ProcessGeometryValue(FdoGeometryValue& expr);


public:

    //-----------------------------------------------------------------
    // SdfQueryOptimizer
    //-----------------------------------------------------------------
    
    recno_list* GetResult();
    FdoFilter* GetOptimizedFilter();

private:

    //helper functions
    recno_list* RecnoFromKey();
    bool IsAxisAlignedRectangle(FdoByteArray* fgf, Bounds& bounds);
    recno_list* recno_list_union(recno_list* left, recno_list* right);
    recno_list* recno_list_intersection(recno_list* left, recno_list* right);
    bool AreEqual(double d1, double d2);

    //working set of features and key property values
    qo_retval_stack m_retvals;
    FdoPropertyValueCollection* m_keyvals;
    filter_stack m_filters;

    //FDO context
    SdfRTree* m_rtree;
    KeyDb* m_keys;
    FdoClassDefinition* m_class;
    FdoDataPropertyDefinitionCollection* m_idprops;

};

