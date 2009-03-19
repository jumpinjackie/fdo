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



class c_KgOraDataReader : public c_KgOraReader< FdoIDataReader>
{
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
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
    virtual FdoString* GetPropertyName(FdoInt32 index);

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

#endif