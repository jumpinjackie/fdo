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



//#include "RegisterMappings.h"
#include "c_OCI_API.h"


//#include "c_LogAPI.h"



//#define USE_OCII_CONNECTION_POOLING

OCIEnv 		*c_OCI_API::m_OciHpEnvironment=NULL;                       /* OCI general handles */
OCIError 	*c_OCI_API::m_OciHpError=NULL;

c_OCI_API::c_OCI_API(void)
{
}

c_OCI_API::~c_OCI_API(void)
{
}

//FdoCommonThreadMutex c_OCI_API::m_Mutex;

//static oracle::occi::Environment *g_Env = NULL;

//static oracle::occi::StatelessConnectionPool* g_ConnPool= NULL;




/*******************************************************************************
**
** Routine:     c_OCI_API::OciCheckError
**
** Description: Error message routine
**
*******************************************************************************/
void c_OCI_API::OciCheckError(OCIError *errhp, sword status)
{
  switch (status)
  {
    case OCI_SUCCESS:
      break;
    case OCI_SUCCESS_WITH_INFO:
      //fprintf(stderr, "OCI_SUCCESS_WITH_INFO\n");
      
      break;
    case OCI_ERROR:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status);
      OCIErrorGet((dvoid *)errhp, (ub4)1, (text *)NULL, ociexc->GetErrorCodePtr(), 
        ociexc->GetErrorTextBuffPtr(), (ub4)ociexc->GetErrorTextBuffSize(), OCI_HTYPE_ERROR);
      throw ociexc;
    }
    break;
    case OCI_NEED_DATA:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status,0,L"OCI_NEED_DATA");
      throw ociexc;      
    }
    break;
    case OCI_NO_DATA:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status,0,L"OCI_NO_DATA");
      throw ociexc;
    }
    break;
    case OCI_INVALID_HANDLE:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status,0,L"OCI_INVALID_HANDLE");
      throw ociexc;
    }
    break;
    
    case OCI_STILL_EXECUTING:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status,0,L"OCI_STILL_EXECUTING");
      throw ociexc;
    }
    break;
    case OCI_CONTINUE:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status,0,L"OCI_CONTINUE");
      throw ociexc;
    }
    break;
    default:
    {
      c_Oci_Exception *ociexc = new c_Oci_Exception(status,0,L"UNKNOWN OCI ERROR STATUS");
      throw ociexc;
    }
    break;
  }

  if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO)
  {
    //disconnect();
  }
}





/* Max, min and increment connections */
static ub4 conMin = 1;
static ub4 conMax = 3;
static ub4 conIncr = 1;

void c_OCI_API::OciInit()
{
  if(!m_OciHpEnvironment) 
    //OCIEnvNlsCreate (&m_OciHpEnvironment, OCI_THREADED | OCI_OBJECT, (dvoid *)0,  NULL, NULL, NULL, 0, (dvoid **)0,OCI_UTF16ID,OCI_UTF16ID);
    OCIEnvNlsCreate (&m_OciHpEnvironment, OCI_THREADED | OCI_OBJECT, (dvoid *)0,  NULL, NULL, NULL, 0, (dvoid **)0,OCI_UTF16ID,OCI_UTF16ID);

  if( !m_OciHpError ) 
    OCIHandleAlloc((dvoid *) m_OciHpEnvironment, (dvoid **) &m_OciHpError, OCI_HTYPE_ERROR,(size_t) 0, (dvoid **) 0);

/*
  (void) OCIHandleAlloc((dvoid *) m_OciHpEnvironment, (dvoid **) &m_OciHpPool, OCI_HTYPE_CPOOL,
    (size_t) 0, (dvoid **) 0);

  // CREATE THE CONNECTION POOL 
  sword lstat;
  if (lstat = OCIConnectionPoolCreate(m_OciHpEnvironment,
    m_OciHpError,m_OciHpPool, &m_OciPoolName, &m_OciPoolNameLen,
    database,(sb4)strlen((const signed char *)database),
    conMin, conMax, conIncr,
    appusername,(sb4)strlen((const signed char *)appusername),
    0,0,
    apppassword,(sb4)strlen((const signed char *)apppassword)
    0,0,
    ,OCI_DEFAULT))
  {
    c_OCI_API::OciCheckError(m_OciHpError,lstat);
    exit(1);
  }
*/  
}//end of c_OCI_API::OcciInit

