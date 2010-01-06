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
  *
 */
#include "stdafx.h"
#include <FdoExpressionEngineCopyFilter.h>

FdoFilter* FdoExpressionEngineCopyFilter::Copy( FdoFilter *filter )
{
	return FdoExpressionEngineCopyFilter::Copy(filter, NULL);
}

FdoExpression* FdoExpressionEngineCopyFilter::Copy( FdoExpression *expression )
{
	return FdoExpressionEngineCopyFilter::Copy(expression, NULL);
}

FdoFilter* FdoExpressionEngineCopyFilter::Copy( FdoFilter *filter, FdoIdentifierCollection* idList )
{
	FdoExpressionEngineCopyFilter  helper(idList);
	filter->Process( &helper );
	return helper.GetFilter();
}

FdoExpression* FdoExpressionEngineCopyFilter::Copy( FdoExpression *expression, FdoIdentifierCollection* idList )
{
	FdoExpressionEngineCopyFilter  helper(idList);
	expression->Process( &helper );
	return helper.GetExpression();
}

void FdoExpressionEngineCopyFilter::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
	FdoExpressionEngineCopyFilter  leftHelper(m_pIdentifierCollection);
	FdoExpressionEngineCopyFilter  rightHelper(m_pIdentifierCollection);
	FdoPtr<FdoExpression>(expr.GetLeftExpression())->Process( &leftHelper );
	FdoPtr<FdoExpression>(expr.GetRightExpression())->Process( &rightHelper );

	m_pExpression = FdoBinaryExpression::Create( FdoPtr<FdoExpression>(leftHelper.GetExpression()), expr.GetOperation(), 
												 FdoPtr<FdoExpression>(rightHelper.GetExpression()) );
}

void FdoExpressionEngineCopyFilter::ProcessBooleanValue(FdoBooleanValue& expr) 
{  
	m_pExpression = FdoBooleanValue::Create( expr.GetBoolean() );
}

void FdoExpressionEngineCopyFilter::ProcessByteValue(FdoByteValue& expr)
{
	m_pExpression = FdoByteValue::Create( expr.GetByte() );
}

void FdoExpressionEngineCopyFilter::ProcessDateTimeValue(FdoDateTimeValue& expr)
{   
	m_pExpression = FdoDateTimeValue::Create( expr.GetDateTime() );
}

void FdoExpressionEngineCopyFilter::ProcessDoubleValue(FdoDoubleValue& expr)
{
	m_pExpression = FdoDoubleValue::Create( expr.GetDouble() );
}

void FdoExpressionEngineCopyFilter::ProcessDecimalValue(FdoDecimalValue& expr)
{
	m_pExpression = FdoDecimalValue::Create( expr.GetDecimal() );
}

void FdoExpressionEngineCopyFilter::ProcessInt16Value(FdoInt16Value& expr)
{
	m_pExpression = FdoInt16Value::Create( expr.GetInt16() );
}

void FdoExpressionEngineCopyFilter::ProcessInt32Value(FdoInt32Value& expr)
{
	m_pExpression = FdoInt32Value::Create( expr.GetInt32() );
}

void FdoExpressionEngineCopyFilter::ProcessInt64Value(FdoInt64Value& expr)
{
	m_pExpression = FdoInt64Value::Create( expr.GetInt64() );
}
void FdoExpressionEngineCopyFilter::ProcessSingleValue(FdoSingleValue& expr)
{
	m_pExpression = FdoSingleValue::Create( expr.GetSingle() );
}

void FdoExpressionEngineCopyFilter::ProcessStringValue(FdoStringValue& expr)
{
	m_pExpression = FdoStringValue::Create( expr.GetString() );
}

void FdoExpressionEngineCopyFilter::ProcessBLOBValue(FdoBLOBValue& expr)
{   
    if(!expr.IsNull())
    {
        FdoPtr<FdoByteArray> buffData = expr.GetData();
        m_pExpression = FdoBLOBValue::Create(buffData);
    }
    else
        m_pExpression = FdoBLOBValue::Create();
}

