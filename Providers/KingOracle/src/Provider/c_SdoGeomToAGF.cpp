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
#include "c_SdoGeomToAGF.h"


#define D_BUFF_SIZE_INC 300 * 3 * 8 // three hundred 3 dim points
#define D_BUFF_SIZE_RESERVE 512


c_SdoGeomToAGF::c_SdoGeomToAGF(SDO_GEOMETRY* Geom)
{
  m_SdoGeom = Geom;
  
  m_BuffSize = D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;
  
  m_BuffMem = new char[m_BuffSize];
  
  m_BuffLen = 0;
  m_BuffCurr = (int*)m_BuffMem;
}

c_SdoGeomToAGF::~c_SdoGeomToAGF(void)
{
  delete m_BuffMem;
}



int c_SdoGeomToAGF::ToAGF()
{
  
  m_BuffLen = 0;
  m_BuffCurr = (int*)m_BuffMem;
  
  //int* AgfBuff = (int*)m_BuffFGF;
  
  m_ElemInfoSize = m_SdoGeom->getSdo_elem_info().size();
  m_OrdinatesSize = m_SdoGeom->getSdo_ordinates().size();
  
  if( m_SdoGeom->getSdo_gtype().isNull() ) return 0;
  
  int ora_gtype = ((int)m_SdoGeom->getSdo_gtype()) % 100;
  
   
  //////////////////////////////
  // CoordinateDimensionality
  //////////////////////////////
  int ora_dim = ((int)m_SdoGeom->getSdo_gtype()) / 1000;
  
  int mdim = ((int)m_SdoGeom->getSdo_gtype()) / 100;  
  mdim = mdim % 10;
  
  switch(ora_dim)
  {
    case 2: // XY      
    {
      m_PointSize = 2;
      m_CoordDim = CoordDim_XY;
    }  
    break;
    case 3: // XYZ      
    {
      m_PointSize = 3;
      if( mdim > 0 )
      {
        m_CoordDim = CoordDim_XY | CoordDim_M;
      }
      else
      {
        m_CoordDim = CoordDim_XY | CoordDim_Z;
      }
    }
    break;
    case 4: // XYZ      
    {
      m_PointSize = 4;
      m_CoordDim = CoordDim_XY | CoordDim_Z | CoordDim_M;
    }
    break;
    default:
      return 0;
    break;
  }
  
  
  
  // count of numbers for one point in sdo_ordinates list
  
  
  switch(ora_gtype)
  {
    case 1: // Point
    {
      AGF_WriteGeometryType(FdoGeometryType_Point);
      AGF_WriteDimensionality();
      
      int eleminfo_ind=0; // starting poit fro reading subelements of points
      AGF_Get_GType1_Point(eleminfo_ind);   
    }
    break;
    
    case 2: // Line
    {
       
      int eleminfo_ind=0; // starting poit fro reading subelements of string
      
      bool islinear = true;
  
      AGF_Get_GType2_CurveOrLine(eleminfo_ind);  
      
      
      
    }
    break;
    
//////////////////////////////////////////////////////////////////////////////////    
//
//    Reading Ppolygon GTYPE 3
//
//////////////////////////////////////////////////////////////////////////////////
    
    case 3: // Polygon
    {
      
      
      
    /*  Polygon structure 
      struct LinearRing
      {
      int numPts; // >0
      double[] coords; // size = numPts* PositionSize(polygon)
      
      }
      struct Polygon
      {
      int geomType;
      CoordinateDimensionality type;
      int numRings; // >= 1 as there has to be at least one ring
      LinearRing[] lineStrings; // size = numRings
      }
   */   
    // read sdo_elem_info
    
      int eleminfo_ind=0; // starting poit fro reading subelements of polygon
  
      AGF_Get_GType3_PolygonOrCurvePolygon(eleminfo_ind);
      
    }  
    break;
    
    case 5:   // Multipoint      
    {
      int eleminfo_ind=0; // starting poit fro reading subelements of polygon
      AGF_WriteGeometryType(FdoGeometryType_MultiPoint);
  
      AGF_GetType5_Multi_Point(eleminfo_ind);
      
    }
    break;
    case 6:   // Multiline or MultiCurve
    {
      
      
      int eleminfo_ind=0; // starting poit fro reading subelements of polygon
      AGF_Get_GType6_Multi_LineOrCurve(eleminfo_ind);
      
      
      
    }
    break;
    
    case 7:
    {
      int eleminfo_ind=0; // starting poit fro reading subelements of polygon
      AGF_Get_GType7_Multi_PolygonOrCurvePolygon(eleminfo_ind);
    }
    break;
    
    default:
      return 0;
    break;
  }
  
  return m_BuffLen;
}//end of c_SdoGeomToAGF::ToAKB


void c_SdoGeomToAGF::AGF_WriteGeometryType(FdoGeometryType GeometryType)
{
  
  *m_BuffCurr++ = GeometryType;
  m_BuffLen += sizeof(int);
  
}//end of c_SdoGeomToAGF::AGF_WriteGeometryType

void c_SdoGeomToAGF::AGF_WriteDimensionality()
{
  
  *m_BuffCurr++ = m_CoordDim;
  m_BuffLen += sizeof(int);
  
}//end of c_SdoGeomToAGF::AGF_WriteDimensionality

