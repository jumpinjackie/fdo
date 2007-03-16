//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "ExpressionProcessor.h"

#include <cassert>

namespace fdo { namespace postgis {

ExpressionProcessor::ExpressionProcessor()
{
}

ExpressionProcessor::~ExpressionProcessor()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDelete interface
///////////////////////////////////////////////////////////////////////////////

void ExpressionProcessor::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIExpressionProcessor interface
///////////////////////////////////////////////////////////////////////////////

void ExpressionProcessor::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
}

void ExpressionProcessor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
}

void ExpressionProcessor::ProcessFunction(FdoFunction& expr)
{
}

void ExpressionProcessor::ProcessIdentifier(FdoIdentifier& expr)
{
}
 	
void ExpressionProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
}

void ExpressionProcessor::ProcessParameter(FdoParameter& expr)
{
}

void ExpressionProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
{
}

void ExpressionProcessor::ProcessByteValue(FdoByteValue& expr)
{
}

void ExpressionProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
}

void ExpressionProcessor::ProcessDecimalValue(FdoDecimalValue& expr)
{
}

void ExpressionProcessor::ProcessDoubleValue(FdoDoubleValue& expr)
{
}

void ExpressionProcessor::ProcessInt16Value(FdoInt16Value& expr)
{
}

void ExpressionProcessor::ProcessInt32Value(FdoInt32Value& expr)
{
}

void ExpressionProcessor::ProcessInt64Value(FdoInt64Value& expr)
{
}

void ExpressionProcessor::ProcessSingleValue(FdoSingleValue& expr)
{
}

void ExpressionProcessor::ProcessStringValue(FdoStringValue& expr)
{
}

void ExpressionProcessor::ProcessBLOBValue(FdoBLOBValue& expr)
{
}

void ExpressionProcessor::ProcessCLOBValue(FdoCLOBValue& expr)
{
}

void ExpressionProcessor::ProcessGeometryValue(FdoGeometryValue& expr)
{
}

}} // namespace fdo::postgis
