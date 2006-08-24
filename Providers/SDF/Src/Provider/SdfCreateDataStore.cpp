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
#include "SdfCreateDatastore.h"
#include "SdfConnection.h"
#include "SdfCreateSpatialContext.h"
#include <FdoCommonStringUtil.h>

SdfCreateDataStore::SdfCreateDataStore(SdfConnection* connection)
: SdfCommand<FdoICreateDataStore>(connection)
{
    m_dataStorePropertyDictionary = new FdoCommonDataStorePropDictionary (connection);
    char* mbPropName = NULL;
    wide_to_multibyte(mbPropName, PROP_NAME_FILE);
    FdoPtr<ConnectionProperty> newProp = new ConnectionProperty (PROP_NAME_FILE,
            NlsMsgGetMain(SDFPROVIDER_48_PROP_NAME_FILE, mbPropName),
            EMPTY_VALUE, true, false, false, true, false, false, false, 0, NULL);
    m_dataStorePropertyDictionary->AddProperty(newProp);
}

void SdfCreateDataStore::Execute()
{
    // Cloned from SdfCreateSDFFile
 
    _ASSERT(m_connection);

    if (m_connection->GetConnectionState() != FdoConnectionState_Closed)
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_30_CONNECTION_OPEN)));

    FdoStringP    filename = m_dataStorePropertyDictionary->GetProperty(PROP_NAME_FILE);

    //check if the specified filename exists
    FdoStringP    filenameEx = filename.Replace(L"\"", L"");
    size_t len = wcstombs(NULL, (FdoString*)filenameEx, 0);
    char* mbsName = new char[len + 1];
    wcstombs(mbsName, (FdoString*)filenameEx, len+1);
    
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
    std::wstring connStr = L"File=" + std::wstring(filename) + L";ReadOnly=FALSE";

    m_connection->SetCreateSDF();
    m_connection->SetConnectionString(connStr.c_str());

    if (m_connection->Open() == FdoConnectionState_Open)
    {
        //use the unpublished CreateSpatialContext command.
        //It does exactly what we need to set the coord sys. Use default values
        SdfCreateSpatialContext* create = new SdfCreateSpatialContext(m_connection);

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

