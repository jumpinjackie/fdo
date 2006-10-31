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
#include "stdafx.h"
#include "SdfCreateSpatialContext.h"
#include "SdfConnection.h"
#include "BinaryWriter.h"
#include "SchemaDb.h"

SdfCreateSpatialContext::SdfCreateSpatialContext(SdfConnection *connection)
: SdfCommand<FdoICreateSpatialContext>(connection)
{
    m_scName = NULL;
    m_description = NULL;
    m_coordSysName = NULL;
    m_coordSysWkt = NULL;

    m_extentType = FdoSpatialContextExtentType_Dynamic;
    m_extent = NULL;

    m_xyTolerance = 0;
    m_zTolerance = 0;

    m_updateExisting = false;

    m_dimensionality = (FdoDimensionality_XY | FdoDimensionality_Z);

}

SdfCreateSpatialContext::~SdfCreateSpatialContext()
{
    if( m_scName != NULL )
	{
		delete[] m_scName;
		m_scName = NULL;
	}
    if( m_description != NULL )
	{
		delete[] m_description;
		m_description = NULL;
	}
    if( m_coordSysName != NULL )
	{
		delete[] m_coordSysName;
		m_coordSysName = NULL;
	}
    if( m_coordSysWkt != NULL )
	{
		delete[] m_coordSysWkt;
		m_coordSysWkt = NULL;
	}
	FDO_SAFE_RELEASE(m_extent);
}

//-------------------------------------------------------
// FdoICreateSpatialContext implementation
//-------------------------------------------------------

FdoString* SdfCreateSpatialContext::GetName()
{
    return m_scName;
}

void SdfCreateSpatialContext::SetName(FdoString* value)
{
    if( m_scName != NULL )
	{
		delete[] m_scName;
		m_scName = NULL;
	}

	if( value != NULL )
	{
		m_scName = new wchar_t[ wcslen( value ) + 1];
		wcscpy( m_scName, value );
	}
}

FdoString* SdfCreateSpatialContext::GetDescription()
{
    return m_description;
}

void SdfCreateSpatialContext::SetDescription(FdoString* value)
{
    if( m_description != NULL )
	{
		delete[] m_description;
		m_description = NULL;
	}

	if( value != NULL )
	{
		m_description = new wchar_t[ wcslen( value ) + 1];
		wcscpy( m_description, value );
	}
}


void SdfCreateSpatialContext::SetGeometricProperty(FdoPropertyDefinition *value)
{
    //TODO: don't think this is relevant for SDF+ since we use one coord sys for everything
    ;
}


void SdfCreateSpatialContext::SetDimensionality(int value)
{
    m_dimensionality = value;
}


int SdfCreateSpatialContext::GetDimensionality()
{
    return m_dimensionality;
}


FdoString* SdfCreateSpatialContext::GetCoordinateSystem()
{
    return m_coordSysName;
}

void SdfCreateSpatialContext::SetCoordinateSystem(FdoString* value)
{
    if( m_coordSysName != NULL )
	{
		delete[] m_coordSysName;
		m_coordSysName = NULL;
	}

	if( value != NULL )
	{
		m_coordSysName = new wchar_t[ wcslen( value ) + 1];
		wcscpy( m_coordSysName, value );
	}
}

FdoString* SdfCreateSpatialContext::GetCoordinateSystemWkt()
{
    return m_coordSysWkt;
}

void SdfCreateSpatialContext::SetCoordinateSystemWkt(FdoString* value)
{
    if( m_coordSysWkt != NULL )
	{
		delete[] m_coordSysWkt;
		m_coordSysWkt = NULL;
	}

	if( value != NULL )
	{
		m_coordSysWkt = new wchar_t[ wcslen( value ) + 1];
		wcscpy( m_coordSysWkt, value );
	}
}

FdoSpatialContextExtentType SdfCreateSpatialContext::GetExtentType()
{
    return m_extentType;
}

void SdfCreateSpatialContext::SetExtentType(FdoSpatialContextExtentType value)
{
    m_extentType = value;
}

FdoByteArray* SdfCreateSpatialContext::GetExtent()
{
	FDO_SAFE_ADDREF( m_extent );
    return m_extent;
}

void SdfCreateSpatialContext::SetExtent(FdoByteArray* value)
{
	FDO_SAFE_RELEASE(m_extent);
    m_extent = value;
	FDO_SAFE_ADDREF( m_extent );
}

const double SdfCreateSpatialContext::GetXYTolerance()
{
    return m_xyTolerance;
}

void SdfCreateSpatialContext::SetXYTolerance(double value)
{
    m_xyTolerance = value;
}

const double SdfCreateSpatialContext::GetZTolerance()
{
    return m_zTolerance;
}

void SdfCreateSpatialContext::SetZTolerance(const double value)
{
    m_zTolerance = value;
}

const bool SdfCreateSpatialContext::GetUpdateExisting()
{
    return m_updateExisting;
}

void SdfCreateSpatialContext::SetUpdateExisting(const bool value)
{
    m_updateExisting = value;
}

void SdfCreateSpatialContext::Execute()
{
    // verify connection
    if (m_connection == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_39_NO_CONNECTION)));

	if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    bool readOnly = m_connection->GetReadOnly();
    if (readOnly)
		throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

    BinaryWriter wrt(256);

    wrt.WriteString(m_scName);
    wrt.WriteString(m_description);
    wrt.WriteString(m_coordSysWkt);
    wrt.WriteInt32(m_dimensionality);
    wrt.WriteInt32(m_extentType);
    
    if (m_extent)
    {
        wrt.WriteInt32(m_extent->GetCount());
        wrt.WriteBytes(m_extent->GetData(), m_extent->GetCount());
    }
    else
        wrt.WriteInt32(0);

    
    wrt.WriteDouble(m_xyTolerance);
    wrt.WriteDouble(m_zTolerance);

    
    BinaryWriter oldCs(256);

    m_connection->GetSchemaDb()->ReadCoordinateSystemRecord(oldCs);

    bool exists = false;

    if (oldCs.GetDataLen() > 0)
        exists = true;

    //do the update
    //just overwrite the old CS value with the new one
    m_connection->GetSchemaDb()->WriteCoordinateSystemRecord(wrt);

}