void c_SdoGeomToAGF::AGF_UpdateInt(unsigned int BuffPos,int Val)
{
  int* ptr = (int*)&m_BuffMem[BuffPos];
  
  *ptr = Val;
}//end of c_SdoGeomToAGF::AGF_UpdateInt

void c_SdoGeomToAGF::AGF_WriteInt(int Val)
{
  *m_BuffCurr++ = Val;
  m_BuffLen += sizeof(int);
}//end of c_SdoGeomToAGF::AGF_WriteInt

void c_SdoGeomToAGF::RestoreBuff(int BuffPos)
{
  m_BuffCurr = (int*)&m_BuffMem[BuffPos];
  m_BuffLen = BuffPos;
}//end of c_SdoGeomToAGF::AGF_WriteInt

// It will write to buffer NumPoint from index
// start reading ordinates from OrdIndex
//
//  It will increment buffer pointer and ordinates index
//
void c_SdoGeomToAGF::AGF_WritePointsFromOrdinates(int& OrdIndex,int NumPoints)
{
  int bfs = m_PointSize * NumPoints * sizeof(double); // size is in int
  if( (m_BuffLen+bfs) > ( m_BuffSize-D_BUFF_SIZE_RESERVE) )
  {
    m_BuffSize = m_BuffLen + bfs + D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;
    
    char *newbuff = new char[m_BuffSize];
    
    memcpy(newbuff,m_BuffMem,m_BuffLen);
    
    delete m_BuffMem;
    
    m_BuffMem = newbuff;
    
    m_BuffCurr = (int*)&m_BuffMem[m_BuffLen];
    
  }
  
  switch( m_PointSize ) 
  {
    case 2:
    {              
      double *pt;      
      pt = (double *)m_BuffCurr;
      
      for(int ind=0;ind<NumPoints;ind++)
      {
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];          
        
      }
      
      m_BuffLen += NumPoints*2*sizeof(double);
      
      m_BuffCurr = (int*)pt;
    }
    break;
    
    case 3:
    {
      double *pt;      
      pt = (double *)m_BuffCurr;
      
      for(int ind=0;ind<NumPoints;ind++)
      {
        // X
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
        // Y
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
        // Z
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
      }
      
      m_BuffLen += NumPoints*3*sizeof(double);
      
      m_BuffCurr = (int*)pt;
    }
    break;
    case 4:
    {
      double *pt;      
      pt = (double *)m_BuffCurr;
      
      for(int ind=0;ind<NumPoints;ind++)
      {
        // X
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
        // Y
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
        // Z
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
        // M
        *pt++ = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
        
      }
      
      m_BuffLen += NumPoints*4*sizeof(double);
      
      m_BuffCurr = (int*)pt;
    }
    break;
  }     

}//end of   c_SdoGeomToAGF::AGF_WritePointsFromOrdinates

// 
//
//  This is used to write type rectnlge (1003,3) to FGF polygon
//
void c_SdoGeomToAGF::AGF_WriteRectangle(int& OrdIndex)
{
  
  int NumPoints = 5;
  
  int bfs = m_PointSize * NumPoints * sizeof(double); // size is in int
  if( (m_BuffLen+bfs) > ( m_BuffSize-D_BUFF_SIZE_RESERVE) )
  {
    m_BuffSize = m_BuffLen + bfs + D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;
    
    char *newbuff = new char[m_BuffSize];
    
    memcpy(newbuff,m_BuffMem,m_BuffLen);
    
    delete m_BuffMem;
    
    m_BuffMem = newbuff;
    
    m_BuffCurr = (int*)&m_BuffMem[m_BuffLen];
    
  }
  
  double minx,miny,minz,minm;
  double maxx,maxy,maxz,maxm;
  
  switch( m_PointSize ) 
  {
    case 2:
    {              
      double *pt;      
      pt = (double *)m_BuffCurr;
      
      minx = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      miny = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      
      maxx = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      maxy = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      
      
      // point 1
      *pt++ = minx;      
      *pt++ = miny;          
      
      // point 2
      *pt++ = maxx;      
      *pt++ = miny;          
      
      // point 3
      *pt++ = maxx;      
      *pt++ = maxy;          
      
      // point 4
      *pt++ = minx;      
      *pt++ = maxy;          
      
      // point 5 same as Point 1
      *pt++ = minx;      
      *pt++ = miny;          
        
      m_BuffLen += NumPoints*2*sizeof(double);
      
      m_BuffCurr = (int*)pt;
    }
    break;
    
    case 3:
    {
      double *pt;      
      pt = (double *)m_BuffCurr;
      
      
      minx = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      miny = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      minz = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      
      maxx = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      maxy = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      maxz = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      
      
      // point 1
      *pt++ = minx;      
      *pt++ = miny;          
      *pt++ = minz;          
      
      // point 2
      *pt++ = maxx;      
      *pt++ = miny;
      *pt++ = minz;          
      
      // point 3
      *pt++ = maxx;      
      *pt++ = maxy;          
      *pt++ = maxz;          
      
      // point 4
      *pt++ = minx;      
      *pt++ = maxy;          
      *pt++ = maxz;          
      
      // point 5 same as Point 1
      *pt++ = minx;      
      *pt++ = miny;
      *pt++ = minz;     
      
      m_BuffLen += NumPoints*3*sizeof(double);
      
      m_BuffCurr = (int*)pt;
    }
    break;
    case 4:
    {
      double *pt;      
      pt = (double *)m_BuffCurr;
      
      minx = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      miny = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      minz = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      minm = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      
      maxx = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      maxy = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      maxz = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      maxm = m_SdoGeom->getSdo_ordinates()[OrdIndex++];
      
      
      // point 1
      *pt++ = minx;
      *pt++ = miny;
      *pt++ = minz;
      *pt++ = minm;
      
      // point 2
      *pt++ = maxx;
      *pt++ = miny;
      *pt++ = minz;
      *pt++ = minm;
      
      // point 3
      *pt++ = maxx;
      *pt++ = maxy;
      *pt++ = maxz;
      *pt++ = maxm;
      
      // point 4
      *pt++ = minx;      
      *pt++ = maxy;          
      *pt++ = maxz;
      *pt++ = maxm;          
      
      // point 5 same as Point 1
      *pt++ = minx;      
      *pt++ = miny;
      *pt++ = minz;  
      *pt++ = minm;  
      
      m_BuffLen += NumPoints*4*sizeof(double);
      
      m_BuffCurr = (int*)pt;
    }
    break;
  }     

}//end of   c_SdoGeomToAGF::AGF_WriteRectangle

