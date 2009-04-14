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

# include <occi.h>
using namespace oracle::occi;
#include "RegisterMappings.h"
#include "c_OCCI_API.h"


#include "c_LogAPI.h"



//#define USE_OCII_CONNECTION_POOLING


c_OCCI_API::c_OCCI_API(void)
{
}

c_OCCI_API::~c_OCCI_API(void)
{
}

//FdoCommonThreadMutex c_OCCI_API::m_Mutex;

//static oracle::occi::Environment *g_Env = NULL;

//static oracle::occi::StatelessConnectionPool* g_ConnPool= NULL;

typedef struct t_ConnPoolDesc
{
  string m_UserName;
  string m_Password;
  string m_DbLink;
  
  oracle::occi::StatelessConnectionPool* m_ConnPool;
  
  vector <StatelessConnectionPool*> m_Connections;
  
}t_ConnPoolDesc;

static vector<t_ConnPoolDesc> g_ConnPoolDesc;

typedef struct t_ConnToPool
{
  Connection* m_Connection;
  int m_IndCPDesc; // index in g_ConnPoolArray from which pool this connection was open
}t_ConnToPool;

static vector<t_ConnToPool> g_ConnToPool;

void c_OCCI_API::OcciInit()
{
try {
  //m_Mutex.Enter();
  //printf("\nOCCI Init");
	//g_Env = oracle::occi::Environment::createEnvironment( (Environment::Mode)(oracle::occi::Environment::OBJECT  | oracle::occi::Environment::THREADED_MUTEXED) );
	
  //RegisterClasses(g_Env);    
  //m_Mutex.Leave();
}
catch(oracle::occi::SQLException& ea)
{
  
  FdoStringP gstr = ea.getMessage().c_str();
  throw FdoException::Create (gstr);
  
}
catch(...)
{
  throw FdoException::Create (L"Unknown exception");
}  

}//end of c_OCCI_API::OcciInit

bool c_OCCI_API::IsInit() 
{ 
  //return g_Env != NULL; 
  return true;
}

//oracle::occi::Environment* c_OCCI_API::GetEnvironment() { return g_Env; }

/*
StatelessConnectionPool* c_OCCI_API::GetConnPool(const char*User,const char*Password,const char* DbLink,int& IndCpDesc)
{
  vector< t_ConnPoolDesc>::iterator iter;
  IndCpDesc = 0;
  
  for(iter =  g_ConnPoolDesc.begin();iter != g_ConnPoolDesc.end(); iter++  )
  {
    if( (iter->m_UserName.compare(User) == 0) && (iter->m_Password.compare(Password) == 0) && (iter->m_DbLink.compare(DbLink) == 0) )
    {
      StatelessConnectionPool* retconn = iter->m_ConnPool;
      
      return retconn;
    }
    IndCpDesc++;
  }
  

  #ifdef _DEBUG
    printf("\Creating new connection pool!");
  #endif  
  t_ConnPoolDesc newdesc;
  StatelessConnectionPool *newpool = g_Env->createStatelessConnectionPool(User, Password, DbLink, 10, 2, 2,StatelessConnectionPool::HOMOGENEOUS);
  
  newdesc.m_ConnPool = newpool;
  newdesc.m_UserName = User;
  newdesc.m_Password = Password;
  newdesc.m_DbLink = DbLink;

  //D_KGORA_ELOG_WRITE1("Created new Connection Pool '%s'",newpool->getPoolName().c_str());  
  
  
  
  g_ConnPoolDesc.push_back(newdesc);
  
  IndCpDesc = g_ConnPoolDesc.size()-1;
  

  return newpool;  
  
}//end of c_OCCI_API::GetConnPool
*/

 void c_OCCI_API::CreateConnection(const char*User,const char*Password,const char* DbLink,oracle::occi::Connection*& Conn,oracle::occi::Environment*& Env)
{

#ifdef USE_OCII_CONNECTION_POOLING
  int ind_cpdesc=0;

  //m_Mutex.Enter();
    
  StatelessConnectionPool *connpool = GetConnPool(User,Password,DbLink,ind_cpdesc);
  
    
  #ifdef _DEBUG    
    printf("\n Pool '%s' Create Conn: open:%ld busy:%ld ",connpool->getPoolName().c_str(),(long)connpool->getOpenConnections(),(long)connpool->getBusyConnections());    
  #endif


  //D_KGORA_ELOG_WRITE3("Pool  '%s' Create Conn: open:%ld busy:%ld",connpool->getPoolName().c_str(),(long)connpool->getOpenConnections(),(long)connpool->getBusyConnections());

  oracle::occi::Connection * newconn = connpool->getConnection();
  
  t_ConnToPool contopool;
  
  contopool.m_Connection = newconn;
  contopool.m_IndCPDesc = ind_cpdesc;
      
  // remember this connection in list
  
  g_ConnToPool.push_back(contopool);
  //m_Mutex.Leave();
  
  return newconn;

#else  
  Env = oracle::occi::Environment::createEnvironment( (Environment::Mode)(oracle::occi::Environment::OBJECT  | oracle::occi::Environment::THREADED_MUTEXED) );
	
  RegisterClasses(Env);    
  Conn= Env->createConnection(User, Password, DbLink);
#endif    
}//end of c_OCCI_API::CreateConnection

