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
#include "c_FgfToSdoGeom.h"
#include "c_Ora_API2.h"


c_FgfToSdoGeom::c_FgfToSdoGeom()
{
  
}

c_FgfToSdoGeom::~c_FgfToSdoGeom(void)
{
  m_NextOrdOffset = 1;  
  m_LastOrdOffset = 1;  
  m_PointSize = 2;
  m_Ora_Gtype_l = 0;
}


void c_FgfToSdoGeom::OraDim(int FGF_CoordDim)
{
  if( FGF_CoordDim & CoordDim_Z ) 
  { 
    if( FGF_CoordDim & CoordDim_M )
    {
      m_PointSize=4;m_Ora_Gtype_l=4; 
    }
    else
    {
      m_PointSize=3; m_Ora_Gtype_l=0; 
    }
  }
  else
  {
    if( FGF_CoordDim & CoordDim_M )
    {
      m_PointSize=3;m_Ora_Gtype_l=3; 
    }
    else
    {
      m_PointSize=2;m_Ora_Gtype_l=0; 
    }
  }
}




void c_FgfToSdoGeom::PushPoint(const int *& FgfBuff)
{
  const double* ords = (const double*)FgfBuff;
  
  size_t numord;
  switch( m_PointSize )
  {
    case 3:
      
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
      
      numord = 3;
    break;
    case 4:
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
      
      numord = 4;
    break;
    default:
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
      
      numord = 2;
    break;
  }
  
  m_NextOrdOffset += numord;
  
  FgfBuff = (const int *)ords;
}


void c_FgfToSdoGeom::AddElemInfo(int Offset,int Etype,int Interp)
{
  m_SdoGeom->AppendElemInfoArray(Offset );
  m_SdoGeom->AppendElemInfoArray(Etype );
  m_SdoGeom->AppendElemInfoArray(Interp );
  
}

void c_FgfToSdoGeom::AddOrdinates(const int *& FgfBuff,size_t NumPoints,int Etype,int Interp)
{
  const double* ords = (const double*)FgfBuff;
  
  size_t numord;
  switch( m_PointSize )
  {
    case 3:
      for( size_t ind=0;ind<NumPoints;ind++)
      {
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
      }
      numord = 3 * NumPoints;
    break;
    case 4:
      for( size_t ind=0;ind<NumPoints;ind++)
      {
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
      }
      numord = 4 * NumPoints;
    break;
    default:
      for( size_t ind=0;ind<NumPoints;ind++)
      {
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
        m_SdoGeom->AppendSdoOrdinates(*ords++ );
      }
      numord = 2 * NumPoints;
    break;
  }
  
  m_NextOrdOffset += numord;
  
  AddElemInfo(m_LastOrdOffset,Etype,Interp);
  m_LastOrdOffset = m_NextOrdOffset;  
  
  
  
  FgfBuff = (const int *)ords;
}

