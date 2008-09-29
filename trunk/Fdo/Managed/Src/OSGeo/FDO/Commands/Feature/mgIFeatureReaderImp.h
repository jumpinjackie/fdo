/*
* Copyright (C) 2004-2006  Autodesk, Inc.
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
*
*/

#pragma once

#include "FDO\Commands\Feature\mgIReaderImp.h"
#include "FDO\Commands\Feature\mgIFeatureReader.h"

class FdoIFeatureReader;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class ClassDefinition;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The IFeatureReaderImp class is a concrete implementation class for interface IFeatureReader.
/// The IFeatureReader interface provides a forward-only, read-only iterator
/// for reading feature data.  A reference to an IFeatureReader is returned
/// from the Select and SelectAndLock commands. Because the initial position of the
/// IFeatureReader is prior to the first item, you must call
/// ReadNext to begin accessing any data.
private __gc class IFeatureReaderImp : public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IReaderImp, 
                                       public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader
{
public:
    /// \brief
    /// Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.
    /// 
    /// \return
    /// Returns the class definition object.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* GetClassDefinition();

    /// \brief
    /// Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.
    /// 
    /// \return
    /// Returns the feature reader depth
    /// 
	System::Int32 GetDepth();

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns a pointer to the byte array in FGF format.
    /// 
	System::Byte GetGeometry(System::String* propertyName) [];

    /// \brief
    /// Gets a reference to an IFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the nested feature reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* GetFeatureObject(System::String* propertyName);

public private:
	IFeatureReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IReaderImp(unmanaged, autoDelete)
	{

	}

	inline FdoIFeatureReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


