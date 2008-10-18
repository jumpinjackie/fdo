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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Overrides/FdoGrfpRasterDefinition.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-26 13:02:46 -0500 (Thu, 26 Jan 2006) $
 * $Revision: 24 $
 *
 */

#include "Overrides.h"
#include "FdoGrfpXmlGlobals.h"
#include <GdalFile/Override/FdoGrfpRasterLocation.h>
#include <GdalFile/Override/FdoGrfpRasterDefinition.h>

FdoGrfpRasterDefinition::FdoGrfpRasterDefinition(void)
{
	m_rasterLocations = FdoGrfpRasterLocationCollection::Create(this);
}

FdoGrfpRasterDefinition::~FdoGrfpRasterDefinition(void)
{
}

void FdoGrfpRasterDefinition::Dispose()
{
	delete this;
}

FdoGrfpRasterDefinition* FdoGrfpRasterDefinition::Create()
{
	return new FdoGrfpRasterDefinition();
}

FdoGrfpRasterLocationCollection* FdoGrfpRasterDefinition::GetLocations()
{
	return FDO_SAFE_ADDREF(m_rasterLocations.p);
}


void FdoGrfpRasterDefinition::InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs)
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
FdoXmlSaxHandler* FdoGrfpRasterDefinition::XmlStartElement(
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
			if (STREQUAL(name, FdoGrfpXmlGlobals::Location))
			{
				FdoGrfpRasterLocationP pNew = FdoGrfpRasterLocation::Create();
				pNew->InitFromXml(context, atts);
				m_rasterLocations->Add(pNew);
				pRet = pNew;
			}
			else // The <RasterDefinition> must be followed by <Location>
				throw FdoCommandException::Create(NlsMsgGet(GRFP_104_LOCATION_TAG_NOT_FOUND_UNDER_RASTERDEFINITION, "The RasterFile Configuration Override definition requires that element <RasterDefinition> must be followed by <Location>."));
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
FdoBoolean FdoGrfpRasterDefinition::XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname)
{
	return BaseType::XmlEndElement(context, uri, name, qname);
}

// Write this element to XML.
void FdoGrfpRasterDefinition::_writeXml( FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags )
{
    if (xmlWriter == NULL || flags == NULL) 
        throw FdoCommandException::Create(NlsMsgGet(GRFP_3_NULL_ARGUMENT, "A required argument was set to NULL"));
	
	xmlWriter->WriteStartElement(FdoGrfpXmlGlobals::RasterDefinition);
	BaseType::_writeXml(xmlWriter, flags);

	FdoInt32 count = m_rasterLocations->GetCount();
	for (int i = 0; i < count; i++)
	{
		FdoGrfpRasterLocationP location = m_rasterLocations->GetItem(i);
		location->_writeXml(xmlWriter, flags);
	}

	xmlWriter->WriteEndElement();

}


