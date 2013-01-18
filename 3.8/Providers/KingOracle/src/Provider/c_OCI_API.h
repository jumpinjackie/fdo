/*
* Copyright (C) 2009  SL-King d.o.o
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

#ifndef _c_OCI_API_h
#define _c_OCI_API_h



#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif

#include "OCI_SDO_GEOM_TYPES.h"
#include "c_SDO_GEOMETRY.h"
#include "c_Oci_ColumnData.h"
#include "c_Oci_Statement.h"


/*---------------------------------------------------------------------------
PUBLIC TYPES AND CONSTANTS
---------------------------------------------------------------------------*/

#define D_OCI_ARRAY_SIZE                 32
#define D_OCI_TYPE_OWNER                 L"MDSYS"
#define D_OCI_SDO_GEOMETRY               L"MDSYS.SDO_GEOMETRY"
#define D_OCI_SDO_DIM_ARRAY            L"MDSYS.SDO_DIM_ARRAY"
#define D_OCI_SDO_DIM_ELEMENT            L"MDSYS.SDO_DIM_ELEMENT"


/*******************************************************************************
**                        PRIVATE GLOBALS
*******************************************************************************/

//static OCIServer 	*srvhp;
//static OCISvcCtx 	*svchp;
//static OCISession 	*usrhp;
//static OCIStmt 		*stmthp;
//static OCIDescribe	*dschp = NULL;

//static OCINumber         global_gid[D_OCI_ARRAY_SIZE];
//static SDO_GEOMETRY_TYPE *global_geom_obj[D_OCI_ARRAY_SIZE];/* spatial object buffer */
//static SDO_GEOMETRY_ind  *global_geom_ind[D_OCI_ARRAY_SIZE]; /* Object indicator */

class c_Oci_Connection;



class c_Oci_Connection
{
  public:
    c_Oci_Connection(OCIEnv* OciEnv, OCIError* OciError);
    ~c_Oci_Connection();
    
    c_Oci_Statement* CreateStatement()
    {
      return new c_Oci_Statement(this);
    }
    void TerminateStatement(c_Oci_Statement* Stm)
    {
      delete Stm;
    }
    
    void LogOn(const wchar_t* UserName,const wchar_t* Password,const wchar_t* DbLink);
    void LogOff();
    void OciCheckError(sword status);
    
    bool IsSdoTypes() { return m_OciType_SdoGeometry!=NULL && m_OciType_SdoDimArray!=NULL && m_OciType_SdoDimElement!=NULL; }

  public:
  
    OCIEnv 		*m_OciHpEnvironment;  // do not free in destructor
    OCIError 	*m_OciHpError;        // do not free in destructor
  
  // Next handlers are allocated in  c_Oci_Connection 
  // Need to free them in destructor 
    OCISvcCtx *m_OciHpServiceContext;
    OCIDescribe	*m_OciHpDescribe;
    OCIType *m_OciType_SdoGeometry;
    OCIType *m_OciType_SdoDimArray;
    OCIType *m_OciType_SdoDimElement;
    
  protected:
    OCIType *GetOciType(const wchar_t *TypeName);    
};

#define D_OCI_ERROR_LEN 1024
class c_Oci_Exception
{
  public:
    c_Oci_Exception(int ErrorStatus,int ErrorCode=0,const wchar_t* ErrorText=NULL)
    {
      m_ErrorStatus = ErrorStatus;
      m_ErrorCode = ErrorCode;
      if( ErrorText )
      {
        if( wcslen(ErrorText)>D_OCI_ERROR_LEN )
          wcscpy(m_ErrorText,L"c_Oci_Exception: Error Text to big to fit in buffer.");
        else
          wcscpy(m_ErrorText,ErrorText);
      }
      else
        m_ErrorText[0]=0;
    }
    
    const wchar_t* GetErrorText() { return &m_ErrorText[0]; }
    const wchar_t* what() { return GetErrorText(); }
    int GetErrorCode() { return m_ErrorCode; }
    
    int* GetErrorCodePtr() { return &m_ErrorCode; }
    unsigned char* GetErrorTextBuffPtr() { return (unsigned char*)&m_ErrorText[0]; }
    int GetErrorTextBuffSize() { return D_OCI_ERROR_LEN; }
    
  protected:
    int m_ErrorStatus;
    
    wchar_t m_ErrorText[D_OCI_ERROR_LEN+2];    
    unsigned int m_ErrorTextLen;
    int m_ErrorCode ;
};






class c_OCI_API
{
public:
  c_OCI_API(void);
public:
  ~c_OCI_API(void);
  
protected:  
  
  
public:  
  static bool IsInit();
  
  static void OciInit();
  
  //oracle::occi::Connection* m_Conn;
  //oracle::occi::Environment* m_Env;
  
  
  static c_Oci_Connection* CreateConnection(const wchar_t*User,const wchar_t*Password,const wchar_t* DbLink);
  //static oracle::occi::Connection * CreateConnection(const char*User,const char*Password,const char*DbLink);
  
  
  //static void c_OCI_API::CloseConnection(oracle::occi::Connection * Conn);
  static void CloseConnection(c_Oci_Connection* Conn);
  static void OciCheckError(OCIError *errhp, sword status);
  static void OciTerminate();

  // Returns column name's of primary key of given table.
  // No primary key - returns no column names
  //static int GetTablePkeyColumns(oracle::occi::Connection * OcciConnection,const char* Owner,const char* TableName,vector<string>& ColNames);
  static int GetTablePkeyColumns(c_Oci_Connection * OciConn,const wchar_t* Owner, const wchar_t* TableName,std::vector<std::wstring>& ColNames);
  
  //static FdoCommonThreadMutex m_Mutex;

  //static oracle::occi::Environment* GetEnvironment();
  
  
  
  
  
  
  
  
public:
  //static oracle::occi::StatelessConnectionPool* c_OCI_API::GetConnPool(const char*User,const char*Password,const char* DbLink,int& IndCpDesc);
  /*******************************************************************************
  **                        PUBLIC GLOBALS
  *******************************************************************************/
  static OCIEnv 		*m_OciHpEnvironment;                       /* OCI general handles */
  static OCIError 	*m_OciHpError;
  //static OCICPool   *m_OciHpPool;
 
  
};




#endif
