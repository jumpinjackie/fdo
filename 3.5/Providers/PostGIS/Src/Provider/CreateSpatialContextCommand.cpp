//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "Connection.h"
#include "SpatialContext.h"
#include "SpatialContextReader.h"
#include "SpatialContextCollection.h"
#include "CreateSpatialContextCommand.h"

#include <cassert>

namespace fdo { namespace postgis {

CreateSpatialContextCommand::CreateSpatialContextCommand(Connection *conn)
  : Base(conn)
{
    m_Name = SpatialContextDefaultName;    
    m_Description = SpatialContextDefaultDesc;    
    
    m_CoordinateSystem = SpatialContextDefaultCoordSysName;
    m_CoordinateSystemWkt = SpatialContextDefaulWkt;
    
    m_SpatialContextExtentType = FdoSpatialContextExtentType_Dynamic;
    
    //FdoPtr<FdoByteArray> m_Extent;
    
    m_XYTolerance = SpatialContextDefaultXYTolerance;
    m_ZTolerance = SpatialContextDefaultZTolerance;
    m_UpdateExisting = false;
}

CreateSpatialContextCommand::~CreateSpatialContextCommand(void)
{
}

 /// \brief
/// Gets the name of the context to create as a string.
/// 
/// \return
/// Returns the name of the spatial context
/// 
FdoString* CreateSpatialContextCommand::GetName()
{
  return m_Name;
}

/// \brief
/// Sets the name of the context to create as a string.
/// 
/// \param value 
/// Input the name of the spatial context
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetName(FdoString* Name)
{
  m_Name = Name;
}

/// \brief
/// Gets the description of the context to create as a string.
/// 
/// \return
/// Returns the description of the spatial context
/// 
FdoString* CreateSpatialContextCommand::GetDescription()
{
  return m_Description;
}

/// \brief
/// Sets the description of the context to create as a string.
/// 
/// \param value 
/// Input the description of the spatial context
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetDescription(FdoString* Desc)
{
  m_Description = Desc;
}

/// \brief
/// Gets the coordinate system name as a string.
/// 
/// \return
/// Returns the coordinate system
/// 
FdoString* CreateSpatialContextCommand::GetCoordinateSystem()
{
  return m_CoordinateSystem;
}

/// \brief
/// Sets the coordinate system name as a string.
/// 
/// \param value 
/// Input the coordinate system
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetCoordinateSystem(FdoString* CoordinateSystem)
{ 
  m_CoordinateSystem = CoordinateSystem;
}

/// \brief
/// Gets the coordinate system definition in OGC SRS WKT format for the 
/// spatial context currently being created. If the result is an empty string, 
/// the coordinate system WKT description has not been set.
/// 
/// \return
/// Returns the coordinate system description in WKT of of the spatial context.
/// 
FdoString* CreateSpatialContextCommand::GetCoordinateSystemWkt()
{
  return m_CoordinateSystemWkt;
}

/// \brief
/// Sets the coordinate system definition in OGC SRS WKT 
/// format for the spatial context currently being created.
/// 
/// \param value 
/// Input the coordinate system WKT
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetCoordinateSystemWkt(FdoString* CoordinateSystemWkt)
{
  m_CoordinateSystemWkt = CoordinateSystemWkt;
}

/// \brief
/// Gets the desired extent type of the context to create, either static or dynamic.
/// 
/// \return
/// Returns the extent type
/// 
FdoSpatialContextExtentType CreateSpatialContextCommand::GetExtentType()
{
  return m_SpatialContextExtentType;
}

/// \brief
/// Sets the desired extent type of the context to create, either static or dynamic.
/// 
/// \param value 
/// Input the extent type
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetExtentType(FdoSpatialContextExtentType SpatialContextExtentType)
{
  m_SpatialContextExtentType = SpatialContextExtentType;
}

/// \brief
/// Gets the extent of the context to create as a byte array in AGF format.
/// If the extent type is dynamic, any value specified via this function is ignored.
/// 
/// \return
/// Returns the extent of the spatial context
/// 
FdoByteArray* CreateSpatialContextCommand::GetExtent()
{
  return FDO_SAFE_ADDREF(m_Extent.p);
}

/// \brief
/// Sets the extent of the context to create as a byte array in AGF format.
/// If the extent type is dynamic this is optional and any value specified is ignored.
/// 
/// \param value 
/// Input the extent of the spatial context
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetExtent(FdoByteArray* Extent)
{
  m_Extent = Extent;
  FDO_SAFE_ADDREF(m_Extent.p);
}

/// \brief
/// Gets the tolerance value to use for X/Y ordinates. Tolerances are used
/// in some geometric tests, mostly for equality between coordinates.  This
/// occurs frequently in spatial queries, especially with "on boundary" or "just
/// touching" conditions.  It is also an objective amount that can be used
/// when formulating step distances for parametric curve interpolation.
/// 
/// \return
/// Returns the tolerance
/// 
const double CreateSpatialContextCommand::GetXYTolerance()
{
  return m_XYTolerance;
}

/// \brief
/// Sets the tolerance value to use for X/Y ordinates. Tolerances are used
/// in some geometric tests, mostly for equality between coordinates.  This
/// occurs frequently in spatial queries, especially with "on boundary" or "just
/// touching" conditions.  It is also an objective amount that can be used
/// when formulating step distances for parametric curve interpolation.
/// 
/// \param value 
/// Input the tolerance
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetXYTolerance(const double XYTolerance)
{
  m_XYTolerance = XYTolerance;
}

/// \brief
/// Gets the tolerance value to use for Z ordinates. Tolerances are used in
/// some geometric tests, mostly for equality between coordinates.  This
/// occurs frequently in spatial queries, especially with "on boundary" or "just
/// touching" conditions.  It is also an objective amount that can be used
/// when formulating step distances for parametric curve interpolation.
/// 
/// \return
/// Returns the tolerance
/// 
const double CreateSpatialContextCommand::GetZTolerance()
{
  return m_ZTolerance;
}

/// \brief
/// Sets the tolerance value to use for Z ordinates. Tolerances are used in
/// some geometric tests, mostly for equality between coordinates.  This
/// occurs frequently in spatial queries, especially with "on boundary" or "just
/// touching" conditions.  It is also an objective amount that can be used
/// when formulating step distances for parametric curve interpolation.
/// 
/// \param value 
/// Input the tolerance
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetZTolerance(const double ZTolerance)
{
  m_ZTolerance = ZTolerance;
}

/// \brief
/// Gets a Boolean flag that indicates how the CreateSpatialContext command
/// should behave if the spatial context already exists. If update existing
/// is true and a spatial context with the specified name already exists
/// then it will be updated. If update existing is false and a spatial
/// context with the specified name already exists, command execution
/// will fail.
/// 
/// \return
/// Returns true if command should update existing contexts
/// 
const bool CreateSpatialContextCommand::GetUpdateExisting()
{
  return m_UpdateExisting;
}

/// \brief
/// Sets a Boolean flag that indicates how the CreateSpatialContext command
/// should behave if the spatial context already exists. If update existing
/// is true and a spatial context with the specified name already exists
/// then it will be updated. If update existing is false and a spatial
/// context with the specified name already exists, command execution
/// will fail.
/// 
/// \param value 
/// Input true if existing contexts should be updated
/// 
/// \return
/// Returns nothing
/// 
void CreateSpatialContextCommand::SetUpdateExisting(const bool UpdateExisting)
{
  m_UpdateExisting = UpdateExisting;
}

/// \brief
/// Executes the CreateSpatialContext command. An exception is thrown if
/// the spatial context already exists, or if the coordinate system or
/// extent type is not supported by the provider.
/// 
/// \return
/// Returns nothing
/// 

// This will simulate adding new spatial context.
// It will just add into current spatial context list of connection
// It will add if I can find same coordinate system into oracle

// This function is implemented in such way - so ApplySchema could find this spatial context
// Client would call create spatial context before apply command
// So when Apply command is checking for spatial context this one will be found.
// Important thing is that create SC and Apply Schema are call one after other without closing conection
// so this spatial context list will stay.
void CreateSpatialContextCommand::Execute()
{
  SpatialContextCollection::Ptr spContexts = mConn->GetSpatialContexts();
  
  SpatialContext::Ptr spContext = spContexts->FindItem(GetName());
  
  if( spContext != NULL  )
  {
    // already exist - do nothing
    return;
  }

  // spatial context doesnt exist - create one
  std::string csName,csWkt;
  FdoStringP orig_name,orig_csname,orig_wkt,sridText;
  orig_name = GetName();
  orig_csname = GetCoordinateSystem();
  orig_wkt = GetCoordinateSystemWkt();
  
  FdoStringP name = GetName();
  FdoStringP temp = name.Mid(0,8);
  //Find using srid in name
  if( temp.ICompare("PostGIS_") == 0)
  {
    FdoStringP temp = name.Mid(8,name.GetLength()-8);
    sridText =  temp;
    if(mConn->GetCoordinateSystemWkt(static_cast<char const*>(sridText),csName,csWkt))
    {
      orig_wkt = csWkt.c_str();
    }
  }
  else
  {  
    //Find using srid in csname
    FdoStringP tempname = GetCoordinateSystem();    
    FdoStringP temp = tempname.Mid(0,8);
    if( temp.ICompare("PostGIS_") == 0)
    {
      FdoStringP temp = tempname.Mid(8,tempname.GetLength()-8);
      sridText =  temp;
      if(mConn->GetCoordinateSystemWkt(static_cast<char const*>(sridText),csName,csWkt))
      {
        orig_wkt = csWkt.c_str();
      }
    }
    else
    {
      //Find srid using in csname in wkt
      std::string tmpSrid;
      if(mConn->GetSrid(static_cast<char const*>(orig_csname),tmpSrid) == false)
      {
        FdoStringP msg = FdoStringP::Format(L"The Spatial Reference System not found.\nCreate an entry in spatial_ref_sys\n Name: %ls \nWkt:%ls",
            static_cast<FdoString*>(orig_csname), 
            static_cast<FdoString*>(orig_wkt));
        FDOLOG_WRITE("ERROR: %ls", static_cast<FdoString*>(msg));
        throw FdoCommandException::Create(msg);    
      }
      sridText = tmpSrid.c_str();
    }
  }
  
  SpatialContext::Ptr newSpContext(new SpatialContext());
  newSpContext->SetSRID(sridText.ToLong());
  newSpContext->SetName(orig_name);
  newSpContext->SetDescription(GetDescription()); 
  newSpContext->SetCoordinateSystem(orig_csname);
  newSpContext->SetCoordinateSystemWkt(orig_wkt);
  newSpContext->SetXYTolerance( GetXYTolerance() );
  newSpContext->SetZTolerance( GetZTolerance() );
  spContexts->Add( newSpContext );
}//end of CreateSpatialContextCommand::Execute

}} // namespace fdo::postgis
