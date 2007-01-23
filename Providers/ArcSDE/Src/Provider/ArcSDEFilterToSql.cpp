/*
 * 
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
#include "ArcSDEUtils.h"


ArcSDEFilterToSql::ArcSDEFilterToSql (ArcSDEConnection *conn, FdoClassDefinition* definition)
{
    mSql = new wchar_t[8];
    wcscpy (mSql, WHERE);
    m_Connection = FDO_SAFE_ADDREF(conn);
    mDefinition = FDO_SAFE_ADDREF(definition);
    mSpatialFilters.clear();
}

ArcSDEFilterToSql::~ArcSDEFilterToSql (void)
{
    delete[] mSql;
}

void ArcSDEFilterToSql::Dispose ()
{
    delete this;
}

void ArcSDEFilterToSql::AppendString (const wchar_t* value)
{
    wchar_t* next;

    next = new wchar_t[(int)wcslen (mSql) + (int)wcslen (value) + 1];
    wcscpy (next, mSql);
    wcscat (next, value);
    delete[] mSql;
    mSql = next;
}

wchar_t* ArcSDEFilterToSql::GetSql ()
{
    return (mSql);
}

//
// FdoIExpressionProcessor
//

/// <summary>Processes the FdoBinaryExpression passed in as an argument.</summary>
/// <param name="expr">Input binary expression</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessBinaryExpression (FdoBinaryExpression& expr)
{ 
    AppendString (OPEN_PAREN);
    HandleExpr (FdoPtr<FdoExpression>(expr.GetLeftExpression ()));
    AppendString (CLOSE_PAREN);
    switch (expr.GetOperation ())
    {
        case FdoBinaryOperations_Add:
            AppendString (ARITHMETIC_PLUS);
            break;
        case FdoBinaryOperations_Divide:
            AppendString (ARITHMETIC_DIV);
            break;
        case FdoBinaryOperations_Multiply:
            AppendString (ARITHMETIC_MULT);
            break;
        case FdoBinaryOperations_Subtract:
            AppendString (ARITHMETIC_MINUS);
            break;
        default:
            throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_BINARY_EXPRESSION, "The given binary expression is not supported."));
    }
    AppendString (OPEN_PAREN);
    HandleExpr (FdoPtr<FdoExpression>(expr.GetRightExpression ()));
    AppendString (CLOSE_PAREN);
}

///<summary>Processes the FdoUnaryExpression passed in as an argument.</summary>
/// <param name="expr">Input unary expression</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessUnaryExpression (FdoUnaryExpression& expr)
{
    if (FdoUnaryOperations_Negate == expr.GetOperation ())
    {
        AppendString (OPEN_PAREN);
        AppendString (UNARY_MINUS);
        AppendString (OPEN_PAREN);
        HandleExpr (FdoPtr<FdoExpression> (expr.GetExpression ()));
        AppendString (CLOSE_PAREN);
        AppendString (CLOSE_PAREN);
    }
    else
        throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_UNARY_EXPRESSION, "The given unary expression is not supported."));
}

/// <summary>Processes the FdoFunction passed in as an argument.</summary>
/// <param name="expr">Input function</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessFunction (FdoFunction& expr)
{
    AppendString (expr.GetName ());
    AppendString (OPEN_PAREN);
    expr.Process (this);
    AppendString (CLOSE_PAREN);
}

/// <summary>Processes the FdoIdentifier passed in as an argument.</summary>
/// <param name="expr">Input indentifier</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessIdentifier (FdoIdentifier& expr)
{
    // Validate scope not being used (not supported yet):
    FdoInt32 length;
    expr.GetScope (length);
    if (0 != length)
        throw FdoFilterException::Create (NlsMsgGet1(ARCSDE_IDENTIFIER_SCOPES_UNSUPPORTED, "FDO Identifier scopes are not supported; detected scopes in identifier '%1$ls'.", expr.ToString()));

    FdoString* schema = expr.GetSchemaName ();
    if (0 != wcslen (schema))
    {
        AppendString (schema);
        AppendString (L".");
    }

    AppendString (expr.GetName ());
}

/// <summary>Processes the FdoParameter passed in as an argument.</summary>
/// <param name="expr">Input parameter</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessParameter (FdoParameter& expr)
{
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_FILTER_PARAMETERS_NOT_SUPPORTED, "Filter parameters are not supported."));
}

/// <summary>Processes the FdoBooleanValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessBooleanValue (FdoBooleanValue& expr)
{
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_BOOLEAN, "The Boolean data type is not supported by ArcSDE."));
}

/// <summary>Processes the FdoByteValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessByteValue (FdoByteValue& expr)
{
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_BYTE, "The Byte data type is not supported by ArcSDE."));
}

/// <summary>Processes the FdoDateTimeValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessDateTimeValue (FdoDateTimeValue& expr)
{
    FdoDateTime dateTime = expr.GetDateTime();
    wchar_t wcsBuffer[50];

    switch(m_Connection->RdbmsId())
    {
        case SE_DBMS_IS_ORACLE:

            // Start the Oracle datetime literal:
            AppendString(ORACLE_TODATE);
            AppendString(OPEN_PAREN);

            // Append value string:
            AppendString(QUOTE);
            if (dateTime.IsDate())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), ORACLE_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), ORACLE_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsDateTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), ORACLE_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);

                AppendString(SPACE);

                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), ORACLE_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds);
                AppendString(wcsBuffer);
            }
            AppendString(QUOTE);

            // Append separator between format and value:
            AppendString(COMMA);

            // Append format string:
            AppendString(QUOTE);
            if (dateTime.IsDate())
            {
                AppendString(ORACLE_DATEFORMAT);
            }
            else if (dateTime.IsTime())
            {
                AppendString(ORACLE_TIMEFORMAT);
            }
            else if (dateTime.IsDateTime())
            {
                AppendString(ORACLE_DATEFORMAT);
                AppendString(ORACLE_TIMEFORMAT);
            }
            AppendString(QUOTE);

            // Append separator between format and nls language:
            AppendString(COMMA);

            // Append nls language:
            AppendString(QUOTE);
            AppendString(ORACLE_NLS_AMERICAN);
            AppendString(QUOTE);

            // End the Oracle datetime literal:
            AppendString(CLOSE_PAREN);
        break;


        case SE_DBMS_IS_INFORMIX:
        case SE_DBMS_IS_IUS:
            // Append value string:
            AppendString(QUOTE);
            if (dateTime.IsDate())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), INFORMIX_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), INFORMIX_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsDateTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), INFORMIX_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);

                AppendString(SPACE);

                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), INFORMIX_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds);
                AppendString(wcsBuffer);
            }
            AppendString(QUOTE);

            // Append suffix indicating datetime 'range':
            AppendString(SPACE);
            if (dateTime.IsDate())
                AppendString(INFORMIX_SUFFIX_DATE);
            else if (dateTime.IsTime())
                AppendString(INFORMIX_SUFFIX_TIME);
            else if (dateTime.IsDateTime())
                AppendString(INFORMIX_SUFFIX_DATETIME);
        break;


        case SE_DBMS_IS_SQLSERVER:
        {
            // Calculate seconds and milliseconds;
            // We use floor instead of round to avoid having a huge amount of logic to figure out how to roll over
            //   minutes, hours, days, and years  (and take care of leap years, etc).
            int iMilliSeconds = (int)floor(1000.0*(dateTime.seconds - floor(dateTime.seconds)));

            // Append value string:
            AppendString(QUOTE);
            if (dateTime.IsDate())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), SQLSERVER_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), SQLSERVER_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds, iMilliSeconds);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsDateTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), SQLSERVER_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);

                AppendString(SPACE);

                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), SQLSERVER_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds, iMilliSeconds);
                AppendString(wcsBuffer);
            }
            AppendString(QUOTE);
        }
        break;


        case SE_DBMS_IS_DB2:
        case SE_DBMS_IS_DB2_EXT:
            // Append prefix string:
            if (dateTime.IsDate())
                AppendString(DB2_PREFIX_DATE);
            else if (dateTime.IsTime())
                AppendString(DB2_PREFIX_TIME);
            else if (dateTime.IsDateTime())
                AppendString(DB2_PREFIX_DATETIME);
            AppendString(SPACE);

            // Append value string:
            AppendString(QUOTE);
            if (dateTime.IsDate())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), DB2_DATEFORMAT_PRINTF, dateTime.year, dateTime.month, dateTime.day);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), DB2_TIMEFORMAT_PRINTF, dateTime.hour, dateTime.minute, (double)dateTime.seconds);
                AppendString(wcsBuffer);
            }
            else if (dateTime.IsDateTime())
            {
                FdoCommonOSUtil::swprintf(wcsBuffer, ELEMENTS(wcsBuffer), DB2_DATETIMEFORMAT_PRINTF,
                    dateTime.year, dateTime.month, dateTime.day,
                    dateTime.hour, dateTime.minute, (double)dateTime.seconds);
                AppendString(wcsBuffer);
            }
            AppendString(QUOTE);
        break;

        default:
            throw FdoFilterException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));
        break;
    }

}

/// <summary>Processes the FdoDecimalValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessDecimalValue (FdoDecimalValue& expr)
{
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_DECIMAL, "The Decimal data type is not supported by ArcSDE."));
}

/// <summary>Processes the FdoDoubleValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessDoubleValue (FdoDoubleValue& expr)
{
    double value;
    char text[100];
    wchar_t* buffer;
    
    value = expr.GetDouble ();

    FdoCommonOSUtil::gcvt (value, 14, text); /*todo: proper conversion with wide char support*/
    multibyte_to_wide (buffer, text);
    AppendString (buffer);
}

