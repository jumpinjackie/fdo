#ifndef _FdoExpressionEngineCopyFilter_
#define _FdoExpressionEngineCopyFilter_  1
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
// This helper class traverse the filter object and creates a deep copy.
class FdoExpressionEngineCopyFilter : public virtual FdoIExpressionProcessor, public virtual FdoIFilterProcessor
{
protected:
	EXPRESSIONENGINE_API FdoExpressionEngineCopyFilter() 
	{
		m_pIdentifierCollection = NULL;
	}
	EXPRESSIONENGINE_API FdoExpressionEngineCopyFilter( FdoIdentifierCollection* idList ) 
	{
		if( idList != NULL )
			idList->AddRef();

		m_pIdentifierCollection = idList;
	}
	EXPRESSIONENGINE_API virtual ~FdoExpressionEngineCopyFilter() {}

public: // Used by the caller
	
	EXPRESSIONENGINE_API static FdoExpressionEngineCopyFilter* Create()
	{
		return new FdoExpressionEngineCopyFilter();
	}

	EXPRESSIONENGINE_API static FdoFilter* Copy( FdoFilter *filter );

	EXPRESSIONENGINE_API static FdoExpression* Copy( FdoExpression *expression );

	EXPRESSIONENGINE_API static FdoFilter* Copy( FdoFilter *filter, FdoIdentifierCollection* idList  );

	EXPRESSIONENGINE_API static FdoExpression* Copy( FdoExpression *expression, FdoIdentifierCollection* idList  );
	
public: // Required by Fdo interfaces

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

    EXPRESSIONENGINE_API virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessBooleanValue(FdoBooleanValue& expr) ;
    
	EXPRESSIONENGINE_API virtual void ProcessByteValue(FdoByteValue& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    EXPRESSIONENGINE_API virtual void ProcessDoubleValue(FdoDoubleValue& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessDecimalValue(FdoDecimalValue& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessInt16Value(FdoInt16Value& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessInt32Value(FdoInt32Value& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessInt64Value(FdoInt64Value& expr);

    EXPRESSIONENGINE_API virtual void ProcessSingleValue(FdoSingleValue& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessStringValue(FdoStringValue& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessBLOBValue(FdoBLOBValue& expr);
    
	EXPRESSIONENGINE_API virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    EXPRESSIONENGINE_API virtual void ProcessFunction(FdoFunction& expr);

    EXPRESSIONENGINE_API virtual void ProcessGeometryValue(FdoGeometryValue& expr);

    EXPRESSIONENGINE_API virtual void ProcessParameter(FdoParameter& expr);

    EXPRESSIONENGINE_API virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    EXPRESSIONENGINE_API virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    EXPRESSIONENGINE_API virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    EXPRESSIONENGINE_API virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

    EXPRESSIONENGINE_API virtual void ProcessInCondition(FdoInCondition& filter);

    EXPRESSIONENGINE_API virtual void ProcessNullCondition(FdoNullCondition& filter);

    EXPRESSIONENGINE_API virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    EXPRESSIONENGINE_API virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    EXPRESSIONENGINE_API virtual void ProcessIdentifier(FdoIdentifier& expr);

    EXPRESSIONENGINE_API virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    EXPRESSIONENGINE_API virtual void ProcessSubSelectExpression(FdoSubSelectExpression& expr);

	EXPRESSIONENGINE_API FdoExpression*	GetExpression();

	EXPRESSIONENGINE_API FdoFilter*	GetFilter();

private:
	FdoPtr<FdoFilter>				m_pFilter;

	FdoPtr<FdoExpression>			m_pExpression;

	FdoPtr<FdoIdentifierCollection> m_pIdentifierCollection;
};

#endif // _FdoExpressionEngineCopyFilter_
