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
#include <float.h>

//for rmdir()
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "SdfExtHashMap.h"
#include "SdfConnection.h"

#include "SdfSelectAggregatesCommand.h"
#include <FdoCommonConnStringParser.h>

#include "utf8_.h"

#include "SdfCommandCapabilities.h"
#include "SdfConnectionCapabilities.h"
#include "SdfExpressionCapabilities.h"
#include "SdfFilterCapabilities.h"
#include "SdfSchemaCapabilities.h"
#include "SdfRasterCapabilities.h"
#include "SdfTopologyCapabilities.h"
#include "SdfGeometryCapabilities.h"
#include "SdfConnectionInfo.h"

#include "SdfDescribeSchema.h"
#include "SdfApplySchema.h"
#include "SdfInsert.h"
#include "SdfSelect.h"
#include "SdfUpdate.h"
#include "SdfDelete.h"
#include "SdfCreateSpatialContext.h"
#include "SdfGetSpatialContexts.h"
#include "SdfSelectAggregates.h"
#include "SdfCreateDatastore.h"
#include "SdfDeleteDataStore.h"

#include "SDF/SdfCommandType.h"
#include "SdfExtendedSelect.h"
#include "SdfImpExtendedSelect.h"
#include "SdfCreateSDFFile.h"

#include "SchemaDb.h"
#include "DataDb.h"
#include "KeyDb.h"
#include "ExInfoDb.h"
#include "DataIO.h"

#include "PropertyIndex.h"


SdfConnection::SdfConnection()
: m_mbsFullPath(NULL),
  mConnectionString ((wchar_t*)NULL),
  m_env(NULL),
  m_connState(FdoConnectionState_Closed),
  m_connInfo(NULL),
  m_bReadOnly(false),
  m_dbSchema(NULL),
  m_dbExtendedInfo(NULL)
{
    m_bCreate = false;
}

SdfConnection::~SdfConnection()
{
    CloseDatabases();

    if (m_mbsFullPath)
        delete [] m_mbsFullPath;

    FDO_SAFE_RELEASE(m_connInfo);
}

// dispose this object
void SdfConnection::Dispose()
{
    delete this;
}

SdfConnection* SdfConnection::Create()
{
    return new SdfConnection();
}


FdoIConnectionCapabilities* SdfConnection::GetConnectionCapabilities()
{
    return new SdfConnectionCapabilities();
}

FdoISchemaCapabilities* SdfConnection::GetSchemaCapabilities()
{
    return new SdfSchemaCapabilities();
}

FdoICommandCapabilities* SdfConnection::GetCommandCapabilities()
{
    return new SdfCommandCapabilities();
}

FdoIFilterCapabilities* SdfConnection::GetFilterCapabilities()
{
    return new SdfFilterCapabilities();
}

FdoIExpressionCapabilities* SdfConnection::GetExpressionCapabilities()
{
    return new SdfExpressionCapabilities();
}

FdoIRasterCapabilities* SdfConnection::GetRasterCapabilities()
{
    return new SdfRasterCapabilities();
}

FdoITopologyCapabilities* SdfConnection::GetTopologyCapabilities()
{
    return new SdfTopologyCapabilities();
}

FdoIGeometryCapabilities* SdfConnection::GetGeometryCapabilities()
{
    return new SdfGeometryCapabilities();
}


FdoString* SdfConnection::GetConnectionString()
{
    return mConnectionString;
}

void SdfConnection::SetConnectionString(FdoString* value)
{
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
    {
        // Update the connection string:
        mConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
        FdoPtr<FdoCommonConnPropDictionary> connDict = dynamic_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
        connDict->UpdateFromConnectionString(mConnectionString);
    }
    else
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_30_CONNECTION_OPEN)));
}

FdoIConnectionInfo* SdfConnection::GetConnectionInfo()
{
    if (m_connInfo == NULL)
        m_connInfo = new SdfConnectionInfo(this);

    // addref the connection info because we own it
    FDO_SAFE_ADDREF(m_connInfo);
    return m_connInfo;
}

FdoConnectionState SdfConnection::GetConnectionState()
{
    return m_connState;
}

FdoInt32 SdfConnection::GetConnectionTimeout()
{
    return 0;
}

void SdfConnection::SetConnectionTimeout(FdoInt32 value)
{
    throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_8_TIMEOUT_NOT_SUPPORTED)));
}

