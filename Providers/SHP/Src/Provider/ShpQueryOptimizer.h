#ifndef SHPQUERYOPTIMIZER_H
#define SHPQUERYOPTIMIZER_H
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
#include <FdoExpressionEngineImp.h>


class ShpFeatureReader;


///<summary>
///</summary>

class ShpQueryOptimizer : public FdoExpressionEngineImp
{
    friend class ShpFeatureReader;

public:
    ShpQueryOptimizer();

    static ShpQueryOptimizer* Create(FdoIReader* reader, FdoIdentifierCollection* compIdents);
    ~ShpQueryOptimizer();

    virtual FdoPropertyDefinition* GetPropInfo (FdoString* name);

protected:
    ShpQueryOptimizer(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* compIdents, ShpSpatialIndex* rtree);
    
    FdoPtr<FdoPropertyDefinitionCollection> mProperties;

    virtual void Dispose();

public:

    ////--------------------------------------------------------------------
    //// FdoIFilterProcessor
    ////--------------------------------------------------------------------

    //virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    //virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    //virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    //virtual void ProcessInCondition(FdoInCondition& filter);

    //virtual void ProcessNullCondition(FdoNullCondition& filter);

    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    //virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);


 //   //--------------------------------------------------------------------
 //   // FdoIExpressionProcessor
 //   //--------------------------------------------------------------------

 //   virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

 //   virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

 //   virtual void ProcessFunction(FdoFunction& expr);

 //   virtual void ProcessIdentifier(FdoIdentifier& expr);

 //   virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr) { return; }

 //   virtual void ProcessParameter(FdoParameter& expr);

 //   virtual void ProcessBooleanValue(FdoBooleanValue& expr);

 //   virtual void ProcessByteValue(FdoByteValue& expr);

 //   virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

 //   virtual void ProcessDecimalValue(FdoDecimalValue& expr);

 //   virtual void ProcessDoubleValue(FdoDoubleValue& expr);

 //   virtual void ProcessInt16Value(FdoInt16Value& expr);

 //   virtual void ProcessInt32Value(FdoInt32Value& expr);

 //   virtual void ProcessInt64Value(FdoInt64Value& expr);

 //   virtual void ProcessSingleValue(FdoSingleValue& expr);

 //   virtual void ProcessStringValue(FdoStringValue& expr);

 //   virtual void ProcessBLOBValue(FdoBLOBValue& expr);

 //   virtual void ProcessCLOBValue(FdoCLOBValue& expr);

 //   virtual void ProcessGeometryValue(FdoGeometryValue& expr);

private:

    //helper functions
    bool                    AreEqual( double d1, double d2 );

    FdoGeometricPropertyDefinition* FindGeomProp(FdoClassDefinition* classDef);

    FdoPtr<ShpConnection>   m_Connection;
    FdoPtr<FdoClassDefinition> m_Class;
    FdoStringP              m_LogicalIdentityPropertyName;
    ShpSpatialIndex*        m_RTree;  
    retno_lists             m_FeatidLists;
};
#endif

