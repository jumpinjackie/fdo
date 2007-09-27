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
//
// This helper class traverse the filter object and creates a deep copy.
class FdoExpressionEngineCopyFilter : public virtual FdoIExpressionProcessor, public virtual FdoIFilterProcessor
{
protected:
	FdoExpressionEngineCopyFilter() 
	{
		m_pIdentifierCollection = NULL;
	}
	FdoExpressionEngineCopyFilter( FdoIdentifierCollection* idList ) 
	{
		if( idList != NULL )
			idList->AddRef();

		m_pIdentifierCollection = idList;
	}
	virtual ~FdoExpressionEngineCopyFilter() {}

public: // Used by the caller
	
	static FdoExpressionEngineCopyFilter* Create()
	{
		return new FdoExpressionEngineCopyFilter();
	}

	static FdoFilter* Copy( FdoFilter *filter );

	static FdoExpression* Copy( FdoExpression *expression );

	static FdoFilter* Copy( FdoFilter *filter, FdoIdentifierCollection* idList  );

	static FdoExpression* Copy( FdoExpression *expression, FdoIdentifierCollection* idList  );
	
public: // Required by Fdo interfaces

    virtual void Dispose() { delete this; }

     /// <summary>Increase the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns>
    FdoInt32 AddRef()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
        //  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::AddRef();
    }

    /// <summary>Decrease the reference count.</summary>
    /// <returns>Returns the new reference count (value for debugging use only).</returns>
    FdoInt32 Release ()
    {
        // NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
        //  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::Release();
    }

    virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);
    
	virtual void ProcessBooleanValue(FdoBooleanValue& expr) ;
    
	virtual void ProcessByteValue(FdoByteValue& expr);
    
	virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    virtual void ProcessDoubleValue(FdoDoubleValue& expr);
    
	virtual void ProcessDecimalValue(FdoDecimalValue& expr);
    
	virtual void ProcessInt16Value(FdoInt16Value& expr);
    
	virtual void ProcessInt32Value(FdoInt32Value& expr);
    
	virtual void ProcessInt64Value(FdoInt64Value& expr);

    virtual void ProcessSingleValue(FdoSingleValue& expr);
    
	virtual void ProcessStringValue(FdoStringValue& expr);
    
	virtual void ProcessBLOBValue(FdoBLOBValue& expr);
    
	virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    virtual void ProcessFunction(FdoFunction& expr);

    virtual void ProcessGeometryValue(FdoGeometryValue& expr);

    virtual void ProcessParameter(FdoParameter& expr);

    virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

    virtual void ProcessInCondition(FdoInCondition& filter);

    virtual void ProcessNullCondition(FdoNullCondition& filter);

    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    virtual void ProcessIdentifier(FdoIdentifier& expr);

    virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

	FdoExpression*	GetExpression();

	FdoFilter*	GetFilter();

private:
	FdoPtr<FdoFilter>				m_pFilter;

	FdoPtr<FdoExpression>			m_pExpression;

	FdoPtr<FdoIdentifierCollection> m_pIdentifierCollection;
};

#endif // _FdoExpressionEngineCopyFilter_
