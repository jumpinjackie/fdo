/******************************************************************************
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
  *
 ******************************************************************************/

#include "stdafx.h"
#include "FdoRdbmsOdbcSpatialSqlFilter.h"

#define SQL_OPEN_PAREN              L" ( "
#define SQL_CLOSE_PAREN             L" ) "
#define SQL_LOGICAL_AND             L" AND "
#define SQL_LOGICAL_OR              L" OR "
#define SQL_GREATER_THAN_OP         L" > "
#define SQL_GREATER_OR_EQUAL_OP     L" >= "
#define SQL_LESS_THAN_OP            L" < "
#define SQL_LESS_OR_EQUAL_OP        L" <= "
#define SQL_DOT                     L"."

FdoRdbmsOdbcSpatialSqlFilter *
FdoRdbmsOdbcSpatialSqlFilter::Create(
    const FdoSmLpGeometricPropertyDefinition * geometricProperty,
    const FdoGeometricCondition * geometricCondition)
{
    FdoRdbmsOdbcSpatialSqlFilter * newObject =
        new FdoRdbmsOdbcSpatialSqlFilter(geometricProperty, geometricCondition);
    if (NULL == newObject)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return newObject;
}


const char *
FdoRdbmsOdbcSpatialSqlFilter::FilterToSql()
{
    const char * sqlCharString = (const char *) m_sqlClause;
    return sqlCharString;
}


FdoRdbmsOdbcSpatialSqlFilter::FdoRdbmsOdbcSpatialSqlFilter()
{
    // This will never be implemented (should be protected from external caller).
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
}


FdoRdbmsOdbcSpatialSqlFilter::FdoRdbmsOdbcSpatialSqlFilter(
    const FdoSmLpGeometricPropertyDefinition * geometricProperty,
    const FdoGeometricCondition * geometricCondition)
{
    // There is only one thing to do with this class.  We might as well perform
    // the translation now and hold onto its results, rather than holding 
    // onto the input parameters.

    FdoStringP tableName = geometricProperty->GetContainingDbObjectName();
    FdoStringP columnNameX = /*tableName + SQL_DOT +*/ geometricProperty->GetColumnNameX();
    FdoStringP columnNameY = /*tableName + SQL_DOT +*/ geometricProperty->GetColumnNameY();

	if( NULL != dynamic_cast<const FdoDistanceCondition*>( geometricCondition ) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_85_DISTANCE_SPATIAL_CONDITION_NOT_SUPPORTED)));

	const FdoSpatialCondition * spatialCondition = dynamic_cast<const FdoSpatialCondition*>( geometricCondition );
    
    if ( NULL == spatialCondition )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

    // Note: needed to cast away constness.
    FdoPtr<FdoExpression> geometryExpression = ((FdoSpatialCondition *)spatialCondition)->GetGeometry();
	FdoGeometryValue * geometryValue = dynamic_cast<FdoGeometryValue*>(geometryExpression.p);
	if (NULL == geometryValue)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_84_SPATIAL_CONDITION_NOT_LITERAL_GEOMETRY)));

    FdoPtr<FdoByteArray> geometryFgf = geometryValue->GetGeometry();
    if (geometryFgf == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_60_NULL_POINTER)));

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geometryObj = gf->CreateGeometryFromFgf(geometryFgf);
    FdoPtr<FdoIEnvelope> envelope = geometryObj->GetEnvelope();
    double minX = envelope->GetMinX();
    double minY = envelope->GetMinY();
    double maxX = envelope->GetMaxX();
    double maxY = envelope->GetMaxY();

    // Note: needed to cast away constness.
    FdoSpatialOperations operation = ((FdoSpatialCondition *)spatialCondition)->GetOperation();

    m_sqlClause = SQL_OPEN_PAREN;

    ProcessOrdinateRange(
        columnNameX,
        minX,
        maxX,
        operation);

    m_sqlClause += SQL_LOGICAL_AND;

    ProcessOrdinateRange(
        columnNameY,
        minY,
        maxY,
        operation);

    m_sqlClause += SQL_CLOSE_PAREN;
}


FdoRdbmsOdbcSpatialSqlFilter::~FdoRdbmsOdbcSpatialSqlFilter()
{
}

void FdoRdbmsOdbcSpatialSqlFilter::ProcessOrdinateRange(
    FdoString * ordinateColumnName,
    double minimum,
    double maximum,
    FdoSpatialOperations spatialOp )
{
    wchar_t numberBuffer[21];
    FdoString * minRangeComparison = NULL;
    FdoString * maxRangeComparison = NULL;

    switch (spatialOp)
    {
        case FdoSpatialOperations_Intersects:
        case FdoSpatialOperations_Within:
        case FdoSpatialOperations_EnvelopeIntersects:
            minRangeComparison = SQL_GREATER_OR_EQUAL_OP;
            maxRangeComparison = SQL_LESS_OR_EQUAL_OP;
            break;
        case FdoSpatialOperations_Inside:
            minRangeComparison = SQL_GREATER_THAN_OP;
            maxRangeComparison = SQL_LESS_THAN_OP;
            break;
        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_111, "Unsupported spatial operation"));
    }

    m_sqlClause += SQL_OPEN_PAREN;

    m_sqlClause += ordinateColumnName;
    m_sqlClause += minRangeComparison;
    FdoCommonStringUtil::FormatDouble(minimum, numberBuffer, sizeof(numberBuffer)-1, false);
    m_sqlClause += numberBuffer;

    m_sqlClause += SQL_LOGICAL_AND;

    m_sqlClause += ordinateColumnName;
    m_sqlClause += maxRangeComparison;
    FdoCommonStringUtil::FormatDouble(maximum, numberBuffer, sizeof(numberBuffer)-1, false);
    m_sqlClause += numberBuffer;

    m_sqlClause += SQL_CLOSE_PAREN;
}