c_FgfToSdoGeom::e_TransformResult c_FgfToSdoGeom::ToSdoGeom(const int* FGFbuff,long OraSrid,c_SDO_GEOMETRY* SdoGeom,bool UseOptimizedRect)
{
  
  m_SdoGeom = SdoGeom;
  m_NextOrdOffset = 1;  
  m_LastOrdOffset = 1;
  m_PointSize = 2;
  m_Ora_Gtype_l = 0;
  
  
  int ora_gtype_tt;
  
  const int* fbuff = FGFbuff;
  
  int fgf_gtype = *fbuff;
  
  
  if( fgf_gtype == FdoGeometryType_MultiGeometry )
  {
    fbuff ++ ;
    ora_gtype_tt = 4;
    int numgeoms = *fbuff++;  
    bool repeat=true;    
    while(numgeoms--)
    {
      repeat = FgfGeomToSdoGeom(fbuff) != e_UknownFGFtype;
    }
  }
  else
  {
  
    switch( fgf_gtype )
    {
      case FdoGeometryType_Point:
      {
        ora_gtype_tt = 1;        
      }  
      break;
      
      case FdoGeometryType_LineString:
      {
        ora_gtype_tt = 2;              
      }
      break;
      
      case FdoGeometryType_CurveString:
      {
        ora_gtype_tt = 2;        
      }  
      break;
      
      case FdoGeometryType_Polygon:
      {
        ora_gtype_tt = 3;            
      }
      break;
      
      case FdoGeometryType_CurvePolygon:
      {
        ora_gtype_tt = 3;        
      }
      break;
      
      case FdoGeometryType_MultiPoint:
      {
        ora_gtype_tt = 5;       
      }
      break;
      case FdoGeometryType_MultiLineString:
      {
        ora_gtype_tt = 6;  
      }
      break;
      
      case FdoGeometryType_MultiCurveString:
      {
        ora_gtype_tt = 6;
      }
      break;
      
      case FdoGeometryType_MultiPolygon:
      {  
        ora_gtype_tt = 7; 
      }
      break;
      
      case FdoGeometryType_MultiCurvePolygon:
      {
        ora_gtype_tt = 7;           
      }
      break;        
      
      default:
        return e_UknownFGFtype;
      break;    
    }
    
    FgfGeomToSdoGeom(fbuff);
  }
  
  
  int ora_gtype = m_PointSize * 1000 + m_Ora_Gtype_l*100 + ora_gtype_tt;
  
  SdoGeom->SetSdoGtype(ora_gtype);
  
  
  if( OraSrid > 0 )
  {
    
    SdoGeom->SetSdoSrid(OraSrid);
  }
  else
  {
    SdoGeom->SetNull_SdoSrid();
  }
  
 
 #ifdef _DEBUG 
  char* tempbuff = c_Ora_API2::SdoGeomToString(SdoGeom);
  if( tempbuff )
  {
    delete [] tempbuff;
  }
  #endif
    
  return e_Ok;
}//end of c_FgfToSdoGeom::ToSdoGeom


