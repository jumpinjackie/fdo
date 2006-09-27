#ifndef FDO_XML_LPSCHEMA_H_
#define FDO_XML_LPSCHEMA_H_
//

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

#include "LpSchemaElement.h"
#include "LpCollection.h"

class FdoXmlLpClassDefinition;
class FdoXmlLpGmlElementDefinition;
class FdoXmlLpClassCollection;
class FdoXmlLpGmlElementCollection;

// GML LogicalPhysicalSchema Schema Object.
// Contains the definition of a Feature Schema plus its corresponding GML Schema Mappings.
class FdoXmlLpSchema: public FdoXmlLpSchemaElement {

public:
	static FdoXmlLpSchema* Create( 
        FdoFeatureSchema* schema,           // can be NULL
        FdoXmlSchemaMapping* mapping        // can be NULL
    );

    // returns the feature schema.
    // NULL if not available
    FdoFeatureSchema* GetFeatureSchema();
    // returns the schema mappings.
    // NULL if not available
    FdoXmlSchemaMapping* GetMappings();

    // Given a URI and local name for a GML element, return its Element Mapping definition
    FdoXmlLpGmlElementDefinition* ElementFromGmlName( FdoString* uri, FdoString* localName, bool bCaseSens = true );

    // Given a URI and local name for a GML type, return the LogicalPhysical class definition
    FdoXmlLpClassDefinition* ClassFromGmlType( FdoString* uri, FdoString* localName, bool bCaseSens = true );

    // Given an FDO Feature class name, return the LogicalPhysical class definition.
    FdoXmlLpClassDefinition* ClassFromName( FdoString* className );

    virtual FdoString* GetName();



protected:
    FdoXmlLpSchema();
	FdoXmlLpSchema( FdoFeatureSchema* schema, FdoXmlSchemaMapping* mapping);
	virtual ~FdoXmlLpSchema();

	virtual void Dispose ();

private:

    FdoPtr<FdoFeatureSchema> m_schema;
    FdoPtr<FdoXmlSchemaMapping> m_mapping;

    FdoPtr<FdoXmlLpClassCollection> m_classes;
    FdoPtr<FdoXmlLpGmlElementCollection> m_elements;


// helpers
private:
    FdoXmlLpGmlElementCollection* _elements();
    FdoXmlLpClassCollection* _classes();

};

typedef FdoPtr<FdoXmlLpSchema> FdoXmlLpSchemaP;

class FdoXmlLpSchemaCollection : public FdoXmlLpCollection<FdoXmlLpSchema>
{
public:
    static FdoXmlLpSchemaCollection* Create(FdoXmlLpSchemaElement* parent);

protected:
    FdoXmlLpSchemaCollection() : FdoXmlLpCollection<FdoXmlLpSchema>()
    {
    }

    FdoXmlLpSchemaCollection(FdoXmlLpSchemaElement* parent) 
        : FdoXmlLpCollection<FdoXmlLpSchema>(parent)
    {
    }
    virtual ~FdoXmlLpSchemaCollection();

    virtual void Dispose();
};

typedef FdoPtr<FdoXmlLpSchemaCollection> FdoXmlLpSchemasP;


#endif


