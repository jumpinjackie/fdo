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

#ifndef _c_KgOraDataReader_H
#define _c_KgOraDataReader_H

#include "c_SdoGeomToAGF2.h"
#include <time.h>
#ifdef _FDO_3_5
#include "FdoDefaultDataReader.h"
#endif


#ifdef _FDO_3_5
class c_KgOraDataReader : public c_KgOraReader< FdoDefaultDataReader>
#else
  class c_KgOraDataReader : public c_KgOraReader< FdoIDataReader>
#endif
{
  #ifdef _FDO_3_5
    typedef c_KgOraReader<FdoDefaultDataReader> superclass;
  #else
    typedef c_KgOraReader<FdoIDataReader> superclass;
  #endif

    public:
        c_KgOraDataReader(c_KgOraConnection * Connection
                            ,c_Oci_Statement* OciStatement 
                            ,FdoClassDefinition* ClassDef
                            ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns
                            , FdoIdentifierCollection* Props);

    protected:
        virtual ~c_KgOraDataReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
    // dispose this object
        virtual void Dispose();

    

    public:
        
      /// \brief
    /// Gets the number of propertys in the result set.
    /// 
    /// \return
    /// Returns the number of propertys.
    /// 
    virtual FdoInt32 GetPropertyCount();

    /// \brief
    /// Gets the data type of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the type of the property.
    /// 
    virtual FdoDataType GetDataType(FdoString* propertyName);

    /// \brief
    /// Gets the FDO property type of a given property. This is used
    ///  to indicate if a given property is a geometric property or a data property. If the property is
    ///  a FdoPropertyType_DataProperty, then GetDataType can be used to to find the data type of the property.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FDO property type.
    /// 
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName);
    
       

    private:
     
        
       
        
        FdoIdentifierCollection* m_Props;
        
        FdoPtr<FdoClassDefinition> m_ClassDef;
            
        
};

class c_KgOraSdeDataReader : public c_KgOraDataReader
{
public:
  c_KgOraSdeDataReader(c_KgOraConnection * Connection
    ,c_Oci_Statement* OciStatement 
    ,FdoClassDefinition* ClassDef
    ,c_KgOraSridDesc& SridDesc
    ,int SdeGeometryType
    ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns
    , FdoIdentifierCollection* Props,FdoString* SdeSpatialExtent_ColumnName );
      
protected:
  virtual ~c_KgOraSdeDataReader();

  //-------------------------------------------------------
  // FdoIDisposable implementation
  //-------------------------------------------------------

protected:
  // dispose this object
  virtual void Dispose();

public:
  FDOKGORA_API virtual bool IsNull(FdoString* propertyName);
  
  FDOKGORA_API virtual const FdoByte * GetGeometry(FdoString* propertyName, FdoInt32 * count);

  /// \brief
  /// Gets the geometry value of the specified property as a byte array in 
  /// AGF format. Because no conversion is performed, the property must be
  /// of Geometric type; otherwise, an exception is thrown.
  /// 
  /// \param propertyName 
  /// Input the property name.
  /// 
  /// \return
  /// Returns the byte array in AGF format.
  /// 
  FDOKGORA_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);  

protected:
  c_SdeGeom2AGF m_SdeAgfConv;  
  c_KgOraSridDesc m_SridDesc;
  int m_SdeGeometryType;
  FdoStringP m_SdeSpatialExtent_ColumnName; // this is name of property which represents spatial extent of geometry; 
                                            // As spatial extent is returned in query not as geometry but as four 4 coordinates represenitng min,max 
                                            // values, it needs spatial way to generate fdo geometry


};

#endif
