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
#include "c_SDO_DIM_ELEMENT.h"

#include "c_OCI_API.h"










c_SDO_DIM_ELEMENT::c_SDO_DIM_ELEMENT( OCIEnv* Env/*=NULL*/,OCIError *Err/*=NULL*/,SDO_DIM_ELEMENT* Dim_Element/*=NULL*/,SDO_DIM_ELEMENT_ind* Dim_Element_Ind/*=NULL*/ )
{
  m_OciEnv = Env;
  m_OciErr = Err;
  m_FreeObject = false;
  m_Dim_Element = Dim_Element;
  m_Dim_Element_Ind = Dim_Element_Ind;
}
c_SDO_DIM_ELEMENT::~c_SDO_DIM_ELEMENT()
{
  if( m_FreeObject )
  {
    if(m_Dim_Element) c_OCI_API::OciCheckError(m_OciErr, OCIObjectFree(m_OciEnv, m_OciErr,m_Dim_Element,0));      
    if(m_Dim_Element_Ind) delete m_Dim_Element_Ind;
  }
}
c_SDO_DIM_ELEMENT* c_SDO_DIM_ELEMENT::Create( c_Oci_Connection* Conn )
{
  SDO_DIM_ELEMENT * oci_geom;
  Conn->OciCheckError(OCIObjectNew(Conn->m_OciHpEnvironment, Conn->m_OciHpError, Conn->m_OciHpServiceContext,
    OCI_TYPECODE_OBJECT, Conn->m_OciType_SdoGeometry, (dvoid *) 0,
    OCI_DURATION_DEFAULT, TRUE, (dvoid **) &oci_geom));
    
  
  c_SDO_DIM_ELEMENT * newgeom = new c_SDO_DIM_ELEMENT(Conn->m_OciHpEnvironment, Conn->m_OciHpError);
  
  newgeom->m_Dim_Element = oci_geom;
  
  newgeom->m_Dim_Element_Ind = new SDO_DIM_ELEMENT_ind;
  newgeom->m_Dim_Element_Ind->_atomic = OCI_IND_NOTNULL;
  
  newgeom->m_Dim_Element_Ind->SDO_DIMNAME = OCI_IND_NULL;
  newgeom->m_Dim_Element_Ind->SDO_LB = OCI_IND_NULL;
  newgeom->m_Dim_Element_Ind->SDO_TOLERANCE = OCI_IND_NULL;
  newgeom->m_Dim_Element_Ind->SDO_UB = OCI_IND_NULL;
  
  
  newgeom->m_FreeObject = true;
  
  return newgeom;
}

c_SDO_DIM_ELEMENT* c_SDO_DIM_ELEMENT::CreateNull( c_Oci_Connection* Conn )
{
/*
  SDO_GEOMETRY_TYPE * oci_geom;
  Conn->OciCheckError(OCIObjectNew(Conn->m_OciHpEnvironment, Conn->m_OciHpError, Conn->m_OciHpServiceContext,
    OCI_TYPECODE_OBJECT, Conn->m_OciType_SdoGeometry, (dvoid *) 0,
    OCI_DURATION_CALL, TRUE, (dvoid **) &oci_geom));
*/    
  c_SDO_DIM_ELEMENT * newgeom = new c_SDO_DIM_ELEMENT(Conn->m_OciHpEnvironment, Conn->m_OciHpError);
  
  newgeom->m_Dim_Element = NULL;
  
  newgeom->m_Dim_Element_Ind = new SDO_DIM_ELEMENT_ind;
  newgeom->m_Dim_Element_Ind->_atomic = OCI_IND_NULL;
  
  newgeom->m_Dim_Element_Ind->SDO_DIMNAME = OCI_IND_NULL;
  newgeom->m_Dim_Element_Ind->SDO_LB = OCI_IND_NULL;
  newgeom->m_Dim_Element_Ind->SDO_TOLERANCE = OCI_IND_NULL;
  newgeom->m_Dim_Element_Ind->SDO_UB = OCI_IND_NULL;
  
  newgeom->m_FreeObject = true;
  
  return newgeom;
}

wchar_t* c_SDO_DIM_ELEMENT::GetDimName()
{
  return (wchar_t*)OCIStringPtr(m_OciEnv, m_Dim_Element->SDO_DIMNAME);
}

double c_SDO_DIM_ELEMENT::GetUB()
{
  double val;
  OCINumberToReal(m_OciErr, &m_Dim_Element->SDO_UB,sizeof(double),&val);
  return val;
}

double c_SDO_DIM_ELEMENT::GetLB()
{
  double val;
  OCINumberToReal(m_OciErr, &m_Dim_Element->SDO_LB,sizeof(double),&val);
  return val;
}

double c_SDO_DIM_ELEMENT::GetTolerance()
{
  double val;
  OCINumberToReal(m_OciErr, &m_Dim_Element->SDO_TOLERANCE,sizeof(double),&val);
  return val;
}

bool c_SDO_DIM_ELEMENT::IsNullTolerance()
{
  if( m_Dim_Element_Ind && m_Dim_Element_Ind->SDO_TOLERANCE == OCI_IND_NULL ) return true; return false;
}

bool c_SDO_DIM_ELEMENT::IsNullUB()
{
  if( m_Dim_Element_Ind && m_Dim_Element_Ind->SDO_UB == OCI_IND_NULL ) return true; return false;
}

bool c_SDO_DIM_ELEMENT::IsNullLB()
{
  if( m_Dim_Element_Ind && m_Dim_Element_Ind->SDO_LB == OCI_IND_NULL ) return true; return false;
}

bool c_SDO_DIM_ELEMENT::IsNullDimName()
{
  if( m_Dim_Element_Ind && m_Dim_Element_Ind->SDO_DIMNAME == OCI_IND_NULL ) return true; return false;
}

bool c_SDO_DIM_ELEMENT::IsNull()
{
  if( !m_Dim_Element) return false; if( m_Dim_Element_Ind && m_Dim_Element_Ind->_atomic == OCI_IND_NULL ) return true; return false;
}

void c_SDO_DIM_ELEMENT::SetLB( double Val )
{
  OCINumberFromReal(m_OciErr, &Val,sizeof(Val),&m_Dim_Element->SDO_LB);
  m_Dim_Element_Ind->SDO_LB = OCI_IND_NOTNULL;
}
void c_SDO_DIM_ELEMENT::SetUB( double Val )
{
  OCINumberFromReal(m_OciErr, &Val,sizeof(Val),&m_Dim_Element->SDO_UB);
  m_Dim_Element_Ind->SDO_UB = OCI_IND_NOTNULL;
}
void c_SDO_DIM_ELEMENT::SetTolerance( double Val )
{
  OCINumberFromReal(m_OciErr, &Val,sizeof(Val),&m_Dim_Element->SDO_TOLERANCE);
  m_Dim_Element_Ind->SDO_TOLERANCE = OCI_IND_NOTNULL;
}

void c_SDO_DIM_ELEMENT::SetDimName( const wchar_t* Val )
{
  OCIStringAssignText(m_OciEnv, m_OciErr, (const oratext*)Val, wcslen(Val)*sizeof(wchar_t), &m_Dim_Element->SDO_DIMNAME  );
  m_Dim_Element_Ind->SDO_DIMNAME = OCI_IND_NOTNULL;
}