void c_OCI_API::OciTerminate()
{
  

  if( !m_OciHpError ) OCIHandleFree((dvoid *)m_OciHpError, OCI_HTYPE_ERROR);
  if( !m_OciHpEnvironment ) OCIHandleFree((dvoid *)m_OciHpEnvironment, OCI_HTYPE_ENV);
  

  /*
  (void) OCIHandleAlloc((dvoid *) m_OciHpEnvironment, (dvoid **) &m_OciHpPool, OCI_HTYPE_CPOOL,
  (size_t) 0, (dvoid **) 0);

  // CREATE THE CONNECTION POOL 
  sword lstat;
  if (lstat = OCIConnectionPoolCreate(m_OciHpEnvironment,
  m_OciHpError,m_OciHpPool, &m_OciPoolName, &m_OciPoolNameLen,
  database,(sb4)strlen((const signed char *)database),
  conMin, conMax, conIncr,
  appusername,(sb4)strlen((const signed char *)appusername),
  0,0,
  apppassword,(sb4)strlen((const signed char *)apppassword)
  0,0,
  ,OCI_DEFAULT))
  {
  c_OCI_API::OciCheckError(m_OciHpError,lstat);
  exit(1);
  }
  */  
}//end of c_OCI_API::OcciInit

bool c_OCI_API::IsInit() 
{ 
  return m_OciHpEnvironment != NULL; 
  
}

//oracle::occi::Environment* c_OCI_API::GetEnvironment() { return g_Env; }



c_Oci_Connection* c_OCI_API::CreateConnection(const wchar_t*User,const wchar_t*Password,const wchar_t* DbLink)
{
  
  c_Oci_Connection* newconn = new c_Oci_Connection(m_OciHpEnvironment,m_OciHpError);

try
{
  newconn->LogOn(User,Password,DbLink);
}
catch (c_Oci_Exception* exc)
{
	delete newconn;
	throw exc;
}
  
  
  
  return newconn;
}//end of c_OCI_API::CreateConnection

void c_OCI_API::CloseConnection(c_Oci_Connection* Conn)
{
 
  Conn->LogOff(); 
  
  delete Conn;
}//end of c_OCI_API::CloseConnection

/*
  Returns names of columns which are included in primary key of table.
  Returns number of columns included in primary key.
  If primary key doesn't exist returns 0 and empty ColNames list.
*/


int c_OCI_API::GetTablePkeyColumns(c_Oci_Connection * OciConn,const wchar_t* Owner, const wchar_t* TableName,std::vector<std::wstring>& ColNames)
{
    int numcols=0;
    
  c_Oci_Statement *stm = OciConn->CreateStatement(); 
  
  
  if( Owner )
  {
    stm->Prepare(L"SELECT acc.column_name"
                L" FROM all_constraints ac, all_cons_columns acc"
                L" WHERE ac.owner = :1 and ac.table_name = :2 AND ac.constraint_type = 'P'"
                L" AND ac.owner = acc.owner AND ac.constraint_name = acc.constraint_name"
               );
    
    stm->BindString(1,Owner);
    stm->BindString(2,TableName);
  }
  else
  {
    stm->Prepare(L"SELECT acc.column_name"
                L" FROM user_constraints ac, user_cons_columns acc"
                L" WHERE ac.table_name = :1 AND ac.constraint_type = 'P'"
                L" AND ac.constraint_name = acc.constraint_name"
               );
    
    stm->BindString(1,TableName);
  }
  
  stm->ExecuteSelectAndDefine();
  //ResultSet * rs = stm->executeQuery();
  while( stm->ReadNext() )
  {
    ColNames.push_back(stm->GetString(1));
    numcols++;
  }
  
  OciConn->TerminateStatement (stm);
        
  return numcols;
}//end of c_OCI_API::GetTablePkeyColumns



c_Oci_Connection::c_Oci_Connection( OCIEnv* OciEnv, OCIError* OciError )
{
  m_OciHpEnvironment = OciEnv;
  m_OciHpError = OciError;
  
  m_OciType_SdoGeometry=NULL;
  m_OciHpServiceContext=NULL;
}

c_Oci_Connection::~c_Oci_Connection(  )
{
  
}