//
// Reads point
// Starts from element 'ElemInfo_Index' which is index of first triplet in m_SdoGeom->getSdo_elem_info() describing polygon
// WkbBuff.. points to buffer which will recevie transformation to AKB
// Returns number og bytes which are written to WkbBuff
// 0.. zero return means some error
// After function returns:
// - ElemInfo_Index..points on next triplet in m_SdoGeom->getSdo_elem_info() which is to be read ( or to the end of m_SdoGeom->getSdo_elem_info() array if polygon is last)
// - AgfBuff.. points onto the position after this polygon ( writting in WkbBuff is done with *WkbBuff++ = ... 
//
bool c_SdoGeomToAGF::AGF_Get_GType1_Point(int& ElemInfo_Index)
{
  
  
  if( (m_ElemInfoSize>0) && (m_OrdinatesSize>0) )
  {
  // Point is defined with
    int eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    int eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; // do not step forward with ++
    int eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];
    
    if( eleminfo_etype != 1 ) return false;
    
    ElemInfo_Index += 3;
    
    int ord = eleminfo_offset-1;
    double *pt;
    
    pt = (double *)m_BuffCurr;
    
    *pt++ = m_SdoGeom->getSdo_ordinates()[ord++];
    m_BuffLen += sizeof(double);
    
    *pt++ = m_SdoGeom->getSdo_ordinates()[ord++];
    m_BuffLen += sizeof(double);
    
    if( m_PointSize == 3 ) 
    {
      *pt++ = m_SdoGeom->getSdo_ordinates()[ord++];
      m_BuffLen += sizeof(double);
    }
    m_BuffCurr = (int*)pt;
  }
  else  
  {
  // then point should be defined with SDO_POINT_TYPE
    if( m_SdoGeom->getSdo_point() )
    {
      double *pt;
      
      pt = (double *)m_BuffCurr;
      
      *pt++ = m_SdoGeom->getSdo_point()->getX();
      m_BuffLen += sizeof(double);
      
      *pt++ = m_SdoGeom->getSdo_point()->getY();
      m_BuffLen += sizeof(double);
      
      if( m_PointSize == 3 ) 
      {
        *pt++ = m_SdoGeom->getSdo_point()->getZ();
        m_BuffLen += sizeof(double);
      }
      m_BuffCurr = (int*)pt;
      

    }
  }
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_GType1_Point


//
//  Fi
//
// struct MultiPoint
// {
//   int geomType;
//   int numPoints; // > 0
//   Point[] points; // size = numPoints
// }
//
bool c_SdoGeomToAGF::AGF_GetType5_Multi_Point(int& ElemInfo_Index)
{
  
  
  if( (m_ElemInfoSize>0) && (m_OrdinatesSize>0) )
  {
  // Point is defined with
    int eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    int eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; // do not step forward with ++
    int eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];
    
    if( eleminfo_etype != 1 ) return false;
    
    int numpoints = eleminfo_interp;
    
    // write number of points
    *m_BuffCurr++ = numpoints;
    m_BuffLen += sizeof(int);
    
    int ord = eleminfo_offset-1;
    
    // write Point
    //
    //  struct Point // : Geometry
    //  {
    //    int geomType; // == GeometryType.Point;
    //    CoordinateDimensionality type; // all types allowed
    //    double[] coords; // size = PositionSize(this)
    //  }
    int pind=0;
    while( (pind<numpoints) && (ord<m_OrdinatesSize) )
    {
      AGF_WriteGeometryType(FdoGeometryType_Point);
      AGF_WriteDimensionality();
      
      AGF_WritePointsFromOrdinates(ord,1);
    }
    
    
  }
  else  
  {
  // then point should be defined with SDO_POINT_TYPE
    if( m_SdoGeom->getSdo_point() )
    {
      double *pt;
      
      pt = (double *)m_BuffCurr;
      
      *pt++ = m_SdoGeom->getSdo_point()->getX();
      m_BuffLen += sizeof(double);
      
      *pt++ = m_SdoGeom->getSdo_point()->getY();
      m_BuffLen += sizeof(double);
      
      if( m_PointSize >= 3 ) 
      {
        *pt++ = m_SdoGeom->getSdo_point()->getZ();
        m_BuffLen += sizeof(double);
        
        if( m_PointSize == 4 ) 
        {
          *pt++ = 0.0;
          m_BuffLen += sizeof(double);
        }
      }
      m_BuffCurr = (int*)pt;
      

    }
  }
  
  return true;
}//end of c_SdoGeomToAGF::AGF_GetType5_Multi_Point


