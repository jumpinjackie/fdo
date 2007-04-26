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
#include "FilterProcessor.h"
#include "ExpressionProcessor.h"
// std
#include <cassert>
// boost
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

// Local namespace with definition of SQL operators
// used internally by the processor.
namespace sql 
{
char const* sepLeftTerm  = " ( ";
char const* sepRightTerm = " ) ";
char const* sepComma     = " , ";

// Logical operators
char const* opAnd     = " AND ";
char const* opOr      = " OR ";
char const* opNot     = " NOT ";

// Comparison operators
char const* opIn      = " IN ";
char const* opIsNull  = " IS NULL ";
char const* opEqual            = " = ";
char const* opNotEqual         = " != ";
char const* opGreaterThan      = " > ";
char const* opLessThan         = " < ";
char const* opGreaterThanEqual = " >= ";
char const* opLessThanEqual    = " <= ";

// Pattern matching
char const* opLike = " LIKE ";

} // namespace sql


FilterProcessor::FilterProcessor() :
    mExprProc(new ExpressionProcessor()), mSRID("-1")
{
}

FilterProcessor::FilterProcessor(FdoInt32 srid) :
    mExprProc(new ExpressionProcessor())
{
    FDOLOG_MARKER("FilterProcessor::FilterProcessor");
    try
    {
        FDOLOG_WRITE("SRID = %d", srid);

        mSRID = boost::lexical_cast<std::string>(srid);
    }
    catch (boost::bad_lexical_cast const& e)
    {
        // TODO: Re-throw as FDO exception

        FDOLOG_WRITE("ERROR: Convertion SRID to string failed: %s", e.what());
        mSRID = "-1"; // Indicate unknown SRID
    }
}

FilterProcessor::~FilterProcessor()
{
    // idle
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDelete interface
///////////////////////////////////////////////////////////////////////////////

void FilterProcessor::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIFilerProcessor interface
///////////////////////////////////////////////////////////////////////////////

void FilterProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& op)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessBinaryLogicalOperator");

    ValidateBinaryOperator(op);

    FdoPtr<FdoFilter> operandLeft(op.GetLeftOperand());
    FdoPtr<FdoFilter> operandRight(op.GetRightOperand());

    std::string binaryOp;
    if (FdoBinaryLogicalOperations_And == op.GetOperation())
        binaryOp = sql::opAnd;
    else
        binaryOp = sql::opOr;

    mStatement.append(sql::sepLeftTerm);
    operandLeft->Process(this);
    mStatement.append(binaryOp);
    operandRight->Process(this);
    mStatement.append(sql::sepRightTerm);

    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

void FilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& op)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessUnaryLogicalOperator");

    // Validates operation type and unary operand
    ValidateUnaryOperator(op);

    FdoPtr<FdoFilter> operand(op.GetOperand());

    std::string unaryOp;
    if (FdoUnaryLogicalOperations_Not == op.GetOperation())
    {
        FDOLOG_WRITE("Unary operation: NOT");

        unaryOp = sql::opNot;
    }

    mStatement.append(sql::sepLeftTerm);
    mStatement.append(unaryOp);
    operand->Process(this);
    mStatement.append(sql::sepRightTerm);

    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

void FilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessComparisonCondition");

    ValidateComparisonCondition(cond);

    FdoPtr<FdoExpression> expLeft(cond.GetLeftExpression());
    FdoPtr<FdoExpression> expRight(cond.GetRightExpression());

    std::string compOp;

    switch (cond.GetOperation())
    {
    case FdoComparisonOperations_EqualTo: 
        compOp = sql::opEqual;
        break;
    case FdoComparisonOperations_NotEqualTo: 
        compOp = sql::opNotEqual;
        break;
    case FdoComparisonOperations_GreaterThan: 
        compOp = sql::opGreaterThan;
        break;
    case FdoComparisonOperations_GreaterThanOrEqualTo: 
        compOp = sql::opGreaterThanEqual;
        break;
    case FdoComparisonOperations_LessThan: 
        compOp = sql::opLessThan;
        break;
    case FdoComparisonOperations_LessThanOrEqualTo: 
        compOp = sql::opLessThanEqual;
        break;
    case FdoComparisonOperations_Like: 
        compOp = sql::opLike;
        break;
    default:
        {
            FDOLOG_WRITE("ERROR: Unknown comparison operation");

            throw FdoFilterException::Create(
                NlsMsgGet(MSG_POSTGIS_FILTER_UNKNOWN_COMPARISON,
                "Unknown comparison operation."));
        }
    }

    expLeft->Process(mExprProc);
    std::string left;
    mExprProc->ReleaseExpressionText(left);

    expRight->Process(mExprProc);
    std::string right;
    mExprProc->ReleaseExpressionText(right);

    // Build text representation of the condition
    mStatement.append(sql::sepLeftTerm);
    mStatement.append(left);
    mStatement.append(compOp);
    mStatement.append(right);
    mStatement.append(sql::sepRightTerm);

    // Build text representation of the condition
    //mStatement.append(sql::sepLeftTerm);
    //expLeft->Process(mExprProc);
    //mStatement.append(compOp);
    //expRight->Process(mExprProc);
    //mStatement.append(sql::sepRightTerm);

    FDOLOG_WRITE("Filter:\n\tleft: %s\n\toperator: %s\n\tright: %s",
        left.c_str(), compOp.c_str(), right.c_str());
    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

