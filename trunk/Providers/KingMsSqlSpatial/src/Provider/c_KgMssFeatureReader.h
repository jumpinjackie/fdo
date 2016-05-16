/*
* Copyright (C) 2007  Haris Kurtagic
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


#include <time.h>
#include "FdoDefaultFeatureReader.h"


class c_KgMssFeatureReader : public c_KgMssReader< FdoDefaultFeatureReader>
{
    typedef c_KgMssReader< FdoDefaultFeatureReader> superclass;

    public:
        c_KgMssFeatureReader(c_KgMssConnection * Connection
                            ,otl_stream* OtlStream,c_KgOtlStreamReader* OtlStreamReader
                            ,FdoClassDefinition* ClassDef
                            ,int GeomPropSqlIndex,FdoStringCollection* SqlColumns
                            , FdoIdentifierCollection* Props);

    protected:
        virtual ~c_KgMssFeatureReader();

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
    FDOKGMSS_API virtual FdoClassDefinition* GetClassDefinition();

    /// \brief
    /// Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.
    /// 
    /// \return
    /// Returns the depth
    /// 
    FDOKGMSS_API virtual FdoInt32 GetDepth();

   

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
    FDOKGMSS_API virtual FdoIFeatureReader* GetFeatureObject(FdoString* propertyName);
       

    private:
     
        
       
        
        FdoIdentifierCollection* m_Props;
        
        FdoPtr<FdoClassDefinition> m_ClassDef;
            
        
};

#endif