//
// Reads line string m_SdoGeom->getSdo_gtype() 2
// Starts from element 'ElemInfo_Index' which is index of first triplet in m_SdoGeom->getSdo_elem_info() describing polygon
// WkbBuff.. points to buffer which will recevie transformation to AKB
// Returns number og bytes which are written to WkbBuff
// 0.. zero return means some error
// After function returns:
// - ElemInfo_Index..points on next triplet in m_SdoGeom->getSdo_elem_info() which is to be read ( or to the end of m_SdoGeom->getSdo_elem_info() array if polygon is last)
// - WkbBuff.. points onto the position after this polygon ( writting in WkbBuff is done with *WkbBuff++ = ... 
//
bool c_SdoGeomToAGF::AGF_Get_GType2_CurveOrLine(int& ElemInfo_Index,bool ForceCurve/*=false*/)
{
 
  
  bool islinear = true;
  int geometry_type_buffpos = m_BuffLen;
  
  AGF_WriteGeometryType(FdoGeometryType_LineString);
  AGF_WriteDimensionality();    
      
      
  int eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; // do not step forward with ++
  
  switch( eleminfo_etype )
  {
    case 1:   // point
    {
      return false; // don't now what to do with point in line
    }
    break;
    case 2:   // string
    {
      int eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2]; // do not step forward with ++
      if( eleminfo_interp == 1 ) // line string of points
      {
        if( ForceCurve )
        {
          islinear = false;
          AGF_Get_CurvePointString(ElemInfo_Index);
        }
        else
        {
          islinear = true;
          AGF_Get_LinearString(ElemInfo_Index);
        }
      }
      else
      {
        if( eleminfo_interp == 2 ) // line string of arc's
        {
          islinear = false;
          AGF_Get_CurveArcString(ElemInfo_Index);
         
        }
      }
    }
    break;
    case 4:  // compound line string ( connected with some stright lines and some arcs)
    {
      // elem_interp is number of element contained in this compund line string
      // elem_interp is number of next triplets in m_SdoGeom->getSdo_elem_info()
      // in subelemnts only allowed is etype == 2
      islinear = false;
      AGF_Get_CurveString(ElemInfo_Index);
      
    }
    break;
    case 1003:   // exteriror ring (must be  counterclockwise order of points)
    case 2003:   // interior ring  (must be  clockwise order of points)
    {
      return false;
    }
    break;
    case 1005:   // compound polygon - exteriror ring  (compound some are straight lines, some are arcs
    case 2005:   // compound polygon - interior ring
    {
      return false;
    }
    break;
    default:
      return false;
    break;
    
  }
  
  if( !islinear )
  {  
    AGF_UpdateInt(geometry_type_buffpos,FdoGeometryType_CurveString);
    
  }
  
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_GType2_CurveOrLine

//
// Reads line string m_SdoGeom->getSdo_gtype() 2
// Starts from element 'ElemInfo_Index' which is index of first triplet in m_SdoGeom->getSdo_elem_info() describing polygon
// WkbBuff.. points to buffer which will recevie transformation to AKB
// Returns number og bytes which are written to WkbBuff
// 0.. zero return means some error
// After function returns:
// - ElemInfo_Index..points on next triplet in m_SdoGeom->getSdo_elem_info() which is to be read ( or to the end of m_SdoGeom->getSdo_elem_info() array if polygon is last)
// - WkbBuff.. points onto the position after this polygon ( writting in WkbBuff is done with *WkbBuff++ = ... 
//
bool c_SdoGeomToAGF::AGF_Get_GType6_Multi_LineOrCurve(int& ElemInfo_Index)
{
  
  
  bool islinear = true;
  
  // first check is it all linear or there are
  // is it MultiLine or MultiCurve
  int elemid = ElemInfo_Index;
  
  while( (elemid < m_ElemInfoSize) && islinear )
  {
    int elem_etype = m_SdoGeom->getSdo_elem_info()[elemid+1];
    switch( elem_etype )
    {
      case 2:
      {
        int elem_interp = m_SdoGeom->getSdo_elem_info()[elemid+2];
        switch( elem_interp )
        {
          case 1:
          break;
          case 2: // arcs
            islinear = false;
          break;
          
        }
      }
      break;
      case 4: //compund string
        islinear = false;
      break;
    }
    
    elemid += 3;
  }
  
  
  if( islinear )
    AGF_WriteGeometryType(FdoGeometryType_MultiLineString);
  else
    AGF_WriteGeometryType(FdoGeometryType_MultiCurveString);
    
  
  int num_strings = 0;
  int ptr_num_strings_buffpos = m_BuffLen;
  
  AGF_WriteInt(0); // number of lines
  
  bool forcecurve;
  if( islinear )
  {
  // read as MultiLine - MultiLineString
    forcecurve = false;
  }
  else
  {
    forcecurve = true; // because there are curves (not just linear) than i need to force all to be curves in multi
  }
  
  // read as MultiCurve - MultiCurveString
  int elen=0;
  bool repeat = true;
  while( (ElemInfo_Index < m_ElemInfoSize) && repeat )
  {
    if( AGF_Get_GType2_CurveOrLine(ElemInfo_Index,forcecurve) ) // force da je curve string (not linear)
    {
      num_strings++;  
    }
    else
    {
      repeat = false;
    }
  }
  
  AGF_UpdateInt(ptr_num_strings_buffpos,num_strings);
  
  return true;
  
}//end of c_SdoGeomToAGF::AGF_Get_GType6_Multi_LineOrCurve