void FilterProcessor::ProcessInCondition(FdoInCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessInCondition");

    FdoPtr<FdoIdentifier> propName(cond.GetPropertyName());
    if (NULL == propName)
    {
        FDOLOG_WRITE("ERROR: Property name in the FdoInCondition is missing");
        throw FdoFilterException::Create(L"Property name in the FdoInCondition is missing");
    }

    FdoPtr<FdoValueExpressionCollection> exprCol(cond.GetValues());

    if (NULL == exprCol || exprCol->GetCount() <= 0)
    {
        throw FdoFilterException::Create(L"FdoInCondition has an empty value list");
    }

    mStatement.append(sql::sepLeftTerm);
    propName->Process(mExprProc);
    mStatement.append(sql::opIn);
    mStatement.append(sql::sepLeftTerm);

    FdoPtr<FdoExpression> expr;
    FdoInt32 const count = exprCol->GetCount() - 1;
    FdoInt32 i = 0;
    for (i = 0; i < count; i++)
    {
        expr = exprCol->GetItem(i);
        expr->Process(mExprProc);
        mStatement.append(sql::sepComma);
    }
    expr = exprCol->GetItem(i);
    expr->Process(mExprProc);

    mStatement.append(sql::sepRightTerm);
    mStatement.append(sql::sepRightTerm);

    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

void FilterProcessor::ProcessNullCondition(FdoNullCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessNullCondition");

    FdoPtr<FdoIdentifier> propId(cond.GetPropertyName());

    mStatement.append(sql::sepLeftTerm);
    propId->Process(mExprProc);
    mStatement.append(sql::opIsNull);
    mStatement.append(sql::sepRightTerm);

    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

void FilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessSpatialCondition");

    // TODO: Fill with spatial SQL composition
    FdoPtr<FdoExpression> geomExpr(cond.GetGeometry());
    FdoPtr<FdoIdentifier> geomProp(cond.GetPropertyName());

    // Right-hand operand - name of geometry column
    geomProp->Process(mExprProc);
    std::string columnName;
    mExprProc->ReleaseExpressionText(columnName);

    // Left-hand operand - geometry being tested
    geomExpr->Process(mExprProc);
    std::string geomHex;
    mExprProc->ReleaseExpressionText(geomHex);

    // Spatial predicate
    std::string spatialOp;

    FDOLOG_WRITE("Spatial operation id: %d", cond.GetOperation());

    if (FdoSpatialOperations_EnvelopeIntersects == cond.GetOperation())
    { 
        FDOLOG_WRITE("Spatial operation: EnvelopeIntersects");

        mStatement.append(sql::sepLeftTerm);
        mStatement.append(columnName + " && ");
        mStatement.append("GeomFromWKB(decode(\'" + geomHex + "\', \'hex\')," + mSRID + ")");
        mStatement.append(sql::sepRightTerm);
    }
    else
    {
        switch (cond.GetOperation())
        {
        case FdoSpatialOperations_Contains:
            // TODO: Add BBOX intersection test with &&
            // The only problem is how to construct a geometry once,
            // with GeomFromWKB and avoid calling it twice?
            // It also applies to Within below

            spatialOp = "Contains";
            break;
        case FdoSpatialOperations_Crosses:
            spatialOp = "Crosses";
            break;
        case FdoSpatialOperations_Disjoint:
            spatialOp = "Disjoint";
            break;
        case FdoSpatialOperations_Equals:
            spatialOp = "Equals";
            break;
        case FdoSpatialOperations_Intersects:
            spatialOp = "Intersects";
            break;
        case FdoSpatialOperations_Overlaps:
            spatialOp = "Overlaps";
            break;
        case FdoSpatialOperations_Touches:
            spatialOp = "Touches";
            break;
        case FdoSpatialOperations_Within:
            // TODO: Add BBOX intersection test with &&
            spatialOp = "Within";
            break;
        case FdoSpatialOperations_CoveredBy:
            // TODO: What is the semantic of this op?
            //assert("NOT YET IMPLEMENTED");
            FDOLOG_WRITE("CoveredBy - NOT YET IMPLEMENTED");
            break;
        case FdoSpatialOperations_Inside:
            // TODO: What is the semantic of this op?
            //assert("NOT YET IMPLEMENTED");
            FDOLOG_WRITE("Inside - NOT YET IMPLEMENTED");
            break;
        default:
            {
                FDOLOG_WRITE("ERROR: Unsupported Spatial operation given in the filter");
                throw FdoFilterException::Create(L"Unsupported Spatial operation given in the filter");
            }
        }

        mStatement.append(sql::sepLeftTerm);
        mStatement.append(spatialOp + "(" + columnName + ",");
        mStatement.append("GeomFromWKB(");
        mStatement.append("decode(\'" + geomHex + "\', \'hex\')");
        mStatement.append("," + mSRID + ")");
        mStatement.append(")");
        mStatement.append(sql::sepRightTerm);
    }

    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

void FilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::+ProcessDistanceCondition");

    // TODO: Fill with spatial SQL composition
    FdoPtr<FdoExpression> geomExpr(cond.GetGeometry());
    FdoPtr<FdoIdentifier> geomProp(cond.GetPropertyName());

    // Right-hand operand - name of geometry column
    geomProp->Process(mExprProc);
    std::string columnName;
    mExprProc->ReleaseExpressionText(columnName);

    // Left-hand operand - geometry being tested
    geomExpr->Process(mExprProc);
    std::string geomHex;
    mExprProc->ReleaseExpressionText(geomHex);

    // Prepare distance in textual form
    FdoDouble distance = cond.GetDistance();
    std::string distTxt;
    try
    {
        distTxt = boost::lexical_cast<std::string>(distance);
    }
    catch (boost::bad_lexical_cast& e)
    {   
        FDOLOG_WRITE("ERROR: Types conversion failed: %s", e.what());
        distTxt = "0"; 
    }

    // Prepare geometry literal value
    std::string sqlGeometry("GeomFromWKB(decode(\'" + geomHex + "\', \'hex\'), " + mSRID +")");

    // Prepare distance-based predicate
    switch(cond.GetOperation())
    {
    case FdoDistanceOperations_Within:
        {
            mStatement.append(sql::sepLeftTerm);
            mStatement.append(columnName + " && Expand(" + sqlGeometry + ", " + distTxt + ") ");
            mStatement.append(sql::opAnd);
            mStatement.append("distance(" + columnName + "," + sqlGeometry + ")");
            mStatement.append(sql::opLessThanEqual);
            mStatement.append(distTxt);
            mStatement.append(sql::sepRightTerm);
        }
        break;
    case FdoDistanceOperations_Beyond:
        {
            // TODO: Is behond distance semantic equal to "distance > N"
            //       IOW, opposite comparison to within distance?

            FDOLOG_WRITE("ERROR: NOT IMPLEMENTED");
        }
        break;
    default:
        {
            FDOLOG_WRITE("ERROR: Unsupported Distance operation given in the filter");
            throw FdoFilterException::Create(L"Unsupported Distance operation given in the filter");
        }
    }

    FDOLOG_WRITE("Filter statement:\n\t%s", mStatement.c_str());
}

///////////////////////////////////////////////////////////////////////////////
// FilterProcessor interface
///////////////////////////////////////////////////////////////////////////////

std::string const& FilterProcessor::GetFilterStatement() const
{
    FDOLOG_MARKER("FilterProcessor::+GetFilterStatement");

    FDOLOG_WRITE("Filter:\n\t%s", mStatement.c_str());

    return mStatement;
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void FilterProcessor::ValidateBinaryOperator(FdoBinaryLogicalOperator& op)
{
    FdoPtr<FdoFilter> left(op.GetLeftOperand());
    FdoPtr<FdoFilter> right(op.GetRightOperand());

    if (NULL == left)
    {
        FDOLOG_WRITE("ERROR: Missing left operand in binary logical expression");

        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_MISSING_LEFT_OPERAND,
            "Missing left operand in binary logical expression."));
    }

    if (NULL == right)
    {
        FDOLOG_WRITE("ERROR: Missing right operand in binary logical expression");

        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_MISSING_RIGHT_OPERAND,
            "Missing right operand in binary logical expression."));
    }

    if (FdoBinaryLogicalOperations_And != op.GetOperation()
        && FdoBinaryLogicalOperations_Or != op.GetOperation())
    {
        FDOLOG_WRITE("ERROR: Unknown binary logical operation");
        
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_UNKNOWN_BINARY_PREDICATE,
            "Unknown binary logical operation."));
    }
}