void SdfConnection::SetConfiguration(FdoIoStream* stream)
{
    throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_9_CONFIGURATION_NOT_SUPPORTED)));
}
FdoConnectionState SdfConnection::Open()
{
    return Open(NULL);
}

FdoConnectionState SdfConnection::Open( SdfCompareHandler* cmpHandler )
{
    //look at the needed property value in the ConnectionInfo first
    UpdateConnectionString();

    m_CompareHandler = FDO_SAFE_ADDREF( cmpHandler );
    
    if (m_mbsFullPath == NULL)
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_7_ERROR_CONNECTING_TO_FILE)));

    //if we are not in create SDF mode, check if the file client 
    //is trying to connect to exists and can be opened in the 
    //requested mode (read or read/write).
    if (m_bCreate == false && strcmp(m_mbsFullPath,":memory:") != 0 )
    {
#ifdef _WIN32
        FILE* f = _wfopen((const wchar_t*)FdoStringP(m_mbsFullPath), m_bReadOnly ? L"rb" : L"rb+");
#else
        FILE* f = fopen(m_mbsFullPath, m_bReadOnly ? "rb" : "rb+");
#endif
        if (!f)
            throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_50_NONEXISTING_FILE), "SDF connect failed. File does not exist or cannot be opened in specified access mode."));

        //while we have the file open, check if it is
        //an older SDF (2.1 or older) by reading the first few bytes.
        //all this is done in little-endian.
        unsigned long value;
        fread(&value, sizeof(unsigned long), 1, f);
        fclose(f);

        if((value & 0x0000ffff) == 0x00002c00)
        {
            // 2.x versions
            // Might also include 1.0, but it is not
            //guaranteed to be old SDF since other binary files
            //can have the same initial bytes

            //Note that for SDF3 (SQLite) files, the first 4 DWORDS are:
            //value[0] == 0x00000000
            //value[1] == 0x00000001
            //value[2] == 0x00000000
            //value[3] == 0x00053162
            //But the correct way to check version from now on is to 
            //open the Schema database and read the version number, which
            //we do later in this function.

            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_55_OLD_SDF),"SDF file appears to be version 2.1 or older. Please convert to SDF version 3."));
        }
    }

    //open database first
    m_env = new SQLiteDataBase();

    //open the database 
    if (m_env->open(SQLiteDB_CREATE, 0) != 0)
    {
        delete m_env;
        m_env = NULL;
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_2_ALLOCATE_ENV_HANDLE_FAILED)));
    }
    
    try
    {
        //get the SDF schema. It could be empty..., but we still have an object for it
        //since things like SDF version are stored in there
        m_dbSchema = new SchemaDb(m_env, m_mbsFullPath, m_bReadOnly);
    }
    catch (FdoException* e)
    {
        //if SchemaDb could not be opened at this point, the file is not a valid SDF 
        //file at all since we know the file is there and we opened it successfully at the 
        //beginning of this function.
        
        //free the database handle
        CloseDatabases();

        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_56_NOT_SDF), "File is not an SDF file, or is an SDF file with an unsupported version."), e);
    }

    try
    {
        // Get all the extended info table; the table might not exist (especially for older SDF files):
        m_dbExtendedInfo = new ExInfoDb(m_env, m_mbsFullPath, m_bReadOnly);
    }
    catch (FdoException* e)
    {
        // ignore error since older SDF files may not contain this table
        e->Release();
    }

    //initialize R-Trees and data databases
    InitDatabases();

    m_connState = FdoConnectionState_Open;

    return m_connState;
}

void SdfConnection::Close()
{
    m_connState = FdoConnectionState_Closed;

    CloseDatabases();

}

void SdfConnection::CloseDatabases()
{
    m_bCreate = false;
    DestroyDatabases();

    if (m_dbSchema)
    {
        delete m_dbSchema;
        m_dbSchema = NULL;
    }

    if (m_dbExtendedInfo)
    {
        delete m_dbExtendedInfo;
        m_dbExtendedInfo = NULL;
    }

    if (m_env)
    {
        m_env->close(0);

        delete m_env;
        m_env = NULL;

    }
}

FdoITransaction* SdfConnection::BeginTransaction()
{
    throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_28_TRANSACTIONS_NOT_SUPPORTED)));
}