//
// This function will read linear rings from ordinates 1003.
// It will read more than one ring.
// It reads rings while there is 1003 or 2003 etype (linear - interpretation == 1) or end of subelements
// It will stop if next subelement is not 1003 etype or the ened of list is reached.
//
// It will write this to AgfBuff:
//
// LinearRing[] lineStrings; // size = NumRings
//
// struct LinearRing
// {
//   int numPts; // >0
//   double[] coords; // size = numPts* PositionSize(polygon)
//
// }
//
bool c_SdoGeomToAGF::AGF_Get_LinearString(int& ElemInfo_Index)
{
  int eleminfo_offset;
  int eleminfo_etype;
  int eleminfo_interp;
  
  
  
     
  eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
  eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];    
  if( eleminfo_interp!=1 ) return 0; // if not ring of straight lines out
    
  eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
  ElemInfo_Index += 3;
    
    
  // read points
      
  int numpoints_string; // number of points in string
      
  // get next offset if exists
  int next_offset = 0;
  if( ElemInfo_Index < m_ElemInfoSize ) 
  {
    next_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
    numpoints_string = (next_offset - eleminfo_offset) / m_PointSize;
  }
  else
  {
  // there is no next 
    numpoints_string = (((int)m_SdoGeom->getSdo_ordinates().size()) + 1 - eleminfo_offset) / m_PointSize;
  }
      
  // write numpoints
  AGF_WriteInt( numpoints_string );
      
// write String Points
  int ord = eleminfo_offset - 1;
  AGF_WritePointsFromOrdinates(ord,numpoints_string);
  
  
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_LinearString

//
// This function will read arc string from ordinates Etype 2 Interp 2.
// It will read more than one arc.
//
// It will write this to AgfBuff:
//
// double StartPoint[];  // one point
// int NumElements; (arcs)
// double Points[];
//
//
bool c_SdoGeomToAGF::AGF_Get_CurveArcString(int& ElemInfo_Index)
{
  int eleminfo_offset;
  int eleminfo_etype;
  int eleminfo_interp;
  
  
  
     
  eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
  eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];    
  eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
  ElemInfo_Index += 3;
    
    
  // read points
      
  int numpoints_string; // number of points in string
      
  // get next offset if exists
  int next_offset = 0;
  if( ElemInfo_Index < m_ElemInfoSize ) 
  {
    next_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
    numpoints_string = (next_offset - eleminfo_offset) / m_PointSize;
  }
  else
  {
  // there is no next ring
    numpoints_string = (((int)m_SdoGeom->getSdo_ordinates().size()) + 1 - eleminfo_offset) / m_PointSize;
  }

  if( numpoints_string < 3 ) return false;

  // first write start_point
  int ord = eleminfo_offset - 1;
  AGF_WritePointsFromOrdinates(ord,1);
  
  // second write number of elements (arcs)
  int numarcs = (numpoints_string-1) / 2;

  AGF_WriteInt(numarcs);
  
  
  // for each arc write int CurveElementType; // == 2
  // and two points for each arc
  for(int ind=0;ind<numarcs;ind++)
  {
    AGF_WriteInt(FdoGeometryComponentType_CircularArcSegment);
    AGF_WritePointsFromOrdinates(ord,2);
  }     
 
  
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_CurveArcString


//
// This function will read point string ( stright line ) but interpreted as part fo curve
//
// It will write this to AgfBuff:
//
// double StartPoint[];  // one point
// int NumElements; (1)
// int ElementTYpe;    // FdoGeometryComponentType_LineStringSegment
// int NumberOfPoints;
// double Points[];
//
//
bool c_SdoGeomToAGF::AGF_Get_CurvePointString(int& ElemInfo_Index)
{
  int eleminfo_offset;
  int eleminfo_etype;
  int eleminfo_interp;
  
  
  
     
  eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
  eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];    
  eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
  ElemInfo_Index += 3;
    
    
  // read points
      
  int numpoints_string; // number of points in string
      
  // get next offset if exists
  int next_offset = 0;
  if( ElemInfo_Index < m_ElemInfoSize ) 
  {
    next_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
    numpoints_string = (next_offset - eleminfo_offset) / m_PointSize;
  }
  else
  {
  // there is no next ring
    numpoints_string = (((int)m_SdoGeom->getSdo_ordinates().size()) + 1 - eleminfo_offset) / m_PointSize;
  }

  // first write start_point
  int ord = eleminfo_offset - 1;
  AGF_WritePointsFromOrdinates(ord,1);
  
  // second write number of elements 1
  AGF_WriteInt(1);
  
  
  AGF_WriteInt(FdoGeometryComponentType_LineStringSegment);
  
  // write number of points
  numpoints_string--; // because start one is already written
  
  AGF_WriteInt(numpoints_string);
  
  AGF_WritePointsFromOrdinates(ord,numpoints_string);
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_CurvePointString


