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
#include "PgGeometry.h"
#include <cassert>
#include <string>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

// TODO: Move this namespace to a separate reusable file
//       combined with fdo::postgis::sql
namespace
{
    char const* sepLeftTerm  = " ( ";
    char const* sepRightTerm = " ) ";
}

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
    FdoStringP name(expr.GetName());
    mBuffer.append(static_cast<char const*>(name));
}
 	
void ExpressionProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    FdoPtr<FdoExpression> exprPtr(expr.GetExpression());
    if (NULL == exprPtr)
    {
        throw FdoFilterException::Create(L"FdoComputedIdentifier is missing");
    }

    mBuffer.append(sepLeftTerm);
    exprPtr->Process(this);
    mBuffer.append(sepRightTerm);
}

void ExpressionProcessor::ProcessParameter(FdoParameter& expr)
{
    // TODO: How does parameters processing work?
    //       Are there any predefined FDO parameters or name part of "name:" 
    //       is a DBMS specific name, so for PostgreSQL we have:
    //       $1, $2, $3,...
    //       What about value?
    throw FdoFilterException::Create(L"TODO: PARAMETERS NOT IMPLEMENTED");
}

void ExpressionProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
{
    if( expr.IsNull() )
    {  
        mBuffer.append("NULL");
    }
    else
    {
        if (expr.GetBoolean())
            mBuffer.append("TRUE");
        else
            mBuffer.append("FALSE");
    }
}

void ExpressionProcessor::ProcessByteValue(FdoByteValue& expr)
{
    if( expr.IsNull() )
    {  
        mBuffer.append("NULL");
    }
    else
    {
        char value[2] = { 0 };
        value[0] = expr.GetByte();
        value[1] = '\0';
        mBuffer.append("'");
        mBuffer.append(value);
        mBuffer.append("'");
    }
}

void ExpressionProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    // TODO: Add datetime support
    throw FdoFilterException::Create(L"TODO: DATETYPE NOT IMPLEMENTED");
}

void ExpressionProcessor::ProcessDecimalValue(FdoDecimalValue& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoDouble const value = expr.GetDecimal();
        mBuffer.append(GetValueAsString(value));
    }
}

void ExpressionProcessor::ProcessDoubleValue(FdoDoubleValue& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoDouble const value = expr.GetDouble();
        mBuffer.append(GetValueAsString(value));
    }
}

void ExpressionProcessor::ProcessInt16Value(FdoInt16Value& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoInt16 const value = expr.GetInt16();
        mBuffer.append(GetValueAsString(value));
    }
}

void ExpressionProcessor::ProcessInt32Value(FdoInt32Value& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoInt32 const value = expr.GetInt32();
        mBuffer.append(GetValueAsString(value));
    }
}

void ExpressionProcessor::ProcessInt64Value(FdoInt64Value& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoInt64 const value = expr.GetInt64();
        mBuffer.append(GetValueAsString(value));
    }
}

void ExpressionProcessor::ProcessSingleValue(FdoSingleValue& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoFloat const value = expr.GetSingle();
        mBuffer.append(GetValueAsString(value));
    }
}

void ExpressionProcessor::ProcessStringValue(FdoStringValue& expr)
{
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        FdoStringP value(expr.GetString());
        mBuffer.append(static_cast<char const*>(value));
    }
}

void ExpressionProcessor::ProcessBLOBValue(FdoBLOBValue& expr)
{
    throw FdoFilterException::Create(L"BLOB value in expression is not supported");
}

void ExpressionProcessor::ProcessCLOBValue(FdoCLOBValue& expr)
{
    throw FdoFilterException::Create(L"CLOB value in expression is not supported");
}

void ExpressionProcessor::ProcessGeometryValue(FdoGeometryValue& expr)
{
    FDOLOG_MARKER("ExpressionProcessor::ProcessGeometryValue");

    // TODO: Verify interpretation of NULL geometries
    if (expr.IsNull())
    {
        mBuffer.append("NULL");

        FDOLOG_WRITE("Geometry is NULL");
    }
    else
    {
        //
        // Convert FGF geometry to OGC WKB stream
        //
        FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());
        assert(NULL != factory);

        FdoPtr<FdoByteArray> fgfBytes(expr.GetGeometry());
        assert(NULL != fgfBytes);

        FdoPtr<FdoIGeometry> fdoGeom = factory->CreateGeometryFromFgf(fgfBytes);
        assert(NULL != fdoGeom);
        
        FDOLOG_WRITE(L"Geometry: %s", fdoGeom->GetText());

        FdoPtr<FdoByteArray> wkbBytes(factory->GetWkb(fdoGeom));
        FdoByte* const bytes = wkbBytes->GetData();

        // Convert raw bytes to hex string
        std::string hexWkb;
        ewkb::ewkb_t wkb(bytes, bytes + wkbBytes->GetCount());
        ewkb::bytes_to_hex(wkb, hexWkb);
        mBuffer.append(hexWkb);

        FDOLOG_WRITE("Geometry WKB as hex string:\n\t%s", hexWkb.c_str());
    }
}

///////////////////////////////////////////////////////////////////////////////
// ExpressionProcessor interface
///////////////////////////////////////////////////////////////////////////////

void ExpressionProcessor::ReleaseExpressionText(std::string& exprText)
{
    exprText.assign(mBuffer);
    mBuffer.clear();
}
    
std::string ExpressionProcessor::ReleaseBuffer()
{
    std::string tmp(mBuffer);
    mBuffer.clear();
    return tmp;
}

}} // namespace fdo::postgis
