#include "stdafx.h"

#include "c_SDO_GEOMETRY.h"
#include "c_OCI_API.h"


c_SDO_GEOMETRY::c_SDO_GEOMETRY( OCIEnv* Env/*=NULL*/,OCIError *Err/*=NULL*/,SDO_GEOMETRY_TYPE* Geom/*=NULL*/,SDO_GEOMETRY_ind* GeomInd/*=NULL*/ )
{
  m_OciEnv = Env;
  m_OciErr = Err;
  
  m_FreeObject = false;
  m_SdoGeom = Geom;
  m_SdoGeomInd = GeomInd;
}
c_SDO_GEOMETRY::c_SDO_GEOMETRY(const c_SDO_GEOMETRY* Geom)
{
  m_OciEnv = Geom->m_OciEnv;
  m_OciErr = Geom->m_OciErr;
  
  m_FreeObject = false;
  m_SdoGeom = Geom->m_SdoGeom;
  m_SdoGeomInd = Geom->m_SdoGeomInd;
}

c_SDO_GEOMETRY::~c_SDO_GEOMETRY()
{
  if( m_FreeObject )
  {
    if(m_SdoGeom) c_OCI_API::OciCheckError(m_OciErr, OCIObjectFree(m_OciEnv, m_OciErr,m_SdoGeom,0));      
    if(m_SdoGeomInd) delete m_SdoGeomInd;
  }
}

c_SDO_GEOMETRY* c_SDO_GEOMETRY::Create( c_Oci_Connection* Conn )
{
  SDO_GEOMETRY_TYPE * oci_geom;
  Conn->OciCheckError(OCIObjectNew(Conn->m_OciHpEnvironment, Conn->m_OciHpError, Conn->m_OciHpServiceContext,
    OCI_TYPECODE_OBJECT, Conn->m_OciType_SdoGeometry, (dvoid *) 0,
    OCI_DURATION_DEFAULT, TRUE, (dvoid **) &oci_geom));
    
  
  c_SDO_GEOMETRY * newgeom = new c_SDO_GEOMETRY(Conn->m_OciHpEnvironment, Conn->m_OciHpError);
  
  newgeom->m_SdoGeom = oci_geom;
  newgeom->m_SdoGeomInd = new SDO_GEOMETRY_ind;
  newgeom->m_SdoGeomInd->_atomic = OCI_IND_NOTNULL;
  newgeom->m_SdoGeomInd->sdo_gtype = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_srid = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point._atomic = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point.x = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point.y = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point.z = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_elem_info = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_ordinates = OCI_IND_NULL;    
  
  newgeom->m_FreeObject = true;
  
  return newgeom;
}


c_SDO_GEOMETRY* c_SDO_GEOMETRY::CreateNull( c_Oci_Connection* Conn )
{
/*
  SDO_GEOMETRY_TYPE * oci_geom;
  Conn->OciCheckError(OCIObjectNew(Conn->m_OciHpEnvironment, Conn->m_OciHpError, Conn->m_OciHpServiceContext,
    OCI_TYPECODE_OBJECT, Conn->m_OciType_SdoGeometry, (dvoid *) 0,
    OCI_DURATION_CALL, TRUE, (dvoid **) &oci_geom));
*/    
  c_SDO_GEOMETRY * newgeom = new c_SDO_GEOMETRY(Conn->m_OciHpEnvironment, Conn->m_OciHpError);
  
  newgeom->m_SdoGeom = NULL;
  
  newgeom->m_SdoGeomInd = new SDO_GEOMETRY_ind;
  newgeom->m_SdoGeomInd->_atomic = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_gtype = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_srid = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point._atomic = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point.x = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point.y = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_point.z = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_elem_info = OCI_IND_NULL;
  newgeom->m_SdoGeomInd->sdo_ordinates = OCI_IND_NULL;    
  
  newgeom->m_FreeObject = true;
  
  return newgeom;
}



