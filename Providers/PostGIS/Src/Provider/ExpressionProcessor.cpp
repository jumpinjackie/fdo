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
#include "PgUtility.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

// TODO: Move this namespace to a separate reusable file
//       combined with fdo::postgis::sql
namespace
{
    char const* sepLeftTerm  = " ( ";
    char const* sepRightTerm = " ) ";
    char const* sepComma     = " , ";

    char const* opPlus  = " + ";
    char const* opMinus = " - ";
    char const* opMult  = " * ";
    char const* opDiv   = " / ";

}

ExpressionProcessor::ExpressionProcessor()
{
    // idle
}

ExpressionProcessor::~ExpressionProcessor()
{
    // idle
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
    FDOLOG_MARKER("ExpressionProcessor::+ProcessBinaryExpression");

    FdoPtr<FdoExpression> operandLeft(expr.GetLeftExpression());
    FdoPtr<FdoExpression> operandRight(expr.GetRightExpression());

    if (NULL == operandLeft)
    {
        FDOLOG_WRITE("ERROR: Left operand of FdoBinaryExpression is missing");
        throw FdoFilterException::Create(L"Left operand of FdoBinaryExpression is missing");
    }

    if (NULL == operandRight)
    {
        FDOLOG_WRITE("ERROR: Right operand of FdoBinaryExpression is missing");
        throw FdoFilterException::Create(L"Right operand of FdoBinaryExpression is missing");
    }

    std::string binaryOp;

    switch (expr.GetOperation())
    {
    case FdoBinaryOperations_Add:
        binaryOp = opPlus;
        break;
    case FdoBinaryOperations_Subtract:
        binaryOp = opMinus;
        break;
    case FdoBinaryOperations_Multiply:
        binaryOp = opMult;
        break;
    case FdoBinaryOperations_Divide:
        binaryOp = opDiv;
        break;
    default:
        {
            FDOLOG_WRITE("ERROR: Unsupported type of ProcessBinaryExpression expression");
            throw FdoFilterException::Create(L"Unsupported type of ProcessBinaryExpression expression");
        }
    }

    mBuffer.append(sepLeftTerm);
    operandLeft->Process(this);
    mBuffer.append(binaryOp);
    operandRight->Process(this);
    mBuffer.append(sepRightTerm);

    FDOLOG_WRITE("Expression text: %s", mBuffer.c_str());
}

void ExpressionProcessor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    FDOLOG_MARKER("ExpressionProcessor::+ProcessUnaryExpression");

    FdoPtr<FdoExpression> exprPtr(expr.GetExpression());

    if (NULL == exprPtr)
    {
        FDOLOG_WRITE("ERROR: Missing FdoUnaryExpression expression");
        throw FdoFilterException::Create(L"Missing FdoUnaryExpression expression");
    }

    if (FdoUnaryOperations_Negate == expr.GetOperation())
    {
        mBuffer.append(sepLeftTerm);
        mBuffer.append(" - ");
        mBuffer.append(sepLeftTerm);
        exprPtr->Process(this);
        mBuffer.append(sepRightTerm);
        mBuffer.append(" ");
        mBuffer.append(sepRightTerm);
    }
    else
    {
        FDOLOG_WRITE("ERROR: Unknown operation of unary expression.");
        throw FdoFilterException::Create(L"Unknown operation of unary expression");
    }

    FDOLOG_WRITE("Expression text: %s", mBuffer.c_str());
}

