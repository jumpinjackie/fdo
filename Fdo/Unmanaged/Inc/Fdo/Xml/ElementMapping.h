#ifndef FDO_XMLELEMENTMAPPING_H
#define FDO_XMLELEMENTMAPPING_H

//
// Copyright (C) 2004-2006  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <Std.h>
#include <Fdo/Commands/Schema/PhysicalElementMapping.h>
#include <Fdo/Commands/Schema/PhysicalElementMappingCollection.h>

class FdoXmlClassMapping;

/// \brief
/// FdoXmlElementMapping describes GML to FDO mappings for a particular GML Schema
/// element. Only mappings that cannot be represented in FDO are described.
class FdoXmlElementMapping : public FdoPhysicalElementMapping
{
public:
    /// \brief
    /// Constructs an object describing the GML-FDO mappings for a GML element
    /// 
    /// \param name 
    /// Input element name
    /// 
    /// \return
    /// Returns FdoXmlElementMapping
    /// 
    FDO_API static FdoXmlElementMapping* Create(
        FdoString* name
    );

    /// \brief
    /// Sets the class mapping for this element. The class mapping provides
    /// information about the FDO Class for this element's type.
    /// 
    /// \param classMapping 
    /// Input the class mapping
    /// 
    FDO_API void SetClassMapping(FdoXmlClassMapping* classMapping);

    /// \brief
    /// Gets the class mapping for this element. The class mapping provides
    /// information about the FDO Class for this element's type.
    /// 
    /// \return
    /// Returns FdoXmlClassMapping.
    /// 
    FDO_API FdoXmlClassMapping* GetClassMapping();

    /// \brief
    /// Sets the element class name to a dangling reference. This function can be
    /// used to reference a class that has no FdoXmlClassMapping object.
    /// 
    /// \param className 
    /// Input name of the FDO class corresponding to the 
    /// element's type.
    /// 
    FDO_API void SetClassName(
        FdoString* className
    );

    /// \brief
    /// Gets the FDO Class name.
    /// 
    /// \return
    /// Returns FdoString.
    /// 
    FDO_API FdoString* GetClassName();

    /// \brief
    /// Sets the schema name for a dangling reference to an element's class. 
    /// 
    /// \param schemaName 
    /// Input name of the Feature Schema containing the FDO class. If
    /// L"" then it defaults to the schema containing this element.
    /// 
    FDO_API void SetSchemaName(
        FdoString* schemaName = L""
    );

    /// \brief
    /// Gets the Schema Name for the FDO Class.
    /// 
    /// \return
    /// Returns FdoString.
    /// 
    FDO_API FdoString* GetSchemaName();

    /// \brief
    /// Sets the namespace URI for this element. 
    /// 
    /// \param schemaName 
    /// Input a URI. Defaults to the targetNamespace
    /// for the schema containing this element.
    /// 
    FDO_API void SetGmlUri(
        FdoString* gmlUri = L""
    );

    /// \brief
    /// Gets the URI for the referenced element. 
    /// 
    /// \return
    /// Returns FdoString.
    /// 
    FDO_API FdoString* GetGmlUri();

    /// \brief
    /// Sets the local name for this element. 
    /// 
    /// \param schemaName 
    /// Input the local name (unprefixed name) of this
    /// element. Defaults to the element name.
    /// 
    FDO_API void SetGmlLocalName(
        FdoString* gmlLocalName = L""
    );

    /// \brief
    /// Gets the URI for the referenced element. 
    /// 
    /// \return
    /// Returns FdoString. L"" if this element does not reference
    /// another element.
    /// 
    FDO_API FdoString* GetGmlLocalName();

    /// Functions for XML support

/// \cond DOXYGEN-IGNORE
    /// \brief
    /// Initializes this Xml Element Mapping from its XML attributes. Called when
    /// the element is deserialized from XML. 
    /// 
    /// \param pContext 
    /// Input context contain information about the current deserialization operation
    /// \param attrs 
    /// Input the XML attributes.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);

    /// \brief
    /// Writes this XML Element Mapping to XML. Called when
    /// the element is serialized to XML. 
    /// 
    /// \param xmlWriter 
    /// Input write the element to this XML writer
    /// \param flags 
    /// Input flags that control the writing of the element.
    /// 
    /// \return
    /// Returns nothing
    /// 
   	virtual void _writeXml(
    	FdoXmlWriter* xmlWriter, 
    	const FdoXmlFlags* flags
	);
/// \endcond


protected:
/// \cond DOXYGEN-IGNORE
    FdoXmlElementMapping() {}

    FdoXmlElementMapping(
        FdoString* name
    );

    virtual ~FdoXmlElementMapping();

    virtual void Dispose();
/// \endcond

private:
    FdoStringP                  mClassName;
    FdoStringP                  mSchemaName;
    FdoStringP                  mGmlUri;
    FdoStringP                  mGmlLocalName;
    FdoPtr<FdoXmlClassMapping>  mClassMapping;
};

/// \brief
/// FdoXmlElementMappingP is a FdoPtr on FdoXmlElementMapping, provided for convenience.
typedef FdoPtr<FdoXmlElementMapping> FdoXmlElementMappingP;

#endif


