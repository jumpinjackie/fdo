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

#include "stdafx.h"
#include "c_SdeGeom2AGF.h"


#define D_BUFF_SIZE_INC 300 * 3 * 8 // three hundred 3 dim points
#define D_BUFF_SIZE_RESERVE 512


c_SdeGeom2AGF::c_SdeGeom2AGF()
{
  
  m_SdeGeomLength = 0;
  m_SdeGeom = NULL;
  
  m_BuffSize = D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;
  
  m_BuffMem = new char[m_BuffSize];
  
  m_BuffLen = 0;
  m_BuffCurr = (int*)m_BuffMem;
  
  
  m_UnpackedIntegers=NULL; 
  m_AllocatedUnpackedIntegers=0; 
  
  m_NumberOfParts=0;
  m_AllocatedParts = 8;
  m_FeatureParts = new t_SdeFeaturePart[m_AllocatedParts];
  
}

c_SdeGeom2AGF::~c_SdeGeom2AGF(void)
{
  if( m_BuffMem ) delete [] m_BuffMem;
  
  if( m_FeatureParts ) delete []m_FeatureParts;
  
  if( m_UnpackedIntegers ) delete []m_UnpackedIntegers; 
  
}




//
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
//
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
//
//

int c_SdeGeom2AGF::ToAGF(double MinX,double MinY,double MaxX,double MaxY)
{
  m_BuffLen = 0;
  m_BuffCurr = (int*)m_BuffMem;
  m_NumberOfParts = 0;
  
  m_CoordDim = CoordDim_XY;
  m_PointSize = 2;
  
  AGF_WriteGeometryType(FdoGeometryType_Polygon);
  AGF_WriteDimensionality();
  
  AGF_WriteInt(  1 );     // number of rings
  AGF_WriteInt( 5 );   // number of points
  
    
  int NumPoints = 5;

  int bfs = m_PointSize * NumPoints * sizeof(double); // size is in int
  if( (m_BuffLen+bfs) > ( m_BuffSize-D_BUFF_SIZE_RESERVE) )
  {
    m_BuffSize = m_BuffLen + bfs + D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;

    char *newbuff = new char[m_BuffSize];

    memcpy(newbuff,m_BuffMem,m_BuffLen);

    delete [] m_BuffMem;

    m_BuffMem = newbuff;

    m_BuffCurr = (int*)&m_BuffMem[m_BuffLen];

  }
  
  double *pt;      
  pt = (double *)m_BuffCurr;

  
  // point 1
  *pt++ = MinX;      
  *pt++ = MinY;          

  // point 2
  *pt++ = MaxX;      
  *pt++ = MinY;          

  // point 3
  *pt++ = MaxX;      
  *pt++ = MaxY;          

  // point 4
  *pt++ = MinX;      
  *pt++ = MaxY;          

  // point 5 same as Point 1
  *pt++ = MinX;      
  *pt++ = MinY;          

  m_BuffLen += NumPoints*2*sizeof(double);

  m_BuffCurr = (int*)pt;
  
  return m_BuffLen;
}
int c_SdeGeom2AGF::ToAGF()
{
  
  m_BuffLen = 0;
  m_BuffCurr = (int*)m_BuffMem;
  m_NumberOfParts = 0;
  
  
  
  m_CoordStreamLen = (unsigned int)m_SdeGeom[0] | (unsigned int)m_SdeGeom[1]<<8 | (unsigned int)m_SdeGeom[2]<<16  | (unsigned int)m_SdeGeom[3]<<24; // number of bytes used for coordinates
  
  /*
  bool is_z = m_SdeGeom[4] & 0x01;
  bool is_m = m_SdeGeom[4] & 0x02;
  m_CoordDim = CoordDim_XY;
  if( is_z ) m_CoordDim = m_CoordDim | CoordDim_Z;
  if( is_m ) m_CoordDim = m_CoordDim | CoordDim_M;
  */
  // Dimensionaluty is set in same way fro SDE and FDO geometry format 0.. XY bit 1 is Z bit 2 is M
  m_CoordDim = m_SdeGeom[4];
  
  m_PointSize = 2;
  if( m_CoordDim & CoordDim_Z ) m_PointSize++;
  if( m_CoordDim & CoordDim_M ) m_PointSize++;
  
  
  
  UnpackParts();
   
  
   
  // count of numbers for one point in sdo_ordinates list
  
  
  switch(m_GeometryType)
  {
    case 1: // Point
    {
      AGF_WriteGeometryType(FdoGeometryType_Point);
      AGF_WriteDimensionality();
      
      int parts = GetNumberOfParts();
      if( parts != 1 )
      {
        FdoStringP err = FdoStringP::Format(L"Wrong Number of parts in SDE feature (%d)",parts);
        throw FdoException::Create( err );      
      }
      
      int numofintegers;
      int* ptr_integers = GetPartIntegers(0,numofintegers);
      
      
      AGF_WritePointsFromIntegers(ptr_integers,numofintegers);
      
    }
    break;
    
    
    case 3: // Line
    {

      int eleminfo_ind=0; // starting point for reading sub-elements of string

      bool islinear = true;

     
      AGF_WriteLineString();

    }
    break;
    case 5: // Polygon
    {

      int eleminfo_ind=0; // starting point for reading sub-elements of string

      bool islinear = true;
      AGF_WritePolygon();
      

    }
    break;
    case 7: // MultiPoint
    {
      AGF_WriteGeometryType(FdoGeometryType_MultiPoint);
      // number of points
      
      int parts = GetNumberOfParts();
      AGF_WriteInt(parts); // number of points
      
      
      for(int ind=0;ind<parts;ind++)
      {
      
        int numofintegers;
        int* ptr_integers = GetPartIntegers(ind,numofintegers);
       
        AGF_WriteGeometryType(FdoGeometryType_Point);
        AGF_WriteDimensionality();
        AGF_WritePointsFromIntegers(ptr_integers,numofintegers);
      }
    }
    break;
    case 9: // MultiLine
    {

      int eleminfo_ind=0; // starting point for reading sub-elements of string

      bool islinear = true;
      AGF_WriteGeometryType(FdoGeometryType_MultiLineString);
      
      int parts = GetNumberOfParts();
      AGF_WriteInt(parts);
      for(int ind=0;ind<parts;ind++)
      {
        AGF_WriteLineString(ind);
      }

    }
    break;
    case 11: // MultiPolygon
    {

      int eleminfo_ind=0; // starting point for reading sub-elements of string

      bool islinear = true;

      AGF_WriteGeometryType(FdoGeometryType_MultiPolygon);
      int ptr_num_strings_buffpos = m_BuffLen;
      
      
      AGF_WriteInt(1);
      int numpoly = AGF_WriteMultiPolygon();
      AGF_UpdateInt(ptr_num_strings_buffpos,numpoly);

    }
    break;
    

    default:
    {
      FdoStringP err = FdoStringP::Format(L"Unsupported Geomtry Type (%d)",m_GeometryType);
      throw FdoException::Create( err );      
    }
    break;
  }
  
  return m_BuffLen;
}//end of c_SdeGeom2AGF::ToAKB

