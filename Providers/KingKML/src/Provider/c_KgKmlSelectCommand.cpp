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
#include "c_KgKmlSelectCommand.h"
//#include "c_KgKmlFeatureReader.h"


#include "c_LogApi.h"

#include <malloc.h>

c_KgKmlSelectCommand::c_KgKmlSelectCommand (c_KgKmlConnection* Conn) 
    : c_KgKmlFdoFeatureCommand<FdoISelect> (Conn)
{
  m_OrderingOption = FdoOrderingOption_Ascending;
}



c_KgKmlSelectCommand::~c_KgKmlSelectCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void c_KgKmlSelectCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* c_KgKmlSelectCommand::GetPropertyNames ()
{
    if (m_PropertyNames == NULL)
        m_PropertyNames = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(m_PropertyNames.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType c_KgKmlSelectCommand::GetLockType ()
{
    throw FdoCommandException::Create (L"Locking not supported.");
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void c_KgKmlSelectCommand::SetLockType (FdoLockType value)
{
    throw FdoCommandException::Create (L"Locking not supported (%1$ls).");
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy c_KgKmlSelectCommand::GetLockStrategy ()
{
    throw FdoCommandException::Create (L"Locking not supported.");
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void c_KgKmlSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    throw FdoCommandException::Create (L"Locking not supported (%1$ls).");
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgKmlSelectCommand::Execute ()
{
 
  return NULL;
    
}//end of c_KgKmlSelectCommand::Execute 

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* c_KgKmlSelectCommand::ExecuteWithLock ()
{
  throw FdoCommandException::Create (L"Locking not supported ");
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* c_KgKmlSelectCommand::GetLockConflicts ()
{
    throw FdoCommandException::Create (L"Locking not supported ");
}





// just check for spatial filter
// other filters ignored
void c_KgKmlSelectCommand::CreateFilterSqlString(FdoFilter* Filter,string& WhereBuff)
{
  
    if( !Filter ) return;
  
    FdoSpatialCondition* spatial_filter = NULL;
    
    
    FdoString* testsql = Filter->ToString();        
    
    spatial_filter = dynamic_cast<FdoSpatialCondition*>(Filter);
    if( !spatial_filter ) return;
    
        
    if (spatial_filter->GetOperation() == FdoSpatialOperations_EnvelopeIntersects)
    {
        FdoPtr<FdoExpression> expr = spatial_filter->GetGeometry();
        FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

        if (geomval)
        {
            
            FdoPtr<FdoIdentifier> geomprop = spatial_filter->GetPropertyName();
            FdoStringP gcolname = geomprop->GetName();
            
            
            FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
            FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

            char* sbuff = new char[512];
            char* sbuff2= new char[256];
            
            sprintf(sbuff2,"MDSYS.SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,3),SDO_ORDINATE_ARRAY(%.6lf,%.6lf, %.6lf,%.6lf))"
                          ,envelope->GetMinX(),envelope->GetMinY(),envelope->GetMaxX(),envelope->GetMaxY());
            
            // TODO: for envelope interscet just use primary filter SDO_FILTER and compare perfomance
            //sprintf(sbuff,"SDO_FILTER(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            
            WhereBuff.assign(sbuff);
            
            delete [] sbuff;
            delete [] sbuff2;

        }
    }
    else if (spatial_filter->GetOperation() == FdoSpatialOperations_Intersects)
    {
        
        FdoPtr<FdoExpression> expr = spatial_filter->GetGeometry();
        FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

        if (geomval)
        {
            FdoPtr<FdoIdentifier> geomprop = spatial_filter->GetPropertyName();
            FdoStringP gcolname = geomprop->GetName();
            
            FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
            FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();
            
            char* sbuff = new char[512];
            char* sbuff2= new char[256];
            
            sprintf(sbuff2,"SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,3),SDO_ORDINATE_ARRAY(%.6lf,%.6lf, %.6lf,%.6lf))"
                          ,envelope->GetMinX(),envelope->GetMinY(),envelope->GetMaxX(),envelope->GetMaxY());
            
            sprintf(sbuff,"SDO_ANYINTERACT(a.%s,%s)='TRUE'",(const char*)gcolname,sbuff2);
            
            WhereBuff.assign(sbuff);
            
            delete [] sbuff;
            delete [] sbuff2;
        }
    }
    


}//end of c_KgKmlSelectCommand::CreateFilterSqlString


/// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
/// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* c_KgKmlSelectCommand::GetOrdering()
{ 
  if( m_OrderingIdentifiers.p == NULL )
    m_OrderingIdentifiers = FdoIdentifierCollection::Create();
    
  return FDO_SAFE_ADDREF(m_OrderingIdentifiers.p); 
}

/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
/// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
/// FdoOrderingOption_Ascending is the default value.</param> 
/// <returns>Returns nothing</returns> 
void c_KgKmlSelectCommand::SetOrderingOption( FdoOrderingOption  Option ) 
{
  m_OrderingOption = Option;
}

/// <summary>Gets the ordering option.</summary>
/// <returns>Returns the ordering option.</returns> 
FdoOrderingOption c_KgKmlSelectCommand::GetOrderingOption( )
{ 
  return m_OrderingOption; 
}