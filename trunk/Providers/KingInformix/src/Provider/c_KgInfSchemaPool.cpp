/*
* Copyright (C) 2006  Haris Kurtagic
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
#include "StdAfx.h"
#include "c_KgInfSchemaPool.h"



c_KgInfSchemaPool::c_KgInfSchemaPool(void)
{
}

c_KgInfSchemaPool::~c_KgInfSchemaPool(void)
{
}

FdoCommonThreadMutex c_KgInfSchemaPool::m_Mutex;

/*
c_KgInfSchemaData* c_KgInfSchemaPool::GetSchemaData(c_KgInfConnection* Connection)
{
  vector< t_SchemaPoolDesc>::iterator iter;
  
  FdoStringP connstr = Connection->GetConnectionString();
  m_Mutex.Enter();
  for(iter =  g_SchemaPoolDesc.begin();iter != g_SchemaPoolDesc.end(); iter++  )
  {
    if( (iter->m_ConnectionString.compare(connstr) == 0)  )
    {
      c_KgInfSchemaData* retschema = iter->m_SchemaData.p;
      m_Mutex.Leave();
      return FDO_SAFE_ADDREF( retschema );
    }
    
  }
  m_Mutex.Leave();
  
// there is no feature schema data for this connection    
  FdoPtr<c_KgInfSchemaData> schemadata = c_FdoOra_API::DescribeSchema(Connection->GetOcciConnection());
}//end of   
*/