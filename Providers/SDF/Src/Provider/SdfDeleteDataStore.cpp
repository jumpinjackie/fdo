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
#include "SdfDeleteDataStore.h"
#include "SdfConnection.h"
#include <stdio.h>

#ifndef _WIN32
#include <unistd.h>
#endif

SdfDeleteDataStore::SdfDeleteDataStore(SdfConnection* connection)
: SdfCommand<FdoIDestroyDataStore>(connection)
{
	m_dataStorePropertyDictionary = new SdfDataStorePropertyDictionary(connection);
}

void SdfDeleteDataStore::Execute()
{  
	FdoStringP	filename = m_dataStorePropertyDictionary->GetProperty(PROP_NAME_FILE);

    //check if the specified filename exists
    size_t len = wcstombs(NULL, (FdoString*)filename, 0);
    char* mbsName = new char[len + 1];
    wcstombs(mbsName, (FdoString*)filename, len+1);
    
	// Check if the file exists
    FILE* f = fopen( mbsName, "r" );

    if (!f)
       throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_50_NONEXISTING_FILE), "SDF connect failed. File does not exist or cannot be opened in specified access mode."));

    fclose(f);

	int rc;
#ifdef _WIN32
    rc = ( 0 != DeleteFile( mbsName ) );
#else
	rc = ( 0 == unlink( mbsName ) );
#endif

    delete [] mbsName;

	if ( ! rc )
       throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_70_DELETE_FAILED), "Failed to delete file '%1$ls'", (const wchar_t *)filename));

}


