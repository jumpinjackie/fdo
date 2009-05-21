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


#ifndef _c_KgKmlFdoData_h
#define _c_KgKmlFdoData_h

#ifdef _WIN32
#pragma once
#endif // _WIN32

using namespace std;
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"
#include "c_KgFdoPlacemark.h"

//#include "../Message/Inc/SDFMessage.h"
#include "Sdf.h"
#include "SdfConnection.h"
#include "Sdfprovider.h"
#include <SDF/ICreateSDFFile.h>
#include <SDF/SdfCommandType.h>

#include "c_KgKmlFdoClassConfig.h"
class c_KgKmlSchemaDesc;

//class SdfConnection;

class c_KgKmlFdoData : public FdoIDisposable
{
public:
  c_KgKmlFdoData(void);
public:
  ~c_KgKmlFdoData(void);
  
  void Dispose ();
  int ParseKML(const string& FileName);
  void Flush();

  c_KgKmlSchemaDesc* GetSchemaDesc();
  SdfConnection* GetSdfConn();
  
  int GetNextKmlFdoId() { return m_FdoIdentCounter++ ; }
  
  void SyncPlacemarksFromReader(FdoIReader* Reader);
  void SyncPlacemarksFromSDF();
  
  void SetFdoSchema(FdoFeatureSchema* FdoSchema);
  
  
protected:
  kmldom::ElementPtr m_KmlRoot;
  kmldom::KmlPtr m_KmlKml;
  kmldom::ContainerPtr m_KmlContainer;

  
  
  std::string  m_KmlFileName;
  
  c_KgFdoPlacemarkCollection m_FdoPlacemarks;
  
  bool m_IsDataChanged;
  //std::map<int,kmldom::PlacemarkPtr> m_MapId;
  
  int m_FdoIdentCounter;
  bool m_IsKMZ;
  
  
  FdoPtr<c_KgKmlSchemaDesc> m_FdoSchemaDesc;
  FdoPtr<FdoFeatureClass> m_KmlFdoClassDef;
  FdoPtr<c_KgKmlFdoClassConfig> m_KmlFdoClassConfig;
  FdoPtr<SdfConnection> m_SdfConn;
  
  // Next to members are used to remember which Fdoschema was set against kml connection
  // THIS is workaround when apply schema was executed and connection was closed and than open again
  // n that case (because connection was closed without writing any data) schema is not written in KML
  // and next open of same KML will return empty FDO schema
  // That use case is in Fdo2Fdo when copying data. First apply schema is executed,
  // connection is closed and again opened and copy of data would not work because there is no schema
  //
  // So workaround is that when KML connection is open and if it is empty than connection string will be 
  // checked against m_LastApplySchema_KmlFile.
  // If it is same KML and m_LastApplySchema_FdoSchema is not empty than that schema will be used in new connection.
  static FdoPtr<FdoFeatureSchema> m_LastApplySchema_FdoSchema;
  static std::string m_LastApplySchema_KmlFile;
  
public:
  FDOKGKML_API c_KgKmlFdoClassConfig* GetKmlFdoClassConfig() { return FDO_SAFE_ADDREF(m_KmlFdoClassConfig.p); }
  
protected:
  
  void UpdateFdoPlacemark(c_KgFdoPlacemark * FdoPlacemark, FdoIReader* Reader );
  int ParseExistingKML(const string& FileName);
  void OpenMemorySDF();
  void CreateDefaultSchemaDesc();
  void ApplySpatialContextToSDF();
  c_KgKmlSpatialContext* CreateDefaultSpatialContext();
  void ApplySchemaToSDF();
  void CreateFdoClass(kmldom::Placemark* Place);

};

#endif // _c_KgKmlFdoData_h