FdoICommand* SdfConnection::CreateCommand(FdoInt32 commandType)
{
    switch(commandType)
    {
    case FdoCommandType_Insert:
        return new SdfInsert(this);
    case FdoCommandType_Delete:
        return new SdfDelete(this);
    case FdoCommandType_Update:
        return new SdfUpdate(this);
    case FdoCommandType_Select:
        return new SdfSelect(this);
    case FdoCommandType_SelectAggregates:
        return new SdfSelectAggregates(this);
    case FdoCommandType_DescribeSchema:
        return new SdfDescribeSchema(this);
    case FdoCommandType_ApplySchema:
        return new SdfApplySchema(this);
    case FdoCommandType_CreateSpatialContext:
        return new SdfCreateSpatialContext(this);
    case FdoCommandType_GetSpatialContexts:
        return new SdfGetSpatialContexts(this);
    case FdoCommandType_CreateDataStore:
        return new SdfCreateDataStore(this);
    case FdoCommandType_DestroyDataStore:
        return new SdfDeleteDataStore(this);

    case FdoCommandType_ExtendedSelect:
    case SdfCommandType_ExtendedSelect:
        return new SdfExtendedSelect( new SdfImpExtendedSelect( this ) );

    case SdfCommandType_CreateSDFFile:
        return new SdfCreateSDFFile(this);

    default:
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_3_COMMAND_NOT_SUPPORTED)));
    }

    return NULL;
}


FdoPhysicalSchemaMapping* SdfConnection::CreateSchemaMapping()
{
    throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_6_PHYSICAL_SCHEMA_MAPPING_NOT_SUPPORTED)));
}


// Processes a given connection string into an SDF file path
// Does error checking
void SdfConnection::UpdateConnectionString()
{
    //using the API makes sure a connection info will get created
    //if we don't have one
    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());

    const wchar_t* userPath = dict->GetProperty(PROP_NAME_FILE);

    _ASSERT(userPath);

#ifdef _WIN32
    // We convert the file name to a UTF8 string and then, at the low level SQLite os functions, we
    // convert it back to a wide char an pass it to the wide char version of windows files handling functions.
    // This way we ensure that a localized file name can be created using the various combination of
    // OS regional settings.
    size_t len = MAX_PATH+wcslen(userPath);
    char* fullPath = new char[len* 4 + 1];
    wchar_t* wFullPath = new wchar_t[len];
    wcscpy(wFullPath,userPath);
    if ( (wcscmp(L":memory:",userPath)!=0 && _wfullpath(wFullPath, userPath, len) == NULL) ||
        WideCharToMultiByte ( CP_UTF8, 0, wFullPath, (int)len, fullPath, (int)(len*4 + 1), NULL, NULL) == 0 )
    {
        delete [] wFullPath;
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_7_ERROR_CONNECTING_TO_FILE)));
    }
    delete [] wFullPath;
#else
       //convert path to multibyte
    size_t len = wcslen(userPath) * 4 + 1;
    char* mbsUserPath = new char[len];

    if( wcstombs(mbsUserPath, userPath, len) == -1 )
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_7_ERROR_CONNECTING_TO_FILE)));

    char* fullPath = new char[PATH_MAX];
    char *tmp = (char *) alloca(strlen(mbsUserPath)+1);
    char tmpChar;
    char *last;
    strcpy(tmp, mbsUserPath);
    last = strrchr(tmp, '/');
    if (last != NULL)
    {
        last++;
        tmpChar = *last;
        *last = '\0';
    }
    else
    {
        getcwd(fullPath, PATH_MAX);
        strcat(fullPath, "/");
        strcat(fullPath, mbsUserPath);
    }
    if (last != NULL && realpath(tmp, fullPath) == NULL)
    {
        delete [] mbsUserPath;
        delete [] fullPath;
        throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_7_ERROR_CONNECTING_TO_FILE)));
    }
    else
        delete [] mbsUserPath;

    if (last != NULL)
    {
        strcat(fullPath, "/");
        *last = tmpChar;
        strcat(fullPath, last);
    }
