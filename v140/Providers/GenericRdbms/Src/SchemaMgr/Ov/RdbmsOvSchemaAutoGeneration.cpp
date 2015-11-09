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
#include <Rdbms/Override/RdbmsOvSchemaAutoGeneration.h>

FdoRdbmsOvSchemaAutoGeneration* FdoRdbmsOvSchemaAutoGeneration::Create()
{
    return new FdoRdbmsOvSchemaAutoGeneration();
}

FdoRdbmsOvSchemaAutoGeneration::FdoRdbmsOvSchemaAutoGeneration() :
    mRemoveTablePrefix(true),
    mMaxSampleRows(-1)
{
    mTableListSerializer = FdoXmlNameCollectionHandler::Create();
}

FdoRdbmsOvSchemaAutoGeneration::~FdoRdbmsOvSchemaAutoGeneration()
{
}

void FdoRdbmsOvSchemaAutoGeneration::Dispose()
{
    delete this;
}

FdoString* FdoRdbmsOvSchemaAutoGeneration::GetGenTablePrefix()
{
    return mGenTablePrefix;
}

void FdoRdbmsOvSchemaAutoGeneration::SetGenTablePrefix(FdoString* tablePrefix)
{
    mGenTablePrefix = tablePrefix;
}

FdoBoolean FdoRdbmsOvSchemaAutoGeneration::GetRemoveTablePrefix()
{
    return mRemoveTablePrefix;
}

void FdoRdbmsOvSchemaAutoGeneration::SetRemoveTablePrefix(FdoBoolean removeTablePrefix)
{
    mRemoveTablePrefix = removeTablePrefix;
}

FdoStringCollection* FdoRdbmsOvSchemaAutoGeneration::GetGenTableList()
{
    return mTableListSerializer->GetNames();
}

FdoInt32 FdoRdbmsOvSchemaAutoGeneration::GetMaxSampleRows()
{
    return mMaxSampleRows;
}

void FdoRdbmsOvSchemaAutoGeneration::SetMaxSampleRows(FdoInt32 maxRows)
{
    mMaxSampleRows = maxRows;
}

void FdoRdbmsOvSchemaAutoGeneration::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoPhysicalElementMapping::InitFromXml( pContext, attrs );

    FdoXmlAttributeP att = attrs->FindItem( L"tablePrefix" );
    if ( att ) {
        mGenTablePrefix = att->GetValue();
    }

    att = attrs->FindItem( L"removeTablePrefix" );
    if ( att ) 
        mRemoveTablePrefix = FdoStringP(att->GetValue()).ToBoolean();

    att = attrs->FindItem( L"maxSampleRows" );
    if ( att ) 
        mMaxSampleRows = FdoStringP(att->GetValue()).ToLong();
}

void FdoRdbmsOvSchemaAutoGeneration::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    xmlWriter->WriteStartElement( L"AutoGeneration" );

    if ( mGenTablePrefix != L"" )
        xmlWriter->WriteAttribute( L"tablePrefix", mGenTablePrefix );

    xmlWriter->WriteAttribute( L"removeTablePrefix", mRemoveTablePrefix ? L"true" : L"false" );
    xmlWriter->WriteAttribute( L"maxSampleRows", FdoStringP::Format(L"%d", mMaxSampleRows) );

    FdoPhysicalElementMapping::_writeXml( xmlWriter, flags );

    FdoStringsP tableNames = mTableListSerializer->GetNames();

    if ( tableNames->GetCount() > 0 ) {
        xmlWriter->WriteStartElement( L"TableList" );
        mTableListSerializer->_writeXml( xmlWriter, flags );
        xmlWriter->WriteEndElement();
    }

    xmlWriter->WriteEndElement();
}

FdoXmlSaxHandler* FdoRdbmsOvSchemaAutoGeneration::XmlStartElement(
    FdoXmlSaxContext* context,
    FdoString* uri,
    FdoString* name,
    FdoString* qname,
    FdoXmlAttributeCollection* atts
)
{
    // Handle generic sub-elements.
    FdoXmlSaxHandler* pRet = FdoPhysicalElementMapping::XmlStartElement(context, uri, name, qname, atts);

    if ( !pRet ) {
        // Not a generic sub-element so must be a class; handle it here.

        // Make the TableListSerializer the handler for the table list.
        if ( wcscmp(name, L"TableList") == 0 ) {
            pRet = mTableListSerializer;
        }
    }

    if ( !pRet )
        // report unexpected sub-elements.
        pRet = SubElementError( context, L"AutoGeneration", name );

    return(pRet);
}