void c_OCCI_API::CloseConnection(oracle::occi::Connection *& Conn,oracle::occi::Environment*& Env)
{
 
 #ifdef USE_OCII_CONNECTION_POOLING 
  // 
  //m_Mutex.Enter();
  
  // remove connection from list
  for( vector<t_ConnToPool>::iterator iter = g_ConnToPool.begin(); iter != g_ConnToPool.end();iter++ )
  {
    if( iter->m_Connection == Conn )
    {
      StatelessConnectionPool *connpool = g_ConnPoolDesc[iter->m_IndCPDesc].m_ConnPool;
      #ifdef _DEBUG    
        printf("\n Pool '%s' Close Conn: open:%ld busy:%ld",connpool->getPoolName().c_str(),(long)connpool->getOpenConnections(),(long)connpool->getBusyConnections());    
      #endif

      //D_KGORA_ELOG_WRITE3("Pool '%s' Close Conn: ope:%ld busy:%ld",connpool->getPoolName().c_str(),(long)connpool->getOpenConnections(),(long)connpool->getBusyConnections());
      
  
      g_ConnPoolDesc[iter->m_IndCPDesc].m_ConnPool->releaseConnection(Conn);          
      g_ConnToPool.erase(iter);
      //m_Mutex.Leave();
      break;
    }
  }
  
  //m_Mutex.Leave();
#else
  Env->terminateConnection(Conn);
  oracle::occi::Environment::terminateEnvironment(Env);
#endif  
}//end of c_OCCI_API::CloseConnection

/*
  Returns names of columns which are included in primary key of table.
  Returns number of coulmns included in primary key.
  If primary key doesn't exist returns 0 and emtpy ColNames list.
*/
int c_OCCI_API::GetTablePkeyColumns(oracle::occi::Connection * OcciConnection,const char* Owner, const char* TableName,vector<string>& ColNames)
{
    int numcols=0;
    
  Statement *stm = OcciConnection->createStatement(); 
  
  
  stm->setSQL("SELECT acc.column_name"
              " FROM all_constraints ac, all_cons_columns acc"
              " WHERE ac.owner = :1 and ac.table_name = :2 AND ac.constraint_type = 'P'"
              " AND ac.owner = acc.owner AND ac.constraint_name = acc.constraint_name"
             );
  
  stm->setString(1,Owner);
  stm->setString(2,TableName);
  
    
  /*         
  stm->setSQL("select COLUMN_NAME "
      " FROM ALL_CONS_COLUMNS "
      " WHERE TABLE_NAME = :1 "
      " AND Owner = :2 " 
      " AND CONSTRAINT_NAME = ( SELECT CONSTRAINT_NAME FROM ALL_CONSTRAINTS "
      " WHERE TABLE_NAME = :3 "
      " AND CONSTRAINT_TYPE = 'P' "
      " AND Owner = :4 " 
      " ) ORDER BY POSITION");
             
             
  stm->setString(1,TableName);
  stm->setString(2,Owner);
  stm->setString(3,TableName);
  stm->setString(4,Owner);
  */
  
  ResultSet * rs = stm->executeQuery();
  while( rs->next() )
  {
    ColNames.push_back(rs->getString(1));
    numcols++;
  }
  
  stm->closeResultSet(rs);
  OcciConnection->terminateStatement (stm);
        
  return numcols;
}//end of c_OCCI_API::GetTablePkeyColumns


/*
  
*/

/*
int c_OCCI_API::QuerySpatialContext(oracle::occi::Connection * OcciConnection,const char* Owner, const char* TableName,vector<string>& ColNames)
{
    int numcols=0;
    
  Statement *stm = OcciConnection->createStatement(); 
  
  stm->setSQL("SELECT acc.column_name"
              " FROM all_constraints ac, all_cons_columns acc"
              " WHERE ac.owner = :1 and ac.table_name = :2 AND ac.constraint_type = 'P'"
              " AND ac.constraint_name = acc.constraint_name"
             );
  stm->setString(1,Owner);
  stm->setString(2,TableName);
  ResultSet * rs = stm->executeQuery();
  while( rs->next() )
  {
    ColNames.push_back(rs->getString(1));
    numcols++;
  }
  
  stm->closeResultSet(rs);
  OcciConnection->terminateStatement (stm);
        
  return numcols;
}//end of c_OCCI_API::GetTablePkeyColumns
*/
