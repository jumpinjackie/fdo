// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Fdo/Schema/SchemaAttributeDictionary.h>
#include <Fdo/Schema/SchemaElement.h>
#include <Fdo/Schema/SchemaException.h>
#include <string.h>
#include "StringUtility.h"
#include "XmlContext.h"

#define ALLOC_GROWBY        8

#ifdef _DEBUG
    #include <CrtDbg.h>
    #define ASSERT  _ASSERT
#else
    #define ASSERT(x) {}
#endif

static void DeleteStringArray(wchar_t** aString, FdoInt32 cUsed)
{
    if (aString)
    {
        // Free the elements
        for (FdoInt32 i=0; i < cUsed; i++)
        {
            if (aString[i] != NULL)
                FdoStringUtility::ClearString(aString[i]);
        }

        // Free the pointer array
        delete [] aString;
    }
}

static wchar_t** CopyStringArray(wchar_t** aString, FdoInt32 cUsed, FdoInt32 cAlloc)
{
    if (aString && cAlloc > 0)
    {
        // Allocate array
        wchar_t**     aCopy = new wchar_t*[cAlloc];

        // Zero out array
        memset(aCopy, 0, sizeof(wchar_t*)*cAlloc);

        // Copy members
        for (FdoInt32 i=0; i < cUsed; i++)
        {
            if (aString[i])
            {
                aCopy[i] = FdoStringUtility::MakeString(aString[i]);
            }
        }

        // Free old and assign new
        return aCopy;
    }
    else
        return NULL;        // Nothing to copy!
}

FdoSchemaAttributeDictionary::FdoSchemaAttributeDictionary(FdoSchemaElement& parent)
{
    m_alloc = 0;
    m_used = 0;
    m_names = NULL;
    m_values = NULL;
    m_parent = &parent;    // NOTE: Not addref'ed to avoid circular reference issues.
    m_changeInfoState = 0;
    m_allocCHANGED = 0;
    m_usedCHANGED = 0;
    m_namesCHANGED = NULL;
    m_valuesCHANGED = NULL;
}

FdoSchemaAttributeDictionary::~FdoSchemaAttributeDictionary()
{
    if (m_names != m_namesCHANGED)
        DeleteStringArray(m_namesCHANGED, m_usedCHANGED);
    if (m_values != m_valuesCHANGED)
        DeleteStringArray(m_valuesCHANGED, m_usedCHANGED);
    DeleteStringArray(m_names, m_used);
    DeleteStringArray(m_values, m_used);
}

void FdoSchemaAttributeDictionary::Dispose()
{
    delete this;
}

FdoInt32 FdoSchemaAttributeDictionary::GetCount()
{
    return m_used;
}

FdoString** FdoSchemaAttributeDictionary::GetAttributeNames(FdoInt32& length)
{
    length = m_used;
    return (FdoString**)m_names;
}

FdoString* FdoSchemaAttributeDictionary::GetAttributeValue(FdoString* name)
{
    if (!name)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_1_BADATTRIBUTENAME)));

    // Simple search through array
    for (FdoInt32 i=0; i < m_used; i++)
    {
        if (FdoStringUtility::StringCompare(m_names[i], name) == 0)
        {
            // Found it!
            return m_values[i];
        }
    }

    // Not found
    throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_9_ATTRIBUTENOTFOUND)));
}

void FdoSchemaAttributeDictionary::SetAttributeValue(FdoString* name, FdoString* value)
{
    if (!name)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_1_BADATTRIBUTENAME)));

    // Search for existing value
    for (FdoInt32 i=0; i < m_used; i++)
    {
        if (FdoStringUtility::StringCompare(m_names[i], name) == 0)
        {
            // Found it!

            _StartChanges();
            // Verify allocated space for string
            if (value == NULL)
            {
                FdoStringUtility::ClearString(m_values[i]);
            }
            else
            {
                size_t     cLength = FdoStringUtility::StringLength(value);
                if (FdoStringUtility::StringLength(m_values[i]) < cLength)
                {
                    // Not enough room allocated (at least that we can verify)
                    FdoStringUtility::ClearString(m_values[i]);
                    m_values[i] = FdoStringUtility::MakeString(value);
                }

                // Assign new value to string
                FdoStringUtility::StringCopy(m_values[i], value);
            }

            m_parent->SetElementState(FdoSchemaElementState_Modified);
            return;
        }
    }

    // Not found, TODO: add to the arrays?
    throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_9_ATTRIBUTENOTFOUND)));
}

bool FdoSchemaAttributeDictionary::ContainsAttribute(FdoString* name)
{
    if (!name)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_1_BADATTRIBUTENAME)));

    // Simple search through array
    for (FdoInt32 i=0; i < m_used; i++)
    {
        if (FdoStringUtility::StringCompare(m_names[i], name) == 0)
        {
            // Found it!
            return true;
        }
    }

    return false;
}

void FdoSchemaAttributeDictionary::Add(FdoString* name, FdoString* value)
{
    if (!name)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_1_BADATTRIBUTENAME)));

    if (ContainsAttribute(name))
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_8_ATTRIBUTEALREADYINDICTIONARY)));

    _StartChanges();

    // Verify/allocate sufficient array size
    if (m_used+1 > m_alloc)
    {
        // Grow the pointer array
        wchar_t**     aName = new wchar_t*[m_alloc+ALLOC_GROWBY];
        wchar_t**     values = new wchar_t*[m_alloc+ALLOC_GROWBY];

        // Zero out top of array
        memset(&aName[m_used], 0, sizeof(wchar_t*)*(m_alloc-m_used+ALLOC_GROWBY));
        memset(&values[m_used], 0, sizeof(wchar_t*)*(m_alloc-m_used+ALLOC_GROWBY));

        // Copy members
        if (m_used)
        {
            memcpy(aName, m_names, sizeof(wchar_t*)*m_used);
            memcpy(values, m_values, sizeof(wchar_t*)*m_used);
        }

        // Free old and assign new
        delete [] m_names;
        m_names = aName;

        delete [] m_values;
        m_values = values;

        m_alloc += ALLOC_GROWBY;
    }

    // Verify/allocate strings
    ASSERT(!m_names[m_used] && !m_values[m_used]);    // should always be NULL

    // set the name
    m_names[m_used] = FdoStringUtility::MakeString(name);

    // set the value
    if (value != NULL)
    {
        m_values[m_used] = FdoStringUtility::MakeString(value);
    }

    m_used++;
    m_parent->SetElementState(FdoSchemaElementState_Modified);
}

