/*
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
#include "FdoRdbmsOdbcConnectionInfo.h"
#include "FdoRdbmsOdbcProviderInfo.h"
#include <Sm/Ph/Rd/OwnerReader.h>
#include "FdoCommonStringUtil.h"
#include <FdoCommonFile.h>
#include "../../ODBCDriver/constants.h"

#ifdef _WIN32
#include <tchar.h>
#endif

#define ODBC_CONNECTION_STRING_TAG L"ConnectionString"
#define ODBC_DSN_TAG               L"DataSourceName"


FdoRdbmsOdbcConnectionInfo::FdoRdbmsOdbcConnectionInfo(FdoRdbmsConnection *connection) :
mConnection(connection)
{
    mProviderType          = FdoProviderDatastoreType_Unknown;
    mProviderDataSetLoaded = false;
}

FdoRdbmsOdbcConnectionInfo::~FdoRdbmsOdbcConnectionInfo()
{
}


const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderName()
{
    return RDBMS_ODBC_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_476, RDBMS_ODBC_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_477, RDBMS_ODBC_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderVersion()
{
    return RDBMS_ODBC_PROVIDER_VERSION;
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_ODBC_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsOdbcConnectionInfo::GetConnectionProperties()
{
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary((FdoIConnection*)mConnection);
        
        // Allocate temporary collection to hold the set of SYSTEM/USER DSN's. 
        FdoStringsP dataSourceNameCollection = FdoStringCollection::Create();

        DbiConnection * dbiConnection = mConnection->GetDbiConnection();
        GdbiConnection * gdbiConnection = dbiConnection->GetGdbiConnection();
        rdbi_context_def * rdbiContext = gdbiConnection->GetRdbiContext();

        wchar_t nameBuf[GDBI_MAXIMUM_STRING_SIZE+1];
        rdbi_string_def name;
        name.wString = nameBuf;
        *name.wString = L'\0';
        int   eof = FALSE;

        if (RDBI_SUCCESS == rdbi_stores_act(rdbiContext))
        {
            if (rdbiContext->dispatch.capabilities.supports_unicode == 1)
            {
                while (RDBI_SUCCESS == rdbi_stores_getW(rdbiContext, name.wString, &eof) && !eof)
                    dataSourceNameCollection->Add(name.cwString);
            }
            else
            {
                while (RDBI_SUCCESS == rdbi_stores_get(rdbiContext, name.cString, &eof) && !eof)
                    dataSourceNameCollection->Add(name.ccString);
            }
            (void) rdbi_stores_deac(rdbiContext);
        }

        // Convert DSN collection to an array of strings needed for the dictionary entry.
        wchar_t **dataSourceNames = NULL;
        FdoInt32 dataSourceNameCount = dataSourceNameCollection->GetCount();
        dataSourceNames = (wchar_t **) new wchar_t[sizeof(wchar_t *) * dataSourceNameCount];
        for (FdoInt32 i=0; i<dataSourceNameCount; i++)
        {
		    FdoStringElementP nameElement = dataSourceNameCollection->GetItem(i);
		    FdoStringP name = nameElement->GetString();
            dataSourceNames[i] = new wchar_t[name.GetLength()+1];
            wcscpy(dataSourceNames[i], (const wchar_t*)name);
        }

        FdoPtr<ConnectionProperty> pProp;
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_USERID, NlsMsgGet(FDORDBMS_462, "UserId"), L"", false, false, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
        
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), L"", false, true, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DSN, NlsMsgGet(FDORDBMS_463, "DataSourceName"), L"", false, false, true, false, false, true, false, false, dataSourceNameCount, (const wchar_t **)dataSourceNames); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_CONNSTRING, NlsMsgGet(FDORDBMS_464, "ConnectionString"), L"", false, false, false, false, false, false, true, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_GENDEFGEOMETRYPROP, NlsMsgGet(FDORDBMS_465, "GenerateDefaultGeometryProperty"), L"true", false, false, false, false, false, false, false, 0, NULL); 

        mPropertyDictionary->AddProperty(pProp);
    }
    FDO_SAFE_ADDREF(mPropertyDictionary.p);
    return mPropertyDictionary;
}

FdoProviderDatastoreType FdoRdbmsOdbcConnectionInfo::GetProviderDatastoreType()
{
    if (mProviderDataSetLoaded)
        return mProviderType;

    // The provider data has not been set yet. Attempt to do this and return
    // the requested information.
    ProcessProviderDataRequest();
    return mProviderType;
}

FdoStringCollection* FdoRdbmsOdbcConnectionInfo::GetDependentFileNames()
{
    // If the connection is not yet open return NULL.
    if (mConnection->GetConnectionState() != FdoConnectionState_Open)
        return NULL;

    // If the provider data has already been set return the requested
    // information.
    if (mProviderDataSetLoaded)
        return (FDO_SAFE_ADDREF(mDependentFiles.p));

    // The provider data has not been set yet. Attempt to do this and return
    // the requested information.
    ProcessProviderDataRequest();
    return (FDO_SAFE_ADDREF(mDependentFiles.p));
}

void FdoRdbmsOdbcConnectionInfo::Dispose()
{
    delete this;
}


void FdoRdbmsOdbcConnectionInfo::ProcessProviderDataRequest()
{

    // It is assumed that the process cannot be completed as requested. Therefore,
    // the provider type is set to indicate an unknown type and the list of 
    // dependent files is set to NULL;
    mProviderType = FdoProviderDatastoreType_Unknown;

    // Check if the connection properties are set. If this is not the case the
    // function can be terminated.
    if (mPropertyDictionary == NULL)
        return;

    // Check connection properties for specific entries to decide whether this
    // is a DSN identifier or ODBC specific connection string based connection.
    // Note that both options should not be available in the connection properties
    // at the same time. However, in case it happens, the ODBC specific connection
    // string is preferred.
    FdoStringP dsnIdentifier;
    FdoStringP odbcConnectionString;

    FdoPtr<ConnectionProperty> odbcConnectionStringProperty = mPropertyDictionary->FindProperty(ODBC_CONNECTION_STRING_TAG);
    if (odbcConnectionStringProperty != NULL)
        odbcConnectionString = odbcConnectionStringProperty->GetValue();

    FdoPtr<ConnectionProperty> dsnProperty = mPropertyDictionary->FindProperty(ODBC_DSN_TAG);
    if (dsnProperty != NULL)
        dsnIdentifier = dsnProperty->GetValue();

    if (((odbcConnectionString == NULL) || (odbcConnectionString == L"")) && 
        ((dsnIdentifier        == NULL) || (dsnIdentifier        == L""))    )
        return;

    if ((odbcConnectionString != NULL) && (odbcConnectionString != L""))
        SetProviderDataFromOdbcConnectionString(odbcConnectionString);
    else
        SetProviderDataFromDsn();

    mProviderDataSetLoaded = true;
}


#ifdef _WIN32
static HKEY GetRegistryKey(HKEY topkey, const char * subkeyName)
{
    HKEY hkey = NULL;
    TCHAR value[ODBCDR_CONNECTION_SIZE];
    DWORD size = sizeof(value) / sizeof(value[0]);;
    LONG errStatus = ERROR_SUCCESS;
    int status = FALSE;

    if (ERROR_SUCCESS != RegOpenKeyEx (
        topkey,
        _T(subkeyName),     /* subkey name */
        0L,                 /* reserved */
        KEY_QUERY_VALUE,    /* security access mask */
        &hkey))             /* handle to open key */
    {
        hkey = NULL;
    }
    return hkey;
}

