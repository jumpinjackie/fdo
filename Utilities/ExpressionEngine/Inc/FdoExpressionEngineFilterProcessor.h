#ifndef _FdoExpressionEngineFilterProcessor_
#define _FdoExpressionEngineFilterProcessor_  1
/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
  *
 */
#ifdef _WIN32
#pragma once
#endif

#include "ExpressionEngine.h"

//
// This helper class traverse the filter object. It does no usufull work.
// Other utility classes can subclass from this class and override the specific methods that they need to handle.
// For example to find if a given filter contains a spatial condition, the a base class that overrides only
// ProcessSpatialCondition method can be used to set a bool member variable when the overriden method is called.
// The constractor is not protected to be able to declare this class on the stack:
// class MyFilterProcessor: public FdoExpressionEngineFilterProcessor
// {   
//    protected:
//    MyFilterProcessor() { HasSpatialCondition = false; }
//    public:
//    bool   HasSpatialCondition;
//
//    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter){ HasSpatialCondition = true;  }
// };
// FdoPtr<MyFilterProcessor>processor = MyFilterProcessor::Create();
// myfilter->Process( processor );
// if( processor->HasSpatialCondition )
//     ...
class FdoExpressionEngineFilterProcessor : public virtual FdoIExpressionProcessor, public virtual FdoIFilterProcessor
{
protected:

	EXPRESSIONENGINE_API FdoExpressionEngineFilterProcessor() {}

	EXPRESSIONENGINE_API virtual ~FdoExpressionEngineFilterProcessor() {}

    EXPRESSIONENGINE_API void HandleExpr( FdoExpression *exp )
    {
        exp->Process( this );
    }
    EXPRESSIONENGINE_API void HandleFilter( FdoFilter *filter )
    {
        filter->Process( this );
    }
public:

	EXPRESSIONENGINE_API static FdoExpressionEngineFilterProcessor* Create()
	{
		return new FdoExpressionEngineFilterProcessor();
	}

    EXPRESSIONENGINE_API virtual void Dispose() { delete this; }

     /// <summary>Increase the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns>
    EXPRESSIONENGINE_API FdoInt32 AddRef()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
        //  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::AddRef();
    }

    /// <summary>Decrease the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns>
    EXPRESSIONENGINE_API FdoInt32 Release ()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
        //  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::Release();
    }

    EXPRESSIONENGINE_API virtual void ProcessBinaryExpression(FdoBinaryExpression& expr)
    {
        HandleExpr( FdoPtr<FdoExpression>(expr.GetLeftExpression()) );
        HandleExpr( FdoPtr<FdoExpression>(expr.GetRightExpression()) );
    }
    EXPRESSIONENGINE_API virtual void ProcessBooleanValue(FdoBooleanValue& ) {  }
    EXPRESSIONENGINE_API virtual void ProcessByteValue(FdoByteValue& ){   }
    EXPRESSIONENGINE_API virtual void ProcessDateTimeValue(FdoDateTimeValue& ){   }
    EXPRESSIONENGINE_API virtual void ProcessDoubleValue(FdoDoubleValue& ){   }
    EXPRESSIONENGINE_API virtual void ProcessDecimalValue(FdoDecimalValue& ){   }
    EXPRESSIONENGINE_API virtual void ProcessInt16Value(FdoInt16Value& ){  }
    EXPRESSIONENGINE_API virtual void ProcessInt32Value(FdoInt32Value& ){  }
    EXPRESSIONENGINE_API virtual void ProcessInt64Value(FdoInt64Value& ){  }
    EXPRESSIONENGINE_API virtual void ProcessSingleValue(FdoSingleValue& ){  }
    EXPRESSIONENGINE_API virtual void ProcessStringValue(FdoStringValue& ){  }
    EXPRESSIONENGINE_API virtual void ProcessBLOBValue(FdoBLOBValue& ){   }
    EXPRESSIONENGINE_API virtual void ProcessCLOBValue(FdoCLOBValue& ){  }
    EXPRESSIONENGINE_API virtual void ProcessFunction(FdoFunction& expr)
    {
        FdoPtr<FdoExpressionCollection>col = expr.GetArguments();
        for(int i=0; i<col->GetCount(); i++)
            HandleExpr( FdoPtr<FdoExpression>(col->GetItem( i ) ) );
    }
    EXPRESSIONENGINE_API virtual void ProcessGeometryValue(FdoGeometryValue& ){  }
    EXPRESSIONENGINE_API virtual void ProcessParameter(FdoParameter& ){  }
    EXPRESSIONENGINE_API virtual void ProcessUnaryExpression(FdoUnaryExpression& expr)
    {
        HandleExpr( FdoPtr<FdoExpression>(expr.GetExpression()) );
    }
    EXPRESSIONENGINE_API virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
    {
        HandleFilter( FdoPtr<FdoFilter>(filter.GetLeftOperand()) );
        HandleFilter( FdoPtr<FdoFilter>(filter.GetRightOperand()) );
    }
    EXPRESSIONENGINE_API virtual void ProcessComparisonCondition(FdoComparisonCondition& filter)
    {
        HandleExpr( FdoPtr<FdoExpression>(filter.GetLeftExpression()) );
        HandleExpr( FdoPtr<FdoExpression>(filter.GetRightExpression()) );
    }
    EXPRESSIONENGINE_API virtual void ProcessDistanceCondition(FdoDistanceCondition& ){  }

    EXPRESSIONENGINE_API virtual void ProcessInCondition(FdoInCondition& filter)
    {
        ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
    }
    EXPRESSIONENGINE_API virtual void ProcessNullCondition(FdoNullCondition& filter)
    {
        ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
    }
    EXPRESSIONENGINE_API virtual void ProcessSpatialCondition(FdoSpatialCondition& ){  }

    EXPRESSIONENGINE_API virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
    {
        HandleFilter( FdoPtr<FdoFilter>(filter.GetOperand()) );
    }

    EXPRESSIONENGINE_API virtual void ProcessIdentifier(FdoIdentifier& )
    {
    }

    EXPRESSIONENGINE_API virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr)
    {
        HandleExpr( FdoPtr<FdoExpression>(expr.GetExpression()) );
    }
};

#endif // _FdoExpressionEngineFilterProcessor_