/// <summary>Processes the FdoInt16Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessInt16Value (FdoInt16Value& expr)
{
    wchar_t buffer[100];

    FdoCommonOSUtil::swprintf (buffer, ELEMENTS(buffer), L"%hd", expr.GetInt16());
    AppendString (buffer);
}

/// <summary>Processes the FdoInt32Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessInt32Value (FdoInt32Value& expr)
{
    wchar_t buffer[100];

    FdoCommonOSUtil::swprintf (buffer, ELEMENTS(buffer) / sizeof(wchar_t), L"%d", expr.GetInt32 ());
    AppendString (buffer);
}

/// <summary>Processes the FdoInt64Value passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessInt64Value (FdoInt64Value& expr)
{
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_INT64, "The Int64 data type is not supported by ArcSDE."));
}

/// <summary>Processes the FdoSingleValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessSingleValue (FdoSingleValue& expr)
{
    double value;
    char text[100];
    wchar_t* buffer;
    
    value = (double)expr.GetSingle ();

    FdoCommonOSUtil::gcvt (value, 7, text); /*todo: proper conversion with wide char support*/
    multibyte_to_wide (buffer, text);
    AppendString (buffer);
}

/// <summary>Processes the FdoStringValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessStringValue (FdoStringValue& expr)
{
    // ToString() encloses it in quotes and escapes single quotes within it
    AppendString (expr.ToString ());
}