void ExpressionProcessor::ProcessFunction(FdoFunction& expr)
{
    FDOLOG_MARKER("ExpressionProcessor::+ProcessFunction");

    FdoStringP name(expr.GetName());
    FdoPtr<FdoExpressionCollection> args(expr.GetArguments());
    FdoInt32 nbArgs = args->GetCount();
    std::string sep;
    std::string argSep;
    std::string argCast;
    bool processArgs = true;

    if (0 == name.ICompare(FDO_FUNCTION_CONCAT))
    {
        argSep = " || ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_SPATIALEXTENTS))
    {
        /*
        processArgs = false;
        mBuffer.append("GeomFromEwkb(AsEwkb(Extent(");
        FdoPtr<FdoExpression> geomExpr(args->GetItem(0));
        geomExpr->Process(this);
        mBuffer.append(")))");
        */

        //Coalesce( GeomFromEwkb(AsEwkb(Extent(geometry_1))) , geomfromtext('POLYGON((0 0,0 0,0 0,0 0,0 0))'))
      
        processArgs = false;
        mBuffer.append("Coalesce(GeomFromEwkb(AsEwkb(Extent(");
        FdoPtr<FdoExpression> geomExpr(args->GetItem(0));
        geomExpr->Process(this);
        mBuffer.append("))),geomfromtext('POLYGON((0 0,0 50,50 50,50 0,0 0))'))");
        

    }
    else if (0 == name.ICompare(FDO_FUNCTION_AREA2D))
    {
        mBuffer.append("Area");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_NULLVALUE))
    {
        mBuffer.append("Coalesce");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_INSTR))
    {
        mBuffer.append("Strpos");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_REMAINDER))
    {
        //Remainder(34.5,3) = 34.5 - Round(34.5/3) * 3
        processArgs = false;
        FdoPtr<FdoExpression> arg1Expr(args->GetItem(0));
        FdoStringP strArg1(arg1Expr->ToString());
        FdoPtr<FdoExpression> arg2Expr(args->GetItem(1));
        FdoStringP strArg2(arg2Expr->ToString());

        mBuffer.append(sepLeftTerm);
        mBuffer.append(static_cast<char const*>(strArg1));
        mBuffer.append(" - Round(");
        mBuffer.append(static_cast<char const*>(strArg1));
        mBuffer.append(" / ");
        mBuffer.append(static_cast<char const*>(strArg2));
        mBuffer.append(") * ");
        mBuffer.append(static_cast<char const*>(strArg2));
        mBuffer.append(sepRightTerm);
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TODATE))
    {
        if(nbArgs == 1)
        {
          mBuffer.append("Cast");
          argCast = " as timestamp ";
        }
        else
        {
          mBuffer.append("to_date");
          argSep = sepComma;
        }
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOSTRING))
    {
        if(nbArgs == 1)
        {
          mBuffer.append("Cast");
          argCast = " as text ";
        }
        else
        {
          mBuffer.append("to_char");
          argSep = sepComma;
        }
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TODOUBLE))
    {
        mBuffer.append("Cast");
        argCast = " as float8 ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOFLOAT))
    {
        mBuffer.append("Cast");
        argCast = " as float4 ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOINT32))
    {
        mBuffer.append("Cast");
        argCast = " as int4 ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOINT64))
    {
        mBuffer.append("Cast");
        argCast = " as int8 ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_CURRENTDATE))
    {
        mBuffer.append("now");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_EXTRACTTODOUBLE))
    {
        mBuffer.append("Date_part");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_EXTRACTTOINT))
    {
        processArgs = false;
        mBuffer.append("Cast(Date_part(");
        FdoPtr<FdoExpression> dateExpr1(args->GetItem(0));
        dateExpr1->Process(this);
        mBuffer.append(",");
        FdoPtr<FdoExpression> dateExpr2(args->GetItem(1));
        dateExpr2->Process(this);
        mBuffer.append(") as int4)");
    }

    else if (0 == name.ICompare(FDO_FUNCTION_ADDMONTHS))
    {
        //AddMonth(date,3) = date + interval '3 month'
        processArgs = false;
        mBuffer.append(sepLeftTerm);
        FdoPtr<FdoExpression> dateExpr(args->GetItem(0));
        dateExpr->Process(this);
        mBuffer.append(" + ");
        FdoPtr<FdoExpression> numExpr(args->GetItem(1));
        FdoStringP numMonth(numExpr->ToString());
        mBuffer.append("interval '");
        mBuffer.append(static_cast<char const*>(numMonth));
        mBuffer.append(" month'");
        mBuffer.append(sepRightTerm);
    }
    else if (0 == name.ICompare(FDO_FUNCTION_MONTHSBETWEEN))
    {
        //MonthBetween(date1,date2) = date_part('Month',age(date1,date2)) + date_part('Year',age(date1,date2)) * 12
        processArgs = false;
        FdoPtr<FdoExpression> date1Expr(args->GetItem(0));
        FdoPtr<FdoExpression> date2Expr(args->GetItem(1));

        mBuffer.append("date_part('Month',age(");
        date1Expr->Process(this);
        mBuffer.append(sepComma);
        date2Expr->Process(this);
        mBuffer.append(sepRightTerm);
        mBuffer.append(sepRightTerm);

        mBuffer.append(" + ");

        mBuffer.append("date_part('Year',age(");
        date1Expr->Process(this);
        mBuffer.append(sepComma);
        date2Expr->Process(this);
        mBuffer.append(sepRightTerm);
        mBuffer.append(sepRightTerm);
        mBuffer.append(" * 12 ");
    }
    //Math function with 2 args that only accept numeric 
    else if ((nbArgs == 2) &&
             (0 == name.ICompare(FDO_FUNCTION_LOG) ||
              0 == name.ICompare(FDO_FUNCTION_MOD) ||
              0 == name.ICompare(FDO_FUNCTION_ROUND) ||
              0 == name.ICompare(FDO_FUNCTION_TRUNC)))
            
    {
        //Round(double,1) -> Round(cast(double as numeric) , 1)
        processArgs = false;
        mBuffer.append(static_cast<char const*>(name));
        mBuffer.append(sepLeftTerm);
        mBuffer.append("Cast(");
        FdoPtr<FdoExpression> numExpr1(args->GetItem(0));
        numExpr1->Process(this);
        mBuffer.append(" as numeric)");
        mBuffer.append(sepComma);
        FdoPtr<FdoExpression> numExpr2(args->GetItem(1));
        numExpr2->Process(this);
        mBuffer.append(sepRightTerm);
    }
    
    else
    {
        mBuffer.append(static_cast<char const*>(name));
        argSep = sepComma;
    }


    if(processArgs)
    {
      mBuffer.append(sepLeftTerm);

      for (FdoInt32 i = 0; i < args->GetCount(); i++)
      {
          mBuffer.append(sep);
          FdoPtr<FdoExpression> subExpr(args->GetItem(i));
          subExpr->Process(this);
          if(argCast != "")
          {
            mBuffer.append(argCast);
          }
          sep = argSep;
      }
      mBuffer.append(sepRightTerm);
    }

    FDOLOG_WRITE("Expression text: %s", mBuffer.c_str());
}

void ExpressionProcessor::ProcessIdentifier(FdoIdentifier& expr)
{
    FDOLOG_MARKER("ExpressionProcessor::+ProcessIdentifier");

    FdoStringP name(expr.GetName());

    if (name.Contains(L" "))
    {
        mBuffer.append("'");
        mBuffer.append(static_cast<char const*>(name));
        mBuffer.append("'");
    }
    else
    {
        mBuffer.append(static_cast<char const*>(name));
    }

    FDOLOG_WRITE("Identifier name: %s", static_cast<char const*>(name));
    FDOLOG_WRITE("Expression text: %s", mBuffer.c_str());
}

void ExpressionProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    FDOLOG_MARKER("ExpressionProcessor::+ProcessComputedIdentifier");

    FdoPtr<FdoExpression> exprPtr(expr.GetExpression());
    if (NULL == exprPtr)
    {
        FDOLOG_WRITE("ERROR: FdoComputedIdentifier is missing");
        throw FdoFilterException::Create(L"FdoComputedIdentifier is missing");
    }

    mBuffer.append(sepLeftTerm);
    exprPtr->Process(this);
    mBuffer.append(sepRightTerm);

    FDOLOG_WRITE("Expression text: %s", mBuffer.c_str());
}