#endif

    //keep track of the absolute file path
    if (m_mbsFullPath) delete [] m_mbsFullPath;
    m_mbsFullPath = new char[strlen(fullPath)+1];
    strcpy(m_mbsFullPath, fullPath);


    delete[] fullPath;
    
    const wchar_t* rdonly = dict->GetProperty(PROP_NAME_RDONLY);

    if (_wcsnicmp(rdonly, RDONLY_TRUE, wcslen(RDONLY_TRUE)) == 0)
        m_bReadOnly = true;
    else
        m_bReadOnly = false;

    FdoCommonConnStringParser parser (NULL, GetConnectionString ());
    // check the validity of the connection string, i.e. it doesn’t contain unknown properties
    // e.g. DefaultFLocation instead of DefaultFileLocation
    if (!parser.IsConnStringValid())
        throw FdoConnectionException::Create(NlsMsgGetMain(SDFPROVIDER_83_INVALID_CONNECTION_STRING, "Invalid connection string '%1$ls'", GetConnectionString()));
    if (parser.HasInvalidProperties(dict))
        throw FdoConnectionException::Create(NlsMsgGetMain(SDFPROVIDER_84_INVALID_CONNECTION_PROPERTY_NAME, "Invalid connection property name '%1$ls'", parser.GetFirstInvalidPropertyName(dict)));
}


char* SdfConnection::GetFilename()
{
    return m_mbsFullPath;
}

char* SdfConnection::GetPath()
{
    return m_mbsEnvPath;
}

bool SdfConnection::GetReadOnly()
{
    return m_bReadOnly;
}


DataDb* SdfConnection::GetDataDb(FdoClassDefinition* clas)
{
    PropertyIndex *pi = GetPropertyIndex(clas);
    if (pi == NULL)
        return NULL;
    FdoClassDefinition* base = pi->GetBaseClass();
    return (DataDb*)m_hDataDbs[base];
}


KeyDb* SdfConnection::GetKeyDb(FdoClassDefinition* clas)
{
    PropertyIndex *pi = GetPropertyIndex(clas);
    if (pi == NULL)
        return NULL;
    FdoClassDefinition* base = pi->GetBaseClass();
    return (KeyDb*)m_hKeyDbs[base];
}

SchemaDb* SdfConnection::GetSchemaDb()
{
    return m_dbSchema;
}


//we do not addref in here -- caller will do that. 
//Breaks FDO convention, but is for internal use only...
FdoFeatureSchema* SdfConnection::GetSchema(FdoString *schemaName, bool bNewCopyFromFile)
{
    FdoFeatureSchema* schema = NULL;
    if (bNewCopyFromFile)
    {
        schema = m_dbSchema->ReadSchema();
        m_dbExtendedInfo->ReadExtendedInfo(schema);
    }
    else
        schema = m_dbSchema->GetSchema();

    // Validate schema name, if one is provided:
    if (schemaName && ((schema==NULL) || 0!=wcscmp(schema->GetName(), schemaName)))
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_58_INVALID_SCHEMANAME)));

    return schema; // no addref on purpose;
}

void SdfConnection::SetSchema(FdoFeatureSchema* schema, bool ignoreStates)
{
    // We need to keep the old schema around until destroy is done. Some data structure
    // used by destroy require the old schema.
    FdoPtr<FdoFeatureSchema> oldschema = FDO_SAFE_ADDREF(GetSchema());

    m_dbSchema->SetSchema( this, schema, ignoreStates);
    m_dbExtendedInfo->WriteExtendedSchemaInfo(schema);

    //need to regen the R-Tree objects since
    //the schema changed
    DestroyDatabases();
    InitDatabases();

    // Re-read all the schema, now that its been completed written (from both SCHEMA and EXINFO tables):
    FdoFeatureSchema* internalSchema = GetSchema();
    m_dbExtendedInfo->ReadExtendedInfo(internalSchema);
}


SdfRTree* SdfConnection::GetRTree(FdoClassDefinition* clas)
{
    PropertyIndex *pi = GetPropertyIndex(clas);
    if (pi == NULL)
        return NULL;
    FdoFeatureClass* basefc = pi->GetBaseFeatureClass();
    if (basefc)
        return (SdfRTree*)m_hRTrees[basefc];
    else
        return NULL;
}

PropertyIndex* SdfConnection::GetPropertyIndex(FdoClassDefinition* clas)
{
    return (PropertyIndex*)m_hPropertyIndices[clas];
}