/// <summary>Processes the FdoBLOBValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessBLOBValue (FdoBLOBValue& expr)
{
    // None of the RDBMS's supported by ArcSDE 8.3 (or 9.0) support literal blobs in where clauses.
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_FILTER_BLOBS_UNSUPPORTED, "Literal BLOB expressions are not supported in filters."));
}

/// <summary>Processes the FdoCLOBValue passed in as an argument.</summary>
/// <param name="expr">Input data value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessCLOBValue (FdoCLOBValue& expr)
{
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_CLOB, "The CLOB data type is not supported by ArcSDE."));
}

/// <summary>Processes the FdoGeometryValue passed in as an argument.</summary>
/// <param name="expr">Input geometry value</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessGeometryValue (FdoGeometryValue& expr)
{
    // Most of the RDBMS's supported by ArcSDE 8.3 (or 9.0) do support literal geometries in their where clauses,
    // but only in spatial operators or spatial functions.  Since spatial operators are handled by
    // ProcessSpatialCondition/ProcessSpatialDistance, and R1 won't support functions, throw an exception here.
    throw FdoFilterException::Create (NlsMsgGet(ARCSDE_FILTER_GEOMETRIES_UNSUPPORTED, "Literal geometry expressions are not supported in filters."));
}

//
// FdoFilterProcessor
//