void c_SdeGeom2AGF::AGF_WriteLineString()
{
  AGF_WriteGeometryType(FdoGeometryType_LineString);
  AGF_WriteDimensionality();    
  int parts = GetNumberOfParts();
  if( parts != 1 )
  {
    FdoStringP err = FdoStringP::Format(L"Wrong Number of parts in SDE feature (%d)",parts);
    throw FdoException::Create( err );      
  }

  int numofintegers;
  int* ptr_integers = GetPartIntegers(0,numofintegers);


  int numpoints = numofintegers / 2;
  
  AGF_WriteInt(numpoints);
  AGF_WritePointsFromIntegers(ptr_integers,numofintegers);
}
void c_SdeGeom2AGF::AGF_WriteLineString(int PartIndex)
{
  AGF_WriteGeometryType(FdoGeometryType_LineString);
  AGF_WriteDimensionality();    
  

  int numofintegers;
  int* ptr_integers = GetPartIntegers(PartIndex,numofintegers);


  int numpoints = numofintegers / 2;
  
  AGF_WriteInt(numpoints);
  AGF_WritePointsFromIntegers(ptr_integers,numofintegers);
}
void c_SdeGeom2AGF::AGF_WritePolygon()
{
  AGF_WriteGeometryType(FdoGeometryType_Polygon);
  AGF_WriteDimensionality();    

  int parts = GetNumberOfParts();
  if( parts != 1 )
  {
    FdoStringP err = FdoStringP::Format(L"Wrong Number of parts in SDE feature (%d)",parts);
    throw FdoException::Create( err );      
  }

  int numofintegers;
  int* ptr_integers = GetPartIntegers(0,numofintegers);
  AGF_WriteInt(1); // number of rings is 1


  int numpoints = numofintegers / 2;
  
  AGF_WriteInt(numpoints);
  AGF_WritePointsFromIntegers(ptr_integers,numofintegers);
}

