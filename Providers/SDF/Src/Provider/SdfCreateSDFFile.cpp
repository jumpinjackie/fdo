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
#include "SdfCreateSDFFile.h"
#include "SdfConnection.h"
#include "SdfCreateSpatialContext.h"


SdfCreateSDFFile::SdfCreateSDFFile(SdfConnection* connection) 
: SdfCommand<FdoICreateSDFFile>(connection)
{
    m_filename = (wchar_t*)NULL;
    m_scname = (wchar_t*)NULL;
    m_scdesc = (wchar_t*)NULL;
    m_wkt = (wchar_t*)NULL;
    m_xyTolerance = 0.0;
    m_zTolerance = 0.0;
}

SdfCreateSDFFile::~SdfCreateSDFFile()
{
}


void SdfCreateSDFFile::SetFileName(FdoString* name)
{
    m_filename = name;
}

FdoString* SdfCreateSDFFile::GetFileName()
{
    return (FdoString*)m_filename;
}

void SdfCreateSDFFile::SetSpatialContextName(FdoString* name)
{
    m_scname = name;
}

FdoString* SdfCreateSDFFile::GetSpatialContextName()
{
    return (FdoString*)m_scname;
}

void SdfCreateSDFFile::SetSpatialContextDescription(FdoString* description)
{
    m_scdesc = description;
}

FdoString* SdfCreateSDFFile::GetSpatialContextDescription()
{
    return (FdoString*)m_scdesc;
}

void SdfCreateSDFFile::SetCoordinateSystemWKT(FdoString* wkt)
{
    m_wkt = wkt;
}

FdoString* SdfCreateSDFFile::GetCoordinateSystemWKT()
{
    return (FdoString*)m_wkt;
}

void SdfCreateSDFFile::SetXYTolerance(double tolerance)
{
    m_xyTolerance = tolerance;
}

double SdfCreateSDFFile::GetXYTolerance()
{
    return m_xyTolerance;
}

void SdfCreateSDFFile::SetZTolerance(double tolerance)
{
    m_zTolerance = tolerance;
}

double SdfCreateSDFFile::GetZTolerance()
{
    return m_zTolerance;
}

void SdfCreateSDFFile::Execute()
{
    //this should be fun...
    //Just open and close the connection...
    //that should give us an empty SDF file, hopefully.

    _ASSERT(m_connection);

    if (m_connection->GetConnectionState() != FdoConnectionState_Closed)
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_30_CONNECTION_OPEN)));

    //check if the specified filename exists
    int len = wcstombs(NULL, (FdoString*)m_filename, 0);
    char* mbsName = new char[len + 1];
    wcstombs(mbsName, (FdoString*)m_filename, len+1);
    
    FILE* f = fopen(mbsName, "r");

    //clean up before we die
    delete [] mbsName;

    if (f)
    {
        fclose(f);
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_33_FILE_EXISTS)));
    }
   

    //remember the old connection string...
    std::wstring oldConnStr = m_connection->GetConnectionString();

    //make the usual connection string and also tack on 
    //a flag property telling the connection that we want to create
    //a new SDF file. This will skip some integrity checks
    //when opening the connection.
    std::wstring connStr = L"File=" + std::wstring(m_filename) + L";ReadOnly=FALSE";

	m_connection->SetCreateSDF();
    m_connection->SetConnectionString(connStr.c_str());

    if (m_connection->Open() == FdoConnectionState_Open)
    {
        //use the unpublished CreateSpatialContext command.
        //It does exactly what we need to set the coord sys.
        SdfCreateSpatialContext* create = new SdfCreateSpatialContext(m_connection);

		create->SetCoordinateSystemWkt((FdoString*)m_wkt);
        create->SetDescription((FdoString*)m_scdesc);
        create->SetName((FdoString*)m_scname);
        create->SetXYTolerance(m_xyTolerance);
        create->SetZTolerance(m_zTolerance);

        //set the coordinate system to the SDF file
        create->Execute();

        create->Release();

        m_connection->Close();
    }
    else
    {
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_34_FILE_CREATE_FAILED)));
    }

    //restore the old connection string
    m_connection->SetConnectionString(oldConnStr.c_str());
    
}