//
// Reads curved subelements and ceates CurveString.
// First triplet eleminfo_interp is number of subelements
// Subelemnts are of Etype 2 ( straight lines or arcs)
//
// It will add start_point, number of subelements and subelements to output buffer.
//
// This is used reading ccurved strings ( Etype 4) or compund rings in polygons Etype ( 1005, 2005)
//
bool c_SdoGeomToAGF::AGF_Get_CurveString(int& ElemInfo_Index)
{
  int eleminfo_offset;
  int eleminfo_etype;
  int eleminfo_interp;
  
  
  
     
  eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
  eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];    
  eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    
  ElemInfo_Index += 3;
  
  int num_subs = eleminfo_interp;
  
  if( !num_subs ) return false; // error not supposed to be
  
  // read first subelement
  eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
  
  // read starting point
  int ord = eleminfo_offset - 1;
  AGF_WritePointsFromOrdinates(ord,1);
  
  // add number of subelements
  int num_string_elem_buffpos = m_BuffLen;
  int num_string_elem = 0;
  AGF_WriteInt(num_string_elem);
  
  int subelem_ind=0;
  while(  subelem_ind < num_subs )
  {
    eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
    eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
    eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];    
    ElemInfo_Index += 3;
    
    int numpoints; // number of points in string
      
    // get next offset if exists
    int next_offset = 0;
    if( ElemInfo_Index < m_ElemInfoSize ) 
    {
      next_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index];
      
      numpoints = (next_offset - eleminfo_offset) / m_PointSize;
      
      // if last sub element that I need to reduce number of points by one beacuse
      // reading is shifted for one beacuse of AFG takes first point into previous element
      if( subelem_ind == (num_subs-1) ) { numpoints--; numpoints=numpoints<0 ? 0 : numpoints; }
    }
    else
    {
    // there is no next subelement
      numpoints = (((int)m_SdoGeom->getSdo_ordinates().size()) + 1 - eleminfo_offset) / m_PointSize;
      
      numpoints--; // beacuse first point is already written in previouse subelement
    }
    
    if( eleminfo_interp == 1)
    {
      AGF_WriteInt(FdoGeometryComponentType_LineStringSegment); // arc CurveElmentType
      
      
      // add number of points
      AGF_WriteInt(numpoints);
      
      
      AGF_WritePointsFromOrdinates(ord,numpoints);
      
      num_string_elem++;
    }
    else
    {
      AGF_WriteInt(FdoGeometryComponentType_CircularArcSegment); // arc CurveElmentType
      AGF_WritePointsFromOrdinates(ord,2); // za arc sta dve tocki
      num_string_elem++;
      
      int numarcs = (numpoints + 2) / 3;
      
      numpoints-=2;
      while( numpoints>=2 )
      {
        AGF_WriteInt(FdoGeometryComponentType_CircularArcSegment); // arc CurveElmentType
        AGF_WritePointsFromOrdinates(ord,2); // za arc sta dve tocki
        num_string_elem++;
        numpoints-=2;
      }
    }

    
    subelem_ind++;
  }
   
  AGF_UpdateInt(num_string_elem_buffpos,num_string_elem);
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_CurveString