void c_Oci_Connection::LogOn( const wchar_t* UserName,const wchar_t* Password,const wchar_t* DbLink )
{
    /*
    sword lstat;
    // Logon in Connection Pool mode 
    if (lstat = OCILogon2(m_OciHpEnvironment, m_OciHpError, &m_OciHpServiceContext,
      (CONST OraText *)UserName, (ub4)strlen((const char *)UserName),
      (CONST OraText *)Password, (ub4)strlen((const char *)Password),
      (CONST OraText *)poolName, (ub4)poolNameLen,
      OCI_CPOOL))
    {
      c_OCI_API::OciCheckError(errhp,lstat);
      exit(1);
    }
    */
    

  ub4 plen = wcslen(Password)*sizeof(wchar_t);
  /*
    c_OCI_API::OciCheckError(m_OciHpError, OCILogon2(m_OciHpEnvironment,m_OciHpError,&m_OciHpServiceContext,(CONST OraText *)UserName, (ub4)wcslen(UserName)*sizeof(wchar_t),
      (CONST OraText *)Password, (ub4)wcslen(Password)*sizeof(wchar_t),(CONST OraText *)DbLink, (ub4)wcslen(DbLink)*sizeof(wchar_t),OCI_DEFAULT));
  */
  c_OCI_API::OciCheckError(m_OciHpError, OCILogon(m_OciHpEnvironment,m_OciHpError,&m_OciHpServiceContext,(CONST OraText *)UserName, (ub4)wcslen(UserName)*sizeof(wchar_t),
    (CONST OraText *)Password, (ub4)wcslen(Password)*sizeof(wchar_t),(CONST OraText *)DbLink, (ub4)wcslen(DbLink)*sizeof(wchar_t)));
    
    // describe spatial object types 
    c_OCI_API::OciCheckError(m_OciHpError, OCIHandleAlloc(m_OciHpEnvironment, (dvoid **)&m_OciHpDescribe, 
      (ub4)OCI_HTYPE_DESCRIBE, (size_t)0,
      (dvoid **)0));


    m_OciType_SdoGeometry = NULL;
    m_OciType_SdoDimArray = NULL;
    m_OciType_SdoDimElement = NULL;
    
    // Ignore error of missing SDO_GEMETRY TYPES
    try
    {
      m_OciType_SdoGeometry = GetOciType(D_OCI_SDO_GEOMETRY);
      
      // describe spatial object types 
     
      m_OciType_SdoDimArray = GetOciType(D_OCI_SDO_DIM_ARRAY);
     
      m_OciType_SdoDimElement = GetOciType(D_OCI_SDO_DIM_ELEMENT);
    }
    catch (c_Oci_Exception* e)
    {
      delete e; // ignore error
    }

}


void c_Oci_Connection::LogOff()
{
  //c_OCI_API::OciCheckError(m_OciHpError, OCIObjectUnpin(m_OciHpEnvironment, m_OciHpError, (dvoid *)m_OciType_SdoGeometry));
    
  /* finalize type descriptor */
  if(m_OciHpDescribe) OCIHandleFree((dvoid *)m_OciHpDescribe, (ub4)OCI_HTYPE_DESCRIBE);
  
  OCILogoff(m_OciHpServiceContext,m_OciHpError);
}

/******************************************************************************
**
** Routine:     get_tdo
**
** Description: Returns the 
**
******************************************************************************/
OCIType *c_Oci_Connection::GetOciType(const wchar_t *TypeName)
{
  OCIParam *paramp = NULL;
  OCIRef *type_ref = NULL;
  OCIType *tdo = NULL;

  c_OCI_API::OciCheckError(m_OciHpError, OCIDescribeAny(m_OciHpServiceContext, m_OciHpError, (text *)TypeName, 
    (ub4)wcslen(TypeName)*sizeof(wchar_t), 
    OCI_OTYPE_NAME, (ub1)1,  (ub1)OCI_PTYPE_TYPE, m_OciHpDescribe));
  c_OCI_API::OciCheckError(m_OciHpError, OCIAttrGet((dvoid *)m_OciHpDescribe, (ub4)OCI_HTYPE_DESCRIBE,
    (dvoid *)&paramp, (ub4 *)0, 
    (ub4)OCI_ATTR_PARAM, m_OciHpError));
  c_OCI_API::OciCheckError(m_OciHpError, OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
    (dvoid *)&type_ref, (ub4 *)0, 
    (ub4)OCI_ATTR_REF_TDO, m_OciHpError));
  c_OCI_API::OciCheckError(m_OciHpError, OCIObjectPin(m_OciHpEnvironment, m_OciHpError, type_ref, (OCIComplexObject *)0, 
    OCI_PIN_ANY, OCI_DURATION_SESSION, 
    OCI_LOCK_NONE, (dvoid **)&tdo));
    
  if (!tdo)
  {
    return NULL;
  }

  return tdo;
}

void c_Oci_Connection::OciCheckError( sword status )
{
  c_OCI_API::OciCheckError(m_OciHpError,status);
}




