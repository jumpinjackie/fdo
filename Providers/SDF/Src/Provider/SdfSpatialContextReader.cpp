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
#include "SdfSpatialContextReader.h"

#include "BinaryWriter.h"
#include "SchemaDb.h"
#include "SdfConnection.h"


SdfSpatialContextReader::SdfSpatialContextReader (SdfConnection* conn)
{
    m_connection = conn;
    FDO_SAFE_ADDREF(m_connection);

    m_scName = NULL;
    m_description = NULL;
    m_srsName = NULL;

    m_extent = NULL;

    m_firstRead = true;
}

SdfSpatialContextReader::~SdfSpatialContextReader()
{
    if (m_scName)
        delete [] m_scName;

    if (m_description)
        delete [] m_description;

    if (m_srsName)
        delete [] m_srsName;

    FDO_SAFE_RELEASE(m_connection);
    FDO_SAFE_RELEASE(m_extent);
}

void SdfSpatialContextReader::Dispose()
{
    delete this;
}

FdoString* SdfSpatialContextReader::GetName()
{
    if (!m_firstRead)
        return m_scName;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

FdoString* SdfSpatialContextReader::GetDescription()
{
    if (!m_firstRead)
        return m_description;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

FdoString* SdfSpatialContextReader::GetCoordinateSystem()
{
    //TODO:
    //the custom command does not have separate setters for spatial context
    //name and coord sys name. Here we return the spatial context name.
    if (!m_firstRead)
        return m_srsName;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

FdoString* SdfSpatialContextReader::GetCoordinateSystemWkt()
{
    if (!m_firstRead)
        return m_srsName;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

FdoSpatialContextExtentType SdfSpatialContextReader::GetExtentType()
{
    if (!m_firstRead)
        return m_extentType;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

FdoByteArray* SdfSpatialContextReader::GetExtent()
{
    if (!m_firstRead)
    {
        return m_connection->GetDataExtents();
    }

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

const double SdfSpatialContextReader::GetXYTolerance()
{
    if (!m_firstRead)
        return m_xyTolerance;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

const double SdfSpatialContextReader::GetZTolerance()
{
    if (!m_firstRead)
        return m_zTolerance;

    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_42_READER_NOT_INIT)));
}

const bool SdfSpatialContextReader::IsActive()
{
    //we only have one coordinate system in the SDF file
    //and it is always the active one
    return true;
}

bool SdfSpatialContextReader::ReadNext()
{
    //we only have one coord sys, if it has been read
    //we will return false
    if (!m_firstRead) 
        return false;

    //obtain the coord sys data record
    BinaryWriter wrt(256);
    m_connection->GetSchemaDb()->ReadCoordinateSystemRecord(wrt);

    //if there is no coord sys stored in the SDF+, return
    if (wrt.GetDataLen() == 0)
    {
        m_firstRead = false;
        return false;
    }

    //now we will read all the coord sys properties from the 
    //binary record
    BinaryReader rdr(wrt.GetData(), wrt.GetDataLen());

    size_t len = 0;
    
    //make copy of all string properties since they are returned in
    //buffer memory owned by the BinaryReader
    FdoString* csName = rdr.ReadString();
    if (csName != NULL)
    {
        len = wcslen(csName);
        m_scName = new wchar_t[len+1];
        wcscpy(m_scName, csName);
    }

    FdoString* csDesc = rdr.ReadString();
    if (csDesc)
    {
        len = wcslen(csDesc);
        m_description = new wchar_t[len+1];
        wcscpy(m_description, csDesc);
    }

    FdoString* csSRS = rdr.ReadString();
    if (csSRS)
    {
        len = wcslen(csSRS);
        m_srsName = new wchar_t[len+1];
        wcscpy(m_srsName, csSRS);
    }

    m_dimensionality = rdr.ReadInt32();

    m_extentType = (FdoSpatialContextExtentType)rdr.ReadInt32();

    int extLen = rdr.ReadInt32();

    if (extLen > 0)
    {
        m_extent = FdoByteArray::Create(rdr.GetDataAtCurrentPosition(), extLen);
        //manually advance the reader...
        rdr.SetPosition(rdr.GetPosition() + extLen);
    }
    else
        m_extent = NULL;

    m_xyTolerance = rdr.ReadDouble();
    m_zTolerance = rdr.ReadDouble();
   

    m_firstRead = false;
    return true;
}

