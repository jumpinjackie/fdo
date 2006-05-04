// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "Pch.h"
#include "TestProcessor.h"

UnitTestProcessor::UnitTestProcessor(FdoInt32 tabLevel /* = 0 */) : m_tabLevel(tabLevel)
{
}

void UnitTestProcessor::Dispose()
{
    delete this;
}

void UnitTestProcessor::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    m_tabLevel++;
    FdoExpression*  pExpr = expr.GetLeftExpression();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;

    prependTabs();
    wprintf(L"BinaryExpression : %d\n", expr.GetOperation());

    m_tabLevel++;
    pExpr = expr.GetRightExpression();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;
}

void UnitTestProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
{
    prependTabs();
    wprintf(L"BooleanValue : %s\n", expr.GetBoolean() ? L"TRUE" : L"FALSE");
}

void UnitTestProcessor::ProcessByteValue(FdoByteValue& expr)
{
    prependTabs();
    wprintf(L"ByteValue : %d\n", (FdoInt32)expr.GetByte());
}

void UnitTestProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    prependTabs();
    wprintf(L"DateTimeValue : \n");
}

void UnitTestProcessor::ProcessDecimalValue(FdoDecimalValue& expr)
{
    prependTabs();
    wprintf(L"DecimalValue : %f\n", expr.GetDecimal());
}

void UnitTestProcessor::ProcessDoubleValue(FdoDoubleValue& expr)
{
    prependTabs();
    wprintf(L"DoubleValue : %f\n", expr.GetDouble());
}

void UnitTestProcessor::ProcessInt16Value(FdoInt16Value& expr)
{
    prependTabs();
    wprintf(L"Int16Value : %d\n", (FdoInt32)expr.GetInt16());
}

void UnitTestProcessor::ProcessInt32Value(FdoInt32Value& expr)
{
    prependTabs();
    wprintf(L"Int32Value : %d\n", expr.GetInt32());
}

void UnitTestProcessor::ProcessInt64Value(FdoInt64Value& expr)
{
    prependTabs();
    wprintf(L"Int64Value : %I64d\n", expr.GetInt64());
}

void UnitTestProcessor::ProcessSingleValue(FdoSingleValue& expr)
{
    prependTabs();
    wprintf(L"SingleValue : %f\n", (double)expr.GetSingle());
}

void UnitTestProcessor::ProcessStringValue(FdoStringValue& expr)
{
    prependTabs();
    wprintf(L"StringValue : %s\n", expr.GetString());
}

void UnitTestProcessor::ProcessBLOBValue(FdoBLOBValue& expr)
{
    prependTabs();
    wprintf(L"BLOBValue :\n");
}

void UnitTestProcessor::ProcessCLOBValue(FdoCLOBValue& expr)
{
    prependTabs();
    wprintf(L"CLOBValue :\n");
}

void UnitTestProcessor::ProcessFunction(FdoFunction& expr)
{
    prependTabs();
    wprintf(L"Function : %s\n", expr.GetName());
    FdoExpressionCollection* pColl = expr.GetArguments();
    m_tabLevel++;
    for (FdoInt32 i = 0; i < pColl->GetCount(); i++)
    {
        FdoExpression*   pExpr;

        pExpr = pColl->GetItem(i);
        pExpr->Process(this);
        pExpr->Release();
    }
    m_tabLevel--;
    pColl->Release();
}

void UnitTestProcessor::ProcessGeometryValue(FdoGeometryValue& expr)
{
    prependTabs();
    wprintf(L"GeometryValue : %s\n", expr.IsNull() ? L"NULL" : L"not NULL");
}

void UnitTestProcessor::ProcessIdentifier(FdoIdentifier& expr)
{
    prependTabs();
    wprintf(L"Identifier : %s\n", expr.GetText());
}

void UnitTestProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    prependTabs();
    wprintf(L"Computed Identifier Alias : %s\n", expr.GetText());
    UnitTestProcessor    expProc;
    FdoPtr<FdoExpression> exp = expr.GetExpression();
    if( exp )
    {
        wprintf(L"Computed Identifier Expression: \n");
        exp->Process(&expProc);
    }
    else
        wprintf(L"NULL expression\n");
}

void UnitTestProcessor::ProcessParameter(FdoParameter& expr)
{
    prependTabs();
    wprintf(L"Parameter : %s\n", expr.GetName());
}

void UnitTestProcessor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    prependTabs();
    wprintf(L"UnaryExpression : \n");

    m_tabLevel++;
    FdoExpression*  pExpr = expr.GetExpression();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;
}

void UnitTestProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    m_tabLevel++;
    FdoFilter*  pFilter = filter.GetLeftOperand();
    pFilter->Process(this);
    pFilter->Release();
    m_tabLevel--;

    prependTabs();
    wprintf(L"BinaryLogicalOperator : %d\n", filter.GetOperation());

    m_tabLevel++;
    pFilter = filter.GetRightOperand();
    pFilter->Process(this);
    pFilter->Release();
    m_tabLevel--;
}

void UnitTestProcessor::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    m_tabLevel++;
    FdoExpression*  pExpr = filter.GetLeftExpression();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;

    prependTabs();
    wprintf(L"ComparisonCondition : %d\n", filter.GetOperation());

    m_tabLevel++;
    pExpr = filter.GetRightExpression();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;
}

void UnitTestProcessor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    prependTabs();
    wprintf(L"DistanceCondition : %d - %f\n", filter.GetOperation(), filter.GetDistance());

    m_tabLevel++;
    FdoExpression*  pExpr = filter.GetGeometry();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;
}

void UnitTestProcessor::ProcessInCondition(FdoInCondition& filter)
{
    prependTabs();
    FdoIdentifier*  pIdent = filter.GetPropertyName();
    wprintf(L"InCondition : %s\n", pIdent->GetText());
    pIdent->Release();

    m_tabLevel++;
    FdoValueExpressionCollection* pColl = filter.GetValues();
    for (FdoInt32 i = 0; i < pColl->GetCount(); i++)
    {
        FdoValueExpression*  pExpr;

        pExpr = pColl->GetItem(i);
        pExpr->Process(this);
        pExpr->Release();
    }
    m_tabLevel--;
    pColl->Release();
}

void UnitTestProcessor::ProcessNullCondition(FdoNullCondition& filter)
{
    prependTabs();
    FdoIdentifier*  pIdent = filter.GetPropertyName();
    wprintf(L"NullCondition : %s\n", pIdent->GetText());
    pIdent->Release();
}

void UnitTestProcessor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    prependTabs();
    FdoIdentifier*  pIdent = filter.GetPropertyName();
    wprintf(L"SpatialCondition : %d - %s\n", filter.GetOperation(), pIdent->GetText());
    pIdent->Release();

    m_tabLevel++;
    FdoExpression*  pExpr = filter.GetGeometry();
    pExpr->Process(this);
    pExpr->Release();
    m_tabLevel--;
}

void UnitTestProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    prependTabs();
    wprintf(L"UnaryLogicalOperator : %d\n", filter.GetOperation());

    m_tabLevel++;
    FdoFilter*  pFilter = filter.GetOperand();
    pFilter->Process(this);
    pFilter->Release();
    m_tabLevel--;
}

void UnitTestProcessor::prependTabs()
{
    for (FdoInt32 i = 0; i < m_tabLevel; i++)
#ifdef _WIN32
        _putwch(L' ');
#else
	printf("%ls", L" ");
#endif
}

