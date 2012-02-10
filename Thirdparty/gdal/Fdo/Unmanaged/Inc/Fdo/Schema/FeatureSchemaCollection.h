#ifndef _FEATURESCHEMACOLLECTION_H_
#define _FEATURESCHEMACOLLECTION_H_

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

#include <Fdo/Schema/FeatureSchema.h>
#include <Fdo/Schema/FeatureClassCollection.h>
#include <Fdo/Schema/SchemaCollection.h>
#include <Fdo/Xml/Deserializable.h>
#include <Fdo/Xml/Serializable.h>
#include <Fdo/Commands/Schema/PhysicalSchemaMappingCollection.h>

class FdoSchemaXmlContext;

/// \brief
/// The FdoFeatureSchemaCollection class represents a collection of FdoFeatureSchema objects.
class FdoFeatureSchemaCollection : 
    public FdoSchemaCollection<FdoFeatureSchema>,
    public FdoXmlDeserializable,
    public FdoXmlSerializable
{
/// \cond DOXYGEN-IGNORE
protected:
    FdoFeatureSchemaCollection()
	{
	}
    /// Constructs an instance of a FeatureSchemaCollection using the specified argument.
    FdoFeatureSchemaCollection(FdoSchemaElement* parent) :
        FdoSchemaCollection<FdoFeatureSchema>(parent),
        m_XmlContext(NULL)
    {
        m_setItemParent = true;
    }

    virtual ~FdoFeatureSchemaCollection();

    virtual void Dispose()
    {
        delete this;
    }

    /// FdoXmlSerializable override that serializes a Feature Schema collection to XML.
  	virtual void _writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags );
/// \endcond

public:
    /// \brief
    /// Constructs an instance of an FdoFeatureSchemaCollection using the specified argument.
    /// 
    /// \param parent 
    /// Input parent
    /// 
    /// \return
    /// Returns FdoFeatureSchemaCollection
    /// 
    FDO_API static FdoFeatureSchemaCollection* Create(FdoSchemaElement* parent);

    /// \brief
    /// Gets the stylesheet for converting a Feature Schema collection from 
    /// internal to external XML format. The internal format is very similar to 
    /// the Feature Schema class hierarchy. The external format is OGC GML.
    /// 
    /// \return
    /// Returns the stylesheet
    /// 
    FDO_API virtual FdoXmlReader* GetFromInternalStylesheet();

    /// \brief
    /// Gets the stylesheet for converting a Feature Schema collection from 
    /// external to internal XML format. The internal format is very similar to 
    /// the Feature Schema class hierarchy. The external format is OGC GML.
    /// 
    /// \return
    /// Returns the stylesheet
    /// 
    FDO_API virtual FdoXmlReader* GetFromExternalStylesheet();

    /// \brief
    /// Takes a qualified or unqualified class name and return a collection of class definitions
    /// that match the class name. For qualified name, the collection contains at most one class definition.
    /// 
    /// \param className 
    /// The class name
    /// 
    /// \return
    /// Returns a collection of classes
    /// 
    FDO_API virtual FdoIDisposableCollection* FindClass(const wchar_t* className);

    /// \brief
    /// Gets a Schema Mapping Collection
    /// that describes the mappings between the elements in the GML Schema that were read
    /// and the FDO Feature Schema Elements that were created. This function returns the 
    /// Schema Mapping Collection generated by the last call to ReadXml(). The collection
    /// contains one FdoXmlSchemaMapping per schema that was read.
    /// 
    /// \return
    /// Returns FdoPhysicalSchemaMappingCollection*. Returns NULL if ReadXml() has not been
    /// called on this object.
    /// 
	FDO_API FdoPhysicalSchemaMappingCollection* GetXmlSchemaMappings();

/// \cond DOXYGEN-IGNORE

    /// FdoXmlDeserializable override for getting the SAX Context for reading 
    /// Feature Schemas. Not exposed via the FDO API.
    virtual FdoXmlSaxContext* GetSaxContext();

    /// FdoXmlDeserializable overrides for deserializing a Feature Schema collection 
    /// from XML. These are not exposed via the FDO API.

    virtual void XmlEndDocument(FdoXmlSaxContext* context);
    virtual FdoXmlSaxHandler* XmlStartElement( 
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
/// \endcond

private:
    /// Current deserialization context.
    FdoSchemaXmlContext*     m_XmlContext;
    FdoSchemaMappingsP     m_XmlSchemaMappings; 


};

/// \ingroup (typedefs)
/// \brief
/// FdoFeatureSchemasP is a FdoPtr on FdoFeatureSchemaCollection, provided for convenience.
typedef FdoPtr<FdoFeatureSchemaCollection> FdoFeatureSchemasP;

#endif


