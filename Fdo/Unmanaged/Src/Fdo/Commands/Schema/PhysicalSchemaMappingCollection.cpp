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
#include <iostream>
#include <fstream>
#include <FdoStd.h>
#include <Common/Vector.h>
#include <Fdo/Xml/Context.h>
#include <Fdo/Commands/Schema/PhysicalSchemaMappingCollection.h>
#include <Fdo/ClientServices/ClientServices.h>
#include <Fdo/ClientServices/FeatureAccessManager.h>
#include <Fdo/ClientServices/ConnectionManager.h>
#include <Fdo/ClientServices/ProviderRegistry.h>
#include <Fdo/ClientServices/ClientServiceException.h>
#include <Fdo/ClientServices/Provider.h>
#include <Fdo/ClientServices/ProviderNameTokens.h>

FdoPhysicalSchemaMappingCollection* FdoPhysicalSchemaMappingCollection::Create()
{
    return new FdoPhysicalSchemaMappingCollection();
}

FdoPhysicalSchemaMappingCollection::FdoPhysicalSchemaMappingCollection()
{
}


FdoPhysicalSchemaMapping* FdoPhysicalSchemaMappingCollection::GetItem( FdoInt32 item )
{
    return FdoCollection<FdoPhysicalSchemaMapping, FdoCommandException>::GetItem( item );
}

FdoPhysicalSchemaMapping * FdoPhysicalSchemaMappingCollection::GetItem( 
    FdoString* providerName, 
	FdoString* schemaName
)
{
    FdoInt32                    i;
    FdoPhysicalSchemaMappingP   foundMapping;

    // Parse the provider name into its '.' separated tokens
    FdoProviderNameTokensP providerTokens = FdoProviderNameTokens::Create( providerName );
    
    // Parse out the tokens pertaining to version
    FdoVectorP providerVersion = providerTokens->GetVersionTokens();
    FdoProviderNameTokensP foundProvider;

    if ( providerTokens->GetNameTokens()->GetCount() < 2 ) 
        return( NULL );

    // Look for the schema mapping that matches the given schema name and provider
    for ( i = 0; i < GetCount(); i++ ) {
        FdoPhysicalSchemaMappingP   mapping = GetItem(i);

        // Schema name must match exactly.
        if ( wcscmp(schemaName, mapping->GetName()) == 0 ) {
            FdoProviderNameTokensP mappingTokens = FdoProviderNameTokens::Create( mapping->GetProvider() );

            // Choose the mapping with the latest version not later than the 
            // version of the given provider. If the given provider name 
            // does not contain the version, then just choose the mapping with 
            // the latest version.

            if ( (mappingTokens <= providerTokens) &&
                 ((!foundProvider) || (mappingTokens > foundProvider)) 
            ) {
                foundMapping = mapping;
                foundProvider = mappingTokens;
            }
        }
    }
 
    return FDO_SAFE_ADDREF( (FdoPhysicalSchemaMapping*) foundMapping );
}

FdoPhysicalSchemaMapping* FdoPhysicalSchemaMappingCollection::GetItem( 
	FdoIConnection* connection, 
	FdoString* schemaName
)
{
    // Create a temporary schema mapping to get the connection's provider name.
    FdoPhysicalSchemaMappingP   tempMapping = connection->CreateSchemaMapping();
    return GetItem( tempMapping->GetProvider(), schemaName );
}

FdoXmlSaxContext* FdoPhysicalSchemaMappingCollection::GetSaxContext()
{
    FdoXmlFlagsP flags = GetDeserializationFlags();
    FdoXmlReaderP reader = GetXmlReader();

    return FdoXmlContext::Create( flags, reader );
}


void FdoPhysicalSchemaMappingCollection::XmlEndDocument(FdoXmlSaxContext* context)
{
    // Throw accumulated errors reading schema mappings, if any.
    context->ThrowErrors();
}

