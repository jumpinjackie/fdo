// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Fdo/Filter/DistanceCondition.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

FdoDistanceCondition* FdoDistanceCondition::Create()
{
	return new FdoDistanceCondition();
}

// Constructs an instance of a DistanceCondition using the specified
// arguments.
FdoDistanceCondition* FdoDistanceCondition::Create(FdoString* propertyName, FdoDistanceOperations operation, FdoExpression* geometry, double distance)
{
	return new FdoDistanceCondition(propertyName, operation, geometry, distance);
}

// Constructs an instance of a DistanceCondition using the specified
// arguments.
FdoDistanceCondition* FdoDistanceCondition::Create(FdoIdentifier* propertyName, FdoDistanceOperations operation, FdoExpression* geometry, double distance)
{
	return new FdoDistanceCondition(propertyName, operation, geometry, distance);
}

FdoDistanceCondition::FdoDistanceCondition() :
  FdoGeometricCondition()
{
	m_geometry = NULL;
	m_distance = 0.0;
	m_operation = FdoDistanceOperations_Beyond;
}

// Constructs an instance of a DistanceCondition using the specified
// arguments.
FdoDistanceCondition::FdoDistanceCondition(FdoString* propertyName, FdoDistanceOperations operation, FdoExpression* geometry, double distance) :
  FdoGeometricCondition(propertyName)
{
	m_distance = distance;
	m_operation = operation;
    m_geometry = FDO_SAFE_ADDREF(geometry);
}

// Constructs an instance of a DistanceCondition using the specified
// arguments.
FdoDistanceCondition::FdoDistanceCondition(FdoIdentifier* propertyName, FdoDistanceOperations operation, FdoExpression* geometry, double distance) :
  FdoGeometricCondition(propertyName)
{
	m_distance = distance;
	m_operation = operation;
    m_geometry = FDO_SAFE_ADDREF(geometry);
}

FdoDistanceCondition::~FdoDistanceCondition()
{
    FDO_SAFE_RELEASE(m_geometry);
}

void FdoDistanceCondition::Dispose()
{
    delete this;
}

// Gets the distance operation to use.
FdoDistanceOperations FdoDistanceCondition::GetOperation()
{
    return m_operation;
}

// Sets the distance operation to use.
void FdoDistanceCondition::SetOperation(FdoDistanceOperations value)
{
	m_operation = value;
}

// Gets the literal geometric value.
FdoExpression* FdoDistanceCondition::GetGeometry()
{
    return FDO_SAFE_ADDREF(m_geometry);
}

// Sets the literal geometric value.
void FdoDistanceCondition::SetGeometry(FdoExpression* value)
{
    FDO_SAFE_RELEASE(m_geometry);
	m_geometry = FDO_SAFE_ADDREF(value);
}

// Gets the distance value.
double FdoDistanceCondition::GetDistance()
{
    return m_distance;
}

// Sets the distance value.
void FdoDistanceCondition::SetDistance(double value)
{
	m_distance = value;
}

// Overrides Expression.Process to pass the DistanceCondition to the
// appropriate expression processor operation.
void FdoDistanceCondition::Process(FdoIFilterProcessor* p)
{
    p->ProcessDistanceCondition(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoDistanceCondition::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoIdentifier*      pIdent = GetPropertyName();
    FdoExpression*      pGeom = GetGeometry();
    if (pIdent == NULL || pGeom == NULL)
    {
        FDO_SAFE_RELEASE(pIdent);
        FDO_SAFE_RELEASE(pGeom);
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_3_INCOMPLETEDISTANCECONDITION)));
        throw pExcept;
    }

    wchar_t szDist[32];
    FdoStringUtility::FormatDouble(GetDistance(), szDist, sizeof(szDist) / sizeof(szDist[0]));

    wchar_t*    szOp;
    switch (GetOperation())
    {
    default:
    case FdoDistanceOperations_Within:
        szOp = L" WITHINDISTANCE ";
        break;
    case FdoDistanceOperations_Beyond:
        szOp = L" BEYOND ";
        break;
    }

    try
    {
        m_toString = FdoStringUtility::MakeString(pIdent->ToStringInternal(pIdCol), szOp, pGeom->ToStringInternal( pIdCol ), L" ", szDist);
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pIdent);
        FDO_SAFE_RELEASE(pGeom);
        throw pExcept;
    }
    pIdent->Release();
    pGeom->Release();
    return m_toString;
}