static FdoStringP GetRegistryValue(HKEY hkey, const char * name)
{
    TCHAR value[ODBCDR_CONNECTION_SIZE];
    DWORD size = sizeof(value) / sizeof(value[0]);;
    LONG errStatus = ERROR_SUCCESS;
    DWORD type;
    FdoStringP valueP;

    errStatus = RegQueryValueEx (
        hkey,         /* handle to key */
        _T(name),     /* value name */
        NULL,         /* reserved */
        &type,        /* type buffer */
        (LPBYTE)value,/* data buffer */
        &size);       /* size of data buffer */

    if (ERROR_SUCCESS == errStatus)
    {
        valueP = FdoStringP::Format(L"%hs", (char *) value);
    }
    return valueP;
}

#endif

#define SUBKEYNAME_PREFIX           "Software\\ODBC\\ODBC.INI\\"
#define KEYNAME_DRIVER              "Driver"
#define KEYNAME_DBQ_MS_ACCESS       "DBQ"
#define KEYNAME_DEFDIR_TEXT         "DefaultDir"

#define DRIVER_NAME_ORACLENATIVE  L"SQORA32"
#define DRIVER_NAME_SQLSERVER     L"SQLSRV32.DLL"
#define DRIVER_NAME_MYSQL_WINDOWS L"MYODBC3.DLL"
#define DRIVER_NAME_MS_ACCESS     L"ODBCJT32.DLL"
#define DRIVER_NAME_MS_ACCESS2010 L"ACEODBC.DLL"

#define FILE_NAME_EXTENSION_ASC   L".ASC"
#define FILE_NAME_EXTENSION_CSV   L".CSV"
#define FILE_NAME_EXTENSION_TAB   L".TAB"
#define FILE_NAME_EXTENSION_TXT   L".TXT"

