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

#ifndef _c_KGORAFEATUREREADER_H
#define _c_KGORAFEATUREREADER_H

#include "c_SdoGeomToAGF.h"
#include <time.h>



class c_KgOraFeatureReader : public c_KgOraReader<FdoIFeatureReader>
{
    public:
        c_KgOraFeatureReader(c_KgOraConnection * Connection
                            ,oracle::occi::Statement* OcciStatement ,oracle::occi::ResultSet* OcciResultSet
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

#endif
