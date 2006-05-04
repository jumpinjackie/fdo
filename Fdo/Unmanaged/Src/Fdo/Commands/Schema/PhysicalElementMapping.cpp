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
#include <Std.h>
#include <Fdo/Commands/CommandException.h>
#include <Fdo/Commands/Schema/PhysicalSchemaMapping.h>

FdoPhysicalElementMapping::FdoPhysicalElementMapping() : 
    m_parent(NULL)
{
}


FdoPhysicalElementMapping::FdoPhysicalElementMapping(FdoString* name/*, FdoString* description */) :
    m_parent(NULL)

{
    SetName(name);
//    SetDescription(description);
}

FdoPhysicalElementMapping::~FdoPhysicalElementMapping()
{
}

FdoPhysicalElementMapping* FdoPhysicalElementMapping::GetParent()
{
    return FDO_SAFE_ADDREF( (FdoPhysicalElementMapping*) m_parent);
}


FdoPhysicalSchemaMapping* FdoPhysicalElementMapping::GetSchemaMapping()
{
    FdoPhysicalElementMappingP   parent = GetParent();

    if (parent == NULL)
        return NULL;

    do
    {
        FdoPhysicalSchemaMapping *schema = dynamic_cast<FdoPhysicalSchemaMapping*>((FdoPhysicalElementMapping*) parent);
        if (schema != NULL)
            return FDO_SAFE_ADDREF(schema);

        parent = parent->GetParent();
    } while(parent != NULL);
    
    return NULL;
}

FdoString* FdoPhysicalElementMapping::GetName()
{
    return m_name;
}

FdoStringP FdoPhysicalElementMapping::GetQualifiedName()
{
    return GetName();
}

void FdoPhysicalElementMapping::SetName(FdoString* value)
{
    m_name = value;
}
/*  TODO: Description needs a bit more thought. For the Oracle Provider, need
 to design a place to persist the descriptions.

FdoString* FdoPhysicalElementMapping::GetDescription()
{
    return m_description;
}

void FdoPhysicalElementMapping::SetDescription(FdoString* value)
{
    m_description = value;
}
*/

FdoXmlSaxHandler* FdoPhysicalElementMapping::GetSkipper()
{
    if ( !m_XmlSkipper ) 
        m_XmlSkipper = FdoXmlSkipElementHandler::Create();

    return m_XmlSkipper;
}


FdoXmlSaxHandler* FdoPhysicalElementMapping::SubElementError(
    FdoXmlSaxContext* pContext, 
    FdoString* parentElement, 
    FdoString* subElement 
)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext && (FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High) ) {
            
        FdoStringP providerName = GetProviderLocalName();
            
        fdoContext->AddError(
            FdoCommandExceptionP(
                FdoCommandException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(COMMANDS_8_BADMAPPINGSUBELEMENT),
                        subElement,
                        (FdoString*) providerName,
                        parentElement,
                        GetName()
                    )
                 )
            )
        );
    }

    return GetSkipper();
}

FdoXmlSaxHandler* FdoPhysicalElementMapping::MultiSubElementError(
    FdoXmlSaxContext* pContext, 
    FdoString* parentElement, 
    FdoString* subElement 
)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext && (FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel() <= FdoXmlFlags::ErrorLevel_Normal) ) {
            
        FdoStringP providerName = GetProviderLocalName();
            
        fdoContext->AddError(
            FdoCommandExceptionP(
                FdoCommandException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(COMMANDS_9_MULTIMAPPINGSUBELEMENT),
                        subElement,
                        (FdoString*) providerName,
                        parentElement,
                        GetName()
                    )
                )
            )
        );
    }

    return GetSkipper();
}

FdoXmlSaxHandler* FdoPhysicalElementMapping::ChoiceSubElementError(
    FdoXmlSaxContext* pContext, 
    FdoString* parentElement, 
    FdoStringsP subElements 
)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext && (FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel() <= FdoXmlFlags::ErrorLevel_Normal) ) {
            
        FdoStringP providerName = GetProviderLocalName();
            
        fdoContext->AddError(
            FdoCommandExceptionP(
                FdoCommandException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(COMMANDS_10_CHOICESUBELEMENT),
                        (FdoString*) subElements->ToString(),
                        (FdoString*) providerName,
                        parentElement,
                        GetName()
                    )
                )
            )
        );
    }

    return GetSkipper();
}

FdoXmlSaxHandler* FdoPhysicalElementMapping::DuplicateSubElementError(
    FdoXmlSaxContext* pContext, 
    FdoString* parentElement, 
    FdoString* subElement, 
    FdoString* subElementName
)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    if ( fdoContext && (FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel() <= FdoXmlFlags::ErrorLevel_Normal) ) {
            
        FdoStringP providerName = GetProviderLocalName();
            
        fdoContext->AddError(
            FdoCommandExceptionP(
                FdoCommandException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(COMMANDS_12_DUPLICATESUBELEMENT),
                        subElement,
                        subElementName,
                        (FdoString*) providerName,
                        parentElement,
                        GetName()
                    )
                )
            )
        );
    }

    return GetSkipper();
}

FdoStringP FdoPhysicalElementMapping::GetProviderLocalName()
{
    FdoStringP localName;

    FdoPhysicalSchemaMappingP mapping = GetSchemaMapping();
    if ( mapping ) {
        FdoStringsP providerTokens = FdoStringCollection::Create( mapping->GetProvider(), L"." ); 
        if ( providerTokens->GetCount() > 1 ) 
            localName = providerTokens->GetString(1);
    }
            
    return( localName );
}


void FdoPhysicalElementMapping::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(pContext);

    FdoXmlAttributeP att = attrs->FindItem( L"name" );
    if ( att ) {
        m_name = att->GetValue();
    }
}


void FdoPhysicalElementMapping::_writeXml(
    FdoXmlWriter* xmlWriter, 
    const FdoXmlFlags* flags
)
{
    if ( m_name.GetLength() > 0 ) {
        xmlWriter->WriteAttribute( 
            L"name", 
            GetName() 
        );
    }
}


void FdoPhysicalElementMapping::SetParent(FdoPhysicalElementMapping* value)
{
    if (value)
    {
        // Verify this wouldn't make element its own parent/grandparent
        FdoPhysicalElementMappingP   parent = FDO_SAFE_ADDREF(value);
        while (parent)
        {
            if (parent == this)
            {
                throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(COMMANDS_11_OWNPARENT),(FdoString *) GetQualifiedName()));
            }

            parent = parent->GetParent();
        }
    }

    m_parent = value;
}