///<summary>Processes the FdoBinaryLogicalOperator passed in as an argument.</summary>
/// <param name="filter">Input the FdoBinaryLogicalOperator</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter)
{
    // NOTE: spatial filters and attribute filters can only be combined in certain ways
    //       that ArcSDE understands.  This method handles these special combinations.
    //       Unsupported combinations will cause exceptions during prior calls to HandleFilter().

    ArcSDEFilterTypeEnum leftOpType = GetFilterType(FdoPtr<FdoFilter>(filter.GetLeftOperand ()));
    ArcSDEFilterTypeEnum rightOpType = GetFilterType(FdoPtr<FdoFilter>(filter.GetRightOperand ()));
    if ((leftOpType == ArcSDEFilterType_Spatial) && (rightOpType == ArcSDEFilterType_Attribute))
    {
        // Process attribute portion (append to query string):
        // NOTE: This will always be an AND binary operation; an OR operation would have failed prior to getting here.
        AppendString (OPEN_PAREN);
        HandleFilter (FdoPtr<FdoFilter>(filter.GetRightOperand ()));
        AppendString (CLOSE_PAREN);

        // Process spatial filter portion (it won't be appended to query string):
        HandleFilter (FdoPtr<FdoFilter>(filter.GetLeftOperand ()));
    }
    else if ((leftOpType == ArcSDEFilterType_Attribute) && (rightOpType == ArcSDEFilterType_Spatial))
    {
        // Process attribute portion (append to query string):
        // NOTE: This will always be an AND binary operation; an OR operation would have failed prior to getting here.
        AppendString (OPEN_PAREN);
        HandleFilter (FdoPtr<FdoFilter>(filter.GetLeftOperand ()));
        AppendString (CLOSE_PAREN);

        // Process spatial filter portion (it won't be appended to query string):
        HandleFilter (FdoPtr<FdoFilter>(filter.GetRightOperand ()));
    }
    else if ((leftOpType == ArcSDEFilterType_Attribute) && (rightOpType == ArcSDEFilterType_Attribute))
    {
        // Both operands are "attribute"-style filters, process them both (append both to query string):

        AppendString (OPEN_PAREN);
        HandleFilter (FdoPtr<FdoFilter>(filter.GetLeftOperand ()));
        AppendString (CLOSE_PAREN);
        switch (filter.GetOperation ())
        {
            case FdoBinaryLogicalOperations_And:
                AppendString (LOGICAL_AND);
                break;
            case FdoBinaryLogicalOperations_Or:
                AppendString (LOGICAL_OR);
                break;
            default:
                throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_BINARY_LOGICAL_OPERATOR, "The given binary logical operator is not supported."));
        }
        AppendString (OPEN_PAREN);
        HandleFilter (FdoPtr<FdoFilter>(filter.GetRightOperand ()));
        AppendString (CLOSE_PAREN);
    }
    else if ((leftOpType == ArcSDEFilterType_Spatial) && (rightOpType == ArcSDEFilterType_Spatial))
    {
        // Process both spatial filter portions (it won't be appended to query string):
        // NOTE: This will always be an AND binary operation; an OR operation would have failed prior to getting here.
        HandleFilter (FdoPtr<FdoFilter>(filter.GetLeftOperand ()));
        HandleFilter (FdoPtr<FdoFilter>(filter.GetRightOperand ()));
    }
}

///<summary>Processes the FdoUnaryLogicalOperator passed in as an argument.</summary>
/// <param name="filter">Input the FdoUnaryLogicalOperator</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter)
{
    switch (filter.GetOperation ())
    {
        case FdoUnaryLogicalOperations_Not:
            AppendString (LOGICAL_NOT);
            break;
        default:
            throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_UNARY_LOGICAL_OPERATOR, "The given unary logical operator is not supported."));
    }
    AppendString (OPEN_PAREN);
    HandleFilter (FdoPtr<FdoFilter>(filter.GetOperand ()));
    AppendString (CLOSE_PAREN);
}

///<summary>Processes the FdoComparisonCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoComparisonCondition</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessComparisonCondition (FdoComparisonCondition& filter)
{
    AppendString (OPEN_PAREN);
    HandleExpr (FdoPtr<FdoExpression>(filter.GetLeftExpression ()));
    AppendString (CLOSE_PAREN);
    switch (filter.GetOperation ())
    {
        case FdoComparisonOperations_EqualTo:
            AppendString (EQUAL_OP);
            break;
        case FdoComparisonOperations_GreaterThan:
            AppendString (GREATER_THAN_OP);
            break;
        case FdoComparisonOperations_GreaterThanOrEqualTo:
            AppendString (GREATER_OR_EQUAL_OP);
            break;
        case FdoComparisonOperations_LessThan:
            AppendString (LESS_THAN_OP);
            break;
        case FdoComparisonOperations_LessThanOrEqualTo:
            AppendString (LESS_OR_EQUAL_OP);
            break;
        case FdoComparisonOperations_Like:
            AppendString (LIKE_OP);
            break;
        case FdoComparisonOperations_NotEqualTo:
            AppendString (NOT_EQUAL_OP);
            break;
        default:
            throw FdoFilterException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_COMPARISON_CONDITION, "The given comparison condition is not supported."));
    }
    AppendString (OPEN_PAREN);
    HandleExpr (FdoPtr<FdoExpression>(filter.GetRightExpression ()));
    AppendString (CLOSE_PAREN);

    // Need to add more to make LIKE work properly on some RDBMS (Oracle and SQL Server namely),
    // with respect to escape characters:
    if (filter.GetOperation()==FdoComparisonOperations_Like)
    {
        // NOTE: only Oracle and SQL Server need special 'ESCAPE' clause to specify escape character
        if ((m_Connection->RdbmsId() == SE_DBMS_IS_ORACLE) || (m_Connection->RdbmsId() == SE_DBMS_IS_SQLSERVER))
            AppendString (ESCAPE);
    }
}

