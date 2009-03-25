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

#ifndef _c_SDO_DIM_ELEMENT_
#define _c_SDO_DIM_ELEMENT_



#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif


/*---------------------------------------------------------------------------
PUBLIC TYPES AND CONSTANTS
---------------------------------------------------------------------------*/

#define D_OCI_ARRAY_SIZE                 32
#define D_OCI_TYPE_OWNER                 L"MDSYS"
#define D_OCI_SDO_GEOMETRY               L"MDSYS.SDO_GEOMETRY"
#define D_OCI_SDO_DIM_ARRAY            L"MDSYS.SDO_DIM_ARRAY"
#define D_OCI_SDO_DIM_ELEMENT            L"MDSYS.SDO_DIM_ELEMENT"




struct SDO_DIM_ELEMENT
{
   OCIString * SDO_DIMNAME;
   OCINumber SDO_LB;
   OCINumber SDO_UB;
   OCINumber SDO_TOLERANCE;
};
typedef struct SDO_DIM_ELEMENT SDO_DIM_ELEMENT;

struct SDO_DIM_ELEMENT_ind
{
   OCIInd _atomic;
   OCIInd SDO_DIMNAME;
   OCIInd SDO_LB;
   OCIInd SDO_UB;
   OCIInd SDO_TOLERANCE;
};
typedef struct SDO_DIM_ELEMENT_ind SDO_DIM_ELEMENT_ind;


class c_Oci_Connection;

class c_SDO_DIM_ELEMENT
{
  public:
    c_SDO_DIM_ELEMENT(OCIEnv* Env=NULL,OCIError *Err=NULL,SDO_DIM_ELEMENT* Dim_Element=NULL,SDO_DIM_ELEMENT_ind* Dim_Element_Ind=NULL);
    ~c_SDO_DIM_ELEMENT();
    
    static c_SDO_DIM_ELEMENT* Create(c_Oci_Connection* Conn);
    static c_SDO_DIM_ELEMENT* CreateNull( c_Oci_Connection* Conn );
    
  public:
    OCIEnv* m_OciEnv;
    OCIError *m_OciErr;
    
    bool m_FreeObject;
    SDO_DIM_ELEMENT* m_Dim_Element;
    SDO_DIM_ELEMENT_ind* m_Dim_Element_Ind;
    
  public:
    bool IsNull();
    bool IsNullDimName();
    bool IsNullLB();
    bool IsNullUB();
    bool IsNullTolerance();
    
    wchar_t* GetDimName();
    double GetTolerance();
    double GetLB();
    double GetUB();
    
    void SetLB(double Val);
    void SetUB(double Val);
    void SetTolerance(double Val);
    void SetDimName(const wchar_t* Val);
};


class c_SDO_DIM_ARRAY
{
  public:
    c_SDO_DIM_ARRAY(OCIEnv* Env=NULL,OCIError *Err=NULL,const OCIArray* Array=NULL)
    {
      m_OciEnv = Env;
      m_OciErr = Err;
      m_Array = Array;
    }
    
    
  public:
    const OCIArray* m_Array;
    OCIEnv* m_OciEnv;
    OCIError *m_OciErr;
    
  public:
    int GetSize() 
    { 
      if(!m_Array ) return 0;
      sb4 size;
      OCICollSize(m_OciEnv,m_OciErr,m_Array,&size);
      return size;
    }
    c_SDO_DIM_ELEMENT GetDimElement(int Index)
    {
      boolean exists;
      SDO_DIM_ELEMENT *dimelem;
      SDO_DIM_ELEMENT_ind *dimelem_ind;
      OCICollGetElem(m_OciEnv,m_OciErr,m_Array,Index,&exists,(void**)&dimelem,(void**)&dimelem_ind);
      
      return c_SDO_DIM_ELEMENT(m_OciEnv,m_OciErr,dimelem,dimelem_ind);
      
    }      
};



#endif
