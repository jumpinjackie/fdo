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

#pragma  once

#include "FDO\Xml\mgXmlFlags.h"

class FdoXmlSpatialContextFlags;

BEGIN_NAMESPACE_OSGEO_FDO_XML

/// \ingroup (OSGeoFDOXml)
/// \brief
/// FdoXmlSpatialContextFlags extends FdoXmlFlags to specify flags specific to Spatial Contexts.
public ref class XmlSpatialContextFlags : public NAMESPACE_OSGEO_FDO_XML::XmlFlags
{
public:
    /// \brief
    /// XmlSpatialContextFlags::ConflictOption specifies how 
    /// to handle Spatial Contexts that are already in the DataStore, 
    /// when Deserializing:
    /// 
    /// \param ConflictOption_Add 
    /// just add new spatial contexts to the FDO DataStore. 
    /// If the Spatial Context already exists, an exception is thrown.
    /// \param ConflictOption_Update 
    /// Update the spatial context if it already
    /// exists, add it if it doesn't exist.
    /// \param ConflictOption_Skip 
    /// Just add new spatial contexts. 
    /// Silently skip spatial contexts already in the DataStore.
    /// 
	enum class ConflictOption 
	{
		ConflictOption_Add = FdoXmlSpatialContextFlags::ConflictOption_Add,
		ConflictOption_Update = FdoXmlSpatialContextFlags::ConflictOption_Update,
		ConflictOption_Skip = FdoXmlSpatialContextFlags::ConflictOption_Skip
	};

    /// \brief
    /// Constructs an XmlSpatialContextFlags object.
    /// 
	XmlSpatialContextFlags();

    /// \brief
    /// Constructs an XmlSpatialContextFlags object.
    /// 
    /// \param location 
    /// Base URL for generating well-known references to 
    /// GML coordinate systems and transformations
    /// 
	XmlSpatialContextFlags(System::String^ location);

    /// \brief
    /// Constructs an XmlSpatialContextFlags object.
    /// 
    /// \param location 
    /// Base URL for generating well-known references to 
    /// GML coordinate systems and transformations
    /// \param errorLevel 
    /// Input The error level for reading spatial contexts. Controls how 
    /// strict the error reporting is.
    /// 
	XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel);

    /// \brief
    /// Constructs an XmlSpatialContextFlags object.
    /// 
    /// \param location 
    /// Base URL for generating well-known references to 
    /// GML coordinate systems and transformations
    /// \param errorLevel 
    /// Input The error level for reading spatial contexts. Controls how 
    /// strict the error reporting is.
    /// \param nameAdjust 
    /// Input true: apply name adjustment to all elements. 
    /// false: apply name adjustment only to elements with fdo:nameAdjust="true"
    /// 
	XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust);

    /// \brief
    /// Constructs an XmlSpatialContextFlags object.
    /// 
    /// \param location 
    /// Base URL for generating well-known references to 
    /// GML coordinate systems and transformations
    /// \param errorLevel 
    /// Input The error level for reading spatial contexts. Controls how 
    /// strict the error reporting is.
    /// \param nameAdjust 
    /// Input true: apply name adjustment to all elements. 
    /// false: apply name adjustment only to elements with fdo:nameAdjust="true"
    /// \param conflictOption 
    /// Input option for Deserializing Spatial Contexts. 
    /// Specified how Spatial Contexts, already in  connection, are handled.
    /// 
	XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption conflictOption);

    /// \brief
    /// Constructs an XmlSpatialContextFlags object.
    /// 
    /// \param location 
    /// Base URL for generating well-known references to 
    /// GML coordinate systems and transformations
    /// \param errorLevel 
    /// Input The error level for reading spatial contexts. Controls how 
    /// strict the error reporting is.
    /// \param nameAdjust 
    /// Input true: apply name adjustment to all elements. 
    /// false: apply name adjustment only to elements with fdo:nameAdjust="true"
    /// \param conflictOption 
    /// Input option for Deserializing Spatial Contexts. 
    /// Specified how Spatial Contexts, already in  connection, are handled.
    /// \param includeDefault 
    /// Input true: When Serializing Spatial Contexts, serialize
    /// all contexts including the default.
    /// false: skip the default Spatial Context.
    /// 
	XmlSpatialContextFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption conflictOption, System::Boolean includeDefault);

    /// \brief
    /// Sets the current option for handling Spatial Contexts
    /// already in the connection.
    /// 
    /// \param conflictOption 
    /// Input option for Deserializing Spatial Contexts. 
    /// Specified how Spatial Contexts, already in the FDO connection, are handled.
    /// 
	System::Void SetConflictOption(NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption conflictOption);

    /// \brief
    /// Get the current option for handling Spatial Contexts
    /// already in the connection.
    /// 
    /// \return
    /// Returns the current conflict option.
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags::ConflictOption GetConflictOption();

    /// \brief
    /// Sets the default Spatial Context inclusion flag.
    /// 
    /// \param includeDefault 
    /// Input true: When Serializing Spatial Contexts, serialize
    /// all contexts including the default.
    /// false: skip the default Spatial Context.
    /// 
    /// \brief
    /// Gets the default Spatial Context inclusion flag.
    /// 
    /// \return
    /// Returns the default Spatial Context inclusion flag.
    /// 
    property System::Boolean IncludeDefault
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    /// Constructs a XmlSpatialContextFlags object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XmlSpatialContextFlags(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(unmanaged, autoDelete)
	{
		
	}

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoXmlSpatialContextFlags* GetImpObj()
	{
		return static_cast<FdoXmlSpatialContextFlags*>(__super::UnmanagedObject.ToPointer());
	}
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_XML