void FilterProcessor::ValidateUnaryOperator(FdoUnaryLogicalOperator& op)
{
    FdoPtr<FdoFilter> operand(op.GetOperand());

    if (NULL == operand)
    {
        FDOLOG_WRITE("ERROR: Missing operand in unary logical expression");

        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_MISSING_UNARY_OPERAND,
            "Missing operand in unary logical expression."));
    }

    if (FdoUnaryLogicalOperations_Not != op.GetOperation())
    {
        FDOLOG_WRITE("ERROR: Unknown unary operation");

        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_UNKNOWN_UNARY_PREDICATE,
            "Unknown unary operation."));
    }
}

void FilterProcessor::ValidateComparisonCondition(FdoComparisonCondition& cond)
{
    FdoPtr<FdoExpression> expLeft(cond.GetLeftExpression());
    FdoPtr<FdoExpression> expRight(cond.GetRightExpression());

    if (NULL == expLeft)
    {
        FDOLOG_WRITE("ERROR: Missing left expression of comparison condition");

        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_EXPRESSION_MISSING_LEFT,
            "Missing left expression of comparison condition."));
    }

    if (NULL == expRight)
    {
        FDOLOG_WRITE("ERROR: Missing right expression of comparison condition");

        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_EXPRESSION_MISSING_RIGHT,
            "Missing right expression of comparison condition."));
    }
}

}} // namespace fdo::postgis
