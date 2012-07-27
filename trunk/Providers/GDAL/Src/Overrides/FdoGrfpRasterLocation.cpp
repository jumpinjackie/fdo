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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpRasterLocation.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */

#include "Overrides.h"
#include "FdoGrfpXmlGlobals.h"
#include <GdalFile/Override/FdoGrfpRasterFeatureDefinition.h>
#include <GdalFile/Override/FdoGrfpRasterLocation.h>

FdoGrfpRasterLocation::FdoGrfpRasterLocation(void)
{
	m_featureCatalogue = FdoGrfpRasterFeatureCollection::Create(this);
}

FdoGrfpRasterLocation::~FdoGrfpRasterLocation(void)
{
}

void FdoGrfpRasterLocation::Dispose()
{
	delete this;
}

FdoGrfpRasterLocation* FdoGrfpRasterLocation::Create()
{
	return new FdoGrfpRasterLocation();
}

FdoGrfpRasterFeatureCollection* FdoGrfpRasterLocation::GetFeatureCatalogue()
{
	return FDO_SAFE_ADDREF(m_featureCatalogue.p);
}

void FdoGrfpRasterLocation::SetFeatureCatalogue(FdoGrfpRasterFeatureCollection* catalogue)
{
	m_featureCatalogue = FDO_SAFE_ADDREF(catalogue);
}

void FdoGrfpRasterLocation::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
	try
	{
        if (attrs == NULL || pContext == NULL) 
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

		BaseType::InitFromXml(pContext, attrs);
	}
	catch (FdoException* ex)
	{
		pContext->AddError(ex);
		ex->Release();
	}
}

// Handle the start of a sub-element
FdoXmlSaxHandler* FdoGrfpRasterLocation::XmlStartElement(
   FdoXmlSaxContext* context, 
   FdoString* uri, 
   FdoString* name, 
   FdoString* qname, 
   FdoXmlAttributeCollection* atts)
{
    FdoXmlSaxHandler* pRet = NULL;
    try 
	{
        if (context == NULL || name == NULL)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
		
		pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL)
		{
			if (STREQUAL(name, FdoGrfpXmlGlobals::Feature))
			{
				FdoGrfpRasterFeatureDefinitionP newFeature = FdoGrfpRasterFeatureDefinition::Create();
				newFeature->InitFromXml(context, atts);
				m_featureCatalogue->Add(newFeature);
				pRet = newFeature;
			}
			else // The <Location> should be either followed by <Feature> or by nothing.
				throw FdoCommandException::Create(NlsMsgGet(GRFP_101_FEATURE_TAG_NOT_FOUND_UNDER_LOCATION, "The RasterFile Configuration Override definition requires that element <Location> must be either followed by <Feature> or by nothing."));
		}
	}
    catch (FdoException* ex) 
    {
        context->AddError(ex);
        ex->Release();
    }

    return pRet;
}

// Handle the end of a sub-element
FdoBoolean FdoGrfpRasterLocation::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

// Write this element to XML.
void FdoGrfpRasterLocation::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    if (xmlWriter == NULL || flags == NULL) 
        throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
	
	xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::Location);
	BaseType::_writeXml(xmlWriter, flags);

	FdoInt32 count = m_featureCatalogue->GetCount();
	for (int i = 0; i < count; i++)
	{
		FdoGrfpRasterFeatureDefinitionP featureDef = m_featureCatalogue->GetItem(i);
		featureDef->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement();
}

FdoGrfpRasterLocationCollection* FdoGrfpRasterLocationCollection::Create(FdoGrfpRasterDefinition *parent)
{
	return new FdoGrfpRasterLocationCollection(parent);
}

FdoGrfpRasterLocationCollection::FdoGrfpRasterLocationCollection(FdoGrfpRasterDefinition *parent) : 
	FdoPhysicalElementMappingCollection<FdoGrfpRasterLocation>((FdoPhysicalElementMapping*)parent)
{
}

FdoGrfpRasterLocationCollection::~FdoGrfpRasterLocationCollection()
{
}

void FdoGrfpRasterLocationCollection::Dispose()
{
	delete this;
}

