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

#ifndef _c_FgfToSdoGeom_h
#define _c_FgfToSdoGeom_h

#include "SDO_GEOMETRY.h"

class c_FgfToSdoGeom
{
public:
  c_FgfToSdoGeom(void);
public:
  ~c_FgfToSdoGeom(void);
  
   enum CoordinateDimensionality 
  {
    CoordDim_XY = 0,
    CoordDim_Z = 1,
    CoordDim_M = 2
  };
  
  enum e_TransformResult
  {
    e_Ok=0,
    e_UknownFGFtype,
  };
    
public:

  void SetGeometry(SDO_GEOMETRY* Geom)
  {
    m_SdoGeom = Geom;  
  };
  
  // Convert to Autodesk variant of format very similar to WKB well known binary
  e_TransformResult ToSdoGeom(const int* FGFbuff,long OraSrid,SDO_GEOMETRY*,bool UseOptimizedRect=false);
  
protected:  
  void OraDim(int FGF_CoordDim);
  void AddOrdinates(const int*& FgfBuff,size_t NumPoints,int Etype,int Interp);
  void AddPoint(const int *& FgfBuff);
  void AddElemInfo(int Offset,int Etype,int Interp);
  
 
  
  SDO_GEOMETRY* m_SdoGeom;  
  int m_PointSize;  // count of numbers for one point in sdo_ordinates list
  int m_Ora_Gtype_l;  // coordinate of M  or 0 if no M
  
  unsigned int m_NextOrdOffset; // in process of ading ordinates to array it shows the next index where ordinates will be added
                       // it is SDO_ORDINATE_OFFSET field when adding new ordinates
                       // basucally in process of adding ordinates it is size of ordinates in m_SdoGeom

};

#endif