void c_SdeGeom2AGF::AGF_WritePolygon(int PartIndex)
{
  AGF_WriteGeometryType(FdoGeometryType_Polygon);
  AGF_WriteDimensionality();    

  int numofintegers;
  int* ptr_integers = GetPartIntegers(PartIndex,numofintegers);
  
  int ptr_num_strings_buffpos = m_BuffLen;
  AGF_WriteInt(1); // temporary number of rings is 1

  
  int numrings = AGF_WriteRingsFromIntegers(ptr_integers,numofintegers);
  AGF_UpdateInt(ptr_num_strings_buffpos,numrings);
  
  //AGF_WriteInt(numpoints);
  //AGF_WritePointsFromIntegers(ptr_integers,numofintegers);
}



int c_SdeGeom2AGF::AGF_WriteMultiPolygon()
{
  int parts = c_SdeGeom2AGF::GetNumberOfParts();
  for( int ind=0;ind<parts;ind++)
  {
    AGF_WritePolygon(ind);
  }

  return parts;
}

// It will write to buffer NumPoint from index
// start reading ordinates from OrdIndex
//
//  It will increment buffer pointer and ordinates index
//
void c_SdeGeom2AGF::AGF_WritePointsFromIntegers(int* Integers,int NumIntegers)
{

  int numpoints = NumIntegers / 2;
  

  int bfs = m_PointSize * numpoints * sizeof(double); // size is in int
  if( (m_BuffLen+bfs) > ( m_BuffSize-D_BUFF_SIZE_RESERVE) )
  {
    m_BuffSize = m_BuffLen + bfs + D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;

    char *newbuff = new char[m_BuffSize];

    memcpy(newbuff,m_BuffMem,m_BuffLen);

    delete [] m_BuffMem;

    m_BuffMem = newbuff;

    m_BuffCurr = (int*)&m_BuffMem[m_BuffLen];

  }

  double *fgf_points;      
  fgf_points = (double *)m_BuffCurr;
  double x,y,e1,e2;
  double laste1,laste2,rel_e1,rel_e2;
  double lastx = m_SridDesc->m_SDE_FalseX ;
  double lasty = m_SridDesc->m_SDE_FalseY;
  

  switch( m_PointSize ) 
  {
    case 2:
    {              
      for(int ind=0;ind<numpoints;ind++)
      {
        x = (double)(*Integers++) / m_SridDesc->m_SDE_XYUnit + lastx;
        y = (double)(*Integers++) / m_SridDesc->m_SDE_XYUnit + lasty;
        lastx = x;
        lasty = y;

        *fgf_points++ = x; 
        *fgf_points++ = y;
      }
      m_BuffLen += numpoints*2*sizeof(double);

      m_BuffCurr = (int*)fgf_points;
    }
    break;

    case 3:
    {
      if( m_CoordDim & CoordDim_Z )
      {
        laste1 = m_SridDesc->m_SDE_FalseZ;
        rel_e1 = m_SridDesc->m_SDE_ZUnit;
      }
      else
      {
        laste1 = m_SridDesc->m_SDE_FalseM;
        rel_e1 = m_SridDesc->m_SDE_MUnit;
      }
      int* e1_integers = &Integers[numpoints*2];
      for(int ind=0;ind<numpoints;ind++)
      {
        x = (double)(*Integers++) / m_SridDesc->m_SDE_XYUnit + lastx;
        y = (double)(*Integers++) / m_SridDesc->m_SDE_XYUnit + lasty;
        lastx = x;
        lasty = y;

        *fgf_points++ = x; 
        *fgf_points++ = y;
        
        e1 = (double)(*e1_integers++) / rel_e1 + laste1;
        
        laste1 = e1;
        
        *fgf_points++ = e1;
      }
      

      m_BuffLen += numpoints*3*sizeof(double);

      m_BuffCurr = (int*)fgf_points;
    }
    break;
    case 4:
    {
      laste1 = m_SridDesc->m_SDE_FalseZ;
      rel_e1 = m_SridDesc->m_SDE_ZUnit;
      
      laste2 = m_SridDesc->m_SDE_FalseM;
      rel_e2 = m_SridDesc->m_SDE_MUnit;
      
      int* e1_integers = &Integers[numpoints*2];
      int* e2_integers = &Integers[numpoints*3];
      for(int ind=0;ind<numpoints;ind++)
      {
        x = (double)(*Integers++) / m_SridDesc->m_SDE_XYUnit + lastx;
        y = (double)(*Integers++) / m_SridDesc->m_SDE_XYUnit + lasty;
        lastx = x;
        lasty = y;

        *fgf_points++ = x; 
        *fgf_points++ = y;

        e1 = (double)(*e1_integers++) / rel_e1 + laste1;
        laste1 = e1;
        
        *fgf_points++ = e1;
        
        e2 = (double)(*e2_integers++) / rel_e2 + laste2;
        laste2 = e2;
        
        *fgf_points++ = e2;
        
      }

      m_BuffLen += numpoints*4*sizeof(double);

      m_BuffCurr = (int*)fgf_points;
    }
    break;
  }     

}//end of   c_SdeGeom2AGF::AGF_WritePointsFromIntegers


