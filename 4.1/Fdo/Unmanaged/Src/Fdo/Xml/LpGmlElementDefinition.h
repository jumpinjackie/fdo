#ifndef FDO_XML_LPGMLELEMENTDEFINITION_H_
#define FDO_XML_LPGMLELEMENTDEFINITION_H_
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

class FdoXmlLpClassDefinition;

// GML LogicalPhysicalSchema Element Object.
// Contains the definition of a GML Element mapping plus its corresponding FDO Class Definition.
class FdoXmlLpGmlElementDefinition: public FdoXmlLpSchemaElement {

public:
    // Create a LogicalPhysical GML element
	static FdoXmlLpGmlElementDefinition* Create( 
        FdoXmlLpClassDefinition* classDefinition,    // can be NULL
        FdoXmlElementMapping* mapping           // can be NULL
    );

    // returns the LogicalPhysical class definition.
    // NULL if not available
    FdoXmlLpClassDefinition* GetClassDefinition() const;

    // returns the GML element mapping.
    // NULL if not available
    FdoXmlElementMapping* GetElementMapping() const;

    virtual FdoString* GetName();


protected:
	FdoXmlLpGmlElementDefinition();
	FdoXmlLpGmlElementDefinition( FdoXmlLpClassDefinition* classDefinition, FdoXmlElementMapping* mapping );
	virtual ~FdoXmlLpGmlElementDefinition();

	virtual void Dispose ()	{delete this;}

private:
    FdoPtr<FdoXmlLpClassDefinition> m_classDef;
    FdoPtr<FdoXmlElementMapping> m_elementMapping;


};

typedef FdoPtr<FdoXmlLpGmlElementDefinition> FdoXmlLpGmlElementDefinitionP;

class FdoXmlLpGmlElementCollection : public FdoXmlLpCollection<FdoXmlLpGmlElementDefinition>
{
public:
    static FdoXmlLpGmlElementCollection* Create(FdoXmlLpSchemaElement* parent);

protected:
    FdoXmlLpGmlElementCollection() : FdoXmlLpCollection<FdoXmlLpGmlElementDefinition>()
    {
    }

    FdoXmlLpGmlElementCollection(FdoXmlLpSchemaElement* parent) 
        : FdoXmlLpCollection<FdoXmlLpGmlElementDefinition>(parent)
    {
    }
    virtual ~FdoXmlLpGmlElementCollection()
    {
    }

    virtual void Dispose() { delete this; }
};

typedef FdoPtr<FdoXmlLpGmlElementCollection> FdoXmlLpGmlElementesP;


#endif