bool c_SDO_GEOMETRY::IsNull()
{
  return m_SdoGeomInd->_atomic == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoGtype()
{
  return m_SdoGeomInd->sdo_gtype == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoSrid( )
{
  return m_SdoGeomInd->sdo_srid == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoPoint(  )
{
  return m_SdoGeomInd->sdo_point._atomic == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoPointX(  )
{
  return m_SdoGeomInd->sdo_point.x == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoPointY(  )
{
  return m_SdoGeomInd->sdo_point.y == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoPointZ(  )
{
  return m_SdoGeomInd->sdo_point.z == OCI_IND_NULL;
}

bool c_SDO_GEOMETRY::IsNullSdoGeometry(  )
{
  return m_SdoGeomInd->_atomic == OCI_IND_NULL;
}

int c_SDO_GEOMETRY::GetSdoGtype(  )
{
  int sdo_gtype;
  c_OCI_API::OciCheckError( m_OciErr, OCINumberToInt(m_OciErr, &(m_SdoGeom->sdo_gtype),
    (uword)sizeof(int), OCI_NUMBER_SIGNED,
    (dvoid *)&sdo_gtype));

  return sdo_gtype;
}

int c_SDO_GEOMETRY::GetSdoSrid(  )
{
  int sdo_srid;
  c_OCI_API::OciCheckError( m_OciErr, OCINumberToInt(m_OciErr, &(m_SdoGeom->sdo_srid),
    (uword)sizeof(int), OCI_NUMBER_SIGNED,
    (dvoid *)&sdo_srid));

  return sdo_srid;
}

int c_SDO_GEOMETRY::GetSdoElemInfo( int Index )
{
  boolean exists;
  OCINumber *oci_number;
  int val;

  c_OCI_API::OciCheckError(m_OciErr, OCICollGetElem(m_OciEnv, m_OciErr, 
    (OCIColl *)(m_SdoGeom->sdo_elem_info), 
    (sb4)(Index), 
    (boolean *)&exists, 
    (dvoid **)&oci_number, (dvoid **)0));
  c_OCI_API::OciCheckError(m_OciErr, OCINumberToInt(m_OciErr, 
    oci_number, 
    (uword)sizeof(int), OCI_NUMBER_SIGNED,
    (dvoid *)&val));

  return val;
}

int c_SDO_GEOMETRY::GetSdoOrdinatesSize()
{
  int size;

  OCICollSize(m_OciEnv,m_OciErr,m_SdoGeom->sdo_ordinates,&size);
  return size;
}

double c_SDO_GEOMETRY::GetSdoOrdinate( int Index )
{
  boolean exists;
  OCINumber *oci_number;
  double val;

  c_OCI_API::OciCheckError(m_OciErr, OCICollGetElem(m_OciEnv, m_OciErr, 
    (OCIColl *)(m_SdoGeom->sdo_ordinates), 
    (sb4)(Index), 
    (boolean *)&exists, 
    (dvoid **)&oci_number, (dvoid **)0));
  c_OCI_API::OciCheckError(m_OciErr, OCINumberToReal(m_OciErr, 
    oci_number, 
    (uword)sizeof(double), (dvoid *)&val));

  return val;
}

int c_SDO_GEOMETRY::GetSdoElemInfoSize()
{
  int size;

  OCICollSize(m_OciEnv,m_OciErr,m_SdoGeom->sdo_elem_info,&size);
  return size;
}

double c_SDO_GEOMETRY::GetSdoPointX()
{
  double val;

  c_OCI_API::OciCheckError(m_OciErr, OCINumberToReal(m_OciErr, 
    &m_SdoGeom->sdo_point.x, 
    (uword)sizeof(double), (dvoid *)&val));
  return val;
}

double c_SDO_GEOMETRY::GetSdoPointY()
{
  double val;

  c_OCI_API::OciCheckError(m_OciErr, OCINumberToReal(m_OciErr, 
    &m_SdoGeom->sdo_point.y, 
    (uword)sizeof(double), (dvoid *)&val));
  return val;
}

double c_SDO_GEOMETRY::GetSdoPointZ()
{
  double val;

  c_OCI_API::OciCheckError(m_OciErr, OCINumberToReal(m_OciErr, 
    &m_SdoGeom->sdo_point.z, 
    (uword)sizeof(double), (dvoid *)&val));
  return val;
}


void c_SDO_GEOMETRY::SetSdoGtype( int Gtype )
{
  c_OCI_API::OciCheckError(m_OciErr, OCINumberFromInt(m_OciErr, 
    (const dvoid *)&Gtype, 
    (uword)sizeof(Gtype),
    (uword)OCI_NUMBER_SIGNED, 
    &(m_SdoGeom->sdo_gtype)));
    
  m_SdoGeomInd->sdo_gtype = OCI_IND_NOTNULL;    
}

void c_SDO_GEOMETRY::SetSdoSrid( long Srid )
{
  c_OCI_API::OciCheckError(m_OciErr, OCINumberFromInt(m_OciErr, 
    (const dvoid *)&Srid, 
    (uword)sizeof(Srid),
    (uword)OCI_NUMBER_SIGNED, 
    &(m_SdoGeom->sdo_srid)));
  
  m_SdoGeomInd->sdo_srid = OCI_IND_NOTNULL;
}
void c_SDO_GEOMETRY::SetNull_SdoSrid(  )
{
  
  m_SdoGeomInd->sdo_srid = OCI_IND_NULL;
}

void c_SDO_GEOMETRY::AppendElemInfoArray( int Val )
{
  OCINumber oci_number;

  c_OCI_API::OciCheckError(m_OciErr, 
    OCINumberFromInt(m_OciErr, (dvoid *)&Val,
    (uword)sizeof(Val), OCI_NUMBER_UNSIGNED, 
    &oci_number));

  c_OCI_API::OciCheckError(m_OciErr, OCICollAppend(m_OciEnv, m_OciErr, 
    (const void *)&oci_number, 
    (const void *)0, 
    m_SdoGeom->sdo_elem_info));

  m_SdoGeomInd->sdo_elem_info = OCI_IND_NOTNULL;
}

void c_SDO_GEOMETRY::AppendSdoOrdinates( double Val )
{
  OCINumber oci_number;

  c_OCI_API::OciCheckError(m_OciErr, 
    OCINumberFromReal(m_OciErr, (dvoid *)&Val,
    (uword)sizeof(Val), &oci_number));

  c_OCI_API::OciCheckError(m_OciErr, OCICollAppend(m_OciEnv, m_OciErr, 
    (const void *)&oci_number, 
    (const void *)0, 
    m_SdoGeom->sdo_ordinates));

  m_SdoGeomInd->sdo_ordinates = OCI_IND_NOTNULL;
}