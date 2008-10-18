#ifndef FDO_XML_LPPROPERTYDEFINITION_H_
#define FDO_XML_LPPROPERTYDEFINITION_H_
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

#include <FdoStd.h>
#include "LpSchemaElement.h"
#include "LpCollection.h"


// GML LogicalPhysicalSchema property Object.
// Contains the definition of a property plus its corresponding GML Element Mapping.
class FdoXmlLpPropertyDefinition: public FdoXmlLpSchemaElement {

public:
	static FdoXmlLpPropertyDefinition* Create( 
        FdoPropertyDefinition* propertyDefinition, // can be NULL
        FdoXmlElementMapping* mapping // can be NULL
    );

    // Get the FDO Property definition.
    // NULL if not available
    FdoPropertyDefinition* GetPropertyDefinition() const;

    // Get the GML element mapping.
    // NULL if not available
    FdoXmlElementMapping* GetElementMapping() const;

    virtual FdoString* GetName();

protected:
    FdoXmlLpPropertyDefinition(){}
	FdoXmlLpPropertyDefinition( FdoPropertyDefinition* propertyDefinition, FdoXmlElementMapping* mapping );
    virtual ~FdoXmlLpPropertyDefinition() {}

    virtual void Dispose () { delete this;}

private:
    FdoPtr<FdoPropertyDefinition> m_property;
    FdoPtr<FdoXmlElementMapping> m_mapping;


};

typedef FdoPtr<FdoXmlLpPropertyDefinition> FdoXmlLpPropertyDefinitionP;


class FdoXmlLpPropertyCollection : public FdoXmlLpCollection<FdoXmlLpPropertyDefinition>
{
public:
    static FdoXmlLpPropertyCollection* Create(FdoXmlLpSchemaElement* parent);

protected:
    FdoXmlLpPropertyCollection() : FdoXmlLpCollection<FdoXmlLpPropertyDefinition>()
    {
    }

    FdoXmlLpPropertyCollection(FdoXmlLpSchemaElement* parent) 
        : FdoXmlLpCollection<FdoXmlLpPropertyDefinition>(parent)
    {
    }
    virtual ~FdoXmlLpPropertyCollection()
    {
    }

    virtual void Dispose() {
        delete this;
    }
};

typedef FdoPtr<FdoXmlLpPropertyCollection> FdoXmlLpPropertiesP;




#endif


