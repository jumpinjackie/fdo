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
#include "c_FdoSde_API.h"

    
    
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
 : superclass(Connection,OcciStatement ,GeomPropSqlIndex, SqlColumns)
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






//---------------------------------------------------------------------
//
//    c_KgOraSdeFeatureReader
//
//---------------------------------------------------------------------

c_KgOraSdeFeatureReader::c_KgOraSdeFeatureReader(c_KgOraConnection * Connection
                                           ,c_Oci_Statement* OcciStatement  
                                           ,FdoClassDefinition* ClassDef
                                           ,c_KgOraSridDesc& SridDesc
                                           ,int SdeGeometryType
                                           ,int GeomPropSqlIndex, FdoStringCollection* SqlColumns
                                           ,FdoIdentifierCollection* Props)
                                           : c_KgOraFeatureReader(Connection,OcciStatement ,ClassDef,GeomPropSqlIndex, SqlColumns,Props)
{

 
  m_SridDesc=SridDesc;
  m_SdeGeometryType = SdeGeometryType;
}

c_KgOraSdeFeatureReader::~c_KgOraSdeFeatureReader()
{
  //Close();
  

}

void c_KgOraSdeFeatureReader::Dispose()
{
  delete this;
}

FdoByteArray* c_KgOraSdeFeatureReader::GetGeometry(FdoString* propertyName)
{
  int len = 0;
  const void* ptr = GetGeometry(propertyName, &len);

  if( len > 0 )
    return FdoByteArray::Create((const FdoByte*)ptr, len);
  else
    throw FdoException::Create(L"c_KgOraReader::GetGeometry Invalid Geometry !");
}




const FdoByte* c_KgOraSdeFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* len)
{



  if( m_OciStatement )
  {
    
    try
    {
      int oraind = PropNameToColumnNumber(propertyName); 
      if( !m_OciStatement->IsColumnNull(oraind) )
      {
        //int oraind = PropNameToColumnNumber(propertyName); 
        int sde_numofpts = m_OciStatement->GetInteger(oraind+1);
        int sde_entity = m_OciStatement->GetInteger(oraind+2);
        
        
        long length = m_OciStatement->GetLongRawLength(oraind); // oracle is 1 based - our index is 0 based        
        unsigned char* sdata = m_OciStatement->GetLongRaw(oraind);
        m_SdeAgfConv.SetSdeGeometry(&m_SridDesc,m_SdeGeometryType,sde_numofpts,sde_entity,length,sdata);
        *len = m_SdeAgfConv.ToAGF();
        return (const unsigned char*)m_SdeAgfConv.GetBuff();
      }
    }
    catch(FdoException* ea)
    {      
      throw ea;
    }
    catch(c_Oci_Exception* ea)
    {
      delete ea;
      //printf("\n----------------------c_KgOraReader::GetGeometry: occi::SQLException Exception ---------------------- ");
      *len=0;

      throw FdoException::Create(L"c_KgOraReader::GetGeometry SQLException !");
    }
    catch(...)
    {
      //printf("\n----------------------c_KgOraReader::GetGeometry: Uknown Exception ---------------------- ");
      *len=0;
      throw FdoException::Create(L"c_KgOraReader::GetGeometry Uknown Exception !");
    }

    /*
    *len=0;
    if( geom )
    {
      m_SdoAgfConv.SetGeometry(geom);
      *len = m_SdoAgfConv.ToAGF();

      delete geom;
    }
    else
    {
      throw FdoException::Create(L"c_KgOraReader::GetGeometry NULL SDO_GEOMETRY!");
    }


    return (const unsigned char*)m_SdoAgfConv.GetBuff();
    */
  }
  


  return NULL;
}