// Rings are divided with points which are same as starting point of ring.
int c_SdeGeom2AGF::AGF_WriteRingsFromIntegers(int* Integers,int NumIntegers)
{
  int rings_counter = 0;
  int numpoints = NumIntegers / m_PointSize;
  

  int bfs = m_PointSize * numpoints * sizeof(double); // size is in int
  if( (m_BuffLen+bfs) > ( m_BuffSize-D_BUFF_SIZE_RESERVE) )
  {
    m_BuffSize = m_BuffLen + bfs + D_BUFF_SIZE_INC + D_BUFF_SIZE_RESERVE;

    char *newbuff = new char[m_BuffSize];

    memcpy(newbuff,m_BuffMem,m_BuffLen);

    delete [] m_BuffMem;

    m_BuffMem = newbuff;

    m_BuffCurr = (int*)&m_BuffMem[m_BuffLen];

  }

  double *fgf_points;      
  fgf_points = (double *)m_BuffCurr;
  int ix,iy;
  double x,y,e1,e2;
  double laste1,laste2,rel_e1,rel_e2;
  double lastx = m_SridDesc->m_SDE_FalseX ;
  double lasty = m_SridDesc->m_SDE_FalseY;


  switch( m_PointSize ) 
  {
    case 2:
    { 
      int ring_pt_cnt=0; // counter of points in ring
      int suma_ring_int_x =0,suma_ring_int_y=0; // sum of integers of ring without first point             
      int ptr_num_ringppoints_buffpos=m_BuffLen;
      for(int ind=0;ind<numpoints;ind++)
      {
      
        ix=*Integers++;iy=*Integers++;
        x = (double)(ix) / m_SridDesc->m_SDE_XYUnit + lastx;
        y = (double)(iy) / m_SridDesc->m_SDE_XYUnit + lasty;
        lastx = x;
        lasty = y;
        
        if( ring_pt_cnt == 0 )
        {
          // it is start of ring
          ptr_num_ringppoints_buffpos = m_BuffLen;  
          // write temporary number of ring points
          AGF_WriteInt(1);
          rings_counter++;
          
          suma_ring_int_x =0;suma_ring_int_y=0;
          
          // add point to ring
          AGF_WritePoint(x,y);
          ring_pt_cnt++;
        }
        else
        {
        // add point to ring
          AGF_WritePoint(x,y);
          ring_pt_cnt++;
          
        // check if coordinates of point are equal to start point  
          suma_ring_int_x += ix; suma_ring_int_y += iy;
        // check if point is equal to start point of ring
          if( (suma_ring_int_x == 0) && (suma_ring_int_y == 0) )
          {
            AGF_UpdateInt(ptr_num_ringppoints_buffpos,ring_pt_cnt);
            ring_pt_cnt=0;
          }          
        }
        
        
        //*fgf_points++ = x; 
        //*fgf_points++ = y;
      }
      //m_BuffLen += numpoints*2*sizeof(double);
      //m_BuffCurr = (int*)fgf_points;
      //AGF_UpdateInt(ptr_num_ringppoints_buffpos,ring_pt_cnt);
      if( ring_pt_cnt != 0 )
      {
        // this shouldn't happened if rings are correct - each ring should be closed
        // and ring_pt_cnt should be set to 0 in previous loop
      }
    }
    break;

  case 3:
    {
      int ring_pt_cnt=0; // counter of points in ring
      int suma_ring_int_x =0,suma_ring_int_y=0; // sum of integers of ring without first point             
      int ptr_num_ringppoints_buffpos=m_BuffLen;
      if( m_CoordDim & CoordDim_Z )
      {
        laste1 = m_SridDesc->m_SDE_FalseZ;
        rel_e1 = m_SridDesc->m_SDE_ZUnit;
      }
      else
      {
        laste1 = m_SridDesc->m_SDE_FalseM;
        rel_e1 = m_SridDesc->m_SDE_MUnit;
      }
      int* e1_integers = &Integers[numpoints*2];
      for(int ind=0;ind<numpoints;ind++)
      {
        ix=*Integers++;iy=*Integers++;
        x = (double)(ix) / m_SridDesc->m_SDE_XYUnit + lastx;
        y = (double)(iy) / m_SridDesc->m_SDE_XYUnit + lasty;
        lastx = x;
        lasty = y;
        e1 = (double)(*e1_integers++) / rel_e1 + laste1;
        laste1 = e1;
        
        if( ring_pt_cnt == 0 )
        {
          // it is start of ring
          ptr_num_ringppoints_buffpos = m_BuffLen;  
          // write temporary number of ring points
          AGF_WriteInt(1);
          rings_counter++;

          suma_ring_int_x =0;suma_ring_int_y=0;

          // add point to ring
          AGF_WritePoint(x,y,e1);
          ring_pt_cnt++;
        }
        else
        {
          // add point to ring
          AGF_WritePoint(x,y,e1);
          ring_pt_cnt++;

          // check if coordinates of point are equal to start point  
          suma_ring_int_x += ix; suma_ring_int_y += iy;
          // check if point is equal to start point of ring
          if( (suma_ring_int_x == 0) && (suma_ring_int_y == 0) )
          {
            AGF_UpdateInt(ptr_num_ringppoints_buffpos,ring_pt_cnt);
            ring_pt_cnt=0;
          }          
        }

        //*fgf_points++ = x; 
        //*fgf_points++ = y;

        

        

        //*fgf_points++ = e1;
      }


      //m_BuffLen += numpoints*3*sizeof(double);

      //m_BuffCurr = (int*)fgf_points;
    }
    break;
  case 4:
    {
      int ring_pt_cnt=0; // counter of points in ring
      int suma_ring_int_x =0,suma_ring_int_y=0; // sum of integers of ring without first point             
      int ptr_num_ringppoints_buffpos=m_BuffLen;
      
      laste1 = m_SridDesc->m_SDE_FalseZ;
      rel_e1 = m_SridDesc->m_SDE_ZUnit;

      laste2 = m_SridDesc->m_SDE_FalseM;
      rel_e2 = m_SridDesc->m_SDE_MUnit;

      int* e1_integers = &Integers[numpoints*2];
      int* e2_integers = &Integers[numpoints*3];
      for(int ind=0;ind<numpoints;ind++)
      {
        ix=*Integers++;iy=*Integers++;
        x = (double)(ix) / m_SridDesc->m_SDE_XYUnit + lastx;
        y = (double)(iy) / m_SridDesc->m_SDE_XYUnit + lasty;
        lastx = x;
        lasty = y;

        //*fgf_points++ = x; 
        //*fgf_points++ = y;

        e1 = (double)(*e1_integers++) / rel_e1 + laste1;
        laste1 = e1;

        //*fgf_points++ = e1;

        e2 = (double)(*e2_integers++) / rel_e2 + laste2;
        laste2 = e2;

        //*fgf_points++ = e2;
        
        if( ring_pt_cnt == 0 )
        {
          // it is start of ring
          ptr_num_ringppoints_buffpos = m_BuffLen;  
          // write temporary number of ring points
          AGF_WriteInt(1);
          rings_counter++;

          suma_ring_int_x =0;suma_ring_int_y=0;

          // add point to ring
          AGF_WritePoint(x,y,e1,e2);
          ring_pt_cnt++;
        }
        else
        {
          // add point to ring
          AGF_WritePoint(x,y,e1,e2);
          ring_pt_cnt++;

          // check if coordinates of point are equal to start point  
          suma_ring_int_x += ix; suma_ring_int_y += iy;
          // check if point is equal to start point of ring
          if( (suma_ring_int_x == 0) && (suma_ring_int_y == 0) )
          {
            AGF_UpdateInt(ptr_num_ringppoints_buffpos,ring_pt_cnt);
            ring_pt_cnt=0;
          }          
        }

      }

      //m_BuffLen += numpoints*4*sizeof(double);

      //m_BuffCurr = (int*)fgf_points;
    }
    break;
  }     

  return rings_counter;
}//end of   c_SdeGeom2AGF::AGF_WritePointsFromIntegers


