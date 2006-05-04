#ifndef FDO_XML_LPCLASSDEFINITION_H_
#define FDO_XML_LPCLASSDEFINITION_H_
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

#include <Std.h>
#include "LpSchemaElement.h"
#include "LpCollection.h"

class FdoXmlLpPropertyDefinition;
class FdoXmlLpPropertyCollection;

// GML LogicalPhysicalSchema Class Object.
// Contains the definition of a class plus its corresponding Class Mapping.
class FdoXmlLpClassDefinition: public FdoXmlLpSchemaElement {

public:
    // Create a class definition 
	static FdoXmlLpClassDefinition* Create( 
        FdoClassDefinition* classDefinition,    // can be NULL
        FdoXmlClassMapping* mapping             // can be NULL
    );

    // returns the FDO class definition.
    // NULL if not available
    FdoClassDefinition* GetClassDefinition();

    // returns the class mapping.
    // NULL if not available
    FdoXmlClassMapping* GetClassMapping();

    // Given a GML URI and local name, return the LogicalPhysical property definition.
    // The property can be one inherited from a base class.
    // Returns NULL if this class does not contain this property.
    FdoXmlLpPropertyDefinition* PropertyFromGml( FdoString* uri, FdoString* localName );

    // Given an FDO property name, return the LogicalPhysical property definition.
    // The property can be one inherited from a base class.
    // Returns NULL if this class does not contain this property.
    FdoXmlLpPropertyDefinition* PropertyFromName( FdoString* className );

    // Get the LogicalPhysical definition for the base class.
    // NULL if this class has no base class.
    FdoXmlLpClassDefinition* GetBaseClass();
    void SetBaseClass(FdoXmlLpClassDefinition* baseClass);

    // Get the list of properties (in LogicalPhysical form) for this class.
    // Includes inherited properties.
    FdoXmlLpPropertyCollection* GetProperties();

    virtual FdoString* GetName();


protected:
    FdoXmlLpClassDefinition();
	FdoXmlLpClassDefinition( FdoClassDefinition* classDefinition, FdoXmlClassMapping* classMapping );
    virtual ~FdoXmlLpClassDefinition();

	virtual void Dispose () { delete this; }

private:
    FdoPtr<FdoClassDefinition> m_class;
    FdoPtr<FdoXmlClassMapping> m_mapping;
    FdoPtr<FdoXmlLpClassDefinition> m_baseClass;
    FdoPtr<FdoXmlLpPropertyCollection> m_properties;

// helper
private:
    FdoXmlLpPropertyCollection* _properties();



};

typedef FdoPtr<FdoXmlLpClassDefinition> FdoXmlLpClassDefinitionP;


class FdoXmlLpClassCollection : public FdoXmlLpCollection<FdoXmlLpClassDefinition>
{
public:
    static FdoXmlLpClassCollection* Create(FdoXmlLpSchemaElement* parent);

protected:
    FdoXmlLpClassCollection() : FdoXmlLpCollection<FdoXmlLpClassDefinition>()
    {
    }

    FdoXmlLpClassCollection(FdoXmlLpSchemaElement* parent) 
        : FdoXmlLpCollection<FdoXmlLpClassDefinition>(parent)
    {
    }
    virtual ~FdoXmlLpClassCollection()
    {
    }

    virtual void Dispose() { delete this; }
};

typedef FdoPtr<FdoXmlLpClassCollection> FdoXmlLpClassesP;



#endif


