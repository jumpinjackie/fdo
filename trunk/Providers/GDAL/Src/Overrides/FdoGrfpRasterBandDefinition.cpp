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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpRasterBandDefinition.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */

#include "Overrides.h"
#include "FdoGrfpXmlGlobals.h"
#include <FdoCommonOSUtil.h>
#include <GdalFile/Override/FdoGrfpRasterImageDefinition.h>
#include <GdalFile/Override/FdoGrfpRasterBandDefinition.h>


FdoGrfpRasterBandDefinition::FdoGrfpRasterBandDefinition(void)
    : m_bandNumber(-1)
{
}

FdoGrfpRasterBandDefinition::~FdoGrfpRasterBandDefinition(void)
{
}

void FdoGrfpRasterBandDefinition::Dispose()
{
	delete this;
}

FdoGrfpRasterBandDefinition* FdoGrfpRasterBandDefinition::Create()
{
	return new FdoGrfpRasterBandDefinition();
}

FdoGrfpRasterImageDefinition* FdoGrfpRasterBandDefinition::GetImage()
{
	return FDO_SAFE_ADDREF(m_imageDefinition.p);
}

void FdoGrfpRasterBandDefinition::SetImage(FdoGrfpRasterImageDefinition* imageDef)
{
	m_imageDefinition = FDO_SAFE_ADDREF(imageDef);
	m_imageDefinition->SetParent(this);
}

FdoInt32 FdoGrfpRasterBandDefinition::GetBandNumber()
{
	return m_bandNumber;
}

void FdoGrfpRasterBandDefinition::SetBandNumber(FdoInt32 bandNumber)
{
	m_bandNumber = bandNumber;
}

void FdoGrfpRasterBandDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
{
	try
	{
        if (attrs == NULL || pContext == NULL) 
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

		BaseType::InitFromXml(pContext, attrs);

        FdoInt32 bandNumber = 1;
        FdoXmlAttributeP att = attrs->FindItem(FdoGrfpXmlGlobals::BandNumber);
        if (att != NULL) {
            FdoStringP attrValue = att->GetLocalValue();
            swscanf(attrValue, L"%d", &bandNumber);
            if (bandNumber <= 0) {
                throw FdoCommandException::Create(
                    NlsMsgGet2(GRFP_99_INVALID_XML_ATTRIBUTE, 
                               "Raster Configuration file contains an invalid value '%1$ls' for XML attribute: '%1$ls'.",
                               (FdoString*)attrValue, FdoGrfpXmlGlobals::BandNumber
                    )
                );
            }
        }

        SetBandNumber(bandNumber);
	}
	catch (FdoException* ex)
	{
		pContext->AddError(ex);
		ex->Release();
	}
}

// Handle the start of a sub-element
FdoXmlSaxHandler* FdoGrfpRasterBandDefinition::XmlStartElement(
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
			if (STREQUAL(name, FdoGrfpXmlGlobals::Image))
			{
				FdoGrfpRasterImageDefinitionP imageDefinition = FdoGrfpRasterImageDefinition::Create();
				imageDefinition->InitFromXml(context, atts);
				SetImage(imageDefinition.p);
				pRet = imageDefinition;
			}
			else // The <Band> must be followed by <Image>
				throw FdoCommandException::Create(NlsMsgGet(GRFP_103_IMAGE_TAG_NOT_FOUND_UNDER_BAND, "The RasterFile Configuration Override definition requires that element <Band> must be followed by <Image>."));
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
FdoBoolean FdoGrfpRasterBandDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

// Write this element to XML.
void FdoGrfpRasterBandDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    if (xmlWriter == NULL || flags == NULL) 
        throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
    
	xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::Band);

	BaseType::_writeXml(xmlWriter, flags);

    // Write out the band number
    char buff[20];
    FdoInt32 bandNumber = GetBandNumber();
    FdoCommonOSUtil::itoa(bandNumber, buff); 
    xmlWriter->WriteAttribute(FdoGrfpXmlGlobals::BandNumber, FdoStringP(buff));

    // Write out the child image definition
    m_imageDefinition->_writeXml(xmlWriter, flags);

	xmlWriter->WriteEndElement();	
}

FdoGrfpRasterBandCollection* FdoGrfpRasterBandCollection::Create(FdoGrfpRasterFeatureDefinition *parent)
{
	return new FdoGrfpRasterBandCollection(parent);
}

FdoGrfpRasterBandCollection::FdoGrfpRasterBandCollection(FdoGrfpRasterFeatureDefinition *parent) :
		FdoPhysicalElementMappingCollection<FdoGrfpRasterBandDefinition>((FdoPhysicalElementMapping*)parent)
{
}

FdoGrfpRasterBandCollection::~FdoGrfpRasterBandCollection()
{
}

void FdoGrfpRasterBandCollection::Dispose()
{
	delete this;
}