void FdoRdbmsOdbcConnectionInfo::SetProviderDataFromDsn()
{

#ifdef _WIN32

    // Setup the tag to be searched in the registry and try to find it in either
    // the section "HKEY_CURRENT_USER" or "HKEY_LOCAL_MACHINE".
    FdoStringP dsn = mConnection->GetDbiConnection()->GetDataSource();
    HKEY hkey = NULL;
    char subkeyName[ODBCDR_CONNECTION_SIZE];
    size_t subkeyNameSize = sizeof(subkeyName) / sizeof(subkeyName[0]);
    (void) _snprintf(subkeyName, subkeyNameSize-1, "%s%ls", SUBKEYNAME_PREFIX, (const wchar_t *)dsn);

    hkey = GetRegistryKey(HKEY_CURRENT_USER, subkeyName);
    if (NULL == hkey)
        hkey = GetRegistryKey(HKEY_LOCAL_MACHINE, subkeyName);

    // If the key was not found in the registry terminate the routine.

    if  (NULL == hkey)
        return;

    // The entry in the registry has been found. Get the driver name and use it
    // to identify the provider type. If the provider type represents a file-based
    // provider get the list of dependent files from the entry.
    mProviderType     = FdoProviderDatastoreType_Unknown;
    FdoStringP tmpDriver = GetRegistryValue(hkey, KEYNAME_DRIVER);
    FdoStringP driver = tmpDriver.Upper();

    if ((driver.Contains(DRIVER_NAME_ORACLENATIVE )) ||
        (driver.Contains(DRIVER_NAME_SQLSERVER    )) ||
        (driver.Contains(DRIVER_NAME_MYSQL_WINDOWS))    )
        mProviderType = FdoProviderDatastoreType_DatabaseServer;

    if (driver.Contains(DRIVER_NAME_MS_ACCESS) ||
		driver.Contains(DRIVER_NAME_MS_ACCESS2010))
    {
        mProviderType = FdoProviderDatastoreType_File;
        if (mDependentFiles == NULL)
            mDependentFiles = FdoStringCollection::Create();

        // The Access Driver is used for Access, Excel and Text references. In
        // case of Access and Excel, the dependent file can be retrieved using
        // the DBQ tag. If this is a text reference then it is necessary to get
        // the path information using the DefaultDir tag. All text files in the
        // directory are dependent files.
        // Note that the two tags (DBQ, DefaultDir) are exclusive meaning that
        // if one is present the other is not.

        FdoStringP dbqReference = GetRegistryValue(hkey, KEYNAME_DBQ_MS_ACCESS);
        if (dbqReference.GetLength() > 0)
            mDependentFiles->Add(dbqReference);
        else
        {
            std::vector<std::wstring> allFiles;
            FdoStringP pathInfo = GetRegistryValue(hkey, KEYNAME_DEFDIR_TEXT);
            if ((pathInfo != NULL) && (pathInfo.GetLength() > 0))
            {
                FdoCommonFile::GetAllFiles(pathInfo, allFiles);

                // The previous function returns all files in the named path.
                // The following loop filters out the text files and adds
                // those to the list of dependent files.
                size_t allFilesCount = allFiles.size();
                for (size_t i = 0; i < allFilesCount; i++)
                {
                    FdoStringP fileName = allFiles[i].c_str();
                    FdoStringP tmpFileName = fileName.Upper();
                    if ((tmpFileName.Contains(FILE_NAME_EXTENSION_ASC)) ||
                        (tmpFileName.Contains(FILE_NAME_EXTENSION_CSV)) ||
                        (tmpFileName.Contains(FILE_NAME_EXTENSION_TAB)) ||
                        (tmpFileName.Contains(FILE_NAME_EXTENSION_TXT))    )
                    {
                        // Add the path to the identified file.
                        FdoStringP absolutePathFileName =
                                    FdoStringP::Format(L"%ls/%ls",
                                                       (FdoString *) pathInfo,
                                                       (FdoString *) fileName);
                        mDependentFiles->Add(absolutePathFileName);
                    }
                }
            }
        }
    }

#endif

}

void FdoRdbmsOdbcConnectionInfo::SetProviderDataFromOdbcConnectionString(FdoStringP connectionString)
{
    ParseConnectionString (connectionString);
    SetProviderType();
    SetDependentFiles();
}

