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
#include <Fdo/Filter/SpatialCondition.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of an SpatialCondition.
FdoSpatialCondition* FdoSpatialCondition::Create()
{
	return new FdoSpatialCondition();
}

// Constructs an instance of a SpatialCondition using the specified
// arguments.
FdoSpatialCondition* FdoSpatialCondition::Create(FdoString* propertyName, FdoSpatialOperations operation, FdoExpression* geometry)
{
	return new FdoSpatialCondition(propertyName, operation, geometry);
}

// Constructs an instance of a SpatialCondition using the specified
// arguments.
FdoSpatialCondition* FdoSpatialCondition::Create(FdoIdentifier* propertyName, FdoSpatialOperations operation, FdoExpression* geometry)
{
	return new FdoSpatialCondition(propertyName, operation, geometry);
}

// Constructs a default instance of an SpatialCondition.
FdoSpatialCondition::FdoSpatialCondition() :
  FdoGeometricCondition()
{
	m_geometry = NULL;
	m_operation = FdoSpatialOperations_Contains;
}

// Constructs an instance of a SpatialCondition using the specified
// arguments.
FdoSpatialCondition::FdoSpatialCondition(FdoString* propertyName, FdoSpatialOperations operation, FdoExpression* geometry) :
  FdoGeometricCondition(propertyName)
{
	m_operation = operation;
    m_geometry = FDO_SAFE_ADDREF(geometry);
}

// Constructs an instance of a SpatialCondition using the specified
// arguments.
FdoSpatialCondition::FdoSpatialCondition(FdoIdentifier* propertyName, FdoSpatialOperations operation, FdoExpression* geometry) :
  FdoGeometricCondition(propertyName)
{
	m_operation = operation;
    m_geometry = FDO_SAFE_ADDREF(geometry);
}

FdoSpatialCondition::~FdoSpatialCondition()
{
    FDO_SAFE_RELEASE(m_geometry);
}

void FdoSpatialCondition::Dispose()
{
    delete this;
}

// Gets the spatial operation to use.
FdoSpatialOperations FdoSpatialCondition::GetOperation()
{
    return m_operation;
}

// Sets the spatial operation to use.
void FdoSpatialCondition::SetOperation(FdoSpatialOperations value)
{
	m_operation = value;
}

// Gets the literal geometric value.
FdoExpression* FdoSpatialCondition::GetGeometry()
{
    return FDO_SAFE_ADDREF(m_geometry);
}

// Sets the literal geometric value.
void FdoSpatialCondition::SetGeometry(FdoExpression* value)
{
    FDO_SAFE_RELEASE(m_geometry);
	m_geometry = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the SpatialCondition to the
// appropriate expression processor operation.
void FdoSpatialCondition::Process(FdoIFilterProcessor* p)
{
    p->ProcessSpatialCondition(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoSpatialCondition::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoIdentifier*      pIdent = GetPropertyName();
    FdoExpression*      pGeom = GetGeometry();
    if (pIdent == NULL || pGeom == NULL)
    {
        FDO_SAFE_RELEASE(pIdent);
        FDO_SAFE_RELEASE(pGeom);
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_6_INCOMPLETESPATIALCONDITION)));
        throw pExcept;
    }

    const wchar_t*    szOp;
    switch (GetOperation())
    {
    default:
    case FdoSpatialOperations_Contains:
        szOp = L" CONTAINS ";
        break;
    case FdoSpatialOperations_Crosses:
        szOp = L" CROSSES ";
        break;
    case FdoSpatialOperations_Disjoint:
        szOp = L" DISJOINT ";
        break;
    case FdoSpatialOperations_Equals:
        szOp = L" EQUALS ";
        break;
    case FdoSpatialOperations_Intersects:
        szOp = L" INTERSECTS ";
        break;
    case FdoSpatialOperations_Overlaps:
        szOp = L" OVERLAPS ";
        break;
    case FdoSpatialOperations_Touches:
        szOp = L" TOUCHES ";
        break;
    case FdoSpatialOperations_Within:
        szOp = L" WITHIN ";
        break;
    case FdoSpatialOperations_CoveredBy:
        szOp = L" COVEREDBY ";
        break;
    case FdoSpatialOperations_Inside:
        szOp = L" INSIDE ";
        break;
    case FdoSpatialOperations_EnvelopeIntersects:
        szOp = L" ENVELOPEINTERSECTS ";
        break;
    }

    try
    {
        m_toString = FdoStringUtility::MakeString(pIdent->ToStringInternal( pIdCol ), szOp, pGeom->ToStringInternal( pIdCol ));
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pIdent);
        FDO_SAFE_RELEASE(pGeom);
        throw pExcept;
    }

    pGeom->Release();
    pIdent->Release();
    return m_toString;
}

