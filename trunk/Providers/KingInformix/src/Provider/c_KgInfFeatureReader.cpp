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
#include "StdAfx.h"


#include <time.h>



#include "c_LogApi.h"


    
    
//---------------------------------------------------------------------
//
//    c_KgInfFeatureReader
//
//---------------------------------------------------------------------

c_KgInfFeatureReader::c_KgInfFeatureReader(c_KgInfConnection * Connection
                                      ,otl_stream* OtlStream,c_KgOtlStreamReader* OtlStreamReader
                                        ,FdoClassDefinition* ClassDef
                                        ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
                                        ,FdoIdentifierCollection* Props)
 : c_KgInfReader(Connection,OtlStream,OtlStreamReader,GeomPropSqlIndex, SqlColumns), m_ClassDef(ClassDef)
{
  
  FDO_SAFE_ADDREF (m_ClassDef.p);
  
  if( Props )
  {
    FdoString *name;
    FdoInt32 count = Props->GetCount();
    for(FdoInt32 ind=0;ind<count;ind++)
    {
      FdoIdentifier * ident = Props->GetItem(ind);
      
      name = ident->GetName();
    }
  }
  

  m_Props = Props;
  if (m_Props) m_Props->AddRef();

  
}

c_KgInfFeatureReader::~c_KgInfFeatureReader()
{
    Close();
    FDO_SAFE_RELEASE(m_Props);
    
}

void c_KgInfFeatureReader::Dispose()
{
    delete this;
}

FdoClassDefinition* c_KgInfFeatureReader::GetClassDefinition()
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

FdoInt32 c_KgInfFeatureReader::GetDepth()
{
    return 0;
}



FdoIFeatureReader* c_KgInfFeatureReader::GetFeatureObject(FdoString* propertyName)
{
    return NULL;
}






