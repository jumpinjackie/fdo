/*
* Copyright (C) 2007  Haris Kurtagic
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


#include "c_KgInfSchemaDesc.h"
#include "c_LogApi.h"

#include <time.h>

#define KGORA_MESSAGE_DEFINE





// external access to connection for client services
extern "C" FDOKGINF_API FdoIConnection* CreateConnection ()
{
   return (new c_KgInfConnection ());
}

// Globals to keep track of opened connections and the files to compress after
// delete command. The compression is triggered on the last connection close().
FdoCommonThreadMutex c_KgInfConnection::g_Mutex;
int c_KgInfConnection::g_GlobalRefCount = 0;



c_KgInfConnection::c_KgInfConnection (void) :
    m_ConnectionString ((wchar_t*)NULL),
    m_ConnectionState(FdoConnectionState_Closed)
    
{
  
	g_Mutex.Enter();
	g_GlobalRefCount++;
	g_Mutex.Leave();
	
	// this wil be read from db in open connection command
	
	//m_OracleMainVersion = D_ORACLE_DEFAULT_MAIN_VER;
	//m_OracleSubVersion = D_ORACLE_DEFAULT_SUB_VER;
}

c_KgInfConnection::~c_KgInfConnection (void)
{
  Close ();

	g_Mutex.Enter();
	g_GlobalRefCount--;
	g_Mutex.Leave();
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void c_KgInfConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* c_KgInfConnection::GetConnectionCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetConnectionCapabilities");
    return (new c_KgInfConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* c_KgInfConnection::GetSchemaCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetSchemaCapabilities");
    return (new c_KgInfSchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* c_KgInfConnection::GetCommandCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetCommandCapabilities");
    return (new c_KgInfCommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* c_KgInfConnection::GetFilterCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetFilterCapabilities");
    return (new c_KgInfFilterCapabilities ());
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* c_KgInfConnection::GetExpressionCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetExpressionCapabilities");
    return (new c_KgInfExpressionCapabilities ());
}

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* c_KgInfConnection::GetRasterCapabilities (void)
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetRasterCapabilities");
    return (new c_KgInfRasterCapabilities ());
}

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* c_KgInfConnection::GetTopologyCapabilities ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetTopologyCapabilities");
    return (new c_KgInfTopologyCapabilities ());
}


/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* c_KgInfConnection::GetGeometryCapabilities()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetGeometryCapabilities");
    return (new c_KgInfGeometryCapabilities());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* c_KgInfConnection::GetConnectionString ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetConnectionString");
    return (m_ConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void c_KgInfConnection::SetConnectionString (FdoString* value)
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::SetConnectionString ");
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
    {
        // Update the connection string:
        m_ConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<c_KgInfConnectionInfo> connInfo = static_cast<c_KgInfConnectionInfo*>(this->GetConnectionInfo());
        FdoPtr<FdoCommonConnPropDictionary> connDict = static_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
        connDict->UpdateFromConnectionString(m_ConnectionString);
    }
    else
        throw FdoException::Create (L"The connection is already open.");
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* c_KgInfConnection::GetConnectionInfo ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetConnectionInfo ");
    if( m_ConnectionInfo == NULL )
        m_ConnectionInfo = new c_KgInfConnectionInfo ( this );
    return FDO_SAFE_ADDREF(m_ConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState c_KgInfConnection::GetConnectionState ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetConnectionState ");
    return (m_ConnectionState);
}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 c_KgInfConnection::GetConnectionTimeout ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::GetConnectionTimeout ");
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void c_KgInfConnection::SetConnectionTimeout (FdoInt32 value)
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::SetConnectionTimeout ");
    throw FdoException::Create (L"Connection timeout is not supported.");
}



/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState c_KgInfConnection::Open ()
{
  #ifdef _DEBUG
    printf("\nOpen...");
  #endif
  
  
  
    if (GetConnectionState() == FdoConnectionState_Open)
    {
      D_KGORA_ELOG_WRITE("c_KgInfConnection::Open Exception: Already open");
        #ifdef _FDO_3_1
          throw GisException::Create(GisException::NLSGetMessage(FDO_NLSID(FDO_103_CONNECTION_ALREADY_OPEN)));
        #else
          throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_103_CONNECTION_ALREADY_OPEN)));
        #endif
    }

/*
  if( !c_OCCI_API::IsInit() )
  {
    c_OCCI_API::OcciInit();
  }
*/
	try
	{
	  FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoIConnectionPropertyDictionary> dictionary = info->GetConnectionProperties ();
    
    FdoStringP username = dictionary->GetProperty (D_CONN_PROPERTY_USERNAME);
    FdoStringP password = dictionary->GetProperty (D_CONN_PROPERTY_PASSWORD);
    FdoStringP service = dictionary->GetProperty (D_CONN_PROPERTY_SERVICE_NAME);
    FdoStringP oraschema = dictionary->GetProperty (D_CONN_PROPERTY_TABLE_SCHEMA);
    FdoStringP fdoviewstable = dictionary->GetProperty (D_CONN_PROPERTY_KING_FDO_CLASS);
		
		
		D_KGORA_ELOG_WRITE3("c_KgInfConnection::Open '%s' '%s' '%s'",(const char*)username,(const char*)password,(const char*)service);
		
	  m_KgOtl.Open(username,password,service);
	  
	  
	  m_OraConnectionUserName = username.Upper();
	  m_OraConnectionPassword = password;
	  m_OraConnectionDbLink = service;
	  m_OraSchemaName = oraschema.Upper();
	  m_FdoViewsTable = fdoviewstable.Upper();
	  

	  }
	  catch(c_KgOtlException& ea)
    {
      const wchar_t* what = ea.what();
      throw FdoConnectionException::Create( what );    
    }

    // Connection is now open:
    m_ConnectionState = FdoConnectionState_Open;
    
    D_KGORA_ELOG_WRITE("c_KgInfConnection::Open OK");

  #ifdef _DEBUG
    printf("OK");
  #endif
    
  return (GetConnectionState ());
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void c_KgInfConnection::Close ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::Close");
  
  #ifdef _DEBUG
  printf("\nFDO Close Connection...");
  #endif

  // reset schema
  m_SchemaDesc = NULL;
  
	// Reset the Spatial Contexts collection. Create the default SC.


  try
  {
    m_KgOtl.Close();
  }
  catch(c_KgOtlException& ea)
  {
    const wchar_t* what = ea.what();
    throw FdoConnectionException::Create( what );    
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
FdoITransaction* c_KgInfConnection::BeginTransaction ()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::BeginTransaction ");
    throw FdoException::Create(L"King.Oracle Provider does not support transactions.");
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* c_KgInfConnection::CreateCommand (FdoInt32 CommandId)
{
    FdoPtr<FdoICommand> ret;

  D_KGORA_ELOG_WRITE2("c_KgInfConnection::CreateCommand %ld '%s'",(long)CommandId,(const char*)FdoCommonMiscUtil::FdoCommandTypeToString (CommandId));
  
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
        throw FdoException::Create(L"Connection State is closed or Peneding.");
    switch (CommandId)
    {
        case FdoCommandType_Select:
          ret = new c_KgInfSelectCommand (this);
        break;
        case FdoCommandType_SQLCommand:
          ret = new c_KgInfSQLCommand(this);
        break;
        case FdoCommandType_DescribeSchema:
          ret = new c_KgInfDescribeSchemaCommand (this);
        break;
        case FdoCommandType_GetSpatialContexts:
          ret = new c_KgInfGetSpatialContextsCommand (this);
        break;
        
        case FdoCommandType_CreateSpatialContext:
          ret = new c_KgInfCreateSpatialContext (this);
        break;
        
        case FdoCommandType_Insert:
          ret = new c_KgInfInsert(this);
        break;
        case FdoCommandType_Update:
          ret = new c_KgInfUpdate(this);
        break;
         case FdoCommandType_Delete:
          ret = new c_KgInfDelete(this);
        break;
        case FdoCommandType_ApplySchema:
          ret = new c_KgInfApplySchema (this);
        break;
        case FdoCommandType_CreateDataStore:
          ret = new c_KgInfCreateDataStore (this);
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
          D_KGORA_ELOG_WRITE2("c_KgInfConnection::CreateCommand Unsupported command %ld '%s'",(long)CommandId,(const char*)FdoCommonMiscUtil::FdoCommandTypeToString (CommandId));
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
FdoPhysicalSchemaMapping* c_KgInfConnection::CreateSchemaMapping()
{
  D_KGORA_ELOG_WRITE("c_KgInfConnection::CreateSchemaMapping ");
  
	FdoKgOraPhysicalSchemaMappingP newPhysicalSchemaMapping = c_KgInfOverPhysicalSchemaMapping::Create();
    return FDO_SAFE_ADDREF(newPhysicalSchemaMapping.p);
}


/// <summary>Sets the XML configuration stream used to configure the Data Store. 
/// SetConfiguration can only be called while the connection is closed.</summary>
/// <param name="configStream">Input the XML configuration stream</param> 
/// <returns>Returns nothing</returns> 
void c_KgInfConnection::SetConfiguration(FdoIoStream* configStream)
{
   D_KGORA_ELOG_WRITE("c_KgInfConnection::SetConfiguration ");
}
#ifdef _FDO_3_2
/// \brief
/// Forces the writes of any cached data to the targed datastore.
/// 
/// \return
/// Returns nothing
/// 
void c_KgInfConnection::Flush()
{
}
#endif
    
c_KgInfSpatialContextCollection* c_KgInfConnection::GetSpatialContexts ( bool bDynamic )
{
  FdoPtr<c_KgInfSchemaDesc> schemadesc = GetSchemaDesc();
	return schemadesc->GetSpatialContexts();
}


bool c_KgInfConnection::GetOracleSridDesc(FdoGeometricPropertyDefinition* GeomProp,c_KgInfSridDesc& OraSridDesc)
{
  FdoString *csname = GeomProp->GetSpatialContextAssociation();
  FdoPtr<c_KgInfSpatialContextCollection> sccol = GetSpatialContexts();
  
  FdoPtr<c_KgInfSpatialContext> sc = sccol->FindItem(csname);
  
  if( !sc.p ) return false;
  
  OraSridDesc = sc->GetOraSridDesc();
  return true;
  
}//end of c_KgInfConnection::GetOracleSridDesc

bool c_KgInfConnection::GetOracleSridDesc(FdoClassDefinition* ClassDef,c_KgInfSridDesc& OraSrid)
{
  if( ClassDef->GetClassType() != FdoClassType_FeatureClass )
  {
    return false;
  }
  
  FdoFeatureClass* fclass = (FdoFeatureClass*)ClassDef;
  
  FdoPtr<FdoGeometricPropertyDefinition> geomprop = fclass->GetGeometryProperty();
  
  if( !geomprop.p ) return false;
  
  
  FdoString *csname = geomprop->GetSpatialContextAssociation();
  FdoPtr<c_KgInfSpatialContextCollection> sccol = GetSpatialContexts();
  
  
  
    
  FdoPtr<c_KgInfSpatialContext> sc = sccol->FindItem(csname);
  
  if( !sc.p ) 
  {
    // Now test if
    FdoStringP name = csname;
    FdoStringP temp = name.Mid(0,10);
    if( (temp.ICompare(D_INF_SRID_NAME_PREFIX) == 0) )
    {
      FdoStringP temp = name.Mid(10,name.GetLength()-10);
      OraSrid.m_OraSrid = temp.ToLong();
      
         
      FdoStringP wkt =  sc->GetCoordinateSystemWkt();        
      OraSrid.m_IsGeodetic = c_FdoInf_API::IsGeodeticCoordSystem((const char*)wkt); // TODO: it should return real value
    }
    else
    {
      FdoStringP name = sc->GetCoordinateSystem();
      FdoStringP temp = name.Mid(0,10);
      if( (temp.ICompare(D_INF_SRID_NAME_PREFIX) == 0) )
      {
        FdoStringP temp = name.Mid(10,name.GetLength()-10);
        OraSrid.m_OraSrid = temp.ToLong();
           
        FdoStringP wkt =  sc->GetCoordinateSystemWkt();                   
        OraSrid.m_IsGeodetic = c_FdoInf_API::IsGeodeticCoordSystem((const char*)wkt); // TODO: it should return real value
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
  
}//end of c_KgInfConnection::GetOracleSrid

/*
oracle::occi::Statement* c_KgInfConnection::OCCI_CreateStatement()
{
  return m_OcciConnection->createStatement();  
}//end of c_KgInfConnection::OCCI_CreateStatement

void c_KgInfConnection::OCCI_Commit()
{
  return m_OcciConnection->commit();  
}//end of c_KgInfConnection::OCCI_CreateStatement

void c_KgInfConnection::OCCI_TerminateStatement(oracle::occi::Statement* Statement)
{
  
  if( m_OcciConnection ) m_OcciConnection->terminateStatement (Statement);
  
}//end of c_KgInfConnection::OCCI_CreateStatement
*/

c_KgInfSchemaDesc* c_KgInfConnection::GetSchemaDesc()
{
  if( m_SchemaDesc.p == NULL )
  {
    m_SchemaDesc = c_FdoInf_API::DescribeSchema(this,m_OraSchemaName.c_str(),m_FdoViewsTable.c_str());
  }
  return FDO_SAFE_ADDREF(m_SchemaDesc.p);
}//end of c_KgInfConnection::GetSchemaDesc


otl_stream* c_KgInfConnection::OTL_CreateStream(int BuffSize,const char* SqlStm,const int implicit_select)
{
  try
  {
    return m_KgOtl.CreateStream(BuffSize,SqlStm,implicit_select);
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
  
}//end of c_KgInfConnection::OTL_CreateStream

void c_KgInfConnection::OTL_OpenStream(otl_stream*& Stream,int BuffSize,const char* SqlStm,const int implicit_select)
{
  try
  {
    return m_KgOtl.OpenStream(Stream,BuffSize,SqlStm,implicit_select);
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
  
}//end of c_KgInfConnection::OTL_CreateStream

void c_KgInfConnection::OTL_DeleteStream(otl_stream*& Stream)
{
  if( !Stream ) return;
  
  try
  {
    c_KgOtl::DeleteStream(Stream);
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
}
void c_KgInfConnection::OTL_SetCommit(otl_stream* Stream,int Commit)
{
  if( !Stream ) return;
  
  try
  {
    c_KgOtl::SetCommit(Stream,Commit);
    
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
}
void c_KgInfConnection::OTL_FlushStream(otl_stream* Stream)
{
  if( !Stream ) return;
  
  try
  {
    //Stream->flush();
    c_KgOtl::FlushStream(Stream);
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
}
/*
bool c_KgInfConnection::OTL_ReadNext(otl_stream* Stream)
{
  if( !Stream ) return false;
  
  try
  {
    return c_KgOtl::ReadNext(Stream);    
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
}
*/
long c_KgInfConnection::OTL_GetRpc(otl_stream* Stream)
{
  if( !Stream ) return false;
  
  try
  {
    
    return c_KgOtl::GetRpc(Stream);    
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
}
void c_KgInfConnection::OTL_Commit()
{
  try
  {    
    m_KgOtl.Commit();
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
  
}
long c_KgInfConnection::OTL_ExecuteSql(const char*Sql)
{
  try
  {    
    return m_KgOtl.ExecuteSql(Sql);
  }
  catch(c_KgOtlException& ea)
  {   
    throw FdoException::Create( ea.what() );    
  }
  
}