void FdoExpressionEngineCopyFilter::ProcessCLOBValue(FdoCLOBValue& expr)
{  
    if(!expr.IsNull())
    {
        FdoPtr<FdoByteArray> buffData = expr.GetData();
        m_pExpression = FdoCLOBValue::Create(buffData);
    }
    else
        m_pExpression = FdoCLOBValue::Create();
}
void FdoExpressionEngineCopyFilter::ProcessFunction(FdoFunction& expr)
{
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
    FdoPtr<FdoExpressionCollection>col = expr.GetArguments();
	FdoPtr<FdoExpressionCollection>newCol = FdoExpressionCollection::Create();
    for(int i=0; i<col->GetCount(); i++)
	{
		FdoPtr<FdoExpression>(col->GetItem( i ) )->Process( &helper );
		newCol->Add( FdoPtr<FdoExpression>(static_cast<FdoExpression*>( helper.GetExpression() ) ) );
	}
	m_pExpression = FdoFunction::Create( expr.GetName(), newCol );
}
void FdoExpressionEngineCopyFilter::ProcessGeometryValue(FdoGeometryValue& expr)
{  
	FdoPtr<FdoByteArray> geomArray = expr.GetGeometry();
	FdoPtr<FdoByteArray> newArray = FdoByteArray::Create( geomArray->GetData(), geomArray->GetCount() );
	m_pExpression = FdoGeometryValue::Create( newArray );
}
void FdoExpressionEngineCopyFilter::ProcessParameter(FdoParameter& expr)
{
	m_pExpression = FdoParameter::Create( expr.GetName() );
}
void FdoExpressionEngineCopyFilter::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
	FdoPtr<FdoExpression>(expr.GetExpression())->Process( &helper );
	FdoPtr<FdoIdentifier>id = static_cast<FdoIdentifier*>(helper.GetExpression());
	m_pExpression = FdoUnaryExpression::Create( expr.GetOperation(), FdoPtr<FdoExpression>(helper.GetExpression()));
}
void FdoExpressionEngineCopyFilter::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
	FdoExpressionEngineCopyFilter  leftHelper(m_pIdentifierCollection);
	FdoExpressionEngineCopyFilter  rightHelper(m_pIdentifierCollection);
	FdoPtr<FdoFilter>(filter.GetLeftOperand())->Process( &leftHelper );
	FdoPtr<FdoFilter>(filter.GetRightOperand())->Process( &rightHelper );
	m_pFilter = FdoBinaryLogicalOperator::Create(FdoPtr<FdoFilter>(leftHelper.GetFilter()), filter.GetOperation(), 
												 FdoPtr<FdoFilter>(rightHelper.GetFilter()) );
}
void FdoExpressionEngineCopyFilter::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
	FdoExpressionEngineCopyFilter  leftHelper(m_pIdentifierCollection);
	FdoExpressionEngineCopyFilter  rightHelper(m_pIdentifierCollection);
	FdoPtr<FdoExpression>(filter.GetLeftExpression())->Process( &leftHelper );
	FdoPtr<FdoExpression>(filter.GetRightExpression())->Process( &rightHelper );
	m_pFilter = FdoComparisonCondition::Create( FdoPtr<FdoExpression>(leftHelper.GetExpression()), filter.GetOperation(), 
												FdoPtr<FdoExpression>(rightHelper.GetExpression()) );
}
void FdoExpressionEngineCopyFilter::ProcessDistanceCondition(FdoDistanceCondition& filter)
{  
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
	FdoPtr<FdoExpression>(filter.GetGeometry())->Process( &helper );
    m_pFilter = FdoDistanceCondition::Create( FdoPtr<FdoIdentifier>(filter.GetPropertyName()), filter.GetOperation(), 
											FdoPtr<FdoExpression>(helper.GetExpression()), filter.GetDistance() );
}

void FdoExpressionEngineCopyFilter::ProcessInCondition(FdoInCondition& filter)
{
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
	FdoPtr<FdoValueExpressionCollection> values = filter.GetValues();
	FdoPtr<FdoValueExpressionCollection> newValues = FdoValueExpressionCollection::Create();
	for(int i=0; i<values->GetCount(); i++ )
	{
		FdoPtr<FdoValueExpression>(values->GetItem( i ) )->Process( &helper );
		newValues->Add( FdoPtr<FdoValueExpression>(static_cast<FdoValueExpression*>( helper.GetExpression() ) ) );
	}
	m_pFilter = FdoInCondition::Create(FdoPtr<FdoIdentifier>(filter.GetPropertyName()), newValues );
}
void FdoExpressionEngineCopyFilter::ProcessNullCondition(FdoNullCondition& filter)
{
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
	FdoPtr<FdoExpression>(filter.GetPropertyName())->Process( &helper );
	FdoPtr<FdoIdentifier>id = static_cast<FdoIdentifier*>(helper.GetExpression());
	m_pFilter = FdoNullCondition::Create( id->GetName() );
}
void FdoExpressionEngineCopyFilter::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
	FdoPtr<FdoExpression>(filter.GetGeometry())->Process( &helper );
	m_pFilter = FdoSpatialCondition::Create(FdoPtr<FdoIdentifier>(filter.GetPropertyName()), filter.GetOperation(), FdoPtr<FdoExpression>(helper.GetExpression() ) );
}

void FdoExpressionEngineCopyFilter::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
	FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
	FdoPtr<FdoFilter>(filter.GetOperand())->Process( &helper );
	m_pFilter = FdoUnaryLogicalOperator::Create( FdoPtr<FdoFilter>(helper.GetFilter() ), filter.GetOperation() );
}

void FdoExpressionEngineCopyFilter::ProcessIdentifier(FdoIdentifier& expr)
{
	if( m_pIdentifierCollection != NULL )
	{
		FdoPtr<FdoIdentifier> id = m_pIdentifierCollection->FindItem( expr.GetName() );
		if( id != NULL )
		{
			FdoComputedIdentifier* cid = dynamic_cast<FdoComputedIdentifier*>(id.p);
			if( cid != NULL )
			{
				ProcessComputedIdentifier( *cid );
				return;
			}
		}
	}
	m_pExpression = FdoIdentifier::Create( expr.GetName() );
}

void FdoExpressionEngineCopyFilter::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
	if( m_pIdentifierCollection != NULL )
	{
		m_pExpression = expr.GetExpression();
	}
	else
	{
		FdoExpressionEngineCopyFilter  helper(m_pIdentifierCollection);
		FdoPtr<FdoExpression>(expr.GetExpression())->Process( &helper );
		m_pExpression = FdoComputedIdentifier::Create( expr.GetName(), FdoPtr<FdoExpression>(helper.GetExpression() ) );
	}
}

FdoExpression*	FdoExpressionEngineCopyFilter::GetExpression()
{
	FDO_SAFE_ADDREF( m_pExpression.p );
	return m_pExpression;
}

FdoFilter*	FdoExpressionEngineCopyFilter::GetFilter()
{
	FDO_SAFE_ADDREF( m_pFilter.p );
	return m_pFilter;
}
