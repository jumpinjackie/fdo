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

#ifndef _c_KGORAFEATUREREADER_H
#define _c_KGORAFEATUREREADER_H

#include "c_SdoGeomToAGF2.h"
#include "c_SdeGeom2AGF.h"
#include <time.h>

#ifdef _FDO_3_5
#include "FdoDefaultFeatureReader.h"
#endif

#ifdef _FDO_3_5
  class c_KgOraFeatureReader : public c_KgOraReader< FdoDefaultFeatureReader>
#else 
  class c_KgOraFeatureReader : public c_KgOraReader< FdoIFeatureReader>
#endif
{
    #ifdef _FDO_3_5
      typedef c_KgOraReader<FdoDefaultFeatureReader> superclass;
    #else
      typedef c_KgOraReader<FdoIFeatureReader> superclass;
    #endif
    public:
        c_KgOraFeatureReader(c_KgOraConnection * Connection
                            ,c_Oci_Statement* OciStatement 
                            ,FdoClassDefinition* ClassDef
                            ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns
                            , FdoIdentifierCollection* Props);

    protected:
        virtual ~c_KgOraFeatureReader();

    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    protected:
    // dispose this object
        virtual void Dispose();

    

    public:
        
 
    
    //-------------------------------------------------------
    // FdoIFeatureReader implementation
    //-------------------------------------------------------
        
    /// \brief
    /// Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.
    /// 
    /// \return
    /// Returns the class definition object.
    /// 
    FDOKGORA_API virtual FdoClassDefinition* GetClassDefinition();

    /// \brief
    /// Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.
    /// 
    /// \return
    /// Returns the depth
    /// 
    FDOKGORA_API virtual FdoInt32 GetDepth();

   

    /// \brief
    /// Gets a reference to an FdoIFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the nested feature reader
    /// 
    FDOKGORA_API virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);
       

    private:
     
        
       
        
        FdoIdentifierCollection* m_Props;
        
        FdoPtr<FdoClassDefinition> m_ClassDef;
            
        
};


class c_KgOraSdeFeatureReader : public c_KgOraFeatureReader
{
public:
  c_KgOraSdeFeatureReader(c_KgOraConnection * Connection
    ,c_Oci_Statement* OciStatement 
    ,FdoClassDefinition* ClassDef
    ,c_KgOraSridDesc& SridDesc
    ,int SdeGeometryType
    ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns
    , FdoIdentifierCollection* Props);

protected:
  virtual ~c_KgOraSdeFeatureReader();

  //-------------------------------------------------------
  // FdoIDisposable implementation
  //-------------------------------------------------------

protected:
  // dispose this object
  virtual void Dispose();
  
public:
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


};
#endif
