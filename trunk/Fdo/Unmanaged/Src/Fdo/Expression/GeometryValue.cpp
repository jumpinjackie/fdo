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
#include <Fdo/Expression/GeometryValue.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "StringUtility.h"
#include <FdoGeometry.h>

// Constructs a default instance of a GeometryValue with a value of null.
FdoGeometryValue* FdoGeometryValue::Create()
{
	return new FdoGeometryValue();
}

// Constructs an instance of a GeometryValue using the specified arguments.
FdoGeometryValue* FdoGeometryValue::Create(FdoByteArray* geometry)
{
	return new FdoGeometryValue(geometry);
}

// Constructs a default instance of a GeometryValue with a value of null.
FdoGeometryValue::FdoGeometryValue()
{
	m_geometry = NULL;
}

// Constructs an instance of a GeometryValue using the specified arguments.
FdoGeometryValue::FdoGeometryValue(FdoByteArray* geometry)
{
	m_geometry = NULL;
	SetGeometry(geometry);
}

FdoGeometryValue::~FdoGeometryValue()
{
	SetNullValue();
}

void FdoGeometryValue::Dispose()
{
    delete this;
}

// Returns true if the GeometryValue represents a null value.
bool FdoGeometryValue::IsNull()
{
	return m_geometry == NULL;
}

// Gets the geometery as a byte array. The geometry is expected to be encoded in Autodesk Well-Known Binary format.
FdoByteArray* FdoGeometryValue::GetGeometry()
{
	return FDO_SAFE_ADDREF(m_geometry);
}

// Sets the geometery as a byte array. The geometry is expected to be encoded in Autodesk Well-Known Binary format.
void FdoGeometryValue::SetGeometry(FdoByteArray* value)
{
	FDO_SAFE_RELEASE(m_geometry);
	m_geometry = FDO_SAFE_ADDREF(value);
}

// Sets the GeometryValue to a null value.
void FdoGeometryValue::SetNullValue()
{
	FDO_SAFE_RELEASE(m_geometry);
}

// Overrides Expression.Process to pass the GeometryValue to the
// appropriate expression processor operation.
void FdoGeometryValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessGeometryValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoGeometryValue::ToString ()
{
    FdoStringUtility::ClearString(m_toString);
	if (m_geometry == NULL)
	{
	    m_toString = FdoStringUtility::MakeString(L"GeomFromText('NULL')");
	    return m_toString;
	}

	FdoFgfGeometryFactory* gf = FdoFgfGeometryFactory::GetInstance();
	if (gf != NULL)
	{
		FdoIGeometry* geometry = NULL;
		try
		{
			geometry = gf->CreateGeometryFromFgf(m_geometry);
			m_toString = FdoStringUtility::MakeString(L"GeomFromText('", geometry->GetText(), L"')");
		}
	    catch (FdoException* pExcept)
	    {
			FDO_SAFE_RELEASE(geometry);
			FDO_SAFE_RELEASE(gf);
	        throw pExcept;
		}
		FDO_SAFE_RELEASE(geometry);
	}
	FDO_SAFE_RELEASE(gf);
    return m_toString;
}