//TODO: hardcoded strings here whould be moved to defines or consts
//open a database for the feature data of every class in the SDF+ file
//also initialize R-Tree objects for feature classes
//Note: any time the schema object changes, RTrees need to be
//updated -- because they are indexed by the pointer to the
//feature class
//This function also initializes PropertyIndex objects
//which map class properties to indices in the data records
void SdfConnection::InitDatabases()
{
    //no schema means no R-Trees
    if (GetSchema() == NULL)
        return;

    FdoPtr<FdoClassCollection> classes = GetSchema()->GetClasses();

    if (classes->GetCount() == 0)
        return;

    //do this for all classes
    for (int i=0; i<classes->GetCount(); i++)
    {
        FdoPtr<FdoClassDefinition> clas = classes->GetItem(i);

        //initialize the PropertyIndex table for the class
        PropertyIndex* pi = new PropertyIndex(clas, (unsigned)i);
        m_hPropertyIndices[clas.p] = pi;

        //we need a name for the database, unique for the class...
        //We use separate database for each base class, so the name
        //will be based on the base class
        FdoClassDefinition* baseClass = pi->GetBaseClass();
        FdoString* classname = baseClass->GetName();

        //Open or create the Data DB and insert into hash map
        //child classes share the same DB as the parent class
        if (baseClass != clas)
            m_hDataDbs[clas.p] = m_hDataDbs[baseClass];
        else
            m_hDataDbs[clas.p] = new DataDb(m_env, m_mbsFullPath, classname, m_bReadOnly, clas, pi, m_CompareHandler );


        //now initialize a KeyDb for the class
        //generate a string of the form "KEY:<class name>"
        if (baseClass != clas)
            m_hKeyDbs[clas.p] = m_hKeyDbs[baseClass];
        else
        {
            /* use integer key only if there is one identify property with int32 */
            bool bNoIntKey = true;
            try
            {
                FdoPtr<FdoDataPropertyDefinitionCollection> properties = DataIO::FindIDProps(clas);
                if (properties->GetCount() == 1)
                {
                    FdoPtr<FdoDataPropertyDefinition> property = properties->GetItem(0);
                    if (property->GetDataType() == FdoDataType_Int32)
                        bNoIntKey = false;
                }
            }
            catch (FdoException *exp)
            {
                // ignore exception
                exp->Release();
            }
            m_hKeyDbs[clas.p] = new KeyDb(m_env, m_mbsFullPath, classname, m_bReadOnly, bNoIntKey);
        }


        //now initialize R-Tree if we have a feature class
        //only feature classes have R-Trees, so enforce that here
        //we have one R-Tree per BASE feature class
        if (!pi->GetBaseFeatureClass())
        {
            m_hRTrees[clas.p] = 0;
            continue;
        }
        else
        {
            FdoFeatureClass* baseFeatureClass = pi->GetBaseFeatureClass();
            FdoString* fclassname = baseFeatureClass->GetName();
            //generate the RTree name
            //it is formed as follows:
            // RTREE:<name of class>

            //Open or create the RTree DB and insert into hash map
            if (baseFeatureClass != clas)
                m_hRTrees[clas.p] = m_hRTrees[baseClass];
            else
                m_hRTrees[clas.p] = new SdfRTree(m_env, m_mbsFullPath, classname, m_bReadOnly);
        }
    }
}


