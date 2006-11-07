#include "StdAfx.h"
#include "c_KgOraSchemaPool.h"

#include "c_FdoOra_API.h"

c_KgOraSchemaPool::c_KgOraSchemaPool(void)
{
}

c_KgOraSchemaPool::~c_KgOraSchemaPool(void)
{
}

FdoCommonThreadMutex c_KgOraSchemaPool::m_Mutex;

/*
c_KgOraSchemaData* c_KgOraSchemaPool::GetSchemaData(c_KgOraConnection* Connection)
{
  vector< t_SchemaPoolDesc>::iterator iter;
  
  FdoStringP connstr = Connection->GetConnectionString();
  m_Mutex.Enter();
  for(iter =  g_SchemaPoolDesc.begin();iter != g_SchemaPoolDesc.end(); iter++  )
  {
    if( (iter->m_ConnectionString.compare(connstr) == 0)  )
    {
      c_KgOraSchemaData* retschema = iter->m_SchemaData.p;
      m_Mutex.Leave();
      return FDO_SAFE_ADDREF( retschema );
    }
    
  }
  m_Mutex.Leave();
  
// there is no feature schema data for this connection    
  FdoPtr<c_KgOraSchemaData> schemadata = c_FdoOra_API::DescribeSchema(Connection->GetOcciConnection());
}//end of   
*/