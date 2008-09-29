/*
* Copyright (C) 2006  SL-King d.o.o
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
*/

#include "stdafx.h"

#include "occi.h"

#include "c_OCCI_API.h"
#include "c_Ora_API.h"
#include "c_FdoOra_API.h"
#include "c_KgOraSchemaDesc.h"
#include "c_KgOraSchemaPool.h"
#include "c_LogAPI.h"

#include <time.h>

#ifdef _WIN32

static wchar_t g_AppFileName[MAX_PATH];
static wchar_t g_HomeDir[MAX_PATH];
wchar_t g_LogFileName[MAX_PATH];


//wchar_t g_WcharBuff1024[1024+1];

BOOL APIENTRY DllMain (HANDLE Module, DWORD Reason, LPVOID lpReserved)
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
    if ( Reason == DLL_PROCESS_ATTACH )
    {
        nchars = GetModuleFileNameW ((HINSTANCE)Module, g_AppFileName, MAX_PATH);
        if (0 == nchars)
            ret = FALSE;
        else
        {   
            // scan the string for the last occurrence of a slash
            wcscpy (g_HomeDir, g_AppFileName);
            last = wcsrchr (g_HomeDir, L'\\');
            if (NULL == last)
                ret = FALSE;
            else
            {
                last++; // move past the slash
                *last = L'\0'; // null terminate it there
            }
            
            wcscpy (g_LogFileName, g_HomeDir);
            wcscat (g_LogFileName, L"\\KingOra.log");
        }
    }

    return (ret);
}

#else

wchar_t g_LogFileName[PATH_MAX];

#endif // _WIN32


// external access to connection for client services
extern "C" FDOKGORA_API FdoIConnection* CreateConnection ()
{
   return (new c_KgOraConnection ());
}

// Globals to keep track of opened connections and the files to compress after
// delete command. The compression is triggered on the last connection close().
FdoCommonThreadMutex c_KgOraConnection::g_Mutex;
int c_KgOraConnection::g_GlobalRefCount = 0;



c_KgOraConnection::c_KgOraConnection (void) :
    m_ConnectionString ((wchar_t*)NULL),
    m_ConnectionState(FdoConnectionState_Closed)
    
{
  m_OcciConnection=NULL;
  m_SchemaDesc = NULL;
	// Create the default SC
	

	g_Mutex.Enter();
	g_GlobalRefCount++;
	m_ConnNo = g_GlobalRefCount;
	D_KGORA_ELOG_WRITE2("c_KgOraConnection%d::NEW %d ",m_ConnNo,m_ConnNo);
	g_Mutex.Leave();
	
	

	
	// this wil be read from db in open connection command
	m_OracleMainVersion = D_ORACLE_DEFAULT_MAIN_VER;
	m_OracleSubVersion = D_ORACLE_DEFAULT_SUB_VER;
}

