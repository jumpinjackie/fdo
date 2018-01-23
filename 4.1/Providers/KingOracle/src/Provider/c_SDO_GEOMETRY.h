/*
* Copyright (C) 2008  SL-King d.o.o
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

#ifndef _c_SDO_GEOMETRY_
#define _c_SDO_GEOMETRY_



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

/*************************************************************************/
/* Type definitions                                                      */
/*************************************************************************/
struct sdo_point_type
{
  OCINumber x;
  OCINumber y;
  OCINumber z;
};
typedef struct sdo_point_type sdo_point_type;

typedef OCIArray sdo_elem_info_array;
typedef OCIArray sdo_ordinate_array;

struct sdo_geometry
{
  OCINumber      sdo_gtype;
  OCINumber      sdo_srid;
  sdo_point_type sdo_point;
  OCIArray       *sdo_elem_info;
  OCIArray       *sdo_ordinates;
};

typedef struct sdo_geometry SDO_GEOMETRY_TYPE;

/***
** Indicator structures for SDO_GEOMETRY_TYPE
***/

struct sdo_point_type_ind
{
  OCIInd _atomic;
  OCIInd x;
  OCIInd y;
  OCIInd z;
};
typedef struct sdo_point_type_ind sdo_point_type_ind;

struct SDO_GEOMETRY_ind
{
  OCIInd                    _atomic;
  OCIInd                    sdo_gtype;
  OCIInd                    sdo_srid;
  struct sdo_point_type_ind sdo_point;
  OCIInd                    sdo_elem_info;
  OCIInd                    sdo_ordinates;
};
typedef struct SDO_GEOMETRY_ind SDO_GEOMETRY_ind;



class c_Oci_Connection;



class c_SDO_GEOMETRY
{
  public:
    c_SDO_GEOMETRY(OCIEnv* Env=NULL,OCIError *Err=NULL,SDO_GEOMETRY_TYPE* Geom=NULL,SDO_GEOMETRY_ind* m_SdoGeomInd=NULL);
    c_SDO_GEOMETRY(const c_SDO_GEOMETRY* Geom);
    ~c_SDO_GEOMETRY();
    
    static c_SDO_GEOMETRY* Create(c_Oci_Connection* Conn);
    
    static c_SDO_GEOMETRY* CreateNull( c_Oci_Connection* Conn );
    
  public:
    OCIEnv* m_OciEnv;
    OCIError *m_OciErr;
    bool m_FreeObject;
    SDO_GEOMETRY_TYPE* m_SdoGeom;
    SDO_GEOMETRY_ind* m_SdoGeomInd;
    
  public:
    bool IsNull();
    bool IsNullSdoGtype( );
    bool IsNullSdoSrid( );
    bool IsNullSdoPoint( );
    bool IsNullSdoPointX( );
    bool IsNullSdoPointY( );
    bool IsNullSdoPointZ( );
    bool IsNullSdoGeometry( );
    
    int GetSdoGtype();
    int GetSdoSrid();
    int GetSdoElemInfo(int Index);

    int GetSdoOrdinatesSize();
    double GetSdoOrdinate(int Index);
    
    int GetSdoElemInfoSize();

    double GetSdoPointX();
    double GetSdoPointY();
    double GetSdoPointZ();
    
    
    void SetSdoGtype (int Gtype);
    void SetSdoSrid (long Srid);
    void SetNull_SdoSrid( );
    
    void AppendElemInfoArray (int Val);
    void AppendSdoOrdinates( double Val );
    

};





#endif