//close feature data databases
void SdfConnection::DestroyDatabases()
{
    // Make sure that all updates are committed in an orderly fashion; one class at a time
    // an with a proper low level transaction.
    Flush();

    //iterate over hashmap and close Data databases here..
    //stdext::hash_map<FdoClassDefinition*, DataDb*>::iterator dblist;
    stdext::hash_map<void*, void*>::iterator dblist;
    
    for (dblist = m_hDataDbs.begin(); dblist != m_hDataDbs.end(); dblist++)
    {
        //only delete db for every base class
        //child classes share the same db
        FdoPtr<FdoClassDefinition> base = ((FdoClassDefinition*)dblist->first)->GetBaseClass();
        if (base == NULL)
        {
            if (dblist->second != NULL)
            {
                delete (DataDb*)dblist->second;
                dblist->second = NULL;
            }
        }
    }
    
    m_hDataDbs.clear();

    //iterate over hashmap and close Key databases here..
    //stdext::hash_map<FdoClassDefinition*, KeyDb*>::iterator keylist;
    stdext::hash_map<void*, void*>::iterator keylist;
    
    for (keylist = m_hKeyDbs.begin(); keylist != m_hKeyDbs.end(); keylist++)
    {
        //only delete db for every base class
        //child classes share the same db
        FdoPtr<FdoClassDefinition> base = ((FdoClassDefinition*)keylist->first)->GetBaseClass();
        if (base == NULL)
        {
            if (keylist->second != NULL)
            {
                delete (KeyDb*)keylist->second;
                keylist->second = NULL;
            }
        }
    }

    m_hKeyDbs.clear();
    
    //iterate over hashmap and close R-Tree databases 
    //stdext::hash_map<FdoClassDefinition*, SdfRTree*>::iterator treelist;
    stdext::hash_map<void*, void*>::iterator treelist;

    
    for (treelist = m_hRTrees.begin(); treelist != m_hRTrees.end(); treelist++)
    {
        //only delete db for every base class
        //child classes share the same db
        FdoPtr<FdoClassDefinition> base = ((FdoClassDefinition*)treelist->first)->GetBaseClass();
        if (base == NULL)
        {
            if (treelist->second != NULL)
            {
                delete (SdfRTree*)treelist->second;
                treelist->second = NULL;
            }
        }
    }
    
    m_hRTrees.clear();
    
    //iterate over hashmap and tear down property indices
    //stdext::hash_map<FdoClassDefinition*, PropertyIndex*>::iterator indexlist;
    stdext::hash_map<void*, void*>::iterator indexlist;
    
    for (indexlist = m_hPropertyIndices.begin(); 
        indexlist != m_hPropertyIndices.end(); indexlist++)
    {
        if (indexlist->second != NULL)
        {
            delete (PropertyIndex*)indexlist->second;
            indexlist->second = NULL;
        }
    }

    m_hPropertyIndices.clear();
    
}

void SdfConnection::ReSyncData(FdoClassDefinition *clas)
{
    DataDb  *dataDb = GetDataDb(clas);
    SdfRTree* rt = GetRTree(clas);

    if( dataDb )
        dataDb->SyncIdPool();

    if( rt )
        rt->UpdateRootNode();
}


void SdfConnection::RegenIndex( FdoClassDefinition *clas, KeyDb* keys, DataDb  *dataDb )
{
    int ret = SQLiteDB_ERROR;
    PropertyIndex* propIndex = GetPropertyIndex(clas);

    // Drop the index
    keys->DropIndex();

    SQLiteData* currentKey = new SQLiteData();
    SQLiteData* currentData = new SQLiteData();
    
    if( dataDb->GetFirstFeature( currentKey, currentData ) != SQLiteDB_OK )
    {
        if( currentKey )
            delete currentKey;
        if( currentData )
            delete currentData;

        return;
    }

    
    BinaryReader *dataReader = new BinaryReader(NULL, 0);


    do
    {
        _ASSERT( currentKey->get_size() == 4 ); // This is the record number which is a UInt32
        REC_NO recno = *(REC_NO*)(currentKey->get_data());

        //construct feature key
        BinaryWriter wrtkey(64);
        SQLiteData key(NULL, 0);
        dataReader->Reset((unsigned char*)currentData->get_data(), currentData->get_size());
        if ( propIndex->HasAutoGen() )
            DataIO::MakeKey( clas, propIndex, dataReader, wrtkey, recno );
        else
            DataIO::MakeKey( clas, propIndex, dataReader, wrtkey );

        key.set_data(wrtkey.GetData());
        key.set_size(wrtkey.GetDataLen());

        keys->InsertKey(&key, recno);

    } while ( dataDb->GetNextFeature( currentKey, currentData ) == SQLiteDB_OK );

    if( currentKey )
        delete currentKey;
    if( currentData )
        delete currentData;

    if( dataReader)
        delete dataReader;
}

//
// Flush all data held in the memory cache to the file.
void SdfConnection::FlushAll( FdoClassDefinition *clas, bool forUpdate )
{
    DataDb  *dataDb = GetDataDb(clas);
    SdfRTree* rt = GetRTree(clas);
    KeyDb* keys = GetKeyDb(clas);

	if( ! (
            (keys && keys->NeedsAFlush(true) )     ||
	        (dataDb && dataDb->NeedsAFlush(true) ) ||
	        (rt && rt->NeedsAFlush(true) ) 
          )  )
	    return;

    GetDataBase()->begin_transaction();
    if( keys )
        keys->Flush();
    if( dataDb )
        dataDb->Flush();
    if( rt ) 
        rt->Flush();
    // We only re-gen the index if we are updating the database.
    // If this is called from a select, then we will not update
    // the database since the user is only reading this database.
    if( keys && forUpdate && keys->IndexNeedsRegen() )
    {
    
        RegenIndex( clas, keys, dataDb );
        keys->Flush();
        keys->Regened();
    }
    GetDataBase()->commit();
}