//
// Reads polygon
// Starts from element 'ElemInfo_Index' which is index of first triplet in m_SdoGeom->getSdo_elem_info() describing polygon
// WkbBuff.. points to buffer which will recevie transformation to AKB
// Returns number og bytes which are written to WkbBuff
// 0.. zero return means some error
// After function returns:
// - ElemInfo_Index..points on next triplet in m_SdoGeom->getSdo_elem_info() which is to be read ( or to the end of m_SdoGeom->getSdo_elem_info() array if polygon is last)
// - WkbBuff.. points onto the position after this polygon ( writting in WkbBuff is done with *WkbBuff++ = ... 
//
bool c_SdoGeomToAGF::AGF_Get_GType3_PolygonOrCurvePolygon(int& ElemInfo_Index,bool ForceCurve)
{
  
  //int eleminfo_offset = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index++];
  int eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; // do not step forward with ++
  //int eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index++];
      
  bool islinear = true;
  
  int geometry_type_buffpos = m_BuffLen;
  
  AGF_WriteGeometryType(FdoGeometryType_Polygon);
  AGF_WriteDimensionality();
        
  switch( eleminfo_etype )
  {
    case 1:   // point
    {
      return false; // don't now what to do with point in polygon
    }
    break;
    case 2:   // string of stright lines or arcs
              // if( eleminfo_interp == 1 ) // line string of points
              // if( eleminfo_interp == 2 ) // line string of arc's
    {
      return false; // don't now what to do with that in polygon
    }
    break;
    case 3: // old one
    {
       AGF_WriteInt(1); // number of rings is 1
       AGF_Get_LinearString(ElemInfo_Index);
    }
    break;
    case 4:  // compound line string ( connected with some stright lines and some arcs)
    {
      // elem_interp is number of element contained in this compund line string
      // elem_interp is number of next triplets in m_SdoGeom->getSdo_elem_info()
      // in subelemnts only allowed is etype == 2
      
      return false; // don't now what to do with that in polygon
    }
    break;
    case 1003:   // exteriror ring (must be  counterclockwise order of points)
    //case 2003:   // interior ring  (must be  clockwise order of points)
    {         

       // use temp buffer pointer to increment so we can revert to start
      int akb_tempbuff_buffpos = m_BuffLen;
      int akb_numrings_buffpos = m_BuffLen;     // remember where number of rings is
      
      AGF_WriteInt(0); // number of rings is
      
      
      // 
      bool all_islinear = ForceCurve ? false : true;
      
      // check how exterior ring is defined
      int eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];
      switch( eleminfo_interp )      
      {
        case 1:
        {
          if( ForceCurve )
          {
            if( !AGF_Get_CurvePointString(ElemInfo_Index) ) 
            {
              RestoreBuff(akb_tempbuff_buffpos);
              return false;
            }
            
            all_islinear = false;
          }
          else
          {
            if( !AGF_Get_LinearString(ElemInfo_Index) )
            {
              RestoreBuff(akb_tempbuff_buffpos);
              return false; // ni prebran eterior ring
            }
          }
        }
        break;
        case 2:  // arc string
        {
          if( !AGF_Get_CurveArcString(ElemInfo_Index) ) 
          {
            RestoreBuff(akb_tempbuff_buffpos);
            return false;
          }
          
          all_islinear = false;
        }
        break;
        case 3: // rect
        {
        // rect like MBR
        // now in ordintaes there are 2 points   
          AGF_UpdateInt( akb_numrings_buffpos, 1 );       
          AGF_WriteInt( 5 );
          AGF_WriteRectangle(ElemInfo_Index);
        }
        break;
        case 4: // circle
        {
        }
        break;
        default:
        {
          RestoreBuff(akb_tempbuff_buffpos);
          return false;
        }
        break;
      }
      
      int numrings=1;   // exterior ring is read
      
      
      // read interior rings 
      if( all_islinear && !ForceCurve ) // if up to now is linear then try to read as linear
      {
        bool inner_islinear=true;
        AGF_Get_LinearRings_Etype2003(ElemInfo_Index,numrings,inner_islinear);         
        
        AGF_UpdateInt( akb_numrings_buffpos, numrings );       
        
        // exterior ring was read as linear 
        if( !inner_islinear )
        {
          RestoreBuff(akb_tempbuff_buffpos);
          return false;
        }
      }
      else
      {
        AGF_Get_CurveInnerRings_Etype2003_2005(ElemInfo_Index,numrings);         
        AGF_UpdateInt( akb_numrings_buffpos, numrings );           
      }
      
      islinear = all_islinear; 
      
      
    }
    break;
    
    case 1005:   // compound ring - exteriror ring  (compound some are straight lines, some are arcs
    //case 2005:   // compound ring - interior ring
    {
    // elem_interp is number of subelement contained in this compund line polygon
    // elem_interp is number of next triplets (subelemnts) in m_SdoGeom->getSdo_elem_info()
    // in subelemnts only allowed is etype == 2
      
      int akb_numrings_buffpos = m_BuffLen;
      
      AGF_WriteInt(0); // numrings
      
      int numrings=0;
      // read exterior ring
      if( !AGF_Get_CurveString(ElemInfo_Index) ) return false;
      
      numrings++;
      
      // read interior rings 
      AGF_Get_CurveInnerRings_Etype2003_2005(ElemInfo_Index,numrings);   
      
      AGF_UpdateInt( akb_numrings_buffpos, numrings );   
      
      islinear = false;     
            
    }
    break;
    
  }
  
  if( !islinear )
  {
    AGF_UpdateInt(geometry_type_buffpos,FdoGeometryType_CurvePolygon);
  }
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_GType3_PolygonOrCurvePolygon

bool c_SdoGeomToAGF::AGF_Get_GType7_Multi_PolygonOrCurvePolygon(int& ElemInfo_Index)
{
    
  
  bool islinear = true;
  
  // first check is it all linear or there are
  // is it MultiLine or MultiCurve
  int elemid = ElemInfo_Index;
  
  while( (elemid < m_ElemInfoSize) && islinear )
  {
    int elem_etype = m_SdoGeom->getSdo_elem_info()[elemid+1];
    switch( elem_etype )
    {
      case 1003:
      case 2003:
      {
        int elem_interp = m_SdoGeom->getSdo_elem_info()[elemid+2];
        switch( elem_interp )
        {
          case 1:
          break;
          case 2: // arcs
          case 4: // circle
            islinear = false;
          break;
          
        }
      }
      break;
      case 1005: //compund string
      case 2005: //compund string
        islinear = false;
      break;
    }
    
    elemid += 3;
  }
  
  
  if( islinear )
    AGF_WriteGeometryType(FdoGeometryType_MultiPolygon);
  else
    AGF_WriteGeometryType(FdoGeometryType_MultiCurvePolygon);
    
  
  int num_poly = 0;
  int ptr_num_poly_buffpos = m_BuffLen;
  
  AGF_WriteInt(0); // number of polygons
  
  bool forcecurve;
  if( islinear )
  {
  // read as MultiLine - MultiLineString
    forcecurve = false;
  }
  else
  {
    forcecurve = true; // because there are curves (not just linear) than i need to force all to be curves in multi
  }
  
  // read as MultiCurve - MultiCurveString
  int elen=0;
  bool repeat = true;
  while( (ElemInfo_Index < m_ElemInfoSize) && repeat )
  {
    if( AGF_Get_GType3_PolygonOrCurvePolygon(ElemInfo_Index,forcecurve) ) // force da je curve string (not linear)
    {
      num_poly++;  
    }
    else
    {
      repeat = false;
    }
  }
  
  AGF_UpdateInt(ptr_num_poly_buffpos,num_poly);
  
  return true; 
  
}//end of c_SdoGeomToAGF::AGF_Get_GType7_Multi_PolygonOrCurvePolygon

