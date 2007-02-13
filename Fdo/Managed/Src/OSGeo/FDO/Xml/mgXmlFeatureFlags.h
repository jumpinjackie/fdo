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

class FdoXmlFeatureFlags;

BEGIN_NAMESPACE_OSGEO_FDO_XML

/// \brief
/// XmlFeatureFlags extends XmlFlags to specify flags specific to feature 
/// serialization.
/// \remarks
/// The Error Levels inherited from XmlFlags take on the following meaning when
/// applied to features. Suppose we have GML Document A that conforms to GML Schema A.
/// If Document A is read into FDO and then written to Document B then the Error Levels
/// applied to the Document A read have the following meaning:
/// <ul>
/// <li>ErrorLevel_High: Document B will be structurally identical to Document A.
/// Errors are issued if this cannot be guaranteed. Document B will have the 
/// exact same elements and attributes as Document A.
/// <li>ErrorLevel_Normal: There is no information loss. Document B contains
/// all of the information from Document A and Document B conforms to Schema A.
/// Errors are issued if this cannot be guaranteed.
/// However, Documents A and B might not look exactly alike; some Document A elements
/// may have an equivalent but different structure in Document B.
/// <li>ErrorLevel_Low: There can be information loss. Document B is a subset
/// of Document A but still conforms to Schema A. Errors are issued if 
/// this cannot be guaranteed. Any Document A elements, not recognized by
/// FDO, are discarded on read.
/// <li>ErrorLevel_VeryLow: FDO does a best effort read of Document A and write of
/// Document B. There is no guarantee that Document B will look anything like 
/// Document A or that Document B will conform to Schema A.
/// </ul>
/// 
/// The above assumes that FDO is not provided the GML form of Schema A when writing
/// Document B. For example, supposed that FDO can write a Schema A conformant 
/// Document B only when supplied the GML version of Schema A. FDO will still generate 
/// errors and fail to read Document A under ErrorLevel_Low.
public __gc class XmlFeatureFlags : public NAMESPACE_OSGEO_FDO_XML::XmlFlags
{
public:
    /// \brief
    /// The XmlFeatureFlags::ConflictOption specifies how to handle features that 
    /// are already in the DataStore, when Deserializing:
    /// 
    /// \param ConflictOption_Add 
    /// just add new features to the FDO DataStore. If a feature already exists, an 
    /// exception is thrown.
    /// \param ConflictOption_Update 
    /// Update the feature if it already exists, add it if it doesn't exist. This is 
    /// the slowest option, since the feature must be queried from the DataStore before
    /// it is inserted or updated.
    /// \param ConflictOption_Skip 
    /// just add new features. Silently skip features already in the DataStore.
    /// 
	__value enum ConflictOption 
	{
		ConflictOption_Add = FdoXmlFeatureFlags::ConflictOption_Add,
		ConflictOption_Update = FdoXmlFeatureFlags::ConflictOption_Update,
		ConflictOption_Skip = FdoXmlFeatureFlags::ConflictOption_Skip
	};

    /// \brief
    /// Constructs an XmlFeatureFlags object.
    /// 
	XmlFeatureFlags();

    /// \brief
    /// Constructs an XmlFeatureFlags object.
    /// 
    /// \param location 
    /// See XmlFlags::Create()
    /// 
	XmlFeatureFlags(System::String* location);

    /// \brief
    /// Constructs an XmlFeatureFlags object.
    /// 
    /// \param location 
    /// See XmlFlags::Create()
    /// \param errorLevel 
    /// See XmlFlags::Create()
    /// 
	XmlFeatureFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel);

    /// \brief
    /// Constructs an XmlFeatureFlags object.
    /// 
    /// \param location 
    /// See XmlFlags::Create()
    /// \param errorLevel 
    /// See XmlFlags::Create()
    /// \param nameAdjust 
    /// See XmlFlags::Create()
    /// 
	XmlFeatureFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust);

    /// \brief
    /// Constructs an XmlFeatureFlags object.
    /// 
    /// \param location 
    /// See XmlFlags::Create()
    /// \param errorLevel 
    /// See XmlFlags::Create()
    /// \param nameAdjust 
    /// See XmlFlags::Create()
    /// \param conflictOption 
    /// Conflict option to apply to pre-existing features, when deserializing features
    /// into an FDO datastore.
    /// 
	XmlFeatureFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption conflictOption);

    /// \brief
    /// Sets the current option for handling features already in the connection 
    /// datastore.
    /// 
    /// \param conflictOption 
    /// Specifies how feaures, already in the datastore, are handled.
    /// 
	__property System::Void set_Conflictoption(NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption conflictOption);

    /// \brief
    /// Get the current option for handling feature already in the connection datastore.
    ///
    /// \return
    /// Returns the current conflict option.
	__property NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption get_Conflictoption();

    /// \brief
    /// Sets the write feature collection flag. This flag is initially set to true.
    /// 
    /// \param writeMember 
    /// Input 
    /// true: The serialized features are wrapped in a GML FeatureCollection element.
    /// false: features are serialize without a wrapping collection element.
    /// 
	__property System::Void set_WriteCollection(System::Boolean writeMember);

    /// \brief
    /// Gets the write feature collection flag.
    /// 
    /// \return
    /// Returns the write feature collection flag.
    /// 
	__property System::Boolean get_WriteCollection();

    /// \brief
    /// Sets the write feature member flag. This flag is initially set to true.
    /// 
    /// \param writeMember 
    /// Input 
    /// true: Each serialized feature is wrapped in a GML featureMember element.
    /// false: features are serialized without a wrapping member element. Normally,
    /// writeMember would only be set to false when writing values to a collection
    /// object property.
    /// 
	__property System::Void set_WriteMember( System::Boolean writeMember );

    /// \brief
    /// Gets the write feature member flag.
    /// 
    /// \return
    /// Returns the write feature member flag.
    /// 
	__property System::Boolean get_WriteMember();

    /// \brief
    /// Sets the URI for the feature collection element to wrap around serialized 
    /// features. The initial value is 'http://www.opengis.net/gml'
    /// 
    /// \param uri 
    /// Input the URI. If NULL or blank, the URI reverts back to the initial value.
    /// 
	__property System::Void set_CollectionUri(System::String* uri);

    /// \brief
    /// Gets the feature collection element URI.
    /// 
    /// \return
    /// Returns the URI.
    /// 
	__property System::String* get_CollectionUri();

    /// \brief
    /// Sets the name for the feature collection element to wrap around serialized 
    /// features. The initial value is 'FeatureCollection'
    /// 
    /// \param name 
    /// Input the name. If NULL or blank, the name reverts back to the initial value.
    /// 
	__property System::Void set_CollectionName(System::String* name);

    /// \brief
    /// Gets the feature collection element name.
    /// 
    /// \return
    /// Returns System::String*.
    /// 
	__property System::String* get_CollectionName();

    /// \brief
    /// Sets the URI for the feature member element to wrap around each serialized 
    /// feature. The initial value is 'http://www.opengis.net/gml'
    /// 
    /// \param uri 
    /// Input the URI. If NULL or blank, the URI defaults to the URI of the feature 
    /// collection element's sub-element. If this sub-element cannot be determined 
    /// then the URI defaults to the initial value
    /// 
	__property System::Void set_MemberUri(System::String* uri);

    /// \brief
    /// Gets the feature member element URI.
    /// 
    /// \return
    /// Returns System::String*
    /// 
	__property System::String* get_MemberUri();

    /// \brief
    /// Sets the name for the feature member element to wrap around each serialized 
    /// feature. The initial value is 'featureMember'
    /// 
    /// \param name 
    /// Input the name. If NULL or blank, the URI defaults to the name of the feature 
    /// collection element's sub-element. If this sub-element cannot be determined then
    /// the name defaults to the initial value
    /// 
	__property System::Void set_MemberName(System::String* name);

    /// \brief
    /// Gets the feature member element name.
    /// 
    /// \return
    /// Returns System::String*.
    /// 
	__property System::String* get_MemberName();

    /// \brief
    /// Sets the GML ID prefix for writing features. This prefix is prepended to the 
    /// gml:id value for each feature. When features from different FDO Datastores are
    /// serialized to the same GML document, it is recommended that a different GML ID 
    /// prefix be used for each Datastore, to ensure that the gml:id values in the GML
    /// document are unique. The initial value is L"" (no prefix).
    /// 
    /// \param prefix 
    /// Input the GML ID Prefix
    /// 
	__property System::Void set_GmlIdPrefix(System::String* prefix);

    /// \brief
    /// Gets the current GML ID prefix.
    /// 
    /// \return
    /// Returns System::String
    /// 
	__property System::String* get_GmlIdPrefix();

    /// \brief
    /// Sets the schema locations for writing features. The output GML must have schema
    /// definition for all its referenced namespaces, e.g, gml namespace, wfs namespace, 
    /// user defined namespace. There are default values for gml namespace schema location
    /// and wfs namespace schema location, so the user doesn't need to set locations for those
    /// two namespaces. But the user can still set values to override the default values. The
    /// user could call this function multiple times to set schema location for multiple namespace. 
    /// 
    /// \param schemaNamespace 
    /// Namespace of which the schema location is to be set. Like "http://www.opengis.net/wfs" 
    /// for wfs namespace.
    /// \param schemaLocation 
    /// Standard URL indicating the schema location, like "http://schemas.opengeospatial.net/wfs/1.0.0/WFS-basic.xsd"
    /// for wfs namespace.
    /// 
	__property System::Void set_SchemaLocation(System::String* schemaNamespace, System::String* schemaLocation);

    /// \brief
    /// Gets the schema location for the specified namespace.
    /// 
    /// \param schemaNamespace 
    /// Namespace of which the schema location is to be retrieved. Like "http://www.opengis.net/wfs"
    /// 
    /// \return
    /// Returns the URL indicating the schema location. 
    /// 
	__property System::String* get_SchemaLocation(System::String* schemaNamespace);

    /// \brief
    /// Gets all namespaces that have been set for schemaLocation by SetSchemaLocation().
    /// 
    /// \return
    /// Returns all namespaces that have been set for schemaLocation.
    /// 
	__property NAMESPACE_OSGEO_COMMON::StringCollection* get_Namespaces();

    /// \brief
    /// Sets the default namespace which defines all feature/object types found in the input
    /// feature reader which is to be serialized. The user is also supposed to set the schema
    /// location for this namespace.
    /// 
    /// \param defaultNamespace 
    /// Default namespace.
    /// 
	__property System::Void set_DefaultNamespace(System::String* defaultNamespace);

    /// \brief
    /// Gets the default namespace.
    /// 
    /// \return
    /// Returns the default namespace.
    /// 
	__property System::String* get_DefaultNamespace();

public private:
	XmlFeatureFlags(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlFeatureFlags* GetImpObj();
	
/// \cond DOXYGEN-IGNORE
protected:
    virtual System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_XML