FdoXmlSaxHandler* FdoPhysicalSchemaMappingCollection::XmlStartElement( 
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* ret = NULL;
    FdoPtr<FdoProvider> foundProvider;
    FdoProviderNameTokensP foundTokens;
    FdoInt32 i;

    if ( wcscmp(name, L"SchemaMapping") == 0 ) {
        try {
            // in a Schema Mapping element, try to read it. 

            // Get Schema name for error reporting.
            FdoStringP schemaName;
            FdoXmlAttributeP nameAtt = atts->FindItem( L"name" );

            if ( nameAtt ) 
                schemaName = nameAtt->GetValue();

            FdoXmlAttributeP providerAtt = atts->FindItem( L"provider" );

            // Schema Mapping must have an associated provider.
            if ( providerAtt ) {
                FdoProviderNameTokensP providerTokens = FdoProviderNameTokens::Create( providerAtt->GetValue() );

                if ( providerTokens->GetNameTokens()->GetCount() < 3 ) {
                // Provider name has no version info, log an error.
                    context->AddError( 
                        FdoCommandExceptionP(
                            FdoCommandException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(COMMANDS_4_INVALIDPROVIDERNAME),
                                    (FdoString*) schemaName,
                                    providerAtt->GetValue()
                                )
                            )
                        )
                    );
                }

                // Find a matching provider in the Provider Registry.
                FdoPtr<IProviderRegistry> pRegistry = FdoFeatureAccessManager::GetProviderRegistry();
                const FdoProviderCollection* pProviders = pRegistry->GetProviders();

                for ( i = 0; i < pProviders->GetCount(); i++ ) {
                    FdoPtr<FdoProvider> pProvider = pProviders->GetItem(i);
                    FdoProviderNameTokensP currProviderTokens = FdoProviderNameTokens::Create( pProvider->GetName() );

                    // Skip providers without version ( < 3 tokens ).
                    if ( currProviderTokens->GetNameTokens()->GetCount() >= 3 ) {

                        // Choosen the provider with the lowest version later than the 
                        // version of the Schema Mapping to read. The provider version
                        // has to be later to ensure that it can handle this schema mapping.
                        if ( (currProviderTokens >= providerTokens) &&
                            ( (!foundTokens) || (currProviderTokens < foundTokens) ) 
                        ) {
                            foundProvider = pProvider;
                            foundTokens = currProviderTokens;
                        }
                    }
                }

                if ( foundProvider != NULL ) {
                    // A provider was found, instantiate and read the schema mapping.

                    // First, create a connection on the provider. This is cheap as 
                    // long as we don't actually connect.
                    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
                    FdoPtr<FdoIConnection> connection = manager->CreateConnection (foundProvider->GetName ());
                    if ( connection ) {
                       
                        // Get the Schema Mapping from the connection. This ensures 
                        // we get the appropriate class of Schema Mapping for the provider,
                        // without generic FDO having to know these provider-specific classes.
                        FdoPtr<FdoPhysicalSchemaMapping> mapping = connection->CreateSchemaMapping();

                        if ( mapping ) {
                            // Sanity check to ensure we created the right schema mapping.
                            // mapping provider name must match the name of the provider it 
                            // was manufactured from.
                            if ( wcscmp(mapping->GetProvider(), foundProvider->GetName()) != 0 ) {
                                context->AddError( 
                                    FdoCommandExceptionP(
                                        FdoCommandException::Create(
                                            FdoException::NLSGetMessage(
                                                FDO_NLSID(COMMANDS_5_OVERRIDEPROVIDERMISMATCH),
                                                foundProvider->GetName(),
                                                mapping->GetProvider()
                                            )
                                        )
                                    )
                                );
                            }
                            else {
                                // Everything is ok. Add the mapping to this collection
                                // and set it up as the SAX Handler for all sub-elements.
                                Add(mapping);
                                mapping->InitFromXml( context, atts );
                                ret = mapping;
                            }
                        }
                    }
                }
            }
            else {
                // Schema Mapping set has no associated provider.
                context->AddError( 
                    FdoCommandExceptionP(
                        FdoCommandException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(COMMANDS_6_MISSINGPROVIDERNAME),
                                (FdoString*) schemaName
                            )
                        )
                    )
                );
            }

            if ( !ret ) {
                // A C++ class for this Schema Mapping was not found, skip this mapping.
                if (m_XmlSkipper == NULL)
                    m_XmlSkipper = FdoXmlSkipElementHandler::Create();
                ret = m_XmlSkipper;
            }
        }
        catch ( FdoException* e ) {
            // Catch any of our exceptions and add them to the XML error list.
            context->AddError( e );
            e->Release();
        }
    }

    return ret;
}

void FdoPhysicalSchemaMappingCollection::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    int i;

    // Always wrap schema override sets in the default root element.
    xmlWriter->SetDefaultRoot(true);

    // Write each Schema Override Set to the XML document.
    for ( i = 0; i < GetCount(); i++ ) {
        FdoPhysicalSchemaMappingP schemaMapping = GetItem(i);
        schemaMapping->_writeXml(xmlWriter, flags);
    }
}

