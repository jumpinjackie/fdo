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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_TESTPROCESSOR_H
#define CPP_UNIT_TESTPROCESSOR_H

// Test case for the expression and filter processor
class UnitTestProcessor : public FdoIExpressionProcessor, public FdoIFilterProcessor
{
public:
    UnitTestProcessor(FdoInt32 tabLevel = 0);

    void Dispose();

    void ProcessBinaryExpression(FdoBinaryExpression& expr);
    void ProcessBooleanValue(FdoBooleanValue& expr);
    void ProcessByteValue(FdoByteValue& expr);
    void ProcessDateTimeValue(FdoDateTimeValue& expr);
    void ProcessDoubleValue(FdoDoubleValue& expr);
    void ProcessDecimalValue(FdoDecimalValue& expr);
    void ProcessInt16Value(FdoInt16Value& expr);
    void ProcessInt32Value(FdoInt32Value& expr);
    void ProcessInt64Value(FdoInt64Value& expr);
    void ProcessSingleValue(FdoSingleValue& expr);
    void ProcessStringValue(FdoStringValue& expr);
    void ProcessBLOBValue(FdoBLOBValue& expr);
    void ProcessCLOBValue(FdoCLOBValue& expr);
    void ProcessFunction(FdoFunction& expr);
    void ProcessGeometryValue(FdoGeometryValue& expr);
    void ProcessIdentifier(FdoIdentifier& expr);
    void ProcessComputedIdentifier(FdoComputedIdentifier& expr);
    void ProcessParameter(FdoParameter& expr);
    void ProcessUnaryExpression(FdoUnaryExpression& expr);
    void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);
    void ProcessComparisonCondition(FdoComparisonCondition& filter);
    void ProcessDistanceCondition(FdoDistanceCondition& filter);
    void ProcessInCondition(FdoInCondition& filter);
    void ProcessNullCondition(FdoNullCondition& filter);
    void ProcessSpatialCondition(FdoSpatialCondition& filter);
    void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

protected:
    void prependTabs();

    FdoInt32     m_tabLevel;
};

#endif

