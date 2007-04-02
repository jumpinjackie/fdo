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

#include <cassert>

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
    mExprProc(new ExpressionProcessor())
{
}

FilterProcessor::~FilterProcessor()
{
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
    ValidateBinaryOperator(op);

    FdoPtr<FdoFilter> operandLeft(op.GetLeftOperand());
    FdoPtr<FdoFilter> operandRight(op.GetRightOperand());

    std::string binaryOp;
    if (FdoBinaryLogicalOperations_And == op.GetOperation())
        binaryOp = sql::opAnd;
    else
        binaryOp = sql::opOr;

    operandLeft->Process(this);
    std::string left;
    mExprProc->ReleaseExpressionText(left);

    operandRight->Process(this);
    std::string right;
    mExprProc->ReleaseExpressionText(right);

    mStatement.append(sql::sepLeftTerm);
    mStatement.append(left);
    mStatement.append(binaryOp);
    mStatement.append(right);
    mStatement.append(sql::sepRightTerm);
}

void FilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& op)
{
    FdoPtr<FdoFilter> operand(op.GetOperand());

    if (NULL == operand)
    {
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_MISSING_UNARY_OPERAND,
            "Missing operand in unary logical expression."));
    }

    if (FdoUnaryLogicalOperations_Not == op.GetOperation())
    {
        operand->Process(this);
        std::string operandText;
        mExprProc->ReleaseExpressionText(operandText);

        mStatement.append(sql::sepLeftTerm);
        mStatement.append(sql::opNot);
        mStatement.append(operandText);
        mStatement.append(sql::sepRightTerm);
    }
    else
    {
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_UNKNOWN_COMPARISON,
            "Unknown comparison operation."));
    }

}

void FilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& cond)
{
    FdoPtr<FdoExpression> expLeft(cond.GetLeftExpression());
    FdoPtr<FdoExpression> expRight(cond.GetRightExpression());

    if (NULL == expLeft)
    {
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_EXPRESSION_MISSING_LEFT,
            "Missing left expression of comparison condition."));
    }

    if (NULL == expRight)
    {
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_EXPRESSION_MISSING_RIGHT,
            "Missing right expression of comparison condition."));
    }

    std::string op;

    switch (cond.GetOperation())
    {
    case FdoComparisonOperations_EqualTo: 
        op = sql::opEqual;
        break;
    case FdoComparisonOperations_NotEqualTo: 
        op = sql::opNotEqual;
        break;
    case FdoComparisonOperations_GreaterThan: 
        op = sql::opGreaterThan;
        break;
    case FdoComparisonOperations_GreaterThanOrEqualTo: 
        op = sql::opGreaterThanEqual;
        break;
    case FdoComparisonOperations_LessThan: 
        op = sql::opLessThan;
        break;
    case FdoComparisonOperations_LessThanOrEqualTo: 
        op = sql::opLessThanEqual;
        break;
    case FdoComparisonOperations_Like: 
        op = sql::opLike;
        break;
    default:
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_UNKNOWN_COMPARISON,
            "Unknown comparison operation."));
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
    mStatement.append(op);
    mStatement.append(right);
    mStatement.append(sql::sepRightTerm);
}

void FilterProcessor::ProcessInCondition(FdoInCondition& cond)
{
}

void FilterProcessor::ProcessNullCondition(FdoNullCondition& cond)
{
    FdoPtr<FdoIdentifier> propId(cond.GetPropertyName());
    propId->Process(mExprProc);
    std::string tmp;
    mExprProc->ReleaseExpressionText(tmp);

    mStatement.append(sql::sepLeftTerm);
    mStatement.append(tmp);
    mStatement.append(sql::opIsNull);
    mStatement.append(sql::sepRightTerm);
}

void FilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::ProcessSpatialCondition");

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

    if (FdoSpatialOperations_EnvelopeIntersects == cond.GetOperation())
    { 
        // TODO: The value 32767 is a dummy SRID used for testing
        // It will be replaced with SRID of spatial context
        mStatement.append(sql::sepLeftTerm);
        mStatement.append(columnName + " && ");
        mStatement.append("GeomFromWKB(decode(\'" + geomHex + "\', \'hex\'), 32767)");
        mStatement.append(sql::sepRightTerm);
    }
    else
    {
        switch (cond.GetOperation())
        {
        case FdoSpatialOperations_EnvelopeIntersects:
            {
            }
            break;
        case FdoSpatialOperations_Contains:
            // TODO: Add BBOX intersection test with &&
            // The only problem is how to construct a geometry once,
            // with GeomFromWKB and avoid calling it twice?
            // It also applies to Within below

            spatialOp = "Within";
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
            assert("NOT YET IMPLEMENTED");
            break;
        case FdoSpatialOperations_Inside:
            // TODO: What is the semantic of this op?
            assert("NOT YET IMPLEMENTED");
            break;
        default:
            throw FdoFilterException::Create(L"Unsupported Spatial operation given in the filter");
        }

        mStatement.append(sql::sepLeftTerm);
        mStatement.append(spatialOp + "(" + columnName + ",");
        mStatement.append("GeomFromWKB(");
        mStatement.append("decode(\'" + geomHex + "\', \'hex\')");
        mStatement.append("), 32767)");
        mStatement.append(sql::sepRightTerm);
    }
}

void FilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& cond)
{
    FDOLOG_MARKER("FilterProcessor::ProcessDistanceCondition");
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
        FDOLOG_WRITE("Types conversion failed: %s", e.what());
        distTxt = "0"; 
    }

    // Prepare geometry literal value
    // TODO: Add SRID from spatial context support 
    std::string sqlGeometry("GeomFromWKB(decode(\'" + geomHex + "\', \'hex\'), 32767)");

    // Prepare distance-based predicate
    switch(cond.GetOperation())
    {
    case FdoDistanceOperations_Within:
        {
            mStatement.append(sql::sepLeftTerm);
            mStatement.append(columnName + " && Expand(" + sqlGeometry + "), " + distTxt);
            mStatement.append("AND distance(" + columnName + "," + sqlGeometry + ") < " + distTxt);
            mStatement.append(sql::sepRightTerm);
        }
        break;
    case FdoDistanceOperations_Beyond:
        {
            // TODO: Is behond distance semantic equal to "distance > N"
            //       IOW, opposite comparison to within distance?
        }
        break;
    default:
        throw FdoFilterException::Create(L"Unsupported Distance operation given in the filter");
    }
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
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_MISSING_LEFT_OPERAND,
            "Missing left operand in binary logical expression."));
    }

    if (NULL == right)
    {
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_MISSING_RIGHT_OPERAND,
            "Missing right operand in binary logical expression."));
    }

    if (FdoBinaryLogicalOperations_And != op.GetOperation()
        && FdoBinaryLogicalOperations_Or != op.GetOperation())
    {
        throw FdoFilterException::Create(
            NlsMsgGet(MSG_POSTGIS_FILTER_UNKNOWN_LOGICAL_OP,
            "Unknown logical operation."));
    }
}

void FilterProcessor::ValidateUnaryOperator(FdoUnaryLogicalOperator& op)
{
}

}} // namespace fdo::postgis