void ExpressionProcessor::ProcessParameter(FdoParameter& expr)
{
    // TODO: How does parameters processing work?
    //       Are there any predefined FDO parameters or name part of "name:" 
    //       is a DBMS specific name, so for PostgreSQL we have:
    //       $1, $2, $3,...
    //       What about value?

    FDOLOG_WRITE("ERROR: PROCESSING PARAMETERS NOT IMPLEMENTED");
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
    if (expr.IsNull())
    {
        mBuffer.append("NULL");
    }
    else
    {
        std::string value;
        std::string format;

        FdoDateTime dt(expr.GetDateTime());
        
        if (dt.IsTime())
        {
            value = str(boost::format("'%d:%d:%d'") % (int)dt.hour % (int)dt.minute % (int)dt.seconds);
            format = "'HH24:MI:SS'";
        }
        else if (dt.IsDate())
        {
            value = str(boost::format("'%d-%d-%d'") % (int)dt.month % (int)dt.day % (int)dt.year);
            format = "'MM-DD-YYYY'";
        }
        else if (dt.IsDateTime())
        {
            value = str(boost::format("'%d-%d-%d %d:%d:%d'") % (int)dt.month % (int)dt.day % (int)dt.year % (int)dt.hour % (int)dt.minute % (int)dt.seconds);
            format = "'MM-DD-YYYY HH24:MI:SS'";
        }
        else
        {
            FDOLOG_WRITE("ERROR: Inconsistent specification of FdoDateTime value");
            assert(!"SHOULD NEVER GET HERE");
        }

        mBuffer.append("to_date");
        mBuffer.append(sepLeftTerm);
        mBuffer.append(value);
        mBuffer.append(sepComma);
        mBuffer.append(format);
        mBuffer.append(sepRightTerm);
    }
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
        FdoStringP value(expr.ToString());
        mBuffer.append(static_cast<char const*>(value));

        // TODO: Remove after new version is tested
        //FdoStringP value(expr.GetString());
        //mBuffer.append("'");
        //mBuffer.append(static_cast<char const*>(value));
        //mBuffer.append("'");
    }
}

void ExpressionProcessor::ProcessBLOBValue(FdoBLOBValue& expr)
{
    FDOLOG_WRITE("ERROR: BLOB value in expression is not supported");
    throw FdoFilterException::Create(L"BLOB value in expression is not supported");
}

void ExpressionProcessor::ProcessCLOBValue(FdoCLOBValue& expr)
{
    FDOLOG_WRITE("ERROR: CLOB value in expression is not supported");
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

        FDOLOG_WRITE(L"Geometry: %ls\n\t", fdoGeom->GetText());

        FdoPtr<FdoByteArray> wkbBytes(factory->GetWkb(fdoGeom));
        FdoByte* const bytes = wkbBytes->GetData();

        // Convert raw bytes to hex string
        std::string hexWkb;
        ewkb::ewkb_t wkb(bytes, bytes + wkbBytes->GetCount());
        ewkb::bytes_to_hex(wkb, hexWkb);

        // FIX: Chuck's fix proposed in Ticket #104
        //mBuffer.append(hexWkb);
        mBuffer.append("\'" + hexWkb + "\'");

        FDOLOG_WRITE("Geometry WKB HEX:\n\t%s", hexWkb.c_str());
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

///////////////////////////////////////////////////////////////////////////////
// Private opertaions
///////////////////////////////////////////////////////////////////////////////


}} // namespace fdo::postgis