c_KgOraConnection::~c_KgOraConnection (void)
{
  #ifdef _DEBUG
  printf("\nFDO c_KgOraConnection::Destructor... %p",this);
  #endif
  D_KGORA_ELOG_WRITE2("c_KgOraConnection%d::Delete %d ",m_ConnNo,m_ConnNo);
  Close ();

	//g_Mutex.Enter();
	//g_GlobalRefCount--;
	//g_Mutex.Leave();
	
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void c_KgOraConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* c_KgOraConnection::GetConnectionCapabilities ()
{
  D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::GetConnectionCapabilities",m_ConnNo);
    return (new c_KgOraConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* c_KgOraConnection::GetSchemaCapabilities ()
{
  //D_KGORA_ELOG_WRITE("c_KgOraConnection::GetSchemaCapabilities");
  D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::GetSchemaCapabilities",m_ConnNo);
    return (new c_KgOraSchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* c_KgOraConnection::GetCommandCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetCommandCapabilities");
    return (new c_KgOraCommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* c_KgOraConnection::GetFilterCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetFilterCapabilities");
    return (new c_KgOraFilterCapabilities ());
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* c_KgOraConnection::GetExpressionCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetExpressionCapabilities");
    return (new c_KgOraExpressionCapabilities ());
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* c_KgOraConnection::GetRasterCapabilities (void)
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetRasterCapabilities");
    return (new c_KgOraRasterCapabilities ());
}

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* c_KgOraConnection::GetTopologyCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetTopologyCapabilities");
    return (new c_KgOraTopologyCapabilities ());
}


/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* c_KgOraConnection::GetGeometryCapabilities()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetGeometryCapabilities");
    return (new c_KgOraGeometryCapabilities());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* c_KgOraConnection::GetConnectionString ()
{
  //D_KGORA_ELOG_WRITE("c_KgOraConnection::GetConnectionString");
  D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::GetConnectionString",m_ConnNo);
    return (m_ConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraConnection::SetConnectionString (FdoString* value)
{
  //D_KGORA_ELOG_WRITE("c_KgOraConnection::SetConnectionString ");
  D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::SetConnectionString",m_ConnNo);
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
    {
        // Update the connection string:
        m_ConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<c_KgOraConnectionInfo> connInfo = static_cast<c_KgOraConnectionInfo*>(this->GetConnectionInfo());
        FdoPtr<FdoCommonConnPropDictionary> connDict = static_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
        connDict->UpdateFromConnectionString(m_ConnectionString);
    }
    else
        throw FdoException::Create (NlsMsgGetKgOra(M_KGORA_CONNECTION_ALREADY_OPEN, "The connection is already open."));
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* c_KgOraConnection::GetConnectionInfo ()
{
  //D_KGORA_ELOG_WRITE("c_KgOraConnection::GetConnectionInfo ");
  D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::GetConnectionInfo",m_ConnNo);
    if( m_ConnectionInfo == NULL )
        m_ConnectionInfo = new c_KgOraConnectionInfo ( this );
    return FDO_SAFE_ADDREF(m_ConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState c_KgOraConnection::GetConnectionState ()
{
  //D_KGORA_ELOG_WRITE("c_KgOraConnection::GetConnectionState ");
  D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::GetConnectionState",m_ConnNo);
    return (m_ConnectionState);
}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 c_KgOraConnection::GetConnectionTimeout ()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::GetConnectionTimeout ");
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraConnection::SetConnectionTimeout (FdoInt32 value)
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::SetConnectionTimeout ");
    throw FdoException::Create (NlsMsgGetKgOra(M_KGORA_CONNECTION_TIMEOUT_UNSUPPORTED, "Connection timeout is not supported."));
}



/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState c_KgOraConnection::Open ()
{

g_Mutex.Enter();
  #ifdef _DEBUG
    printf("\nOpen...");
  #endif
  
  
  
    if (GetConnectionState() == FdoConnectionState_Open)
    {
      D_KGORA_ELOG_WRITE("c_KgOraConnection::Open Exception: Already open");
        #ifdef _FDO_3_1
          throw GisException::Create(GisException::NLSGetMessage(FDO_NLSID(FDO_103_CONNECTION_ALREADY_OPEN)));
        #else
          throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_103_CONNECTION_ALREADY_OPEN)));
        #endif
    }

  if( !c_OCCI_API::IsInit() )
  {
    #ifdef _DEBUG
      printf("\nOpen...OcciInit...");
    #endif
    D_KGORA_ELOG_WRITE("c_KgOraConnection::Open c_OCCI_API::OcciInit");
    c_OCCI_API::OcciInit();
  }

	try
	{
	  FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoIConnectionPropertyDictionary> dictionary = info->GetConnectionProperties ();
    
    FdoStringP username = dictionary->GetProperty (D_CONN_PROPERTY_USERNAME);
    FdoStringP password = dictionary->GetProperty (D_CONN_PROPERTY_PASSWORD);
    FdoStringP service = dictionary->GetProperty (D_CONN_PROPERTY_SERVICE_NAME);
    FdoStringP oraschema = dictionary->GetProperty (D_CONN_PROPERTY_ORACLE_SCHEMA);
    FdoStringP fdoviewstable = dictionary->GetProperty (D_CONN_PROPERTY_KING_FDO_CLASS);
		
		
		//FdoStringP username = dictionary->GetProperty (L"Username");
    //FdoStringP password = dictionary->GetProperty (L"Password");
    //FdoStringP service = dictionary->GetProperty (L"Service");
		
		D_KGORA_ELOG_WRITE5("c_KgOraConnection%d::Open %d '%s' '%s' '%s'",m_ConnNo,m_ConnNo,(const char*)username,(const char*)password,(const char*)service);
		
	  c_OCCI_API::CreateConnection((const char*)username, (const char*)password, (const char*)service,m_OcciConnection,m_OcciEnvironment);
	  
	  
	  
	  m_OraConnectionUserName = username.Upper();
	  m_OraConnectionPassword = password;
	  m_OraConnectionDbLink = service;
	  m_OraSchemaName = oraschema.Upper();
	  m_FdoViewsTable = fdoviewstable.Upper();
	  
	  
	  if( !c_Ora_API::GetOracleVersion(m_OcciConnection,m_OracleMainVersion,m_OracleSubVersion) )
	  {
	    m_OracleMainVersion = D_ORACLE_DEFAULT_MAIN_VER;
	    m_OracleSubVersion = D_ORACLE_DEFAULT_SUB_VER;
	  }


     // Connection is now open:
    m_ConnectionState = FdoConnectionState_Open;
    
     #ifdef _DEBUG
    printf(" <Open OK> ");
  #endif
  
    D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::Open OK",m_ConnNo);

    g_Mutex.Leave();
    
    return (GetConnectionState ());
	}
	catch(oracle::occi::SQLException& ea)
  {
    const char* what = ea.what();
    
    #ifdef _DEBUG
      printf(" <Open SQL Exception> ");
    #endif
    
    D_KGORA_ELOG_WRITE2("c_KgOraConnection%d::Open Exception '%s'",m_ConnNo,what);
    
    g_Mutex.Leave();
    
    //wchar_t *cstr = ustr.;
    FdoStringP gstr = what;
    throw FdoConnectionException::Create( gstr );
    //throw FdoConnectionException::Create(L"Connect failed");
  }

  catch(...)
  {
    //const char* what = ea.what();
     #ifdef _DEBUG
      printf(" <Open Unspecified Exception> ");
    #endif
    D_KGORA_ELOG_WRITE("c_KgOraConnection::Open Exception Unknown exception!");
    
    g_Mutex.Leave();
    throw FdoConnectionException::Create( L"c_KgOraConnection::Open Exception Unknown exception!" );
    //throw FdoConnectionException::Create(L"Connect failed");
  }

   
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void c_KgOraConnection::Close ()
{
  D_KGORA_ELOG_WRITE3("c_KgOraConnection%d::Close %d State=%d",m_ConnNo,m_ConnNo,(int)m_ConnectionState);
  
  
  #ifdef _DEBUG
  printf("\nFDO c_KgOraConnection::Close Connection... %p",this);
  #endif

  // reset schema
  m_SchemaDesc = NULL;
  
	// Reset the Spatial Contexts collection. Create the default SC.

g_Mutex.Enter();
  try
  {
    if( m_OcciConnection )
    {
      c_OCCI_API::CloseConnection(m_OcciConnection,m_OcciEnvironment);    
      
      m_OcciConnection=NULL;    
    }
    g_Mutex.Leave();  
  }
  catch(oracle::occi::SQLException& ea)
  {
    g_Mutex.Leave();  
    const char* what = ea.what();
    
    #ifdef _DEBUG
      printf(" <Close Exception> ");
    #endif
    
    D_KGORA_ELOG_WRITE2("c_KgOraConnection%d::Open Exception '%s'",m_ConnNo,what);
    
    FdoStringP gstr = ea.getMessage().c_str();
    throw FdoException::Create( gstr );
  }
  catch(...)
  {
    #ifdef _DEBUG
      printf(" <Close Exception Unspecified> ");
    #endif
    
    D_KGORA_ELOG_WRITE1("c_KgOraConnection%d::Close Exception Unspecified exception",m_ConnNo);
    
    g_Mutex.Leave();  
  }
    // Connection is now closed:
    m_ConnectionState = FdoConnectionState_Closed;
    
  #ifdef _DEBUG
    printf("OK");
  #endif
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* c_KgOraConnection::BeginTransaction ()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::BeginTransaction ");
    throw FdoException::Create(NlsMsgGetKgOra(M_KGORA_CONNECTION_TRANSACTIONS_NOT_SUPPORTED, "King.Oracle Provider does not support transactions."));
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* c_KgOraConnection::CreateCommand (FdoInt32 CommandId)
{
    FdoPtr<FdoICommand> ret;

  D_KGORA_ELOG_WRITE2("c_KgOraConnection::CreateCommand %ld '%s'",(long)CommandId,(const char*)FdoCommonMiscUtil::FdoCommandTypeToString (CommandId));
  
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
        throw FdoException::Create(NlsMsgGetKgOra(M_KGORA_CONNECTION_INVALID, "Connection is invalid."));
    switch (CommandId)
    {
        case FdoCommandType_Select:
          ret = new c_KgOraSelectCommand (this);
        break;
        case FdoCommandType_SQLCommand:
          ret = new c_KgOraSQLCommand(this);
        break;
        case FdoCommandType_DescribeSchema:
          ret = new c_KgOraDescribeSchemaCommand (this);
        break;
        case FdoCommandType_GetSpatialContexts:
          ret = new c_KgOraGetSpatialContextsCommand (this);
        break;
        
        case FdoCommandType_CreateSpatialContext:
          ret = new c_KgOraCreateSpatialContext (this);
        break;
        
        case FdoCommandType_Insert:
          ret = new c_KgOraInsert(this);
        break;
        case FdoCommandType_Update:
          ret = new c_KgOraUpdate(this);
        break;
         case FdoCommandType_Delete:
          ret = new c_KgOraDelete(this);
        break;
        case FdoCommandType_ApplySchema:
          ret = new c_KgOraApplySchema (this);
        break;
        case FdoCommandType_CreateDataStore:
          ret = new c_KgOraCreateDataStore (this);
        break;
        
        
        /*
        case FdoCommandType_SelectAggregates:
          ret = new KgOraSelectAggregates (this);
        break;
        
        case FdoCommandType_DescribeSchemaMapping:
            ret = new KgOraDescribeSchemaMappingCommand (this);
            break;
               
        case FdoCommandType_ApplySchema:
            ret = new KgOraApplySchemaCommand (this);
            break;
        case FdoCommandType_DestroySchema:
            ret = new KgOraDestroySchemaCommand (this);
            break;
        */
        default:
        {
          D_KGORA_ELOG_WRITE2("c_KgOraConnection::CreateCommand Unsupported command %ld '%s'",(long)CommandId,(const char*)FdoCommonMiscUtil::FdoCommandTypeToString (CommandId));
          printf("\n****   Unkown Command: %d     ********************",CommandId);
          #ifdef _FDO_3_1
          throw GisException::Create (GisException::NLSGetMessage (FDO_102_COMMAND_NOT_SUPPORTED, "The command '%1$ls' is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoCommandTypeToString (CommandId))));
          #else
          throw FdoException::Create (FdoException::NLSGetMessage (FDO_102_COMMAND_NOT_SUPPORTED, "The command '%1$ls' is not supported.", (FdoString*)(FdoCommonMiscUtil::FdoCommandTypeToString (CommandId))));
          #endif
        }
    }

    return (FDO_SAFE_ADDREF (ret.p));
}








/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* c_KgOraConnection::CreateSchemaMapping()
{
  D_KGORA_ELOG_WRITE("c_KgOraConnection::CreateSchemaMapping ");
  
	FdoKgOraPhysicalSchemaMappingP newPhysicalSchemaMapping = FdoKgOraPhysicalSchemaMapping::Create();
    return FDO_SAFE_ADDREF(newPhysicalSchemaMapping.p);
}


/// <summary>Sets the XML configuration stream used to configure the Data Store. 
/// SetConfiguration can only be called while the connection is closed.</summary>
/// <param name="configStream">Input the XML configuration stream</param> 
/// <returns>Returns nothing</returns> 
void c_KgOraConnection::SetConfiguration(FdoIoStream* configStream)
{
   D_KGORA_ELOG_WRITE("c_KgOraConnection::SetConfiguration ");
}
#ifdef _FDO_3_2
/// \brief
/// Forces the writes of any cached data to the targed datastore.
/// 
/// \return
/// Returns nothing
/// 
void c_KgOraConnection::Flush()
{
}
#endif
    
c_KgOraSpatialContextCollection* c_KgOraConnection::GetSpatialContexts ( bool bDynamic )
{
  FdoPtr<c_KgOraSchemaDesc> schemadesc = GetSchemaDesc();
	return schemadesc->GetSpatialContexts();
}


bool c_KgOraConnection::GetOracleSridDesc(FdoGeometricPropertyDefinition* GeomProp,c_KgOraSridDesc& OraSridDesc)
{
  FdoString *csname = GeomProp->GetSpatialContextAssociation();
  FdoPtr<c_KgOraSpatialContextCollection> sccol = GetSpatialContexts();
  
  FdoPtr<c_KgOraSpatialContext> sc = sccol->FindItem(csname);
  
  if( !sc.p ) return false;
  
  OraSridDesc = sc->GetOraSridDesc();
  return true;
  
}//end of c_KgOraConnection::GetOracleSridDesc

bool c_KgOraConnection::GetOracleSridDesc(FdoClassDefinition* ClassDef,c_KgOraSridDesc& OraSrid)
{
  if( ClassDef->GetClassType() != FdoClassType_FeatureClass )
  {
    return false;
  }
  
  FdoFeatureClass* fclass = (FdoFeatureClass*)ClassDef;
  
  FdoPtr<FdoGeometricPropertyDefinition> geomprop = fclass->GetGeometryProperty();
  
  if( !geomprop.p ) return false;
  
  
  FdoString *csname = geomprop->GetSpatialContextAssociation();
  FdoPtr<c_KgOraSpatialContextCollection> sccol = GetSpatialContexts();
  
  
  
    
  FdoPtr<c_KgOraSpatialContext> sc = sccol->FindItem(csname);
  
  if( !sc.p ) 
  {
    // Now test if
    FdoStringP name = csname;
    FdoStringP temp = name.Mid(0,10);
    if( (temp.ICompare("OracleSrid") == 0) )
    {
      FdoStringP temp = name.Mid(10,name.GetLength()-10);
      OraSrid.m_OraSrid = temp.ToLong();
      
         
      FdoStringP wkt =  sc->GetCoordinateSystemWkt();        
      OraSrid.m_IsGeodetic = c_Ora_API::IsGeodeticCoordSystem(wkt); // TODO: it should return real value
    }
    else
    {
      FdoStringP name = sc->GetCoordinateSystem();
      FdoStringP temp = name.Mid(0,10);
      if( (temp.ICompare("OracleSrid") == 0) )
      {
        FdoStringP temp = name.Mid(10,name.GetLength()-10);
        OraSrid.m_OraSrid = temp.ToLong();
           
        FdoStringP wkt =  sc->GetCoordinateSystemWkt();                   
        OraSrid.m_IsGeodetic = c_Ora_API::IsGeodeticCoordSystem(wkt); // TODO: it should return real value
      }
      else
      {
        return false;
      }
    }
  }
  else
    OraSrid = sc->GetOraSridDesc();
  return true;
  
}//end of c_KgOraConnection::GetOracleSrid

oracle::occi::Statement* c_KgOraConnection::OCCI_CreateStatement()
{
  return m_OcciConnection->createStatement();  
}//end of c_KgOraConnection::OCCI_CreateStatement

void c_KgOraConnection::OCCI_Commit()
{
  return m_OcciConnection->commit();  
}//end of c_KgOraConnection::OCCI_CreateStatement

void c_KgOraConnection::OCCI_TerminateStatement(oracle::occi::Statement* Statement)
{
  
  if( m_OcciConnection && Statement ) m_OcciConnection->terminateStatement (Statement);
  
}//end of c_KgOraConnection::OCCI_CreateStatement


c_KgOraSchemaDesc* c_KgOraConnection::GetSchemaDesc()
{
  if( m_SchemaDesc.p == NULL )
  {
  // Now check into schema pool
  #ifdef D_ENABLE_SCHEMA_POOL
    m_SchemaDesc = c_KgOraSchemaPool::GetSchemaData(this);
    if( !m_SchemaDesc.p )
    {
      m_SchemaDesc = c_FdoOra_API::DescribeSchema(this,m_OraSchemaName.c_str(),m_FdoViewsTable.c_str());
      if( m_SchemaDesc.p )
      {
        c_KgOraSchemaPool::AddSchemaData(this,m_SchemaDesc.p);
      }
    }
  
  #else
    m_SchemaDesc = c_FdoOra_API::DescribeSchema(this,m_OraSchemaName.c_str(),m_FdoViewsTable.c_str());
  #endif
  }
  return FDO_SAFE_ADDREF(m_SchemaDesc.p);
}//end of c_KgOraConnection::GetSchemaDesc




#ifdef NODEFINE
bool checkError(OCIError* errhp_, int status) 
{
	char errbuf[512];
	sb4 errcode = 0;
	bool returnedVal = false;
		
  string errorMess_;		
	if (status == OCI_ERROR)
	{
		OCIErrorGet((dvoid*)errhp_, (ub4)1, (text*)NULL, &errcode, 
						(text*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		errorMess_ = errbuf;
		return false;
	}

	switch (status)
	{ 
		case OCI_SUCCESS:
			errorMess_ = "Success!";
			returnedVal = true;
			break;

		case OCI_SUCCESS_WITH_INFO:
			errorMess_ = "Success with information!";
			returnedVal = true;
			break;

		case OCI_NEED_DATA:
			errorMess_ = "Need data!";
			break;
		
		case OCI_NO_DATA:
			errorMess_ = "No data!";
			break;

		//An invalid handle was passed as a parameter or a user callback is passed an
		//invalid handle or invalid context. No further diagnostics are available.
		case OCI_INVALID_HANDLE:
			errorMess_ = "Invalid handle!";
			break;

		case OCI_STILL_EXECUTING:
			errorMess_ = "Still executing!";
			break;

		case OCI_CONTINUE:
			errorMess_ = "Continue!";
			break;
		default:
			break;
	}

	return returnedVal;
}

 //! Structure to represent a point indicator as the SDO_POINT type of the Oracle Spatial 
          struct sdo_point_type_ind
          {
             OCIInd _atomic;
             OCIInd x;
             OCIInd y;
             OCIInd z;
          };
          //! Structure to represent a geometry indicator as the SDO_GEOMETRY type of the Oracle Spatial 
          struct SDO_GEOMETRY_ind
          {
             OCIInd                    _atomic;
             OCIInd                    sdo_gtype;
             OCIInd                    sdo_srid;
             struct sdo_point_type_ind sdo_point;
             OCIInd                    sdo_elem_info;
             OCIInd                    sdo_ordinates;
          };
SDO_GEOMETRY *geom_databuff[10];
SDO_GEOMETRY_ind*		global_geom_ind_[10];	//!< buffer to store spatial object indicator  

char reserver[1024];



void c_KgOraConnection::TestArrayFetch(FdoIdentifier* ClassId, FdoFilter* Filter, FdoIdentifierCollection* Props)
{
    FdoString* class_name = ClassId->GetText ();
    
    FdoPtr<c_KgOraSchemaDesc> schemadesc = GetSchemaDesc();
    
    FdoPtr<FdoFeatureSchemaCollection> fschemas = schemadesc->GetFeatureSchema();
    FdoPtr<FdoKgOraPhysicalSchemaMapping> phschemamapping = schemadesc->GetPhysicalSchemaMapping();
      
    
    FdoPtr<FdoClassDefinition> classdef;
    if( fschemas && (fschemas->GetCount() > 0) )
    {
      FdoPtr<FdoFeatureSchema> schm = fschemas->GetItem(0);      
      FdoPtr<FdoClassCollection> classes = schm->GetClasses();
      
      classdef = classes->FindItem( ClassId->GetName() );
          
    }

    string sqlstr;
    
    /* Define properties to be included in SELECT statement */    
    FdoPtr<FdoPropertyDefinition> propdef;
    FdoPtr<FdoStringCollection> sqlcols = FdoStringCollection::Create();
    int geom_prop_sqlindex=-1;
    FdoPtr<FdoPropertyDefinitionCollection> propcol = classdef->GetProperties();
    
    FdoStringP geomcolname;
    
    int count = propcol->GetCount();
    for( int ind = 0; ind < count; ind++ )
    {
      propdef = propcol->GetItem(ind);
      FdoString* propname = propdef->GetName();
      
      sqlcols->Add(propname);
      if( propdef->GetPropertyType() == FdoPropertyType_GeometricProperty )
      {
        geom_prop_sqlindex=ind;
      }
      
      
      if( propdef->GetPropertyType() == FdoPropertyType_GeometricProperty )
      {
        geomcolname = propdef->GetName();        
      }
      
    }
    
    
    string wherestr;
    ApplyFilter(Filter,wherestr);
    
    char* sbuff = new char[1024];
    
    FdoStringP cols = sqlcols->ToString(L",");
    
    const char* cp = cols;
    
    FdoStringP tablename = classdef->GetName();
    
    
    
    string test_sqlstr;
    {
      //sprintf(sbuff, "SELECT a.%s.SDO_GTYPE, a.%s.SDO_ELEM_INFO FROM %s a",(const char*)geomcolname,(const char*)geomcolname,(const char*)tablename);
      //sprintf(sbuff, "SELECT a.%s.SDO_GTYPE FROM %s a",(const char*)geomcolname,(const char*)tablename);
      sprintf(sbuff, "SELECT a.%s FROM %s a",(const char*)geomcolname,(const char*)tablename);
        
      test_sqlstr = sbuff;
      if( wherestr.length() > 0 )
      {
        test_sqlstr += " WHERE ";
        test_sqlstr += wherestr;
      }  
    }
    
    
    
    sprintf(sbuff, "SELECT %s FROM %s a",(const char*)cols,(const char*)tablename);
    
    
    sqlstr = sbuff;
    if( wherestr.length() > 0 )
    {
      sqlstr += " WHERE ";
      sqlstr += wherestr;
    }
    
    delete sbuff;

    
      oracle::occi::Statement* occi_statement = NULL;
      oracle::occi::ResultSet* occi_resultset = NULL;
      try
      {
        occi_statement = OCCI_CreateStatement();
        occi_statement->setPrefetchRowCount(40);

        //m_OcciStatement->setSQL("SELECT GEOMETRY FROM polyline_parcele where rownum < 10 order by rowid");
        //m_OcciStatement->setSQL("SELECT GEOMETRY FROM line1");
        
        //occi_statement->setSQL(sqlstr.c_str());
        
        occi_statement->setSQL(test_sqlstr.c_str()); geom_prop_sqlindex=0; // tukaj je samo geometrija

   
        printf("\nStart query speed test");
        clock_t clock_start = clock();
    
        occi_resultset = occi_statement->executeQuery();
        
        clock_t clock_end_query = clock();
        
        
        int countrows=0;
        int numlines = 0;
        long numordinates=0;
        int uknownagf=0;
        c_SdoGeomToAGF sdoagfconv;
        int buffagf[64*1024];
        
        while( occi_resultset->next() != oracle::occi::ResultSet::END_OF_FETCH )
        {
        
          countrows++;
          
          SDO_GEOMETRY *geom = (SDO_GEOMETRY*)occi_resultset->getObject(1); // oracle is 1 based - our index is 0 based
          //*len=0;
          if( geom )
          {
            sdoagfconv.SetGeometry(geom);
            int agflen = sdoagfconv.ToAGF( &buffagf[0] );
            
            if( !agflen )
            {
              uknownagf++;
            }
             
            int gtype = geom->getSdo_gtype();
            if( gtype == 2 )
            {
              numlines++;
            }
            long size = geom->getSdo_ordinates().size();
            numordinates += size;
            delete geom;
          }
        }
        

        

        
        clock_t clock_end_fetch = clock();
        
        printf("\nQuery: %ld ms Fetc: %ld ms (Rows:%ld lines:%ld ord:%ld bad agf:%ld)",(long)(clock_end_query-clock_start)*  CLOCKS_PER_SEC / 1000,(long)(clock_end_fetch-clock_end_query)*  CLOCKS_PER_SEC / 1000,(long)countrows,(long)numlines,(long)numordinates,uknownagf);
        
        if (occi_statement && occi_resultset)
        {
          occi_statement->closeResultSet(occi_resultset);        
          
        }
        
        if (occi_statement)
        {
          OCCI_TerminateStatement(occi_statement);
          
        }
      }
      catch(const oracle::occi::SQLException& ea)
      {
        
        FdoStringP gstr = ea.getMessage().c_str();
        printf("\nTest occi exception: %s",(const char*)gstr);
        //throw FdoConnectionException::Create( gstr );
        //throw FdoException::Create (NlsMsgGetKgOra(KGORA_CONNECTION_ALREADY_OPEN, "The connection is already open."));
        //throw c_KgDbException(ea.getErrorCode(),ea.getMessage().data(),"c_KgDbBsAPI::Init");
      }
    
      catch(oracle::occi::SQLException* ea)
      {
        
        FdoStringP gstr = ea->getMessage().c_str();
        printf("\nTest occi exception: %s",(const char*)gstr);
        //throw FdoConnectionException::Create( gstr );
        //throw FdoException::Create (NlsMsgGetKgOra(KGORA_CONNECTION_ALREADY_OPEN, "The connection is already open."));
        //throw c_KgDbException(ea.getErrorCode(),ea.getMessage().data(),"c_KgDbBsAPI::Init");
      }
      catch (exception& )                                                      
      {                                                                         
          printf("\nTets query: System exception");
      } 
      catch(...)
      {
        printf("\nTets query unknown exception");
      }
    
    
}//end of c_KgOraConnection::TestArrayFetc


#endif