//returns overall data extents in the SDF native
//coordinate system.
FdoByteArray* SdfConnection::GetDataExtents()
{
    Bounds ret; //initialize to undefined bounds

    stdext::hash_map<void*, void*>::iterator iter;

    //add up bounds for all R-Trees
    for (iter = m_hRTrees.begin();iter != m_hRTrees.end(); iter++)
    {
        SdfRTree* rt = (SdfRTree*)iter->second;

        if ( rt ) {
            Bounds b1 = rt->GetBounds();

            if (!Bounds::IsUndefined(b1))
            {
                if (Bounds::IsUndefined(ret))
                {
                    ret = b1;
                }
                else
                {
                    Bounds b2 = Bounds::CombineBounds(b1, ret);
                    ret = b2;
                }
            }
        }
    }

    //if still no bounds, what to do?
    if (Bounds::IsUndefined(ret))
        return NULL;

    //generate FGF polygon and return as refcounted byte array
    double coords[10];
    coords[0] = ret.minx;
    coords[1] = ret.miny;
    coords[2] = ret.maxx;
    coords[3] = ret.miny;
    coords[4] = ret.maxx;
    coords[5] = ret.maxy;
    coords[6] = ret.minx;
    coords[7] = ret.maxy;
    coords[8] = ret.minx;
    coords[9] = ret.miny;


    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> lr = gf->CreateLinearRing(FdoDimensionality_XY, 10, coords);
    FdoPtr<FdoIPolygon> fgfgeom = gf->CreatePolygon(lr, NULL);
       
    return gf->GetFgf(fgfgeom);
}


bool SdfConnection::VersionIsAtLeast(unsigned char actualMajorVersion, unsigned char actualMinorVersion, unsigned char requiredMajorVersion, unsigned char requiredMinorVersion)
{
    return  (actualMajorVersion >  requiredMajorVersion)
        || ((actualMajorVersion == requiredMajorVersion) && (actualMinorVersion >= requiredMinorVersion));
}

bool SdfConnection::VersionIsAtMost(unsigned char actualMajorVersion, unsigned char actualMinorVersion, unsigned char requiredMajorVersion, unsigned char requiredMinorVersion)
{
    return  (actualMajorVersion <  requiredMajorVersion)
        || ((actualMajorVersion == requiredMajorVersion) && (actualMinorVersion <= requiredMinorVersion));
}


DataDb* SdfConnection::CreateNewDataDb( FdoClassDefinition* clas )
{
    PropertyIndex* pi = (PropertyIndex*)m_hPropertyIndices[clas];

    //we need a name for the database, unique for the class...
    //We use separate database for each base class, so the name
    //will be based on the base class
    FdoClassDefinition* baseClass = pi->GetBaseClass();
    FdoString* classname = baseClass->GetName();

	DataDb *db = CreateNewDataDb( clas, classname, pi );

	return db;
}

DataDb* SdfConnection::CreateNewDataDb( FdoClassDefinition* clas, FdoString* dbName, PropertyIndex* pi )
{
	DataDb *db = new DataDb(m_env, m_mbsFullPath, dbName, true, clas, pi, NULL );

    return db;
}

SdfSchemaMergeContext* SdfConnection::CreateMergeContext( 
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
)
{
    return SdfSchemaMergeContext::Create( this, oldSchemas, newSchema, ignoreStates );
}

void SdfConnection::Flush()
{
    stdext::hash_map<void*, void*>::iterator dblist;
    for (dblist = m_hDataDbs.begin(); dblist != m_hDataDbs.end(); dblist++)
    {
        FdoPtr<FdoClassDefinition> base = ((FdoClassDefinition*)dblist->first)->GetBaseClass();
        if (base == NULL)
        {
            FlushAll( (FdoClassDefinition*)dblist->first, true );
        }
    }
}
