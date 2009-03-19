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
#include <time.h>


#include "c_SdoGeomToAGF2.h"
#include "c_LogAPI.h"


    
    
//---------------------------------------------------------------------
//
//    c_KgOraFeatureReader
//
//---------------------------------------------------------------------

c_KgOraFeatureReader::c_KgOraFeatureReader(c_KgOraConnection * Connection
                                      ,c_Oci_Statement* OcciStatement  
                                        ,FdoClassDefinition* ClassDef
                                        ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
                                        ,FdoIdentifierCollection* Props)
 : c_KgOraReader(Connection,OcciStatement ,GeomPropSqlIndex, SqlColumns)
{

  m_ClassDef = ClassDef;
  FDO_SAFE_ADDREF (m_ClassDef.p);



  m_Props = Props;
  if (m_Props) m_Props->AddRef();

  
}

c_KgOraFeatureReader::~c_KgOraFeatureReader()
{
    //Close();
    FDO_SAFE_RELEASE(m_Props);
    
}

void c_KgOraFeatureReader::Dispose()
{
    delete this;
}

FdoClassDefinition* c_KgOraFeatureReader::GetClassDefinition()
{     
    if( m_ClassDef )
    {
      if( m_Props && (m_Props->GetCount() > 0 ) )
      {
        FdoClassDefinition* newclass = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(m_ClassDef);
        if( newclass )
        {
          FdoPtr<FdoPropertyDefinitionCollection> ids = newclass->GetProperties();
          long count = ids->GetCount();
          long ind =0;
          while(ind<count)
          {
            FdoPtr<FdoPropertyDefinition> classprop = ids->GetItem(ind);
            bool found=false;
            for(long ind2 =0;ind2<m_Props->GetCount();ind2++)
            {
              FdoPtr<FdoIdentifier> prop2 = m_Props->GetItem(ind2);
              if( wcscmp(classprop->GetName(),prop2->GetName()) == 0 )
              {
                found=true;
                break;
              }
            }
            if( !found )
            {
              ids->RemoveAt(ind);
              count = ids->GetCount();
            }
            else
            {
              ind++;
            }                          
          }
        }
        
        return newclass;
      }
      else
      {
        return FDO_SAFE_ADDREF(m_ClassDef.p);
      }
    }
    
    return NULL;
}

FdoInt32 c_KgOraFeatureReader::GetDepth()
{
    return 0;
}



FdoIFeatureReader* c_KgOraFeatureReader::GetFeatureObject(FdoString* propertyName)
{
    return NULL;
}