///<summary>Processes the FdoInCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoInCondition</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessInCondition (FdoInCondition& filter)
{
    // NOTE: we can't simply use ToString operator, even though FDO's In condition grammar syntax is
    //       identical to pretty much all RDBMS's.  This is because the in condition may or may not contain
    //       other expressions where FDO's grammar is NOT the same as underlying RDBMS's.

    ProcessIdentifier (*(FdoPtr<FdoIdentifier>(filter.GetPropertyName ())));
    AppendString (IN_OP);
    AppendString (OPEN_PAREN);

    FdoPtr<FdoValueExpressionCollection> values = filter.GetValues();
    for (FdoInt32 i = 0; i < values->GetCount(); i++)
    {
        if (i > 0)
            AppendString (IN_SEPARATOR);

        FdoPtr<FdoValueExpression> value = values->GetItem(i);
        HandleExpr(value);
    }

    AppendString (CLOSE_PAREN);
}

///<summary>Processes the FdoNullCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoNullCondition</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessNullCondition (FdoNullCondition& filter)
{
    ProcessIdentifier (*(FdoPtr<FdoIdentifier>(filter.GetPropertyName ())));
    AppendString (IS_NULL_OP);
}

///<summary>Processes the FdoSpatialCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoSpatialCondition</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessSpatialCondition (FdoSpatialCondition& fdoFilter)
{
    FdoPtr<FdoIdentifier> fdoIdentifier = fdoFilter.GetPropertyName();
    FdoPtr<FdoExpression> fdoGeometryExpr = fdoFilter.GetGeometry();
    ProcessGeometricCondition(fdoIdentifier, fdoGeometryExpr, fdoFilter.GetOperation());
}

///<summary>Processes the FdoDistanceCondition passed in as an argument.</summary>
/// <param name="filter">Input the FdoDistanceCondition</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEFilterToSql::ProcessDistanceCondition (FdoDistanceCondition& fdoFilter)
{
    // Get fdo spatial operator from fdo distance operator:
    FdoSpatialOperations fdoSpatialOperator = (fdoFilter.GetOperation() == FdoDistanceOperations_Within) ?
        FdoSpatialOperations_Intersects : FdoSpatialOperations_Disjoint;

    FdoPtr<FdoIdentifier> fdoIdentifier = fdoFilter.GetPropertyName();
    FdoPtr<FdoExpression> fdoGeometryExpr = fdoFilter.GetGeometry();
    ProcessGeometricCondition(fdoIdentifier, fdoGeometryExpr, fdoSpatialOperator, fdoFilter.GetDistance());
}


