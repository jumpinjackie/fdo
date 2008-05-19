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

#include "stdafx.h"
#include <Rdbms/Override/RdbmsOvPropertyMappingClass.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingConcrete.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingSingle.h>
#include <Rdbms/Override/RdbmsOvObjectPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvClassDefinition.h>

class FdoRdbmsOvClassDefinition;

FdoRdbmsOvPropertyMappingRelation::FdoRdbmsOvPropertyMappingRelation()
{
}

FdoRdbmsOvPropertyMappingRelation::~FdoRdbmsOvPropertyMappingRelation()
{
}

FdoRdbmsOvClassDefinition* FdoRdbmsOvPropertyMappingRelation::GetInternalClass()
{
    return FDO_SAFE_ADDREF( (FdoRdbmsOvClassDefinition*) mClassDefinition );
}

void FdoRdbmsOvPropertyMappingRelation::SetInternalClass(FdoRdbmsOvClassDefinition* classDefinition)
{
    mClassDefinition = FDO_SAFE_ADDREF(classDefinition);
}

FdoXmlSaxHandler* FdoRdbmsOvPropertyMappingRelation::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;

    // The sub-elements are similar to those of a
    // class definition. Create a class definition and pass the SAX events to it.

    if ( !mClassDefinition )
    {
        FdoRdbmsOvClassP temp = CreateInternalClass(true);
    }

    if ( mClassDefinition ) 
        mClassDefinition->SetParent(this);

    return mClassDefinition ? 
        mClassDefinition->XmlStartElement( context, uri, name, qname, atts ) :
        NULL;
}

FdoBoolean FdoRdbmsOvPropertyMappingRelation::XmlEndElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname
)
{
    // Pass this SAX event to the object property class.
    if ( mClassDefinition )
        return mClassDefinition->XmlEndElement( context, uri, name, qname );

    return false;
}

void FdoRdbmsOvPropertyMappingRelation::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    FdoInt32 idx;

    FdoRdbmsOvPropertyMappingDefinition::_writeXml( xmlWriter, flags );

    if ( mClassDefinition ) {
        FdoRdbmsOvTableP table = mClassDefinition->GetTable();
        if ( table )
            table->_writeXml( xmlWriter, flags );
/* For future
        FdoRdbmsOvUniqueKeysP uniqueKeys = mClassDefinition->GetUniqueKeys();
        for ( idx = 0; idx < uniqueKeys->GetCount(); idx++ )
            FdoRdbmsOvUniqueKeyP(uniqueKeys->GetItem(idx))->_writeXml( xmlWriter, flags );

        FdoRdbmsOvIndexesP indexes = mClassDefinition->GetIndexes();
        for ( idx = 0; idx < indexes->GetCount(); idx++ )
            FdoRdbmsOvIndexP(indexes->GetItem(idx))->_writeXml( xmlWriter, flags );
*/
        FdoRdbmsOvReadOnlyPropertiesP props = mClassDefinition->GetProperties();
        for ( idx = 0; idx <props->GetCount(); idx++ )
            FdoRdbmsOvPropertyP( props->GetItem(idx) )->_writeXml( xmlWriter, flags );
    }
}


