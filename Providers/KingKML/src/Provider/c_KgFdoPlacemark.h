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


#ifndef _c_KgFdoPlacemark_h
#define _c_KgFdoPlacemark_h

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"


class c_KgFdoPlacemark : public FdoIDisposable
{
public:
  c_KgFdoPlacemark(void);
  c_KgFdoPlacemark(int FdoId,kmldom::PlacemarkPtr Placemark);
  ~c_KgFdoPlacemark(void);
  void Dispose ();   
 
 friend class  c_KgFdoPlacemarkCollection;
protected:
  int m_FdoId;
  
  kmldom::PlacemarkPtr  m_Placemark;
  
  bool m_IsKmlGeometryParsed;
  
  FdoPtr<FdoPropertyValueCollection> m_FdoPropertyValues;
  FdoPtr<FdoIGeometry> m_FdoGeometry;
  //m_FdoBBox;
  
public:
  bool HasGeometry();
  FdoByteArray* GetFdoGeometry();
  

  void SetGeometry(FdoByteArray* FdoGeom);
  void SetAttribute( const std::string& Name,const std::string& Value );
  
  const kmldom::PlacemarkPtr& GetPlacemark() { return m_Placemark; }
  
public:
  void ParsePlacemarkGeometry();  
  void ParsePlacemarkData();

  FdoIEnvelope* GetGeometryExtent();
  FdoPropertyValueCollection* GetPropertyValues();
  void SetPlacemarkName( FdoString* Value );
  void SetPlacemarkDescription( FdoString* Value );
  
protected:
  FdoIGeometry* CreateFdoGeometry(const kmldom::GeometryPtr& kmlgeom);
    
};

class  c_KgFdoPlacemarkCollection : public FdoCollection<c_KgFdoPlacemark,FdoException>
{

protected:
  virtual void Dispose()
  {
    delete this;
  } 
  
public:
    
  c_KgFdoPlacemark* FindFdoPlacemark(int FdoId)
  {
    FdoPtr<c_KgFdoPlacemark> fdoplace;
    int count = GetCount();
    for(int ind=0;ind<count;ind++)
    {
      fdoplace = GetItem(ind)  ;
      if( fdoplace ->m_FdoId == FdoId )
      {
        return FDO_SAFE_ADDREF(fdoplace.p);
      }
    }       
    
    return NULL;
  }

};

#endif