void ArcSDEFilterToSql::ProcessGeometricCondition (FdoIdentifier* fdoPropertyName, FdoExpression *fdoGeometryExpr, FdoSpatialOperations fdoSpatialOperation, double bBufferDistance)
{
    LONG lResult = 0;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    CHAR column[SE_QUALIFIED_COLUMN_LEN];

    // IMPORTANT: ArcSDE can't process complex spatial queries like "where height>5 *OR* intersects <geometry>";
    //            ARcSDE treats spatial queries as always "and-ed" to the attribute portion of the query.
    //            Queries that don't conform to this format are verified in HandleFilter() by calling GetFilterType().


    // Validate the FDO property in this filter is an actual geometric property:
    FdoPtr<FdoPropertyDefinitionCollection> fdoProperties = mDefinition->GetProperties();
    FdoPtr<FdoPropertyDefinition> fdoProperty = fdoProperties->GetItem(fdoPropertyName->GetName());
    if (FdoPropertyType_GeometricProperty != fdoProperty->GetPropertyType())
        throw FdoFilterException::Create(NlsMsgGet1(ARCSDE_SPATIAL_QUERY_ON_NONGEOMETRIC_PROPERTY, "Cannot perform a spatial query on non-geometric property '%1$ls'.", fdoPropertyName->GetName()));

    // Validate that the given geometry expression to compare with is a literal geometry expression;
    // ArcSDE cannot handle column-to-column comparisons.
    if (NULL == dynamic_cast<FdoGeometryValue*>(fdoGeometryExpr))
        throw FdoFilterException::Create(NlsMsgGet(ARCSDE_SPATIAL_QUERY_ON_NONLITERAL_GEOMETRY, "ArcSDE cannot perform a spatial query against a non-literal geometry expression."));

    // Get sde column name from spatial filter:
    FdoPtr<FdoIdentifier> fdoIdentifier = FDO_SAFE_ADDREF(fdoPropertyName);
    m_Connection->PropertyToColumn (column, mDefinition, fdoIdentifier);

    // Get sde table name:
    m_Connection->ClassToTable(table, mDefinition);

    // Get the sde coordref for the column the spatial filter applies to:
    SE_COORDREF coordRef;
    lResult = GetCoordRefFromColumn(m_Connection, table, column, coordRef);
    handle_sde_err<FdoCommandException>(m_Connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

    // Get sde shape from spatial filter:
    SE_SHAPE shape;
    SE_SHAPE shapeWithBuffer;
    FdoPtr<FdoGeometryValue> spatialGeometry = (FdoGeometryValue*)FDO_SAFE_ADDREF(fdoGeometryExpr);
    FdoPtr<FdoByteArray> fgf = spatialGeometry->GetGeometry();

    // Convert, cropping to the coordinate reference's extents:
    convert_fgf_to_sde_shape(m_Connection, fgf, coordRef, shape, true);
    fgf = NULL;
    handle_sde_err<FdoCommandException>(m_Connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

    // get query shape type (NULL shapes require special handling):
    LONG shapeType = -1;
    lResult = SE_shape_get_type(shape, &shapeType);
    handle_sde_err<FdoCommandException>(m_Connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

    // add buffer distance, if any:
    if (bBufferDistance < 0.0)
        throw FdoFilterException::Create(NlsMsgGet(ARCSDE_SPATIAL_DISTANCE_BUFER_NEGATIVE, "Cannot perform a spatial query with a negative distance filter."));
    else if (bBufferDistance > 0.0)
    {
        // Only proceed if shape is not null:
        if (SG_NIL_SHAPE != shapeType)
        {
            // NOTE: ArcSDE's buffer-generation API requires we pass a "maximum number of points" to the buffer-generation
            //       function.  If the number is too low, the error SE_INVALID_NUM_OF_PTS is returned.
            //       If we put in something like LONG_MAX, we get the error SE_OUT_OF_CLMEM (out of client memory)
            //       since it's trying to allocate the point array to the size we specify.  To work around this strange
            //       behavior, we start by allocating double the original number of points, then keep doubling until we
            //       get success (or out of memory!).

            LONG lNumPoints = 0L;
            SE_shape_get_num_points(shape,0,0,&lNumPoints);
            lNumPoints *= 10; // Initial attempt will be 10 times the number of coordinates, which seems reasonable from experimenting
            SE_shape_create(coordRef, &shapeWithBuffer);

            do
            {
                // Attempt to buffer geometry with given number of points:
                lResult = SE_shape_generate_buffer(shape, bBufferDistance, lNumPoints, shapeWithBuffer);
                if (lResult != SE_INVALID_NUM_OF_PTS)
                    handle_sde_err<FdoCommandException>(m_Connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

                // double number of points for storage of buffered geometry:
                lNumPoints *= 2;
            }
            while (lResult == SE_INVALID_NUM_OF_PTS);

            // Free original shape, as it is no longer needed:
            SE_shape_free(shape);

            // Debug:
            LONG lNumPointsBuffered = 0L;
            SE_shape_get_num_points(shapeWithBuffer,0,0,&lNumPointsBuffered);
            lNumPointsBuffered = lNumPointsBuffered;
        }
    }

    SE_coordref_free (coordRef);

    // Add one or more spatial filters that represent this spatial operation:
    AddSpatialFilters(m_Connection, fdoSpatialOperation, column, ((0.0 == bBufferDistance) || (shapeType == SG_NIL_SHAPE)) ? shape : shapeWithBuffer, mSpatialFilters);
}

SHORT ArcSDEFilterToSql::GetSpatialFilters(SE_FILTER *&pSpatialFilters)
{
    // Create new array from internal vector, in output argument pSpatialFilters:
    if (mSpatialFilters.size() > 0)
    {
        pSpatialFilters = new SE_FILTER[mSpatialFilters.size()];
        for (unsigned int i=0; i<mSpatialFilters.size(); i++)
            pSpatialFilters[i] = mSpatialFilters[i];
    }
    else
    {
        pSpatialFilters = NULL;
    }

    // return size of array:
    return ((SHORT)mSpatialFilters.size());
}



ArcSDEFilterTypeEnum ArcSDEFilterToSql::GetFilterType(FdoFilter *filter)
{
    ArcSDEFilterTypeEnum filterType;

    if (NULL != dynamic_cast<FdoBinaryLogicalOperator*>(filter))  // And, Or
    {
        FdoBinaryLogicalOperator *binaryFilter = static_cast<FdoBinaryLogicalOperator*>(filter);
        FdoPtr<FdoFilter> leftOp = binaryFilter->GetLeftOperand();
        FdoPtr<FdoFilter> rightOp = binaryFilter->GetRightOperand();
        ArcSDEFilterTypeEnum leftOpType = GetFilterType(leftOp);
        ArcSDEFilterTypeEnum rightOpType = GetFilterType(rightOp);

        if (FdoBinaryLogicalOperations_And == binaryFilter->GetOperation())
        {
            if ((leftOpType==ArcSDEFilterType_Attribute) && (rightOpType==ArcSDEFilterType_Attribute))
                filterType = ArcSDEFilterType_Attribute;
            else if ((leftOpType==ArcSDEFilterType_Spatial) && (rightOpType==ArcSDEFilterType_Spatial))
                filterType = ArcSDEFilterType_Spatial;
            else if ((leftOpType==ArcSDEFilterType_Attribute) && (rightOpType==ArcSDEFilterType_Spatial))
                filterType = ArcSDEFilterType_MixedSupported;
            else if ((leftOpType==ArcSDEFilterType_Spatial) && (rightOpType==ArcSDEFilterType_Attribute))
                filterType = ArcSDEFilterType_MixedSupported;
            else
                filterType = ArcSDEFilterType_MixedUnsupported;
        }
        else // FdoBinaryLogicalOperations_Or
        {
            if ((leftOpType==ArcSDEFilterType_Attribute) && (rightOpType==ArcSDEFilterType_Attribute))
                filterType = ArcSDEFilterType_Attribute;
            else
                filterType = ArcSDEFilterType_MixedUnsupported;
        }
    }
    else if (NULL != dynamic_cast<FdoUnaryLogicalOperator*>(filter))  // not
    {
        FdoUnaryLogicalOperator *unaryFilter = static_cast<FdoUnaryLogicalOperator*>(filter);
        FdoPtr<FdoFilter> operand = unaryFilter->GetOperand();

        if (GetFilterType(operand)==ArcSDEFilterType_Attribute)
            filterType = ArcSDEFilterType_Attribute;
        else
            filterType = ArcSDEFilterType_MixedUnsupported;
    }
    else if (NULL != dynamic_cast<FdoComparisonCondition*>(filter))  // > , <, like, ...
    {
        filterType = ArcSDEFilterType_Attribute;
    }
    else if (NULL != dynamic_cast<FdoInCondition*>(filter))  // In
    {
        filterType = ArcSDEFilterType_Attribute;
    }
    else if (NULL != dynamic_cast<FdoNullCondition*>(filter))  // Is Null
    {
        filterType = ArcSDEFilterType_Attribute;
    }
    else if (NULL != dynamic_cast<FdoSpatialCondition*>(filter))  // spatial
    {
        filterType = ArcSDEFilterType_Spatial;
    }
    else if (NULL != dynamic_cast<FdoDistanceCondition*>(filter))  // spatial distance
    {
        filterType = ArcSDEFilterType_Spatial;
    }
    else  // unknown type
        filterType = ArcSDEFilterType_MixedUnsupported;

    return filterType;
}