int c_SdeGeom2AGF::GetNumberOfParts()
{
  return m_NumberOfParts;
}

int* c_SdeGeom2AGF::GetPartIntegers( int PartIndex,int& NumOfIntegers )
{
  NumOfIntegers=m_FeatureParts[PartIndex].m_NumberOfIntegers;  
  return &m_UnpackedIntegers[m_FeatureParts[PartIndex].m_IndexOfIntegers];
}

t_SdeFeaturePart* c_SdeGeom2AGF::AddPart()
{
  if( m_NumberOfParts < m_AllocatedParts )
  {
    return &m_FeatureParts[m_NumberOfParts++];
  }
  
  int newsize = m_AllocatedParts + 8;
  t_SdeFeaturePart* newmem = new t_SdeFeaturePart[newsize];
  
  memcpy(newmem,m_FeatureParts,m_AllocatedParts*sizeof(t_SdeFeaturePart));
  
  m_FeatureParts = newmem;
  m_AllocatedParts = newsize;
  
  return &m_FeatureParts[m_NumberOfParts++];
}
void c_SdeGeom2AGF::UnpackParts()
{
  int numof_integers = m_NumOfPts * m_PointSize;
  if( numof_integers > m_AllocatedUnpackedIntegers )
  {
    if( m_UnpackedIntegers ) delete []m_UnpackedIntegers; 
    m_AllocatedUnpackedIntegers=numof_integers + 500; 

    m_UnpackedIntegers = new int[m_AllocatedUnpackedIntegers];
  }
  
// unpack integers
  int val,sign,shift,b;
  int *unpacked_ptr = m_UnpackedIntegers;
  int unpacked_count=0;
  const unsigned char *bytes = &m_SdeGeom[8];
  int bytecount = m_CoordStreamLen;
  
  while( (bytecount>0) && (unpacked_count<numof_integers)  )
  {
    val = *bytes & 0x3f;
    sign = *bytes & 0x40 ? -1 : 1;
    shift = 6;
    while( *bytes & 0x80 )
    {
      bytes++;bytecount--;
      b = *bytes & 0x7f;  
      val = b << shift | val;
      
      shift += 7;
    }
    val = val * sign;
    
    *unpacked_ptr++ = val;
    unpacked_count++;
    
    bytes++;bytecount--;
  }
  
  // now go trough integers and look for part separators
  // part separators are x=-1 y=0
  t_SdeFeaturePart* part = AddPart();  
  part->m_IndexOfIntegers = 0;
  
  int int_count=0,x,y,part_start_ind=0,sumx=0,sumy=0;
  int* int_ptr = m_UnpackedIntegers;
  int xy_count = m_NumOfPts*2; // count of xy pairs
  if( unpacked_count < xy_count ) xy_count = unpacked_count;
  while( int_count < xy_count )
  {
    x = *int_ptr;
    sumx += x;
    y = *(int_ptr+1);
    sumy += y;
    if( sumx==-1 )
    {
      
      if( sumy == 0 )
      {
        part->m_NumberOfIntegers = int_count - part_start_ind;
        part = AddPart();
        part->m_IndexOfIntegers = int_count + 2; // skip separator
        part_start_ind = int_count + 2; // skip separator
      }
    }
    
    int_ptr += 2; 
    int_count+=2;
    
  }
  
  part->m_NumberOfIntegers = int_count - part_start_ind;
 
}