void FdoRdbmsOdbcConnectionInfo::ParseConnectionString(FdoStringP connectionString)
{
    FdoStringP tagName;
    FdoStringP tagValue;

    if (mConnectionStringTagNames == NULL)
        mConnectionStringTagNames = FdoStringCollection::Create();

    if (mConnectionStringTagValues == NULL)
        mConnectionStringTagValues = FdoStringCollection::Create();

    // A connection string consists of a sequence of structured pairs. The structure
    // of a pair can be described as "<tag>=<value>[;]". The final semicolon may not
    // be around if the pair represents the last one in the connection string.
    // The following code parses the connection string to identify each of those
    // pairs and then processes each pair to identify the tag and value.
    FdoStringP aConnectionString = FdoStringP::Format(L"%ls", (FdoString *)connectionString);
    while (aConnectionString.Contains(L"="))
    {
        tagName = aConnectionString.Left(L"=");
        aConnectionString = aConnectionString.Right(L"=");
        if (aConnectionString.Contains(L";"))
        {
            tagValue = aConnectionString.Left(L";");
            aConnectionString = aConnectionString.Right(L";");
        }
        else
        {
            tagValue = aConnectionString;
        }

        mConnectionStringTagNames->Add(tagName);
        mConnectionStringTagValues->Add(tagValue);
    }

}

#define DRIVER_TAG_NAME          L"DRIVER"

#define ACCESS_DRIVER_TAG_ID     L"(*.MDB"
#define EXCEL_DRIVER_TAG_ID      L"(*.XLS"
#define MYSQL_DRIVER_TAG_ID      L"MYSQL ODBC"
#define ORACLE_DRIVER_TAG_ID     L"ORACLE"
#define SQLSERVER_DRIVER_TAG_ID  L"SQL SERVER"

void FdoRdbmsOdbcConnectionInfo::SetProviderType()
{
    // The function loops through the parsed connection string data and
    // attempts to identify the driver. The provider type is set based
    // on this piece of information.
    FdoInt32 count = mConnectionStringTagNames->GetCount();
    for (FdoInt32 index = 0; index < count && mProviderType == FdoProviderDatastoreType_Unknown; index++)
    {
        FdoPtr<FdoStringElement> currentTagNameElement = mConnectionStringTagNames->GetItem(index);
        FdoStringP tmpTagName = currentTagNameElement->GetString();
        FdoStringP currentTagName = tmpTagName.Upper();
        if (currentTagName.Contains(DRIVER_TAG_NAME))
        {
            FdoPtr<FdoStringElement> currentTagValueElement = mConnectionStringTagValues->GetItem(index);
            FdoStringP tmpTagValue = currentTagValueElement->GetString();
            FdoStringP currentTagValue = tmpTagValue.Upper();
            if ((currentTagValue.Contains(MYSQL_DRIVER_TAG_ID    )) ||
                (currentTagValue.Contains(ORACLE_DRIVER_TAG_ID   )) ||
                (currentTagValue.Contains(SQLSERVER_DRIVER_TAG_ID))    )
                mProviderType = FdoProviderDatastoreType_DatabaseServer;

            if ((currentTagValue.Contains(ACCESS_DRIVER_TAG_ID)) ||
                (currentTagValue.Contains(EXCEL_DRIVER_TAG_ID ))    )
                mProviderType = FdoProviderDatastoreType_File;
        }
    }
}

#define DBQ_TAG_NAME  L"DBQ"

void FdoRdbmsOdbcConnectionInfo::SetDependentFiles()
{
    // If the current provider does not represent a file-based provider
    // terminate the routine as there are no dependent files.
    if (mProviderType != FdoProviderDatastoreType_File)
        return;

    // Generate the list of dependent files.
    if (mDependentFiles == NULL)
        mDependentFiles = FdoStringCollection::Create();

    // The function loops through the parsed connection string data and
    // attempts to identify the dependent file(s).
    bool tagValueFound = false;
    FdoInt32 count = mConnectionStringTagNames->GetCount();
    for (FdoInt32 index = 0; index < count && !tagValueFound; index++)
    {
        FdoPtr<FdoStringElement> currentTagNameElement = mConnectionStringTagNames->GetItem(index);
        FdoStringP tmpTagName = currentTagNameElement->GetString();
        FdoStringP currentTagName = tmpTagName.Upper();
        if (currentTagName.Contains(DBQ_TAG_NAME))
        {
            tagValueFound = true;
            FdoPtr<FdoStringElement> currentTagValueElement = mConnectionStringTagValues->GetItem(index);
            FdoStringP currentTagValue = currentTagValueElement->GetString();
            if (FdoCommonFile::IsAbsolutePath(currentTagValue))
                mDependentFiles->Add(currentTagValue);
            else
            {
                const wchar_t* currentTagValue1 = FdoCommonFile::GetAbsolutePath(currentTagValue);
                mDependentFiles->Add(currentTagValue1);
            }
        }
    }
}