c_FgfToSdoGeom::e_TransformResult c_FgfToSdoGeom::FgfGeomToSdoGeom(const int*& FGFbuff)
{
  int fgf_coorddim;

  int fgf_gtype = *FGFbuff++;


  switch( fgf_gtype )
  {
    case FdoGeometryType_Point:
    {
      fgf_coorddim = *FGFbuff++;      
      OraDim(fgf_coorddim);   

      AddOrdinates(FGFbuff,1,1,1); // Etype 1 Interp 0        
    }  
    break;

    case FdoGeometryType_LineString:
    {
      
      fgf_coorddim = *FGFbuff++;      
      OraDim(fgf_coorddim);   

      size_t numpts = *FGFbuff++;      

      AddOrdinates(FGFbuff,numpts,2,1);     // Etype 2 Interp 1
    }
    break;

  case FdoGeometryType_CurveString:
    {

      fgf_coorddim = *FGFbuff++;      
      OraDim(fgf_coorddim); 


      // get first point
      PushPoint(FGFbuff);


      // number of elements
      size_t numelems = *FGFbuff++;    

      // add number of elements
      AddElemInfo(m_LastOrdOffset,4,numelems);  // offset ==1; Etype==4 compound' Interp=numelems number of subelements

      int etype;
      for(size_t ind =0;ind<numelems;ind++)
      {
        // now it is int for element type 1==line 2==arc
        etype = *FGFbuff++;
        switch(etype)
        {
        case FdoGeometryComponentType_LineStringSegment:
          {
            size_t numpts = *FGFbuff++;
            m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
            AddOrdinates(FGFbuff,numpts,2,1);     // Etype 2 Interp 1
          }
          break;
        case FdoGeometryComponentType_CircularArcSegment:
          {
            m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
            AddOrdinates(FGFbuff,2,2,2);     // numpts==2 Etype==2 Interp==2 arc            
          }
          break;
        default:
          return e_UknownFGFtype;
          break;
        }
      }
    }
    break;

  case FdoGeometryType_Polygon:
    {
      fgf_coorddim = *FGFbuff++;      
      OraDim(fgf_coorddim); 
            
      size_t numrings = *FGFbuff++;

      // firt one is exterior etype,interp = 1003,1 - others 2003,1
      size_t numpts = *FGFbuff++;
      AddOrdinates(FGFbuff,numpts,1003,1);         

      numrings--;
      for(size_t ind =0;ind<numrings;ind++)
      {
        size_t numpts = *FGFbuff++;
        AddOrdinates(FGFbuff,numpts,2003,1);         
      }
    }
    break;

  case FdoGeometryType_CurvePolygon:
    {
      fgf_coorddim = *FGFbuff++;      
      OraDim(fgf_coorddim); 
      
      size_t numrings = *FGFbuff++;

      //numrings--;
      for(size_t ind =0;ind<numrings;ind++)
      {
        // get first point
        PushPoint(FGFbuff);

        // number of elements
        size_t numelems = *FGFbuff++;    

        // add number of elements
        if( ind==0 )
          AddElemInfo(m_LastOrdOffset,1005,numelems);  // offset ==1; Etype==1005 interiour compound polygon' Interp=numelems number of subelements
        else
          AddElemInfo(m_LastOrdOffset,2005,numelems);  // offset ==1; Etype==2005 exteriour compund polygon' Interp=numelems number of subelements

        int etype;
        for(size_t ind =0;ind<numelems;ind++)
        {
          // now it is int for element type 1==line 2==arc
          etype = *FGFbuff++;
          switch(etype)
          {
          case FdoGeometryComponentType_LineStringSegment:
            {
              size_t numpts = *FGFbuff++;
              m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
              AddOrdinates(FGFbuff,numpts,2,1);     // Etype 2 Interp 1
            }
            break;
          case FdoGeometryComponentType_CircularArcSegment:
            {
              m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
              AddOrdinates(FGFbuff,2,2,2);     // numpts==2 Etype==2 Interp==2 arc            
            }
            break;
          default:
            return e_UknownFGFtype;
            break;
          }
        }
      }

    }
    break;

  case FdoGeometryType_MultiPoint:
    {
      m_PointSize = 2;
      // number of line strings
      int numpoints = *FGFbuff++; 
      if( numpoints > 0 )
      {
        // add first point which will add eleminfo with correct number of points
        *FGFbuff++; // skip geomtry type
        *FGFbuff++; // skip dimensionality

        AddOrdinates(FGFbuff,1,1,numpoints); // one point, Etype==1, Interp==numpoints
        //AddOrdinates(fbuff,1,1,1); // one point, Etype==1, Interp==numpoints
        numpoints--;      
        while(numpoints--)
        {

          // Geometry type - it has to be point
          *FGFbuff++; // skip it    
          *FGFbuff++;  // skip it OraDim(fgf_coorddim);   
          //AddOrdinates(fbuff,1,1,1); 
          PushPoint(FGFbuff); // just add point ot ordiantes list
        }
      }
    }
    break;

    case FdoGeometryType_MultiGeometry:
    {
      int numgeoms = *FGFbuff++;      
      while(numgeoms--)
      {
        FgfGeomToSdoGeom(FGFbuff);
      }
    }
    break;

    case FdoGeometryType_MultiLineString:
    {
      
      // number of line strings
      int numlines = *FGFbuff++;      
      while(numlines--)
      {
        // Geometry type - it has to be line
        int geomtype = *FGFbuff++;

        fgf_coorddim = *FGFbuff++;      
        OraDim(fgf_coorddim);   

        size_t numpts = *FGFbuff++;      

        AddOrdinates(FGFbuff,numpts,2,1);     // Etype 2 Interp 1
      }
    }
    break;

  case FdoGeometryType_MultiCurveString:
    {
      long numcurves = *FGFbuff++;
      while(numcurves--)
      {
        // Geometry type - it has to be curve string
        int geomtype = *FGFbuff++;

        fgf_coorddim = *FGFbuff++;      
        OraDim(fgf_coorddim);         

        // get first point
        PushPoint(FGFbuff);

        // number of elements
        size_t numelems = *FGFbuff++;    

        // add number of elements
        AddElemInfo(m_LastOrdOffset,4,numelems);  // offset ==1; Etype==4 compound' Interp=numelems number of subelements

        int etype;
        for(size_t ind =0;ind<numelems;ind++)
        {
          // now it is int for element type 1==line 2==arc
          etype = *FGFbuff++;
          switch(etype)
          {
          case FdoGeometryComponentType_LineStringSegment:
            {
              size_t numpts = *FGFbuff++;
              m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
              AddOrdinates(FGFbuff,numpts,2,1);     // Etype 2 Interp 1
            }
            break;
          case FdoGeometryComponentType_CircularArcSegment:
            {
              m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
              AddOrdinates(FGFbuff,2,2,2);     // numpts==2 Etype==2 Interp==2 arc            
            }
            break;
          default:
            return e_UknownFGFtype;
            break;
          }
        }  
      }

    }
    break;

  case FdoGeometryType_MultiPolygon:
    {  
      
      int fgf_coorddim;
      size_t numrings,numpts;
      long numpolygons = *FGFbuff++;

      while( numpolygons-- )
      {
        // Geometry type - it has to be polygon skipit
        int geomtype = *FGFbuff++;

        // Coordinate Dimensionality
        fgf_coorddim = *FGFbuff++;      
        OraDim(fgf_coorddim);   

        numrings = *FGFbuff++;

        // firt one is exterior etype,interp = 1003,1 - others 2003,1
        numpts = *FGFbuff++;
        AddOrdinates(FGFbuff,numpts,1003,1);         

        numrings--;
        for(size_t ind =0;ind<numrings;ind++)
        {
          size_t numptsring = *FGFbuff++;
          AddOrdinates(FGFbuff,numptsring,2003,1);         
        }
      }
    }
    break;

  case FdoGeometryType_MultiCurvePolygon:
    {
      
      long numpolygons = *FGFbuff++;
      while( numpolygons-- )
      {
        // Geometry type - it has to be curvepolygon
        int geomtype = *FGFbuff++;

        fgf_coorddim = *FGFbuff++;      
        OraDim(fgf_coorddim);   

        size_t numrings = *FGFbuff++;

        numrings--;
        for(size_t ind =0;ind<numrings;ind++)
        {
          // get first point
          PushPoint(FGFbuff);

          // number of elements
          size_t numelems = *FGFbuff++;    

          // add number of elements
          if( ind==0 )
            AddElemInfo(m_LastOrdOffset,1005,numelems);  // offset ==1; Etype==1005 interiour compound polygon' Interp=numelems number of subelements
          else
            AddElemInfo(m_LastOrdOffset,2005,numelems);  // offset ==1; Etype==2005 exteriour compund polygon' Interp=numelems number of subelements

          int etype;
          for(size_t ind =0;ind<numelems;ind++)
          {
            // now it is int for element type 1==line 2==arc
            etype = *FGFbuff++;
            switch(etype)
            {
            case FdoGeometryComponentType_LineStringSegment:
              {
                size_t numpts = *FGFbuff++;
                m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
                AddOrdinates(FGFbuff,numpts,2,1);     // Etype 2 Interp 1
              }
              break;
            case FdoGeometryComponentType_CircularArcSegment:
              {
                m_LastOrdOffset = m_NextOrdOffset - m_PointSize;
                AddOrdinates(FGFbuff,2,2,2);     // numpts==2 Etype==2 Interp==2 arc            
              }
              break;
            default:
              return e_UknownFGFtype;
              break;
            }
          }
        }
      }
    }
    break;


  default:
    return e_UknownFGFtype;
    break;

  }


  return e_Ok;
}//end of c_FgfToSdoGeom::ToSdoGeom





