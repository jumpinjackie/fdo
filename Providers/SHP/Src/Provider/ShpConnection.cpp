/*
 * 
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
#define SHP_MESSAGE_DEFINE
#include <../Message/Inc/ShpMessage.h>
#include "ShpSchemaUtilities.h"

#include <malloc.h>
#include <string.h>

#include <FdoCommonFile.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonOSUtil.h>
#include <ShpSelectAggregates.h>
#include <FdoCommonConnStringParser.h>

#ifdef _WIN32

#include <windows.h>
#include <crtdbg.h>

#include <limits.h>
#undef min
#undef max

static wchar_t module[MAX_PATH];
static wchar_t home_dir[MAX_PATH];

BOOL APIENTRY DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    DWORD nchars;
    wchar_t* last;
    BOOL ret;

    // UNCOMMENT THE FOLLOWING CODE TO HELP DISCOVER MEMORY LEAKS OR PREMATURE MEMORY DEALLOCATIONS;
    // WARNING: THIS CAN MAKE THE CODE RUN EXTREMELY SLOWLY IN CERTAIN PLACES!
    //
    //int debugFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    //_CrtSetDbgFlag(debugFlags | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);

    ret = TRUE;
    if (DLL_PROCESS_ATTACH == ul_reason_for_call)
    {
        // hModule - The value is the base address of the DLL.
        // The HINSTANCE of a DLL is the same as the HMODULE of the DLL,
        // so hinstDLL can be used in calls to functions that require a module handle.
        nchars = GetModuleFileNameW ((HINSTANCE)hModule, module, MAX_PATH);
        if (0 == nchars)
            ret = FALSE;
        else
        {   
            // scan the string for the last occurrence of a slash
            wcscpy (home_dir, module);
            last = wcsrchr (home_dir, L'\\');
            if (NULL == last)
                ret = FALSE;
            else
            {
                last++; // move past the slash
                *last = L'\0'; // null terminate it there
            }
        }
    }

    return (ret);
}
#endif // _WIN32


// external access to connection for client services
extern "C" FDOSHP_API FdoIConnection* CreateConnection ()
{
   return (new ShpConnection ());
}

// Globals to keep track of opened connections and the files to compress after
// delete command. The compression is triggered on the last connection close().
FdoCommonThreadMutex ShpConnection::mMutex;
int ShpConnection::mGlobalRefCount = 0;

std::vector<std::wstring> ShpConnGlobalFilesToCompress;

ShpConnection::ShpConnection (void) :
    mConnectionString ((wchar_t*)NULL),
    mConnectionState(FdoConnectionState_Closed),
    mSpatialContextColl (new ShpSpatialContextCollection ()),
    mConfigured (false),
    mLastEditedFileSet(NULL)
{
    // Create the default SC
    ShpSpatialContextP defltSpatialContext = new ShpSpatialContext();
    mSpatialContextColl->Add( defltSpatialContext );

    mMutex.Enter();
    mGlobalRefCount++;
    mMutex.Leave();
}

ShpConnection::~ShpConnection (void)
{
    Close ();

    // Do files compression (get rid of the deleted rows)
    CompressFileSets();

    mMutex.Enter();
    mGlobalRefCount--;
    mMutex.Leave();
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void ShpConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* ShpConnection::GetConnectionCapabilities ()
{
    return (new ShpConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* ShpConnection::GetSchemaCapabilities ()
{
    return (new ShpSchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* ShpConnection::GetCommandCapabilities ()
{
    return (new ShpCommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* ShpConnection::GetFilterCapabilities ()
{
    return (new ShpFilterCapabilities ());
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* ShpConnection::GetExpressionCapabilities ()
{
    return (new ShpExpressionCapabilities ());
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* ShpConnection::GetRasterCapabilities (void)
{
    return (new ShpRasterCapabilities ());
}

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* ShpConnection::GetTopologyCapabilities ()
{
    return (new ShpTopologyCapabilities ());
}


/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* ShpConnection::GetGeometryCapabilities()
{
    return (new ShpGeometryCapabilities());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* ShpConnection::GetConnectionString ()
{
    return (mConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void ShpConnection::SetConnectionString (FdoString* value)
{
    FdoConnectionState state = GetConnectionState();
    if (state == FdoConnectionState_Closed || state == FdoConnectionState_Pending)
    {
        // Update the connection string:
        mConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
        FdoPtr<FdoCommonConnPropDictionary> connDict = dynamic_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
        connDict->UpdateFromConnectionString(mConnectionString);
    }
    else
        throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_ALREADY_OPEN, "The connection is already open."));
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* ShpConnection::GetConnectionInfo ()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new ShpConnectionInfo (this);
    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState ShpConnection::GetConnectionState ()
{
    return (mConnectionState);
}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 ShpConnection::GetConnectionTimeout ()
{
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void ShpConnection::SetConnectionTimeout (FdoInt32 value)
{
    throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_TIMEOUT_UNSUPPORTED, "Connection timeout is not supported."));
}

/// <summary>Sets the connection paths
/// the File, Directory and Temporary</summary>
/// <param name="value">None</param> 
/// <returns>Returns nothing</returns> 
void ShpConnection::InitConnectionPaths()
{
    // Get the directory where the file(s) are
    ///////////////////////////////////////////////////////////////////////////

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());
    FdoStringP location = dictionary->GetProperty (CONNECTIONPROPERTY_DEFAULT_FILE_LOCATION);

#ifdef _WIN32
    location = location.Replace(L"/", L"\\");
    FdoString* plocation = (FdoString*)location;
    if (plocation != NULL)
    {
        bool isRemote = (location.GetLength () > 2 && plocation[0] == L'\\' && plocation[1] == L'\\');
        location = location.Replace (L"\\\\", L"\\");
        if (isRemote == true)
            location = (FdoStringP)L"\\" + location;
    }
#else
    location = location.Replace(L"\\", L"/");
    location = location.Replace(L"//", L"/");
#endif

    // make a copy to play around with
    wchar_t* dir = (wchar_t*)alloca (sizeof (wchar_t) *(wcslen (location) + 2)); // 2 because we may need to add delimiter
    wcscpy (dir, location);
    FdoCommonStringUtil::StringTrim (dir);
    location = dir;
    size_t length = wcslen (dir);

    // store the file and/or directory provided:
    if (!FdoCommonFile::FileExists (dir))
    {
        if (length > wcslen (SHP_EXTENSION))
        {
            // is this a file or a folder?
            if (0 == FdoCommonStringUtil::StringCompareNoCase (SHP_EXTENSION, &dir[length - wcslen (SHP_EXTENSION)]))
            {   // presumably it's a file
                mFile = dir;
                mDirectory = L"";
                wchar_t* delim = wcsrchr(dir, FILE_PATH_DELIMITER);
                if (delim == NULL)
                {
                    // Set to current directory if none specified:
                    dir = (wchar_t*)alloca(sizeof(wchar_t)*3);
                    dir[0] = L'.';
                    dir[1] = FILE_PATH_DELIMITER;
                    dir[2] = L'\0';
                }
                else
                {
                    *(++delim) = L'\0';
                }

                mDirectory = dir;
            }
            else
            {
                mDirectory = dir;
                mFile = L"";
            }
        }
    }
    else if (FdoCommonFile::IsDirectory(dir))
        mFile = L"";
    else  // user specified a single file to connect to
    {
        mFile = dir;

        wchar_t* delim = wcsrchr(dir, FILE_PATH_DELIMITER);
        if (delim == NULL)
        {
            // Set to current directory if none specified:
            dir = (wchar_t*)alloca(sizeof(wchar_t)*3);
            dir[0] = L'.';
            dir[1] = FILE_PATH_DELIMITER;
            dir[2] = L'\0';
        }
        else
        {
            *(++delim) = L'\0';
        }

        mDirectory = dir;
    }

    if (NULL == GetFile ())
    {
        // strip off a wrong delimiter
        if (length == 0)
        {
            // Set to current directory if none specified:
            dir = (wchar_t*)alloca(sizeof(wchar_t)*3);
            dir[0] = L'.';
            dir[1] = FILE_PATH_DELIMITER;
            dir[2] = L'\0';
            length = 2;
        }

        // add the delimiter if not already present
        if (FILE_PATH_DELIMITER != dir[length - 1])
        {
            dir[length] = FILE_PATH_DELIMITER;
            dir[length + 1] = L'\0';
        }

        // Store the directory:
        mDirectory = dir;
    }
    if (NULL != GetDirectory () && !FdoCommonFile::FileExists (GetDirectory()))
        throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_LOCATION_NOT_EXIST, "The directory '%1$ls' does not exist.", GetDirectory()));
    if (NULL != GetFile () && !FdoCommonFile::FileExists (GetFile ()))
        throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_LOCATION_NOT_EXIST, "The File '%1$ls' does not exist.", GetFile()));

    // Get the temporary directory
    ///////////////////////////////////////////////////////////////////////////

    FdoStringP temporary = dictionary->GetProperty (CONNECTIONPROPERTY_TEMPORARY_FILE_LOCATION);

    // make a copy to play around with
    wchar_t* tmp = (wchar_t*)alloca (sizeof (wchar_t) *(wcslen (temporary) + 2)); // 2 because we may need to add delimiter
    wcscpy (tmp, temporary);

    // check it for validity
    FdoCommonStringUtil::StringTrim (tmp);
    length = wcslen (tmp);
    if (0 != length)
    {
        if (!FdoCommonFile::FileExists (tmp))
            throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_LOCATION_NOT_EXIST, "The directory '%1$ls' does not exist.", tmp));

        // strip off a wrong delimiter
        if (FILE_PATH_DELIMITER2 == tmp[length - 1])
        {
            tmp[length - 1] = L'\0';
            length--;
        }

        // add the delimiter if not already present
        if (FILE_PATH_DELIMITER != tmp[length - 1])
        {
            tmp[length] = FILE_PATH_DELIMITER;
            tmp[length + 1] = L'\0';
        }
        // remember the temporary directory
        mTemporary = tmp;
    }
    else
        mTemporary = L"";
    FdoCommonConnStringParser parser (NULL, GetConnectionString ());
    // check the validity of the connection string, i.e. it doesn’t contain unknown properties
    // e.g. DefaultFLocation instead of DefaultFileLocation
    if (!parser.IsConnStringValid())
        throw FdoException::Create (NlsMsgGet(SHP_INVALID_CONNECTION_STRING, "Invalid connection string '%1$ls'", GetConnectionString ()));
    if (parser.HasInvalidProperties(dictionary))
        throw FdoException::Create (NlsMsgGet(SHP_INVALID_CONNECTION_PROPERTY_NAME, "Invalid connection property name '%1$ls'", parser.GetFirstInvalidPropertyName (dictionary)));
}

/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState ShpConnection::Open ()
{
    if (GetConnectionState() == FdoConnectionState_Open)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_103_CONNECTION_ALREADY_OPEN)));

    InitConnectionPaths();
    
    // if the schema.xml file exists, set it as the configuration
    if (!IsConfigured () && (NULL == GetFile ()))
    {
        wchar_t* config = (wchar_t*)alloca (sizeof (wchar_t) *(wcslen (GetDirectory ()) + wcslen (DEFAULT_SCHEMA_XML) + 1));
        wcscpy (config, GetDirectory ());
        wcscat (config, DEFAULT_SCHEMA_XML);
        if (FdoCommonFile::FileExists (config))
        {
            FdoXmlReaderP reader = FdoXmlReader::Create (config);
            FdoIoStreamP stream = reader->GetStream ();
            SetConfiguration (stream);
            mConfigured = false; // schema.xml is not a real configuration
        }
    }

    // Connection is now open:
    mConnectionState = FdoConnectionState_Open;

    return (GetConnectionState ());
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void ShpConnection::Close ()
{
    // Clear the LogicalPhysical/physical/override caches:
    mLpSchemas = NULL;
    mPhysicalSchema = NULL;

    // Clear the config file cache:
    mConfigLogicalSchemas = NULL;
    mConfigSchemaMappings = NULL;
    mConfigured = false;

    mFile = L"";
    mDirectory = L"";
    mLastEditedFileSet = NULL;

    // Reset the Spatial Contexts collection. Create the default SC.
    mSpatialContextColl = new ShpSpatialContextCollection();
    ShpSpatialContextP defltSpatialContext = new ShpSpatialContext();
    mSpatialContextColl->Add( defltSpatialContext );

    // Connection is now closed:
    mConnectionState = FdoConnectionState_Closed;
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* ShpConnection::BeginTransaction ()
{
    throw FdoException::Create(NlsMsgGet(SHP_CONNECTION_TRANSACTIONS_NOT_SUPPORTED, "Shp Provider does not support transactions."));
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* ShpConnection::CreateCommand (FdoInt32 commandType)
{
    FdoPtr<FdoICommand> ret;

    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
        throw FdoException::Create(NlsMsgGet(SHP_CONNECTION_INVALID, "Connection is invalid."));
    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new ShpSelectCommand (this);
            break;
        case FdoCommandType_SelectAggregates:
            ret = new ShpSelectAggregates (this);
            break;
        case FdoCommandType_DescribeSchema:
            ret = new ShpDescribeSchemaCommand (this);
            break;
        case FdoCommandType_DescribeSchemaMapping:
            ret = new ShpDescribeSchemaMappingCommand (this);
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = new ShpGetSpatialContextsCommand (this);
            break;
        case FdoCommandType_Insert:
            ret = new ShpInsertCommand (this);
            break;
        case FdoCommandType_Update:
            ret = new ShpUpdateCommand (this);
            break;
        case FdoCommandType_Delete:
            ret = new ShpDeleteCommand (this);
            break;
        case FdoCommandType_ApplySchema:
            ret = new ShpApplySchemaCommand (this);
            break;
        case FdoCommandType_DestroySchema:
            ret = new ShpDestroySchemaCommand (this);
            break;
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_102_COMMAND_NOT_SUPPORTED, "The command '%1$ls' is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoCommandTypeToString (commandType))));
    }

    return (FDO_SAFE_ADDREF (ret.p));
}


/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* ShpConnection::CreateSchemaMapping()
{
    FdoShpOvPhysicalSchemaMappingP newPhysicalSchemaMapping = FdoShpOvPhysicalSchemaMapping::Create();
    return FDO_SAFE_ADDREF(newPhysicalSchemaMapping.p);
}


/// <summary>Sets the XML configuration stream used to configure the Data Store. 
/// SetConfiguration can only be called while the connection is closed.</summary>
/// <param name="configStream">Input the XML configuration stream</param> 
/// <returns>Returns nothing</returns> 
void ShpConnection::SetConfiguration(FdoIoStream* configStream)
{
    VALIDATE_ARGUMENT(configStream);

    if (GetConnectionState() != FdoConnectionState_Closed) {
        throw FdoConnectionException::Create(NlsMsgGet(SHP_CONNECTION_ALREADY_OPEN, "The connection is already open."));
    }


    // Read Spatial Context from XML stream:
    //////////////////////////////////////////////////////////////////////////

    configStream->Reset();

    FdoXmlReaderP reader = FdoXmlReader::Create(configStream);
    FdoXmlSpatialContextReaderP scReader = FdoXmlSpatialContextReader::Create(reader);


    ShpSpatialContextP defltSpatialContext = mSpatialContextColl->GetItem(0);

    if (scReader->ReadNext()) {
        defltSpatialContext->SetName(scReader->GetName());
        defltSpatialContext->SetDescription(scReader->GetDescription());
        defltSpatialContext->SetCoordSysName(scReader->GetCoordinateSystem());
        defltSpatialContext->SetCoordinateSystemWkt(scReader->GetCoordinateSystemWkt());
        defltSpatialContext->SetExtent(FdoPtr<FdoByteArray> (scReader->GetExtent()));
        defltSpatialContext->SetExtentType(scReader->GetExtentType());
        defltSpatialContext->SetXYTolerance(scReader->GetXYTolerance());
        defltSpatialContext->SetZTolerance(scReader->GetZTolerance());
		defltSpatialContext->SetIsFromConfigFile(true);
    }
    
    if (scReader->ReadNext()) {
        throw FdoException::Create(
            NlsMsgGet(
                SHP_SPATIALCONTEXT_INVALIDCONFIGURATION, 
                "Cannot have more than one spatial context in a configuration file."
            )
        );
    }

    // Read logical FDO schemas from XML stream:
    //////////////////////////////////////////////////////////////////////////

    configStream->Reset();

    reader = FdoXmlReader::Create(configStream);

    FDO_SAFE_RELEASE(mConfigLogicalSchemas.p);
    mConfigLogicalSchemas = FdoFeatureSchemaCollection::Create(NULL);

    try 
    {
        mConfigLogicalSchemas->ReadXml(reader);
    } 
    catch (FdoException* e) 
    {
        FdoSchemaExceptionP schemaException = 
            FdoSchemaException::Create(
                NlsMsgGet(SHP_SCHEMA_FAIL_READ_FEATURE_SCHEMAS, "Failed to de-serialize the feature schemas from the configuration."),
                e);
        e->Release();
        throw schemaException;
    }


    // Read FDO schema mappings from XML stream:
    //////////////////////////////////////////////////////////////////////////

    configStream->Reset();

    reader = FdoXmlReader::Create(configStream);

    FdoSchemaMappingsP localSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();
    
    try 
    {
        localSchemaMappings->ReadXml(reader);
    } 
    catch (FdoException* e) 
    {
        FdoSchemaExceptionP schemaException = 
            FdoSchemaException::Create(
                NlsMsgGet(SHP_SCHEMA_FAIL_READ_SCHEMA_MAPPINGS, "Failed to de-serialize the schema mappings from the configuration."),
                e);
        e->Release();
        throw schemaException;
    }

    // Create an internal schema mappings collection to hold the SHP specific mappings
    ////////////////////////////////////////////////////////////////////////////////////

    FDO_SAFE_RELEASE(mConfigSchemaMappings.p);
    mConfigSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();

    // Iterate the deserialized mappings and add only the SHP mappings to the internal collection
    /////////////////////////////////////////////////////////////////////////////////////////////////
    for (FdoInt32 i=0; i<localSchemaMappings->GetCount(); i++) {
        FdoPhysicalSchemaMappingP schemaMapping = localSchemaMappings->GetItem(i);
        
        FdoStringsP sProviderIds = FdoStringCollection::Create(schemaMapping->GetProvider(), L".");
        if (sProviderIds->GetCount() < 3) {
            throw FdoException::Create(
                NlsMsgGet(SHP_INVALID_PHYSICAL_SCHEMA_PROVIDER_NAME, "Invalid Schema Mapping Provider Name"));
        }

        FdoStringP sAuthor = FdoStringElementP(sProviderIds->GetItem(0))->GetString();
        FdoStringP sName = FdoStringElementP(sProviderIds->GetItem(1))->GetString();
        FdoStringP sVersion = FdoStringElementP(sProviderIds->GetItem(2))->GetString();
        FdoInt32 iVersion = FdoCommonOSUtil::wtoi(sVersion);
 
        if (sAuthor == SHP_PROVIDER_AUTHOR && sName == SHP_PROVIDER_SHORTNAME && iVersion >= 3) {
            FdoShpOvPhysicalSchemaMapping* shpSchemaMapping = static_cast<FdoShpOvPhysicalSchemaMapping*>(schemaMapping.p);

            // Find corresponding shapefile name for the schema mapping:
            FdoPtr<FdoShpOvClassCollection> classMappings = shpSchemaMapping->GetClasses();
            for (FdoInt32 j=0; j<classMappings->GetCount(); j++)
            {
                FdoPtr<FdoShpOvClassDefinition> classMapping = classMappings->GetItem (j);
                FdoString* pNameShape = classMapping->GetShapeFile ();
                if (pNameShape == NULL || wcslen (pNameShape) < 5 || FdoCommonFile::IsAbsolutePath (pNameShape))
                    continue;
                FdoStringP pNameFile;
                if (GetDirectory() == NULL)
                    InitConnectionPaths ();
                if (GetDirectory() != NULL && !classMapping->IsPathUpdated ())
                {
                    pNameFile = GetDirectory();
                    pNameFile += pNameShape;
                    classMapping->SetShapeFile (pNameFile);
                    classMapping->SetPathUpdated ();
                }
            }
            mConfigSchemaMappings->Add(shpSchemaMapping);
        }
    }

    mConfigured = true;
}

ShpSpatialContextCollection* ShpConnection::GetSpatialContexts ( bool bDynamic )
{
    // If not required to recompute the extents, quick exit
    if ( !bDynamic )
        return FDO_SAFE_ADDREF( mSpatialContextColl.p );

    // Make sure the Coordinate System definition (from PRJ files) is read in.
    FdoPtr<ShpPhysicalSchema> physicalSchema = GetPhysicalSchema();

    // Check the default Spatial Context - it is special.
    ShpSpatialContextP  dfltSpatialContext = mSpatialContextColl->GetItem(0);
    FdoStringP          dfltSpatialContextName = dfltSpatialContext->GetName();
	bool				dfltSpatialContextUsed = false;

    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();

    // The default is Static, cf. spec. This is not useful and it has been
    // agreed the spec. should be changed. For the time being, change the
    // type on the fly, allowing for retrieving the actual extents of geometries.
    // In the presence of a configuration file, disallow this.
    bool    dfltScFromConfig = dfltSpatialContext->GetIsFromConfigFile();
    
    // Mark the extents as 'dirty' for all Spatial Contexts
    for ( int i = 0; i < mSpatialContextColl->GetCount(); i++ )
    {
        FdoPtr<ShpSpatialContext>	spatialContext = mSpatialContextColl->GetItem(i);
        spatialContext->SetIsExtentUpdated(false);
    }

    // Recompute the extents with the actual extends of all Shape files in the dataset,
    // grouped by the common Coordinate System name.
    FdoPtr<ShpLpFeatureSchemaCollection> schemas = GetLpSchemas();
    int count = schemas->GetCount();

    for ( int i = 0; i < count; i++ )
    {
        FdoPtr<ShpLpFeatureSchema> lpSchema = schemas->GetItem (i);
        FdoPtr<ShpLpClassDefinitionCollection> classes = lpSchema->GetLpClasses ();
        int class_count = classes->GetCount ();

        bool                        changed = false;
        FdoSpatialContextExtentType type;

        for (int j = 0; j < class_count; j++)
        {
            FdoPtr<ShpLpClassDefinition> lpClass = classes->GetItem (j);

            ShpFileSet* fileset = lpClass->GetPhysicalFileSet ();
            
            // Get the spatial context from the PRJ file. If there is no PRJ file,
            // then it's the default one.
            ShapePRJ*    prj = fileset->GetPrjFile();
            FdoStringP    scname;

            if ( prj )
                scname = prj->GetCoordSysName();
            else
			{
                scname = dfltSpatialContextName;
				dfltSpatialContextUsed = true;
			}

            // Don't update it if from configuration file.
            if ( ( wcscmp( scname, dfltSpatialContextName) == 0 ) && dfltScFromConfig )
                continue;

            // Get the actual extents of the geometries from the SHP file
            double f1 = fileset->GetShapeFile()->GetBoundingBoxMinX();
            double f2 = fileset->GetShapeFile()->GetBoundingBoxMinY();
            double f3 = fileset->GetShapeFile()->GetBoundingBoxMaxX();
            double f4 = fileset->GetShapeFile()->GetBoundingBoxMaxY();

            // Skip if no data
            if ( f1 == fNO_DATA || f2 == fNO_DATA || f3 == fNO_DATA || f4 == fNO_DATA )
                continue;

            // Find the corresponding Spatial Context in the collection
            FdoPtr<ShpSpatialContext>	spatialContext = mSpatialContextColl->GetItem( scname );
            bool    extUpd = spatialContext->GetIsExtentUpdated();

            FdoPtr<FdoByteArray>  fgf_box = spatialContext->GetExtent();         
            FdoPtr<FdoIGeometry>  geom_box = factory->CreateGeometryFromFgf( fgf_box );
            FdoPtr<FdoIEnvelope>  box = geom_box->GetEnvelope();       

            // 1st time do initialization, update otherwise
            double min_x = min( f1, extUpd? box->GetMinX() : f1 );
            double min_y = min( f2, extUpd? box->GetMinY() : f2 );
            double max_x = max( f3, extUpd? box->GetMaxX() : f3 );
            double max_y = max( f4, extUpd? box->GetMaxY() : f4 );

            if ( !extUpd )
                spatialContext->SetIsExtentUpdated( true );

            type = FdoSpatialContextExtentType_Dynamic;

            // Update the SC extents
            FdoPtr<FdoIEnvelope> envelope = factory->CreateEnvelopeXY ( min_x, min_y, max_x, max_y );
            FdoPtr<FdoIGeometry> geometry = factory->CreateGeometry (envelope);
            FdoPtr<FdoByteArray> fgf = factory->GetFgf (geometry);
            spatialContext->SetExtent( fgf );
            spatialContext->SetExtentType( type );
        }     
    }

	// Take the opportunity and remove the hard coded default spatial context in case: 
	// it is not used, it is not the only one and not from configuration file. 
	if ( ( wcscmp( dfltSpatialContext->GetName(), SPATIALCONTEXT_DEFAULT_NAME) == 0 ) && 
		  !dfltSpatialContextUsed && 
		  ( mSpatialContextColl->GetCount() > 1 ) && 
		  !dfltScFromConfig )
	{
		mSpatialContextColl->RemoveAt(0);
	}

    return FDO_SAFE_ADDREF( mSpatialContextColl.p );
}


FdoString* ShpConnection::GetDirectory ()
{
    return (mDirectory == L"" ? NULL : (FdoString*)mDirectory);
}

FdoString* ShpConnection::GetFile ()
{
    return (mFile == L"" ? NULL : (FdoString*)mFile);
}

FdoString* ShpConnection::GetTemporary ()
{
    return (mTemporary == L"" ? NULL : (FdoString*)mTemporary);
}

void ShpConnection::AddPhysicalShapefileNames(FdoStringsP& physicalShapefileNames)
{
    if (NULL != mConfigSchemaMappings)
    {
        FdoInt32 count = mConfigSchemaMappings->GetCount();

        for (FdoInt32 i=0; i<count; i++)
        {
            FdoPhysicalSchemaMappingP fdoMapping = mConfigSchemaMappings->GetItem(i);
            VALIDATE_POINTER(fdoMapping);

            if (0==FdoCommonOSUtil::wcsicmp(fdoMapping->GetProvider(), SHP_PROVIDER_NAME))
            {
                FdoShpOvPhysicalSchemaMapping * shpMapping = static_cast<FdoShpOvPhysicalSchemaMapping *>(fdoMapping.p);

                // Find corresponding shapefile name:
                FdoPtr<FdoShpOvClassCollection> classMappings = shpMapping->GetClasses();
                for (FdoInt32 j=0; j<classMappings->GetCount(); j++)
                {
                    FdoStringP pNameFile;
                    FdoPtr<FdoShpOvClassDefinition> classMapping = classMappings->GetItem (j);
                    FdoString * pActualName = classMapping->GetShapeFile ();
                    if (pActualName == NULL || wcslen (pActualName) == 0)
                    {
                        // Create a shape name for the classes from schema.xml which doesn't have shape file.
                        pNameFile = GetDirectory ();
                        pNameFile += classMapping->GetName ();
                        pNameFile += L".shp";
                    }
                    else
                    {
                        if(!FdoCommonFile::IsAbsolutePath (pActualName))
                        {
                            if (!classMapping->IsPathUpdated () || GetDirectory() == NULL)
                            {
                                pNameFile = GetDirectory();
                                pNameFile += classMapping->GetShapeFile ();
                            }
                            else
                                pNameFile = classMapping->GetShapeFile ();

                            classMapping->SetPathUpdated ();
                        }
                        else
                            pNameFile = pActualName;
                    }

                    FdoString* base = ShpFileSet::CreateBaseName (pNameFile);
                    if (-1 == physicalShapefileNames->IndexOf (base))
                        physicalShapefileNames->Add (base);
                    delete[] base;
                }
            }
        }
    }
}

ShpPhysicalSchema* ShpConnection::GetPhysicalSchema(void)
{
    if (mPhysicalSchema == NULL)
    {
        mPhysicalSchema = new ShpPhysicalSchema(GetTemporary());

        // Get a list of all referenced shapefiles:
        FdoStringsP         physicalShapefileNames = FdoStringCollection::Create();
        if (IsConfigured ())  // Get the shapefiles from the overrides
        {
            AddPhysicalShapefileNames(physicalShapefileNames);
        }
        else  // Get the shapefiles from the specified directory or file
        {
            if (GetFile() != NULL)
            {
                FdoString* base = ShpFileSet::CreateBaseName (GetFile ());
                physicalShapefileNames->Add (base);
                delete[] base;
            }
            else if (GetDirectory() != NULL)
            {
                bool bSchemaExist = false;
                // if the schema.xml file exists, set it as the configuration
                if (!IsConfigured () && (NULL == GetFile ()))
                {
                    wchar_t* config = (wchar_t*)alloca (sizeof (wchar_t) *(wcslen (GetDirectory ()) + wcslen (DEFAULT_SCHEMA_XML) + 1));
                    wcscpy (config, GetDirectory ());
                    wcscat (config, DEFAULT_SCHEMA_XML);
                    bSchemaExist = FdoCommonFile::FileExists (config);
                }

                if (bSchemaExist)
                {
                    AddPhysicalShapefileNames(physicalShapefileNames);
                }
                else
                {
                    std::vector<std::wstring> files;
                    FdoCommonFile::GetAllFiles (GetDirectory (), files);
                    int count = (int)files.size ();
                    size_t ext1_len = ELEMENTS(SHP_EXTENSION) - 1;
                    size_t ext2_len = ELEMENTS(DBF_EXTENSION) - 1;
                    for (int i = 0; i < count; i++)
                    {
                        const wchar_t* name;
                        size_t length;
                        std::wstring path;
                        FdoString* base;
                        
                        name = files[i].c_str ();
                        length = wcslen (name);
                        if (((ext1_len < length) && (0 == FdoCommonOSUtil::wcsicmp (&(name[length - ext1_len]), SHP_EXTENSION)))
                            || ((ext2_len < length) && (0 == FdoCommonOSUtil::wcsicmp (&(name[length - ext2_len]), DBF_EXTENSION))))
                        {
                            path = GetDirectory ();
                            path += name;
                            base = ShpFileSet::CreateBaseName (path.c_str ());
                            if (-1 == physicalShapefileNames->IndexOf (base))
                                physicalShapefileNames->Add (base);
                            delete[] base;
                        }
                    }
                }
            }
        }


        // Create one 'default' physical schema if there are physical files or no overrides:
        FdoInt32 count = physicalShapefileNames->GetCount ();
        if ((0 < count) || (mConfigSchemaMappings == NULL))
        {
            // Add all the shapefiles to the physical schema:
            for (FdoInt32 i=0; i < count; i++)
            {
                FdoPtr<FdoStringElement> stringElement = physicalShapefileNames->GetItem(i);
                FdoStringP physicalShapefileName = stringElement->GetString();
                mPhysicalSchema->AddFileSet((const wchar_t*)physicalShapefileName);
            }

            // Extract the PRJ info from file sets and store it into the Spatial Context Collection.
            for ( FdoInt32 i=0; i< mPhysicalSchema->GetFileSetCount(); i++)
            {
                ShapePRJ*  prj = mPhysicalSchema->GetFileSet(i)->GetPrjFile();

                if ( NULL != prj )
                {
                    FdoStringP    wkt = prj->GetWKT();                
                    bool        found = false;
                    FdoStringP    cs_name = prj->GetCoordSysName();

                    // Check if an identical one as WKT exists
                    for ( FdoInt32 j = 0; j < mSpatialContextColl->GetCount() && !found; j++ )
                    {
                        FdoPtr<ShpSpatialContext> sp = mSpatialContextColl->GetItem(j);
                        found = ( wkt == sp->GetCoordinateSystemWkt() );
                    }

                    // Append the new Coordinate System
                    if ( !found )
                    {
                        FdoPtr<ShpSpatialContext> new_sp = new ShpSpatialContext();
                        FdoInt32 idxGenName = 1;
                        FdoStringP newName = cs_name;
                        // search for SC name duplicates
                        while (mSpatialContextColl->FindItem(newName))
                        {
                            newName = FdoStringP::Format(L"%ls_%d", (FdoString*)cs_name, idxGenName);
                            idxGenName++;
                        }
                        // set the new SC name only in case duplicate found
                        if (idxGenName != 1)
                            prj->SetTempCoordSysName( newName );
                        new_sp->SetName( newName );
                        new_sp->SetCoordSysName( cs_name );
                        new_sp->SetCoordinateSystemWkt( wkt );

                        mSpatialContextColl->Add( new_sp );
                    }                    
                }
            }
        }
    }

    // Return the singleton physical schema:
    return FDO_SAFE_ADDREF(mPhysicalSchema.p);
}

ShpLpFeatureSchemaCollection* ShpConnection::GetLpSchemas(void)
{
    if (mLpSchemas == NULL)
    {
        FdoPtr<ShpPhysicalSchema> pPhysicalSchema = GetPhysicalSchema ();
        mLpSchemas = new ShpLpFeatureSchemaCollection(
            this,
            pPhysicalSchema,
            mConfigLogicalSchemas,
            mConfigSchemaMappings);

        // Associate all geometric properties to the current spatial context:
        // TODO: this may need to be done elsewhere, when the configuration file handling is in place:
        //TODO:FdoSdpSchemaUtil::SetSpatialContextAssociations(this, m_Connection->GetSpatialContext());
    }

    return FDO_SAFE_ADDREF(mLpSchemas.p);
}

// NOTE: this method returns all LogicalPhysical schemas regardless of whether or not a schema name is given;
//       This is important when there is no schema name passed in, which occurs when someone asks for a class
//       but the classname isn't qualified with a schema name.
ShpLpFeatureSchemaCollection* ShpConnection::GetLpSchema(const wchar_t *logicalSchemaName)
{
    FdoPtr<ShpLpFeatureSchemaCollection> lpSchemas = GetLpSchemas();
    FdoPtr<ShpLpFeatureSchema> lpSchema;

    // If user provided a specific schema name, validate it:
    if ((logicalSchemaName!=NULL) && (wcslen(logicalSchemaName)>0))
    {
        try
        {
            lpSchema = lpSchemas->FindItem(logicalSchemaName);
        }
        catch (FdoException *e)
        {
            throw FdoException::Create(NlsMsgGet(SHP_SCHEMA_NOT_FOUND, "Schema '%1$ls' not found.", logicalSchemaName), e);
        }
    }

    return FDO_SAFE_ADDREF(lpSchemas.p);
}

bool ShpConnection::IsConfigured ()
{
    return (mConfigured);
}

#define CPY_SUFFIX    L"_cpy"
#define EXECUTE_NO_EX(f)  try { f; } catch (FdoException *ex) { ex->Release(); }

void ShpConnection::CompressFileSets()
{
    // Do files compression (get rid of the deleted rows)
    if ( mGlobalRefCount == 1 )
    {
        for (size_t i = 0; i < ShpConnGlobalFilesToCompress.size(); i++ )
        {
            CompressFileSet( ShpConnGlobalFilesToCompress[i].c_str() );
        }
        ShpConnGlobalFilesToCompress.clear();
    }
}

void ShpConnection::CompressFileSet (const wchar_t*    baseName)
{
    eShapeTypes        type;
    bool            compressed = false;

    // Check the file set still exists
    FdoStringP        test_name = FdoStringP::Format(L"%ls%ls", baseName, DBF_EXTENSION);

    if ( !FdoCommonFile::FileExists( (FdoString*) test_name) )
        return;

    // Use the current directory. At this point we know it is writable.
    FdoString*        tmpDir = NULL;    

    // Create a file set object.
    ShpFileSet*  fileset = new ShpFileSet(baseName, tmpDir);
    
    // Save the file names
    FdoStringP    dbf_name = FdoStringP(fileset->GetDbfFile()->FileName());
    FdoStringP    shp_name = FdoStringP(fileset->GetShapeFile()->FileName());
    FdoStringP    shx_name = FdoStringP(fileset->GetShapeIndexFile()->FileName());
    FdoStringP    ssi_name = FdoStringP(fileset->GetSpatialIndex(true)->FileName());

    // Compressed file names
    FdoStringP    dbfC_name = FdoStringP::Format(L"%ls%ls", (FdoString *)dbf_name, CPY_SUFFIX);
    FdoStringP    shpC_name = FdoStringP::Format(L"%ls%ls", (FdoString *)shp_name, CPY_SUFFIX);
    FdoStringP    shxC_name = FdoStringP::Format(L"%ls%ls", (FdoString *)shx_name, CPY_SUFFIX);
    FdoStringP    ssiC_name = FdoStringP::Format(L"%ls%ls", (FdoString *)ssi_name, CPY_SUFFIX);

    // Create compressed DBF file
    ShapeDBF *dbfC = new ShapeDBF ((FdoString *)dbfC_name, fileset->GetDbfFile()->GetColumnInfo());
    delete dbfC;

    dbfC = new ShapeDBF ((FdoString *)dbfC_name);
    dbfC->Reopen( FdoCommonFile::IDF_OPEN_UPDATE);
    dbfC->PutFileHeaderDetails ();
    fileset->SetDbfFileC( dbfC );

    // Create compressed SHP file
    ShapeFile *shpC = new ShapeFile ((FdoString *)shpC_name, fileset->GetShapeFile()->GetFileShapeType(), false);
    shpC->Reopen( FdoCommonFile::IDF_OPEN_UPDATE);
    fileset->SetShapeFileC( shpC );

    // Create compressed SHX file
    ShapeIndex *shxC = new ShapeIndex ((FdoString *)shxC_name, shpC, tmpDir);
    shxC->Reopen( FdoCommonFile::IDF_OPEN_UPDATE);
    fileset->SetShapeIndexFileC( shxC );

    // Create compressed IDX file (spatial index)
    ShpSpatialIndex *ssiC = new ShpSpatialIndex ((FdoString *)ssiC_name, tmpDir, shpC->GetFileShapeType (), shxC->HasMData ());
    fileset->SetSpatialIndexC( ssiC );

    ShapeDBF *dbf = fileset->GetDbfFile();
    for ( int i = 0, j = 0; i < dbf->GetNumRecords(); i++)
    {
        RowData *data = NULL;
        Shape    *shape = NULL;

        fileset->GetObjectAt( &data, type, &shape, i);
        if ( data && !data->IsDeleted())
        {
            // Change the record number and save it (batch mode)
            shape->SetRecordNum(j+1);

            fileset->SetObjectAt(data, shape, true, true );

            j++;
        }
        delete data;
        delete shape;
    }
    
    // Flush the compressed file set
    fileset->Flush (true);

    // Cleanup
    delete fileset;
    delete shpC;
    delete dbfC;
    delete shxC;
    delete ssiC;

    // Copy over the compressed files
    bool dbf_renamed = FdoCommonFile::Move((FdoString *)dbfC_name, (FdoString *)dbf_name);
    bool shp_renamed = FdoCommonFile::Move((FdoString *)shpC_name, (FdoString *)shp_name);
    bool shx_renamed = FdoCommonFile::Move((FdoString *)shxC_name, (FdoString *)shx_name);

    // Check results.
    if ( dbf_renamed && shp_renamed && shx_renamed )
    {
        bool ssi_renamed = FdoCommonFile::Move((FdoString *)ssiC_name, (FdoString *)ssi_name);

        // Remove .sbx file in case it exists (it is stale now, ESRI tools is using it)
        FdoStringP  sbx_name = FdoStringP::Format(L"%ls%ls", baseName, L".sbx");
        EXECUTE_NO_EX( FdoCommonFile::Delete((FdoString *)sbx_name, true));
    }
    else
    {
        // Something went wrong (like sharing violation); remove the files.
        EXECUTE_NO_EX( FdoCommonFile::Delete((FdoString *)dbfC_name, true));
        EXECUTE_NO_EX( FdoCommonFile::Delete((FdoString *)shpC_name, true));
        EXECUTE_NO_EX( FdoCommonFile::Delete((FdoString *)shxC_name, true));
        EXECUTE_NO_EX( FdoCommonFile::Delete((FdoString *)ssiC_name, true));
    }
}
