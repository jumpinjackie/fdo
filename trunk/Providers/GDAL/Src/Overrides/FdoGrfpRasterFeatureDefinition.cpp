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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpRasterFeatureDefinition.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */

#include "Overrides.h"
#include "FdoGrfpXmlGlobals.h"
#include <GdalFile/Override/FdoGrfpRasterFeatureDefinition.h>
#include <GdalFile/Override/FdoGrfpRasterBandDefinition.h>

FdoGrfpRasterFeatureDefinition::FdoGrfpRasterFeatureDefinition(void)
{
	m_rasterBands = FdoGrfpRasterBandCollection::Create(this);
}

FdoGrfpRasterFeatureDefinition::~FdoGrfpRasterFeatureDefinition(void)
{
}

void FdoGrfpRasterFeatureDefinition::Dispose()
{
	delete this;
}

FdoGrfpRasterFeatureDefinition* FdoGrfpRasterFeatureDefinition::Create()
{
	return new FdoGrfpRasterFeatureDefinition();
}

FdoGrfpRasterBandCollection* FdoGrfpRasterFeatureDefinition::GetBands()
{
	return FDO_SAFE_ADDREF(m_rasterBands.p);
}

void FdoGrfpRasterFeatureDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
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
FdoXmlSaxHandler* FdoGrfpRasterFeatureDefinition::XmlStartElement(
   FdoXmlSaxContext* context, 
   FdoString* uri, 
   FdoString* name, 
   FdoString* qname, 
   FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* pRet = NULL;
	try
	{
		if (context == NULL || name == NULL)
            throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
		
		pRet = BaseType::XmlStartElement(context, uri, name, qname, atts);
		if (pRet == NULL)
		{
			if (STREQUAL(name, FdoGrfpXmlGlobals::Band))
			{
				FdoGrfpRasterBandDefinitionP pNewBand = FdoGrfpRasterBandDefinition::Create();
				pNewBand->InitFromXml(context, atts);

                //
				// verify whether the band number is sequential 
                //
				FdoInt32 number = pNewBand->GetBandNumber();
                if (number != m_rasterBands->GetCount()+1) {
					throw FdoException::Create(NlsMsgGet( GRFP_92_BAND_NUMBER_NOT_SEQUENTIAL, "Band number is not sequential."));
                }

				m_rasterBands->Add(pNewBand);	
				pRet = pNewBand;
			}
			else //The <Feature> must be followed by <Band>
				throw FdoCommandException::Create(NlsMsgGet(GRFP_102_BAND_TAG_NOT_FOUND_UNDER_FEATURE, "The RasterFile Configuration Override definition requires that element <Feature> must be followed by <Band>."));
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
FdoBoolean FdoGrfpRasterFeatureDefinition::XmlEndElement(
   FdoXmlSaxContext* context, 
   FdoString* uri, 
   FdoString* name, 
   FdoString* qname
)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

void FdoGrfpRasterFeatureDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    if (xmlWriter == NULL || flags == NULL) 
        throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));

	xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::Feature);
	BaseType::_writeXml(xmlWriter, flags);

	FdoInt32 count = m_rasterBands->GetCount();
	for (int i = 0; i < count; i++)
	{
		FdoGrfpRasterBandDefinitionP bandDef = m_rasterBands->GetItem(i);
		bandDef->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement();

}

FdoGrfpRasterFeatureCollection* FdoGrfpRasterFeatureCollection::Create(FdoGrfpRasterLocation *parent)
{
	return new FdoGrfpRasterFeatureCollection(parent);
}

FdoGrfpRasterFeatureCollection::FdoGrfpRasterFeatureCollection(FdoGrfpRasterLocation *parent) :
		FdoPhysicalElementMappingCollection<FdoGrfpRasterFeatureDefinition>((FdoPhysicalElementMapping*)parent)
{
}

FdoGrfpRasterFeatureCollection::~FdoGrfpRasterFeatureCollection()
{
}

void FdoGrfpRasterFeatureCollection::Dispose()
{
	delete this;
}
