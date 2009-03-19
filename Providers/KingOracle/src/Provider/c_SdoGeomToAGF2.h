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

#ifndef _c_SdoGeomToAGF2_h
#define _c_SdoGeomToAGF2_h



#include "c_OCI_API.h"


  enum CoordinateDimensionality 
  {
    CoordDim_XY = 0,
    CoordDim_Z = 1,
    CoordDim_M = 2
  };
  
class c_SdoGeomToAGF2 
{
public:
  c_SdoGeomToAGF2(SDO_GEOMETRY_TYPE* Geom=NULL,SDO_GEOMETRY_ind *GeomInd=NULL);
  ~c_SdoGeomToAGF2(void);
  
public:

  void SetGeometry(c_SDO_GEOMETRY* Geom)
  {
    m_SdoGeom = Geom->m_SdoGeom; 
    m_SdoGeomInd = Geom->m_SdoGeomInd; 
  };
  
  void SetGeometry(SDO_GEOMETRY_TYPE* Geom,SDO_GEOMETRY_ind* GeomInd)
  {
    m_SdoGeom = Geom; 
    m_SdoGeomInd = GeomInd; 
  };
  
protected:

  SDO_GEOMETRY_TYPE* m_SdoGeom;
  SDO_GEOMETRY_ind *m_SdoGeomInd;
  
  int m_ElemInfoSize;  // size of SDO_ELEM_INFO array
  int m_OrdinatesSize;  // size of SDO_ELEM_INFO array
  int m_PointSize;  // count of numbers for one point in sdo_ordinates list
  int m_CoordDim; // coordinate dimensionality kot je definiran pri Autodesku  
                  // This is a bit field, for example, xym == coordinateDimensionality.XY |
                  //  CoordinateDimensionality.M. The following sequence defines the type of coordinates used for this object:
                  
  char *m_BuffMem;
  
  int* m_BuffCurr;
  
  unsigned int m_BuffSize;
  unsigned int m_BuffLen;

public:  
  // Convert to Autodesk variant of format very similar to WKB well known binary
  int ToAGF();
  int ToAGFTest();
  int ToAGFTest_Recursive(int level);
  
  void *GetBuff() { return (void*)m_BuffMem; }
  unsigned int GetBuffLen() { return m_BuffLen; }
  
protected:  
  bool AGF_Get_GType1_Point(int& ElemInfo_Index);
  bool AGF_Get_GType2_CurveOrLine(int& ElemInfo_Index,bool ForceCurve=false);
  bool AGF_Get_GType3_PolygonOrCurvePolygon(int& ElemInfo_Index,bool ForceCurve=false);
  
  bool AGF_GetType5_Multi_Point(int& ElemInfo_Index);
  bool AGF_Get_GType6_Multi_LineOrCurve(int& ElemInfo_Index);
  bool AGF_Get_GType7_Multi_PolygonOrCurvePolygon(int& ElemInfo_Index);
  
  // subelements
  bool AGF_Get_LinearRings_Etype2003(int& ElemInfo_Index,int& NumRings,bool& IsLinear);
  bool AGF_Get_LinearString(int& ElemInfo_Index);
  bool AGF_Get_CurveArcString(int& ElemInfo_Index);
  bool AGF_Get_CurvePointString(int& ElemInfo_Index);
  
  bool AGF_Get_CurveString(int& ElemInfo_Index);
  
  //int AGF_Get_CurveRings_Etype2005(int& ElemInfo_Index,int*& AkbBuff,int& NumRings);
  
  bool AGF_Get_CurveInnerRings_Etype2003_2005(int& ElemInfo_Index,int& NumRings,bool ForceCurve);
  
  void AGF_WriteGeometryType(FdoGeometryType GeometryType);
  void AGF_WriteDimensionality();
  void AGF_WritePointsFromOrdinates(int& OrdIndex,int NumPoints);
  
  void AGF_WriteRectangle(int& OrdIndex);
  
  void AGF_UpdateInt(unsigned int BuffPos,int Val);
  void AGF_WriteInt(int Val);
  
  void RestoreBuff(int BuffPos);
  int GetSdoElemInfo(int Index);
  double GetSdoOrdinate(int Index);
  int GetSdoOrdinatesSize();
  double GetSdoPointX();
  double GetSdoPointY();
  double GetSdoPointZ();
};

#endif