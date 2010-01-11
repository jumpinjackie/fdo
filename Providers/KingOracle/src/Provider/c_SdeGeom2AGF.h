/*
* Copyright (C) 2009  SL-King d.o.o
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

#ifndef _c_SdeGeom2AGF_h
#define _c_SdeGeom2AGF_h



#include "c_OCI_API.h"


typedef struct t_SdeFeaturePart
{
int m_NumberOfIntegers;
int m_IndexOfIntegers;
}t_SdeFeaturePart;
  
  
class c_SdeGeom2AGF 
{
public:
  c_SdeGeom2AGF();
  ~c_SdeGeom2AGF(void);
  
public:
  enum CoordinateDimensionality 
  {
    CoordDim_XY = 0,
    CoordDim_Z = 1,
    CoordDim_M = 2
  };
    
  
  
  
protected:
  // GeometryType—The type of geometry 
  // 0 = Geometry
  // 1 = point
  // 2 = curve
  // 3 = linestring
  // 4 = surface
  // 5 = polygon
  // 6 = collection
  // 7 = multipoint
  // 8 = multicurve
  // 9 = multilinestring
  // 10 = multisurface
  // 11 = multipolygon
  int m_GeometryType;
  const c_KgOraSridDesc* m_SridDesc;
  int m_NumOfPts;
  //
  // EntityType -- type of feature stored in points in F* tables
  //
  // NIL_SHAPE 0 
  // POINT_SHAPE 1 
  // LINE_SHAPE 2 
  // SIMPLE_LINE_SHAPE 4 
  // AREA_SHAPE 8 
  // SHAPE_CLASS_MASK 255 
  // SHAPE_MULTI_PART_MASK 256 
  // MULTI_POINT_SHAPE 257 
  // MULTI_LINE_SHAPE 258 
  // MULTI_SIMPLE_LINE_SHAPE 260 
  // MULTI_AREA_SHAPE 264
  int m_EntityType;
  
  unsigned int m_SdeGeomLength;
  const unsigned char* m_SdeGeom;
  //SDO_GEOMETRY_ind *m_SdoGeomInd;
  
  //int m_ElemInfoSize;  // size of SDO_ELEM_INFO array
  //int m_OrdinatesSize;  // size of SDO_ELEM_INFO array  
  int m_PointSize;  // count of numbers for one point in sdo_ordinates list
  
  
  
  int m_CoordDim; // coordinate dimensionality kot je definiran pri Autodesku  
                  // This is a bit field, for example, xym == coordinateDimensionality.XY |
                  //  CoordinateDimensionality.M. The following sequence defines the type of coordinates used for this object:
  
  
  unsigned int m_CoordStreamLen;
  
                  
  char *m_BuffMem;
  
  int* m_BuffCurr;
  
  unsigned int m_BuffSize;
  unsigned int m_BuffLen;
  
protected:  
  int* m_UnpackedIntegers; // size of this allocation has to be at least m_NumOfPts * m_PointInSize
  int m_AllocatedUnpackedIntegers; 
  
  int m_NumberOfParts;
  t_SdeFeaturePart * m_FeatureParts;
  int m_AllocatedParts;

public:

  void SetSdeGeometry(const c_KgOraSridDesc* SridDesc,int GeometryType,int NumOfPts,int EntityType,unsigned int Length,const unsigned char* Geom)
  {
    m_GeometryType=GeometryType;
    m_SridDesc=SridDesc;
    m_GeometryType=GeometryType;
    m_NumOfPts=NumOfPts;
    m_EntityType=EntityType;

    m_SdeGeomLength = Length;
    m_SdeGeom = Geom;

  };
  
public:  
  // Convert to Autodesk variant of format very similar to WKB well known binary
  int ToAGF();
  int ToAGF(double MinX,double MinY,double MaxX,double MaxY);
  int ToAGFTest();
  int ToAGFTest_Recursive(int level);
  
  void *GetBuff() { return (void*)m_BuffMem; }
  unsigned int GetBuffLen() { return m_BuffLen; }
  
protected:  
 
  
  
  
  void AGF_WriteGeometryType(FdoGeometryType GeometryType);
  void AGF_WriteDimensionality();
  void AGF_WritePointsFromOrdinates(int& OrdIndex,int NumPoints);
  
  void AGF_WriteRectangle(int& OrdIndex);
  
  void AGF_UpdateInt(unsigned int BuffPos,int Val);
  void AGF_WriteInt(int Val);
  
  void RestoreBuff(int BuffPos);
  void UnpackParts();
  int GetNumberOfParts();
  int* GetPartIntegers(int PartIndex,int& NumOfIntegers);;
  t_SdeFeaturePart* AddPart();
  void AGF_WritePointsFromIntegers(int* Integers,int NumIntegers);
  
  void AGF_WriteLineString();
  void AGF_WriteLineString(int PartIndex);

  void AGF_WritePolygon();
  void AGF_WritePolygon(int PartIndex);
  int AGF_WriteMultiPolygon();


};

#endif