void FdoSchemaAttributeDictionary::Remove(FdoString* name)
{
    if (!name)
        throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_1_BADATTRIBUTENAME)));

    // Simple search through array
    for (FdoInt32 i=0; i < m_used; i++)
    {
        if (FdoStringUtility::StringCompare(m_names[i], name) == 0)
        {
            // Found it!
            
            _StartChanges();

            // Free name and value
            FdoStringUtility::ClearString(m_names[i]);
            FdoStringUtility::ClearString(m_values[i]);

            // Shift array down (unless removing last item)
            if (i+1 < m_used)
            {
                memmove(&m_names[i], &m_names[i+1], sizeof(wchar_t*)*(m_used-i-1));
                memmove(&m_values[i],&m_values[i+1], sizeof(wchar_t*)*(m_used-i-1));
            }

            // Decrement count and clear last item
            m_used--;
            m_names[m_used] = NULL;
            m_values[m_used] = NULL;

            m_parent->SetElementState(FdoSchemaElementState_Modified);
            return;
        }
    }

    // Not found!
    throw FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_6_NAMENOTINATTRIBUTEDICTIONARY)));
}

void FdoSchemaAttributeDictionary::Clear()
{
    _StartChanges();
    ASSERT((m_names == NULL) == (m_values == NULL));

    DeleteStringArray(m_names, m_used);
    DeleteStringArray(m_values, m_used);

    m_names = NULL;
    m_values = NULL;
    m_alloc = 0;
    m_used = 0;
    m_parent->SetElementState(FdoSchemaElementState_Modified);
}

void FdoSchemaAttributeDictionary::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        // Save initial state in case changes are rejected

        m_allocCHANGED = m_alloc;
        m_usedCHANGED = m_used;
        m_namesCHANGED = CopyStringArray(m_names, m_used, m_alloc);
        m_valuesCHANGED = CopyStringArray(m_values, m_used, m_alloc);

        m_changeInfoState |= CHANGEINFO_PRESENT;
    }
}

void FdoSchemaAttributeDictionary::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    m_changeInfoState |= CHANGEINFO_PROCESSED;

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore

        ASSERT((m_values == m_valuesCHANGED) == (m_names == m_namesCHANGED));
        if (m_names != m_namesCHANGED)
        {
            Clear();
            m_alloc = m_allocCHANGED;
            m_used = m_usedCHANGED;
            m_names = m_namesCHANGED;
            m_values = m_valuesCHANGED;
        }
        else
        {
            DeleteStringArray(m_namesCHANGED, m_usedCHANGED);
            DeleteStringArray(m_valuesCHANGED, m_usedCHANGED);
        }

        // reset
        m_namesCHANGED = NULL;
        m_valuesCHANGED = NULL;
    }
}

void FdoSchemaAttributeDictionary::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    m_changeInfoState |= CHANGEINFO_PROCESSED;

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // reset
        if (m_names != m_namesCHANGED)
            DeleteStringArray(m_namesCHANGED, m_usedCHANGED);
        if (m_values != m_valuesCHANGED)
            DeleteStringArray(m_valuesCHANGED, m_usedCHANGED);
        m_namesCHANGED = NULL;
        m_valuesCHANGED = NULL;
    }
}

void FdoSchemaAttributeDictionary::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    ASSERT(!(m_changeInfoState & (CHANGEINFO_PROCESSED|CHANGEINFO_PROCESSING)));
    m_changeInfoState |= CHANGEINFO_PROCESSING;
}

void FdoSchemaAttributeDictionary::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
    {
        ASSERT(m_changeInfoState == 0);
        return; // processing flag already cleared
    }

    ASSERT(m_changeInfoState & CHANGEINFO_PROCESSED);
    m_changeInfoState = 0;
}

FdoXmlSaxHandler* FdoSchemaAttributeDictionary::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;

    if ( wcscmp( name, L"SADItem" ) == 0 ) {
        m_nextName = FdoXmlAttributeP(atts->GetItem( L"name" ))->GetValue();

        // Set up a SAX handler to read description from current sub-element content
        m_XmlContentHandler = FdoXmlCharDataHandler::Create();
        pRet = m_XmlContentHandler;
    }

    return(pRet);
}

FdoBoolean FdoSchemaAttributeDictionary::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    if ( wcscmp(name, L"SADItem") == 0 ) 
       Add( m_nextName, m_XmlContentHandler->GetString() );

    return(false);
}

void FdoSchemaAttributeDictionary::_writeXml( FdoSchemaXmlContext* pContext )
{
    FdoInt32            i;
    FdoInt32            nameCount = 0;
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    writer->WriteStartElement( L"SAD" );

    FdoString** names = GetAttributeNames( nameCount );

    for ( i = 0; i < nameCount; i++ )  {
        writer->WriteStartElement( L"SADItem" );
        writer->WriteAttribute( L"name", names[i] );
        writer->WriteCharacters( GetAttributeValue(names[i]) );
        writer->WriteEndElement();
    }

    writer->WriteEndElement();
}