void c_SdeGeom2AGF::AGF_WriteGeometryType(FdoGeometryType GeometryType)
{
  
  *m_BuffCurr++ = GeometryType;
  m_BuffLen += sizeof(int);
  
}//end of c_SdeGeom2AGF::AGF_WriteGeometryType

void c_SdeGeom2AGF::AGF_WriteDimensionality()
{
  
  *m_BuffCurr++ = m_CoordDim;
  m_BuffLen += sizeof(int);
  
}//end of c_SdeGeom2AGF::AGF_WriteDimensionality

void c_SdeGeom2AGF::AGF_UpdateInt(unsigned int BuffPos,int Val)
{
  int* ptr = (int*)&m_BuffMem[BuffPos];
  
  *ptr = Val;
}//end of c_SdeGeom2AGF::AGF_UpdateInt

void c_SdeGeom2AGF::AGF_WriteInt(int Val)
{
  *m_BuffCurr++ = Val;
  m_BuffLen += sizeof(int);
}//end of c_SdeGeom2AGF::AGF_WriteInt

void c_SdeGeom2AGF::AGF_WritePoint(double X,double Y)
{
  double *ptbuff = (double*)m_BuffCurr;
  *ptbuff++ = X;
  *ptbuff++ = Y;
  m_BuffCurr = (int*)ptbuff;
  m_BuffLen += 2*sizeof(double);
}//end of c_SdeGeom2AGF::AGF_WritePoint
void c_SdeGeom2AGF::AGF_WritePoint(double X,double Y,double Z)
{
  double *ptbuff = (double*)m_BuffCurr;
  *ptbuff++ = X;
  *ptbuff++ = Y;
  *ptbuff++ = Z;
  m_BuffCurr = (int*)ptbuff;
  m_BuffLen += 3*sizeof(double);
}//end of c_SdeGeom2AGF::AGF_WritePoint
void c_SdeGeom2AGF::AGF_WritePoint(double X,double Y,double Z,double M)
{
  double *ptbuff = (double*)m_BuffCurr;
  *ptbuff++ = X;
  *ptbuff++ = Y;
  *ptbuff++ = Z;
  *ptbuff++ = M;
  m_BuffCurr = (int*)ptbuff;
  m_BuffLen += 4*sizeof(double);
}//end of c_SdeGeom2AGF::AGF_WritePoint

void c_SdeGeom2AGF::RestoreBuff(int BuffPos)
{
  m_BuffCurr = (int*)&m_BuffMem[BuffPos];
  m_BuffLen = BuffPos;
}//end of c_SdeGeom2AGF::AGF_WriteInt