//
// This function will read linear rings from ordinates 1003.
// It will read more than one ring.
// It reads rings while there is 1003 or 2003 etype (linear - interpretation == 1) or end of subelements
// It will stop if next subelement is not 1003 etype or the ened of list is reached.
//
// It will write this to AgfBuff:
//
// LinearRing[] lineStrings; // size = NumRings
//
// struct LinearRing
// {
//   int numPts; // >0
//   double[] coords; // size = numPts* PositionSize(polygon)
//
// }
//
bool c_SdoGeomToAGF::AGF_Get_LinearRings_Etype2003(int& ElemInfo_Index,int& NumRings,bool& IsLinear)
{
  int eleminfo_etype;
  int eleminfo_interp;
  
  
  
  IsLinear = true;
  
  while( ElemInfo_Index<m_ElemInfoSize )
  {  
    eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
    eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];
    
    if( eleminfo_etype != 2003 ) break; // if not interior ring out
    if( eleminfo_interp!=1 ) 
    {
      IsLinear = false;
      break; // if not ring of straight lines out
    }
        
    // read and write rings )
    AGF_Get_LinearString(ElemInfo_Index);
           
    NumRings++;
 
  }
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_LinearRings_Etype2003

/*
int c_SdoGeomToAGF::AGF_Get_CurveRings_Etype2005(int& ElemInfo_Index,int*& AgfBuff,int& NumRings)
{
  int eleminfo_etype;
  int eleminfo_interp;
  
  int bufflen=0; // how many butes was added to buff
  
  while( ElemInfo_Index<m_ElemInfoSize )
  {  
    eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
    eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];
    
    if( eleminfo_etype != 2005 ) break; // if not interior ring out
        
    // read and write rings )
    bufflen += AGF_Get_CurveString(ElemInfo_Index,AgfBuff);
           
    NumRings++;
 
  }
  
  return bufflen;
}//end of c_SdoGeomToAGF::AGF_Get_CurveRings_Etype2005
*/

bool c_SdoGeomToAGF::AGF_Get_CurveInnerRings_Etype2003_2005(int& ElemInfo_Index,int& NumRings)
{
  int eleminfo_etype;
  int eleminfo_interp;
  
  
  while( ElemInfo_Index<m_ElemInfoSize )
  {  
    eleminfo_etype = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+1]; 
    eleminfo_interp = m_SdoGeom->getSdo_elem_info()[ElemInfo_Index+2];
    
    switch( eleminfo_etype )
    {
      case 2005:
      {
        AGF_Get_CurveString(ElemInfo_Index);
        NumRings++;
      }
      break;
      case 2003:
      {
        switch( eleminfo_interp )
        {
          case 1:   // line string
          {
            AGF_Get_LinearString(ElemInfo_Index);
            NumRings++; 
          }
          break;
          case 2:   // string of arcs
          {
            AGF_Get_CurveArcString(ElemInfo_Index);
            NumRings++; 
          }
          break;
          case 3:   // rect defined with two lower and uppoer points
          {
            ElemInfo_Index+=3; // skip it
          }
          break;
          case 4:   // circle
          {
            ElemInfo_Index+=3; // skip it
          }
          break;          
        }
      }
      break;
      default:
      {
        goto out_rings;
      }
    }          
    
  }
  
  out_rings:;
  
  return true;
}//end of c_SdoGeomToAGF::AGF_Get_CurveInnerRings_Etype2003_2005




/*
  Describing sdo_elem_info
  
 switch( eleminfo_etype )
  {
    case 1:   // point
    {
      return 0; // don't now what to do with point in polygon
    }
    break;
    case 2:   // string
    {
      if( eleminfo_interp == 1 ) // line string of points
      {
      }
      else
      {
        if( eleminfo_interp == 2 ) // line string of arc's
        {
        }
      }
    }
    break;
    case 4:  // compound line string ( connected with some stright lines and some arcs)
    {
      // elem_interp is number of element contained in this compund line string
      // elem_interp is number of next triplets in m_SdoGeom->getSdo_elem_info()
      // in subelemnts only allowed is etype == 2
    }
    break;
    case 1003:   // exteriror ring (must be  counterclockwise order of points)
    case 2003:   // interior ring  (must be  clockwise order of points)
    {
      switch( eleminfo_interp )
      {
        case 1: // ring of straight lines
        break;
        case 2: // ring of just arcs
        break;
        case 3: // optimized ring (two-points) minpoint, maxpoint
        break;
        case 4: // circle
        break;
      }
    }
    break;
    case 1005:   // compound polygon - exteriror ring  (compound some are straight lines, some are arcs
    case 2005:   // compound polygon - interior ring
    {
    // elem_interp is number of subelement contained in this compund line polygon
    // elem_interp is number of next triplets (subelemnts) in m_SdoGeom->getSdo_elem_info()
    // in subelemnts only allowed is etype == 2
    }
    break;
    
  }
  */