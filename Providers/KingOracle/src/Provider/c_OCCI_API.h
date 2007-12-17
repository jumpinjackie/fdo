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

#ifndef _c_OCCI_API_h
#define _c_OCCI_API_h

# include <occi.h>
using namespace oracle::occi;

class c_OCCI_API
{
public:
  c_OCCI_API(void);
public:
  ~c_OCCI_API(void);
  
protected:  
  
  
public:  
  static bool IsInit();
  
  static void OcciInit();
  
  oracle::occi::Connection* m_Conn;
  oracle::occi::Environment* m_Env;
  
  static void CreateConnection(const char*User,
                               const char*Password,
                               const char* DbLink,
                               oracle::occi::Connection*& Conn,
                               oracle::occi::Environment*& Env);

  //static oracle::occi::Connection * CreateConnection(const char*User,const char*Password,const char*DbLink);
  //static void CloseConnection(oracle::occi::Connection * Conn);
  
  static void CloseConnection(oracle::occi::Connection*& Conn, 
                              oracle::occi::Environment*& Env);

  
  // Returns column name's of primary key of given table.
  // No primary key - returns no column names
  static int GetTablePkeyColumns(oracle::occi::Connection * OcciConnection,
                                 const char* Owner, 
                                 const char* TableName,
                                 vector<string>& ColNames);
  
  //static FdoCommonThreadMutex m_Mutex;
  //static oracle::occi::Environment* GetEnvironment();
  
private:
  static oracle::occi::StatelessConnectionPool* GetConnPool(const char*User,
                                                            const char*Password,
                                                            const char* DbLink,
                                                            int& IndCpDesc);
  
};


#endif
